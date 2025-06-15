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
static union {
    unsigned char c[32];
} __attribute__((aligned(64))) Branchtab27_polyA[1]; // Renamed for clarity if multiple tables were used

static union {
    unsigned char c[32];
} __attribute__((aligned(64))) Branchtab27_polyB[1];

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
  struct v27_state temp_dummy_for_init; // For passing polys to actual instance
  temp_dummy_for_init.polys[0] = polys[0];
  temp_dummy_for_init.polys[1] = polys[1];

  for(int i=0; i < 32; i++){ // K=7 -> 2^(7-2) = 32 entries
    // Output bit for G1 if input bit is 0, for state 'i' (representing top half of trellis pairs)
    Branchtab27_polyA[0].c[i] = (unsigned char)(((polys[0] < 0) ^ parity((2*i) & abs(polys[0]))) ? 255 : 0);
    // Output bit for G2 if input bit is 0
    Branchtab27_polyB[0].c[i] = (unsigned char)(((polys[1] < 0) ^ parity((2*i) & abs(polys[1]))) ? 255 : 0);
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

int update_viterbi27_blk(void *p, unsigned char * restrict syms, int nbits) {
  struct v27_state *vp = (struct v27_state *)p;
  if(p == NULL || syms == NULL) return -1;

  decision_t * restrict d_ptr = vp->dp;
  unsigned int * const restrict old_m = vp->old_metrics->ui;
  unsigned int * const restrict new_m = vp->new_metrics->ui;
  
  // Local copies of branch tables for restrict keyword and potentially better cache usage
  const unsigned char * const restrict btab_A = Branchtab27_polyA[0].c;
  const unsigned char * const restrict btab_B = Branchtab27_polyB[0].c;

  for (int bit_idx = 0; bit_idx < nbits; ++bit_idx) {
    const unsigned char sym0 = *syms++;
    const unsigned char sym1 = *syms++;
    
    // Temporary arrays for decisions to help compiler with vectorization by removing loop-carried dependencies.
    // These store which path was chosen (0 for top, 1 for bottom in butterfly).
    unsigned char decisions_p0_temp[32]; // Decisions for new states 0, 2, ..., 62
    unsigned char decisions_p1_temp[32]; // Decisions for new states 1, 3, ..., 63

    // The core Add-Compare-Select (ACS) butterfly operations
    // This loop processes 32 butterflies for K=7 (64 states)
    #pragma clang loop vectorize(enable) interleave(enable) unroll(enable)
    for(int i = 0; i < 32; i++) { // i is the 'upper' state index in the butterfly (0 to 31)
        // Branch metrics are calculated based on (Branchtab ^ Symbol).
        // If Branchtab is 0/255 (for expected bit 0/1) and Symbol is 0/255 (hard decision),
        // XOR gives 0 for match, 255 for mismatch. Sum is 0, 255, or 510.

        // Branch metrics for input bit '0'
        // Path from old_state 'i' (top in butterfly)
        const unsigned int bm0_top = (btab_A[i] ^ sym0) + (btab_B[i] ^ sym1);
        // Path from old_state 'i+32' (bottom in butterfly)
        // Output for (state i+32, input 0) is complementary to (state i, input 0)
        // because G1 and G2 polynomials for K=7 both have the MSB tap (for s5).
        const unsigned int bm0_bot = 510 - bm0_top;

        // Branch metrics for input bit '1'
        // Output for (state i, input 1) is complementary to (state i, input 0).
        const unsigned int bm1_top = 510 - bm0_top;
        // Output for (state i+32, input 1) is complementary to (state i+32, input 0),
        // which means it's same as (state i, input 0).
        const unsigned int bm1_bot = bm0_top;

        // ACS for new_state[2*i] (resulting from input bit 0)
        unsigned int path0_metric = old_m[i]    + bm0_top;
        unsigned int path1_metric = old_m[i+32] + bm0_bot;
        
        if (path0_metric <= path1_metric) {
            new_m[2*i] = path0_metric;
            decisions_p0_temp[i] = 0; // Chose path from old_m[i]
        } else {
            new_m[2*i] = path1_metric;
            decisions_p0_temp[i] = 1; // Chose path from old_m[i+32]
        }

        // ACS for new_state[2*i+1] (resulting from input bit 1)
        path0_metric = old_m[i]    + bm1_top;
        path1_metric = old_m[i+32] + bm1_bot;

        if (path0_metric <= path1_metric) {
            new_m[2*i+1] = path0_metric;
            decisions_p1_temp[i] = 0; // Chose path from old_m[i]
        } else {
            new_m[2*i+1] = path1_metric;
            decisions_p1_temp[i] = 1; // Chose path from old_m[i+32]
        }
    }
    
    // Pack the decisions from temporary arrays into the decision_t structure
    // This part is scalar but small compared to the ACS loop.
    unsigned long current_decisions_word0 = 0;
    unsigned long current_decisions_word1 = 0;
    for (int i = 0; i < 32; i++) {
        if (decisions_p0_temp[i]) current_decisions_word0 |= (1UL << i);
        if (decisions_p1_temp[i]) current_decisions_word1 |= (1UL << i);
    }
    d_ptr->w[0] = current_decisions_word0; // Decisions for even new states (target states for input 0)
    d_ptr->w[1] = current_decisions_word1; // Decisions for odd new states (target states for input 1)
    d_ptr++;
    
    // Metric Normalization: Subtract the minimum metric from all current new_metrics
    // This prevents overflow and keeps metrics in a manageable range.
    unsigned int min_metric = new_m[0];
    #pragma clang loop vectorize(enable)
    for (int i = 1; i < 64; i++) {
        if (new_m[i] < min_metric) {
            min_metric = new_m[i];
        }
    }
    
    // Only subtract if min_metric is substantial, to avoid issues if all are zero.
    // Or, more robustly, subtract if min_metric > 0 or if any metric is large.
    // A common check is if min_metric is large enough that adding max_branch_metric (510) would overflow.
    // For simplicity here, always normalize if min_metric could be non-zero.
    // Or, normalize if min_metric is not already 0 to avoid subtracting 0 from 0 repeatedly.
    if (min_metric > 0) { // More advanced: check if min_metric > THRESHOLD or any metric > MAX_ALLOWED
        #pragma clang loop vectorize(enable)
        for (int i = 0; i < 64; i++) {
            new_m[i] -= min_metric;
        }
    }

    // Swap metric buffers for next iteration
    metric_t *tmp_metrics = vp->old_metrics;
    vp->old_metrics = vp->new_metrics;
    vp->new_metrics = tmp_metrics;
  }
  vp->dp = d_ptr; // Save new decision pointer
  return 0;
}

int chainback_viterbi27(
      void *p,
      unsigned char * restrict data, /* Decoded output data */
      unsigned int nbits, /* Number of data bits */
      unsigned int endstate) /* Terminal encoder state (0-63 for K=7) */
{
  struct v27_state *vp = (struct v27_state *)p;
  if(p == NULL || data == NULL) return -1;

  decision_t * restrict d_base = vp->decisions;
  endstate &= 63; // Ensure endstate is within valid range for K=7

  // Start from the decision corresponding to the last data bit
  decision_t *d_ptr = d_base + nbits; 

  // Chainback, bit by bit
  for (unsigned int k = 0; k < nbits; k++) {
    d_ptr--; // Move to the decisions for the current bit being decoded

    int decision_bit;
    // endstate determines which path (0 or 1) to look at in the decision word.
    // If current endstate's LSB is 0, it came from input bit 0 (use decisions_p0_temp -> d_ptr->w[0])
    // If current endstate's LSB is 1, it came from input bit 1 (use decisions_p1_temp -> d_ptr->w[1])
    if ((endstate % 2) == 0) { // Current state is an even state (e.g. S0...S0), was reached by input 0
        decision_bit = (d_ptr->w[0] >> (endstate / 2)) & 1;
    } else { // Current state is an odd state (e.g. S0...S1), was reached by input 1
        decision_bit = (d_ptr->w[1] >> (endstate / 2)) & 1;
    }
    
    // The decision bit (0 or 1) indicates whether the upper (0) or lower (1) path in the butterfly was chosen.
    // This decision bit is the most significant bit of the *previous* state.
    // The decoded data bit is this decision bit.
    endstate = (endstate >> 1) | (decision_bit << 5); // K-1 = 6. For K=7, state is 6 bits. MSB is bit 5.

    // Store the decoded bit (decision_bit)
    // nbits-1-k gives current bit index from 0 (MSB) to nbits-1 (LSB)
    if (decision_bit) {
        data[(nbits - 1 - k) >> 3] |= (1 << (7 - ((nbits - 1 - k) % 8)));
    } else {
        data[(nbits - 1 - k) >> 3] &= ~(1 << (7 - ((nbits - 1 - k) % 8)));
    }
     // The original code's shorthand: data[byte_idx] = (unsigned char)endstate
     // This works if endstate correctly accumulates the byte.
     // The explicit bit setting above is clearer for MSB-first packing.
     // If using the shorthand, ensure data bytes are cleared first.
     // For this version, let's use explicit bit setting after clearing the data buffer.
  }
   // The above loop fills bits from MSB of stream to LSB.
   // If data array wasn't cleared, it should be before calling this.
   // A common pattern is to clear data for (nbits+7)/8 bytes.
   // For simplicity, assuming data is pre-cleared or this is the intended fill pattern.

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
