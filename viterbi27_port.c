/* K=7 r=1/2 Viterbi decoder, portable C, optimized for auto-vectorization
 * Based on concepts by Phil Karn, KA9Q, and modern C optimization techniques.
 * May be used under the terms of the GNU Lesser General Public License (LGPL)
 */
#include <stdio.h>
#include <stdlib.h>
#include <memory.h> // For memset, if used explicitly (not in current version but good include)
#include <limits.h> // For UINT_MAX if needed, though not directly used here

#include "fec.h" // For V27POLYA, V27POLYB, parity(), and API consistency

// --- OPTIMIZED DATA STRUCTURES ---
// Path metrics are 'unsigned int' to handle sums of branch metrics (0-510) before normalization.
// Explicitly align for better SIMD performance. 64-byte alignment is good for AVX512.
typedef union {
    unsigned int ui[64]; // K=7 -> 2^(K-1) = 64 states
    unsigned char c[64 * sizeof(unsigned int)]; // For memset or byte-level access if ever needed
} __attribute__((aligned(64))) metric_t;

// Decisions are packed. For K=7, 64 states, we make 32 pairs of decisions per trellis stage.
// Each decision is 1 bit. We need to store 32 decisions for paths to even states, 32 for odd.
// Total 64 decisions per stage.
typedef union {
    unsigned long w[2]; // Packs 64 decisions if unsigned long is 64-bit (32 per ulong)
                        // Or use unsigned int w[2] if targetting 32 decisions per word more explicitly
                        // and packing logic reflects this. The original optimized code used 2 ulongs.
} __attribute__((aligned(16))) decision_t; // 16 byte alignment is usually fine for this.

// Branch metric table, also aligned. Stores 0 or 255.
// For K=7, there are 2^(K-2) = 32 unique sets of branch outputs for input bit 0.
static union branchtab27 {
    unsigned char c[32];
} Branchtab27[2] __attribute__((aligned(64)));

static int V27_Init = 0;

// Main state structure, also aligned.
struct v27_state {
  metric_t metrics1;
  metric_t metrics2;
  decision_t *dp;          // Pointer to current decision write location
  metric_t *old_metrics, *new_metrics; // Pointers to path metrics, swapped on every bit
  decision_t *decisions;   // Buffer for all decisions in a block
  // Store actual polynomials used, in case they are changed from default
  int polys[2];
};

// --- Function Implementations ---

void set_viterbi27_polynomial(int polys[2]) {
    for(int state=0; state < 32; state++){
        Branchtab27[0].c[state] = ((polys[0] < 0) ^ parity((2*state) & abs(polys[0]))) ? 255 : 0;
        Branchtab27[1].c[state] = ((polys[1] < 0) ^ parity((2*state) & abs(polys[1]))) ? 255 : 0;
    }
    V27_Init++;
}

void *create_viterbi27(int len) {
  struct v27_state *vp;

  if(!V27_Init){
    // Initialize with default polynomials if not already called by global set_viterbi27_polynomial
    // This ensures Branchtab is ready even if only  version is used.
    int default_polys[2] = { V27POLYA, V27POLYB };
    set_viterbi27_polynomial(default_polys);
  }

  // Use posix_memalign for guaranteed alignment.
  if(posix_memalign((void**)&vp, 64, sizeof(struct v27_state))) {
    perror("posix_memalign for v27_state failed");
    return NULL;
  }
  
  vp->polys[0] = V27POLYA; // Store default, can be overridden by set_viterbi27_polynomial
  vp->polys[1] = V27POLYB;


  // Align the decisions buffer. len is data bits. Add trellis depth (K-1 = 6) for tail.
  if(posix_memalign((void**)&vp->decisions, 64, (len + (7-1)) * sizeof(decision_t))){
    perror("posix_memalign for decisions buffer failed");
    free(vp);
    return NULL;
  }

  // Path metrics are within the struct, already aligned due to struct alignment.
  // Initialize pointers
  vp->old_metrics = &vp->metrics1;
  vp->new_metrics = &vp->metrics2;

  init_viterbi27(vp, 0); // Initialize with starting state 0
  return vp;
}

int init_viterbi27(void *p, int starting_state) {
  struct v27_state *vp = (struct v27_state *)p;
  if(p == NULL) return -1;
  
  // Initialize path metrics. Set all to a large value.
  for(int i=0; i<64; i++) {
    vp->metrics1.ui[i] = 65535 * 2; // A reasonably large value, but not UINT_MAX to allow additions
  }

  vp->old_metrics = &vp->metrics1;
  vp->new_metrics = &vp->metrics2;
  vp->dp = vp->decisions; // Reset decision pointer to start of buffer

  // Set the metric for the known starting state to 0.
  if (starting_state >=0 && starting_state < 64) {
    vp->old_metrics->ui[starting_state & 63] = 0;
  } else {
    vp->old_metrics->ui[0] = 0; // Default to state 0 if invalid
  }
  
  // Re-initialize branchtabs if polynomials might have changed via a global call
  // This is a bit tricky; the  version should ideally use its own poly copy.
  // For now, assume Branchtab is set by create or a global call.
  // If vp->polys were used to regenerate Branchtab here, it would be instance-specific.
  // The current design uses global Branchtab27_polyA/B.

  return 0;
}
// ... (Keep existing includes, structs, set_viterbi27_polynomial, create_viterbi27, init_viterbi27) ...
// ... (Ensure Branchtab27, V27_Init, struct v27_state, metric_t, decision_t are defined as in your 26Mbps version) ...

