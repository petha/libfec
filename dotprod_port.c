/* 16-bit signed integer dot product
 * Portable C version
 * Copyright 2004 Phil Karn
 * May be used under the terms of the GNU Lesser General Public License (LGPL)
 */
#include <stdlib.h>
#include "fec.h"

struct dotprod {
  int len; /* Number of coefficients */
  // Consider making coeffs restrict if the struct itself isn't aliased in problematic ways
  // For now, we'll apply restrict to the pointer when used in the function.
  signed short *coeffs;
};

/* Create and return a descriptor for use with the dot product function */
// No major changes needed here for dotprod optimization, but ensure coeffs is not NULL before use.
void *initdp(signed short coeffs[],int len){
  struct dotprod *dp;
  int j;

  if(len == 0 || coeffs == NULL) // Added NULL check for coeffs
    return NULL;

  dp = (struct dotprod *)calloc(1,sizeof(struct dotprod));
  if (dp == NULL) return NULL; // Check calloc result
  dp->len = len;

  dp->coeffs = (signed short *)calloc(len,sizeof(signed short));
  if (dp->coeffs == NULL) { // Check calloc result
      free(dp);
      return NULL;
  }
  for(j=0;j<len;j++)
    dp->coeffs[j] = coeffs[j];
  return (void *)dp;
}


/* Free a dot product descriptor created earlier */
void freedp(void *p){
  struct dotprod *dp = (struct dotprod *)p;

  if(dp == NULL) return; // Basic null check

  if(dp->coeffs != NULL)
      free(dp->coeffs);
  free(dp);
}

/* Compute a dot product given a descriptor and an input array
 * The length is taken from the descriptor
 */
long dotprod(void * restrict p, signed short a[restrict]){ // Added restrict to p and a
  // The 'restrict' on 'a' indicates that for the scope of this function,
  // 'a' is the sole initial means of accessing the objects it points to.

  if (p == NULL || a == NULL) return 0; // Basic null checks

  // Cast 'p' to the struct type, also with restrict if appropriate for 'dp' itself.
  // However, the primary benefit comes from restricting the array pointers.
  struct dotprod * restrict dp = (struct dotprod *)p;

  long corr;
  int i;

  // Cache dp->len and dp->coeffs in local variables.
  // 'const' for len as it won't change within this function.
  const int len = dp->len;
  // Use 'restrict' for the local pointer to the coefficients.
  signed short * restrict coeffs_ptr = dp->coeffs;

  if (coeffs_ptr == NULL || len <= 0) return 0; // Check after getting local copies

  corr = 0;

  // Optional: Add Clang-specific pragmas if auto-vectorization/unrolling isn't happening
  // #pragma clang loop vectorize(enable)
  // #pragma clang loop unroll(enable) // or unroll_count(N)

  for(i=0; i<len; i++){
    // The compiler now has stronger guarantees that 'a[i]' and 'coeffs_ptr[i]'
    // do not alias each other in unexpected ways, and that accesses
    // through 'a' and 'coeffs_ptr' are the primary ways these memory
    // locations are touched.
    corr += (long)a[i] * coeffs_ptr[i];
  }
  return corr;
}

