/* K=7 r=1/2 Viterbi decoder using VOLK kernels with portable fallback
 * Optimized for performance - minimal branching, no debug code
 */
#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <limits.h>

#include "config.h"
#include "fec.h"

#ifdef HAVE_VOLK
#include <volk/volk.h>
#endif

// Branch table storage - 64 bytes total (32 per polynomial)
static unsigned char Branchtab27_volk[64] __attribute__((aligned(64)));
static int V27_Init_volk = 0;

struct v27_state {
    unsigned char *metrics_X;      // Old metrics (64 bytes, aligned)
    unsigned char *metrics_Y;      // New metrics (64 bytes, aligned)
    unsigned char *decisions_volk; // Decision buffer for VOLK
    unsigned char *dp;             // Current position in decisions
    int framebits;                 // Frame length info
};

void set_viterbi27_polynomial(int polys[2]) {
    // Generate branch table in VOLK format
    // Unroll for better performance
    for(int state = 0; state < 32; state++) {
        Branchtab27_volk[state] = ((polys[0] < 0) ^ parity((2*state) & abs(polys[0]))) ? 255 : 0;
        Branchtab27_volk[32 + state] = ((polys[1] < 0) ^ parity((2*state) & abs(polys[1]))) ? 255 : 0;
    }
    V27_Init_volk = 1;
}

void *create_viterbi27(int len) {
    struct v27_state *vp;
    
    if(!V27_Init_volk) {
        int default_polys[2] = { V27POLYA, V27POLYB };
        set_viterbi27_polynomial(default_polys);
    }
    
    // Calculate total memory needed and allocate in one block for better cache locality
    size_t total_size = sizeof(struct v27_state) + 64 + 64 + ((len + 6) * 8) + 128; // Extra for alignment
    void *mem_block;
    
    if(posix_memalign(&mem_block, 64, total_size)) {
        return NULL;
    }
    
    // Partition the memory block
    vp = (struct v27_state *)mem_block;
    unsigned char *ptr = (unsigned char *)mem_block + sizeof(struct v27_state);
    
    // Align to 64-byte boundary
    ptr = (unsigned char *)(((uintptr_t)ptr + 63) & ~63);
    vp->metrics_X = ptr;
    ptr += 64;
    
    vp->metrics_Y = ptr;
    ptr += 64;
    
    vp->decisions_volk = ptr;
    
    vp->framebits = len;
    vp->dp = vp->decisions_volk;
    
    // Initialize inline
    memset(vp->metrics_X, 63, 64);
    vp->metrics_X[0] = 0;
    
    return vp;
}

int init_viterbi27(void *p, int starting_state) {
    struct v27_state *vp = (struct v27_state *)p;
    
    // Fast path for common case (state 0)
    memset(vp->metrics_X, 63, 64);
    vp->metrics_X[starting_state & 63] = 0;
    vp->dp = vp->decisions_volk;
    
    return 0;
}

#ifndef HAVE_VOLK
// Inline portable ACS butterfly for better performance
static inline void BFLY_portable(int i, unsigned char *syms, unsigned char *Y, 
                                unsigned char *X, unsigned char *decisions) {
    // Compute branch metric
    unsigned int s0 = syms[0] ^ Branchtab27_volk[i];
    unsigned int s1 = syms[1] ^ Branchtab27_volk[32 + i];
    unsigned int metric = ((s0 + s1 + 1) >> 3) & 63;
    
    // Path metrics
    unsigned char m0 = X[i] + metric;
    unsigned char m1 = X[i + 32] + (63 - metric);
    unsigned char m2 = X[i] + (63 - metric);
    unsigned char m3 = X[i + 32] + metric;
    
    // Decisions and survivors
    unsigned int decision0 = (m0 > m1);
    unsigned int decision1 = (m2 > m3);
    
    Y[2 * i] = decision0 ? m1 : m0;
    Y[2 * i + 1] = decision1 ? m3 : m2;
    
    // Pack decisions
    decisions[i >> 2] |= (decision0 << ((2 * i) & 7)) | (decision1 << (((2 * i) + 1) & 7));
}