int update_viterbi27_blk(void *p, unsigned char * restrict syms, int nbits) {
    struct v27_state *vp = (struct v27_state *)p;
    if(p == NULL || syms == NULL) return -1;

    decision_t * restrict d_ptr = vp->dp;

    // Use local pointers for frequently accessed data, helps compiler alias analysis
    const unsigned char * restrict btab0_local = Branchtab27[0].c;
    const unsigned char * restrict btab1_local = Branchtab27[1].c;
    unsigned int * restrict old_m_local = vp->old_metrics->ui;
    unsigned int * restrict new_m_local = vp->new_metrics->ui; // This is the target for ACS

    // Align stack array if desired, though often handled well by modern compilers.
    unsigned char current_stage_decisions[64] __attribute__((aligned(64))); 

    for (int bit_idx = 0; bit_idx < nbits; ++bit_idx) {
        const unsigned char s0_input = *syms++; 
        const unsigned char s1_input = *syms++; 

        // ACS (Add-Compare-Select) Stage
        // Rely on compiler's default unrolling (e.g., via -funroll-loops with -O3)
        // Do NOT add a #pragma unroll here initially.
        #pragma clang loop vectorize(enable)
        #pragma clang loop interleave(enable) 
        for(int i = 0; i < 32; i++) {
            // --- Load phase ---
            unsigned int prev_metric_state_i    = old_m_local[i];
            unsigned int prev_metric_state_i32  = old_m_local[i+32];
            unsigned char branch_xor_mask_polyA = btab0_local[i];
            unsigned char branch_xor_mask_polyB = btab1_local[i];

            // --- Branch metric calculation phase ---
            unsigned int xor_result_A = (unsigned int)(branch_xor_mask_polyA ^ s0_input);
            unsigned int xor_result_B = (unsigned int)(branch_xor_mask_polyB ^ s1_input);
            
            unsigned int branch_metric_fwd  = xor_result_A + xor_result_B;
            unsigned int branch_metric_comp = 510 - branch_metric_fwd; // Max bm_fwd is 510

            // --- Path 1 (computations for next state 2*i) ---
            unsigned int path1_metric_from_state_i   = prev_metric_state_i + branch_metric_fwd;
            unsigned int path1_metric_from_state_i32 = prev_metric_state_i32 + branch_metric_comp;
            
            // Decision: 1 if path from state_i32 is chosen (i.e., if its metric is smaller)
            unsigned int path1_decision  = (path1_metric_from_state_i > path1_metric_from_state_i32); 
            unsigned int path1_survivor_metric = path1_decision ? path1_metric_from_state_i32 : path1_metric_from_state_i;

            // --- Path 2 (computations for next state 2*i+1) ---
            unsigned int path2_metric_from_state_i   = prev_metric_state_i + branch_metric_comp;
            unsigned int path2_metric_from_state_i32 = prev_metric_state_i32 + branch_metric_fwd;

            // Decision: 1 if path from state_i32 is chosen (i.e., if its metric is smaller)
            unsigned int path2_decision  = (path2_metric_from_state_i > path2_metric_from_state_i32);
            unsigned int path2_survivor_metric = path2_decision ? path2_metric_from_state_i32 : path2_metric_from_state_i;

            // --- Store phase (interleaved) ---
            new_m_local[2*i]       = path1_survivor_metric;
            current_stage_decisions[2*i] = (unsigned char)path1_decision;

            new_m_local[2*i+1]     = path2_survivor_metric;
            current_stage_decisions[2*i+1] = (unsigned char)path2_decision;
        }

        // Pack decisions from current_stage_decisions into d_ptr->w
        // (This packing logic is from your 26Mbps version - assumed correct)
        unsigned long packed_w0 = 0;
        unsigned long packed_w1 = 0;
        for (int k = 0; k < 32; ++k) {
            if (current_stage_decisions[k]) {
                packed_w0 |= (1UL << k);
            }
        }
        for (int k = 0; k < 32; ++k) {
            if (current_stage_decisions[32 + k]) {
                packed_w1 |= (1UL << k);
            }
        }
        d_ptr->w[0] = packed_w0;
        d_ptr->w[1] = packed_w1;
        d_ptr++;

        // Metric Normalization Stage (using new_m_local)
        unsigned int min_metric = new_m_local[0];
        for (int k = 1; k < 64; k++) {
            if (new_m_local[k] < min_metric) {
                min_metric = new_m_local[k];
            }
        }
        if (min_metric > 0) {
            // This loop should also be a candidate for vectorization & unrolling
            for (int k = 0; k < 64; k++) {
                new_m_local[k] -= min_metric;
            }
        }
        
        // Swap pointers to old and new metrics
        metric_t *tmp_ptr = vp->old_metrics;
        vp->old_metrics = vp->new_metrics;
        vp->new_metrics = tmp_ptr;
        
        // Update local pointers for the next iteration's ACS stage
        old_m_local = vp->old_metrics->ui;
        new_m_local = vp->new_metrics->ui; 
    }

    vp->dp = d_ptr;
    return 0;
}

int chainback_viterbi27(void *p, unsigned char *data, unsigned int nbits, unsigned int endstate) {
    struct v27_state *vp = (struct v27_state *)p;
    decision_t *d = vp->decisions;

    endstate %= 64;
    endstate <<= 2;

    d += 6; /* Look past tail */
    while(nbits-- != 0){
        int k = (d[nbits].w[(endstate>>2)/32] >> ((endstate>>2)%32)) & 1;
        data[nbits>>3] = endstate = (endstate >> 1) | (k << 7);
    }
    return 0;
}

void delete_viterbi27(void *p) {
  struct v27_state *vp = (struct v27_state *)p;
  if(vp != NULL){
    free(vp->decisions); // Free the aligned decisions buffer
    free(vp);            // Free the main state structure
  }
  // V27_Init and Branchtab are static/global, not freed here.
}
