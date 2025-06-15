/* User include file for libfec
 * Copyright 2004, Phil Karn, KA9Q
 * May be used under the terms of the GNU Lesser General Public License (LGPL)
 */

#ifndef _FEC_H_
#define _FEC_H_

/* r=1/2 k=7 convolutional encoder polynomials */
#define	V27POLYA	0x6d
#define	V27POLYB	0x4f

void *create_viterbi27(int len);
void set_viterbi27_polynomial(int polys[2]);
int init_viterbi27(void *vp,int starting_state);
int update_viterbi27_blk(void *vp,unsigned char sym[],int npairs);
int chainback_viterbi27(void *vp, unsigned char *data,unsigned int nbits,unsigned int endstate);
void delete_viterbi27(void *vp);

/* r=1/2 k=9 convolutional encoder polynomials */
#define	V29POLYA	0x1af
#define	V29POLYB	0x11d

void *create_viterbi29(int len);
void set_viterbi29_polynomial(int polys[2]);
int init_viterbi29(void *vp,int starting_state);
int update_viterbi29_blk(void *vp,unsigned char syms[],int nbits);
int chainback_viterbi29(void *vp, unsigned char *data,unsigned int nbits,unsigned int endstate);
void delete_viterbi29(void *vp);

/* r=1/3 k=9 convolutional encoder polynomials */
#define	V39POLYA	0x1ed
#define	V39POLYB	0x19b
#define	V39POLYC	0x127

void *create_viterbi39(int len);
void set_viterbi39_polynomial(int polys[3]);
int init_viterbi39(void *vp,int starting_state);
int update_viterbi39_blk(void *vp,unsigned char syms[],int nbits);
int chainback_viterbi39(void *vp, unsigned char *data,unsigned int nbits,unsigned int endstate);
void delete_viterbi39(void *vp);

/* r=1/6 k=15 Cassini convolutional encoder polynomials */
#define	V615POLYA	042631
#define	V615POLYB	047245
#define V615POLYC       056507
#define V615POLYD       073363
#define V615POLYE       077267
#define V615POLYF       064537

void *create_viterbi615(int len);
void set_viterbi615_polynomial(int polys[6]);
int init_viterbi615(void *vp,int starting_state);
int update_viterbi615_blk(void *vp,unsigned char *syms,int nbits);
int chainback_viterbi615(void *vp, unsigned char *data,unsigned int nbits,unsigned int endstate);
void delete_viterbi615(void *vp);

/* General purpose RS codec, 8-bit symbols */
void encode_rs_char(void *rs,unsigned char *data,unsigned char *parity);
int decode_rs_char(void *rs,unsigned char *data,int *eras_pos,
           int no_eras);
void *init_rs_char(int symsize,int gfpoly,
           int fcr,int prim,int nroots,
           int pad);
void free_rs_char(void *rs);

/* General purpose RS codec, integer symbols */
void encode_rs_int(void *rs,int *data,int *parity);
int decode_rs_int(void *rs,int *data,int *eras_pos,int no_eras);
void *init_rs_int(int symsize,int gfpoly,int fcr,
          int prim,int nroots,int pad);
void free_rs_int(void *rs);

/* CCSDS standard (255,223) RS codec with conventional symbol representation */
void encode_rs_8(unsigned char *data,unsigned char *parity,int pad);
int decode_rs_8(unsigned char *data,int *eras_pos,int no_eras,int pad);

/* CCSDS standard (255,223) RS codec with dual-basis symbol representation */
void encode_rs_ccsds(unsigned char *data,unsigned char *parity,int pad);
int decode_rs_ccsds(unsigned char *data,int *eras_pos,int no_eras,int pad);

/* Tables to map from conventional->dual (Taltab) and
 * dual->conventional (Tal1tab) bases
 */
extern unsigned char Taltab[],Tal1tab[];

/* Determine parity of argument: 1 = odd, 0 = even */
// Optimized parityb using __builtin_parity.
// Assumes a GCC-compatible compiler.
static inline int parityb(unsigned char x) {
    // __builtin_parity takes an unsigned int; x will be promoted.
    return __builtin_parity(x);
}

// Optimized parity using __builtin_parity.
// Assumes a GCC-compatible compiler.
static inline int parity(int x) {
    return __builtin_parity((unsigned int)x);
}

/* Useful utilities for simulation */
double normal_rand(double mean, double std_dev);
unsigned char addnoise(int sym,double amp,double gain,double offset,int clip);

extern int Bitcnt[];

/* Dot product functions */
void *initdp(signed short coeffs[],int len);
void freedp(void *dp);
long dotprod(void *dp,signed short a[]);

/* Sum of squares - accepts signed shorts, produces unsigned long long */
unsigned long long sumsq(signed short *in,int cnt);

#endif /* _FEC_H_ */