static void viterbi27_update_portable(unsigned char *Y, unsigned char *X, 
                                     unsigned char *syms, unsigned char *dec,
                                     unsigned int nbits) {
    unsigned char *tmp;
    
    for (unsigned int s = 0; s < nbits; s++) {
        memset(&dec[s * 8], 0, 8);
        
        // Unroll butterfly loop for better performance
        for (int i = 0; i < 32; i += 4) {
            BFLY_portable(i, &syms[s * 2], Y, X, &dec[s * 8]);
            BFLY_portable(i + 1, &syms[s * 2], Y, X, &dec[s * 8]);
            BFLY_portable(i + 2, &syms[s * 2], Y, X, &dec[s * 8]);
            BFLY_portable(i + 3, &syms[s * 2], Y, X, &dec[s * 8]);
        }
        
        // Renormalize - find min in one pass
        unsigned char min = Y[0];
        for (int i = 1; i < 64; i++) {
            if (Y[i] < min) min = Y[i];
        }
        
        // Subtract min if needed
        if (min > 0) {
            for (int i = 0; i < 64; i += 8) {
                Y[i] -= min;
                Y[i + 1] -= min;
                Y[i + 2] -= min;
                Y[i + 3] -= min;
                Y[i + 4] -= min;
                Y[i + 5] -= min;
                Y[i + 6] -= min;
                Y[i + 7] -= min;
            }
        }
        
        // Swap pointers
        tmp = X;
        X = Y;
        Y = tmp;
    }
}
#endif

int update_viterbi27_blk(void *p, unsigned char * restrict syms, int nbits) {
    struct v27_state *vp = (struct v27_state *)p;

#ifdef HAVE_VOLK
    volk_8u_x4_conv_k7_r2_8u(
        vp->metrics_Y,      
        vp->metrics_X,      
        syms,               
        vp->dp,             
        nbits,              
        0,                  
        Branchtab27_volk    
    );
    vp->dp += nbits * 8;
#else
    viterbi27_update_portable(vp->metrics_Y, vp->metrics_X, syms, vp->dp, nbits);
    if (nbits & 1) {
        unsigned char *tmp = vp->metrics_X;
        vp->metrics_X = vp->metrics_Y;
        vp->metrics_Y = tmp;
    }
    vp->dp += nbits * 8;
#endif
    
    return 0;
}

int chainback_viterbi27(void *p, unsigned char *data, unsigned int nbits, unsigned int endstate) {
    struct v27_state *vp = (struct v27_state *)p;
    unsigned char *d = vp->decisions_volk;
    
    endstate &= 63;
    memset(data, 0, (nbits + 7) / 8);
    
    // Process tail bits - unrolled for performance
    int idx = nbits + 5;
    endstate = ((endstate >> 1) | (((d[(idx << 3) + (endstate >> 3)] >> (endstate & 7)) & 1) << 5)) & 63;
    idx--;
    endstate = ((endstate >> 1) | (((d[(idx << 3) + (endstate >> 3)] >> (endstate & 7)) & 1) << 5)) & 63;
    idx--;
    endstate = ((endstate >> 1) | (((d[(idx << 3) + (endstate >> 3)] >> (endstate & 7)) & 1) << 5)) & 63;
    idx--;
    endstate = ((endstate >> 1) | (((d[(idx << 3) + (endstate >> 3)] >> (endstate & 7)) & 1) << 5)) & 63;
    idx--;
    endstate = ((endstate >> 1) | (((d[(idx << 3) + (endstate >> 3)] >> (endstate & 7)) & 1) << 5)) & 63;
    idx--;
    endstate = ((endstate >> 1) | (((d[(idx << 3) + (endstate >> 3)] >> (endstate & 7)) & 1) << 5)) & 63;
    
    // Main traceback loop - optimized with prefetching
    for(int i = nbits - 1; i >= 0; i--) {
        #ifdef __builtin_prefetch
        if (i >= 16) __builtin_prefetch(&d[(i-16) << 3], 0, 1);
        #endif
        
        int dec_byte_offset = i << 3;
        int byte_idx = endstate >> 3;
        int bit_idx = endstate & 7;
        
        // Branchless bit storage
        data[i >> 3] |= ((endstate & 1) << (7 - (i & 7)));
        
        // Update state
        int decision = (d[dec_byte_offset + byte_idx] >> bit_idx) & 1;
        endstate = ((endstate >> 1) | (decision << 5)) & 63;
    }
    
    return 0;
}

void delete_viterbi27(void *p) {
    // Only need one free since we allocated as a single block
    free(p);
}