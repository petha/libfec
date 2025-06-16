/* Detailed profiling of viterbi27 decoder */
#include "config.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <sys/resource.h>
#include "fec.h"

#define TRIALS 10000  // Same as vtest27
#define FRAMEBITS 2048

static inline double get_time() {
    struct rusage ru;
    getrusage(RUSAGE_SELF, &ru);
    return ru.ru_utime.tv_sec + 1e-6 * ru.ru_utime.tv_usec;
}

int main() {
    void *vp;
    unsigned char symbols[8*2*(FRAMEBITS+6)];
    unsigned char data[FRAMEBITS/8];
    double t1, t2, t3, t4;
    double time_create, time_total, time_update = 0, time_chainback = 0;
    struct timeval tv1, tv2;
    
    // Create decoder and measure time
    t1 = get_time();
    vp = create_viterbi27(FRAMEBITS);
    t2 = get_time();
    time_create = t2 - t1;
    
    if (!vp) {
        printf("Failed to create decoder\n");
        return 1;
    }
    
    // Initialize symbols
    memset(symbols, 127, sizeof(symbols));
    
    printf("Profiling Viterbi27 decoder (%d trials, %d bits/frame)\n", TRIALS, FRAMEBITS);
    
    // Warm up cache
    for (int i = 0; i < 10; i++) {
        init_viterbi27(vp, 0);
        update_viterbi27_blk(vp, symbols, FRAMEBITS + 6);
        chainback_viterbi27(vp, data, FRAMEBITS, 0);
    }
    
    // Main timing loop - measure each component separately
    // This adds overhead but shows the breakdown
    t1 = get_time();
    
    // Measure update time
    for (int i = 0; i < TRIALS; i++) {
        init_viterbi27(vp, 0);
        gettimeofday(&tv1, NULL);
        update_viterbi27_blk(vp, symbols, FRAMEBITS);
        gettimeofday(&tv2, NULL);
        time_update += (tv2.tv_sec - tv1.tv_sec) + 1e-6 * (tv2.tv_usec - tv1.tv_usec);
    }
    
    // Measure chainback time  
    for (int i = 0; i < TRIALS; i++) {
        init_viterbi27(vp, 0);
        update_viterbi27_blk(vp, symbols, FRAMEBITS);
        gettimeofday(&tv1, NULL);
        chainback_viterbi27(vp, data, FRAMEBITS, 0);
        gettimeofday(&tv2, NULL);
        time_chainback += (tv2.tv_sec - tv1.tv_sec) + 1e-6 * (tv2.tv_usec - tv1.tv_usec);
    }
    
    t2 = get_time();
    
    // Now measure total time without profiling overhead
    t3 = get_time();
    for (int i = 0; i < TRIALS; i++) {
        init_viterbi27(vp, 0);
        update_viterbi27_blk(vp, symbols, FRAMEBITS);
        chainback_viterbi27(vp, data, FRAMEBITS, 0);
    }
    t4 = get_time();
    time_total = t4 - t3;
    
    printf("\nResults (with profiling overhead):\n");
    printf("  VOLK update:  %.3f ms/frame (%.1f%%)\n", 
           1000.0 * time_update / TRIALS, 
           100.0 * time_update / (time_update + time_chainback));
    printf("  Chainback:    %.3f ms/frame (%.1f%%)\n", 
           1000.0 * time_chainback / TRIALS,
           100.0 * time_chainback / (time_update + time_chainback));
    printf("  Total:        %.3f ms/frame\n", 
           1000.0 * (time_update + time_chainback) / TRIALS);
    
    printf("\nResults (without profiling overhead):\n");
    printf("  Total time:   %.3f sec for %d frames\n", time_total, TRIALS);
    printf("  Speed:        %.2f Mbps\n", FRAMEBITS * TRIALS / time_total / 1e6);
    printf("  CPU time measurement overhead: %.1f%%\n", 
           100.0 * ((t2 - t1) - time_total) / time_total);
    
    printf("\nMemory access analysis:\n");
    printf("  Metrics size: 2x64 = 128 bytes (fits in L1 cache)\n");
    printf("  Decision array size: %d bytes\n", (FRAMEBITS + 6) * 8);
    printf("  Symbol array size: %d bytes\n", 2 * (FRAMEBITS + 6));
    printf("  Output size: %d bytes\n", FRAMEBITS / 8);
    printf("  Total working set: ~%d KB\n", 
           (128 + (FRAMEBITS + 6) * 8 + 2 * (FRAMEBITS + 6) + FRAMEBITS / 8) / 1024);
    
    printf("\nCache analysis:\n");
    printf("  L1 cache typical: 32-64 KB (working set fits)\n");
    printf("  Chainback access pattern: semi-random within 8-byte groups\n");
    printf("  Decision byte reuse: low (each byte used ~8 times)\n");
    
    delete_viterbi27(vp);
    return 0;
}
