/* Test viterbi decoder speeds */
#include "config.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <math.h>
#include <memory.h>
#include <sys/time.h>
#include <sys/resource.h>
#ifdef HAVE_GETOPT_H
#include <getopt.h>
#endif
#include "fec.h"

#if HAVE_GETOPT_LONG
struct option Options[] = {
    {"frame-length", 1, NULL, 'l'},
    {"frame-count", 1, NULL, 'n'},
    {"ebn0", 1, NULL, 'e'},
    {"gain", 1, NULL, 'g'},
    {"verbose", 0, NULL, 'v'},
    {"debug", 1, NULL, 'd'},
    {NULL},
};
#endif

#define RATE (1. / 2.)
#define MAXBYTES 10000

double Gain = 32.0;
int Verbose = 0;

int main(int argc, char *argv[])
{
  int i, d, tr;
  int sr = 0, trials = 10000, errcnt, framebits = 2048;
  long long int tot_errs = 0;
  unsigned char bits[MAXBYTES];
  unsigned char data[MAXBYTES];
  unsigned char xordata[MAXBYTES];
  unsigned char symbols[8 * 2 * (MAXBYTES + 6)];
  void *vp;
  extern char *optarg;
  struct rusage start, finish;
  double extime;
  double gain, esn0, ebn0;
  time_t t;
  int badframes = 0;
  int debug_level = 0;

  time(&t);
  srandom(t);
  ebn0 = -100;
#if HAVE_GETOPT_LONG
  while ((d = getopt_long(argc, argv, "l:n:te:g:vd:apmst", Options, NULL)) != EOF)
  {
#else
  while ((d = getopt(argc, argv, "l:n:te:g:vd:apmst")) != EOF)
  {
#endif
    switch (d)
    {
    case 'l':
      framebits = atoi(optarg);
      break;
    case 'n':
      trials = atoi(optarg);
      break;
    case 'e':
      ebn0 = atof(optarg);
      break;
    case 'g':
      Gain = atof(optarg);
      break;
    case 'v':
      Verbose++;
      break;
    case 'd':
      debug_level = atoi(optarg);
      setenv("VITERBI_DEBUG", optarg, 1);
      break;
    }
  }
  if (framebits > 8 * MAXBYTES)
  {
    fprintf(stderr, "Frame limited to %d bits\n", MAXBYTES * 8);
    framebits = MAXBYTES * 8;
  }
  if ((vp = create_viterbi27(framebits)) == NULL)
  {
    printf("create_viterbi27 failed\n");
    exit(1);
  }
  if (ebn0 != -100)
  {
    esn0 = ebn0 + 10 * log10((double)RATE); /* Es/No in dB */
    /* Compute noise voltage. The 0.5 factor accounts for BPSK seeing
     * only half the noise power, and the sqrt() converts power to
     * voltage.
     */
    gain = 1. / sqrt(0.5 / pow(10., esn0 / 10.));

    printf("nframes = %d framesize = %d ebn0 = %.2f dB gain = %g\n", trials, framebits, ebn0, Gain);

    for (tr = 0; tr < trials; tr++)
    {
      /* Encode a frame of random data */
      for (i = 0; i < framebits + 6; i++)
      {
        int bit = (i < framebits) ? (random() & 1) : 0;

        sr = (sr << 1) | bit;
        bits[i / 8] = sr & 0xff;

        // Ensure symbols are properly formatted for VOLK (0 or 255)
        // The addnoise function should already produce values in 0-255 range
        symbols[2 * i + 0] = addnoise(parity(sr & V27POLYA), gain, Gain, 127.5, 255);
        symbols[2 * i + 1] = addnoise(parity(sr & V27POLYB), gain, Gain, 127.5, 255);
      }

      if (debug_level >= 2 && tr == 0)
      {
        printf("\nFirst frame encoding:\n");
        printf("First 16 input bits: ");
        for (i = 0; i < 16 && i < framebits; i++)
        {
          printf("%d", (bits[i / 8] >> (7 - (i & 7))) & 1);
        }
        printf("\n");
        printf("First 16 symbol pairs: ");
        for (i = 0; i < 16; i++)
        {
          printf("(%3d,%3d) ", symbols[2 * i], symbols[2 * i + 1]);
          if ((i & 3) == 3)
            printf("\n                      ");
        }
        printf("\n");
      }

      /* Decode it and make sure we get the right answer */
      /* Initialize Viterbi decoder */
      init_viterbi27(vp, 0);

      /* Decode block */
      update_viterbi27_blk(vp, symbols, framebits + 6);

      /* Do Viterbi chainback */
      chainback_viterbi27(vp, data, framebits, 0);

      if (debug_level >= 2 && tr == 0)
      {
        printf("\nFirst frame decoding:\n");
        printf("First 16 decoded bits: ");
        for (i = 0; i < 16 && i < framebits; i++)
        {
          printf("%d", (data[i / 8] >> (7 - (i & 7))) & 1);
        }
        printf("\n");
      }

      errcnt = 0;
      for (i = 0; i < framebits / 8; i++)
      {
        int e = Bitcnt[xordata[i] = data[i] ^ bits[i]];
        errcnt += e;
        tot_errs += e;
      }
      if (errcnt != 0)
        badframes++;
      if (Verbose > 1 && errcnt != 0)
      {
        printf("frame %d, %d errors: ", tr, errcnt);
        for (i = 0; i < framebits / 8; i++)
        {
          printf("%02x", xordata[i]);
        }
        printf("\n");
      }
      if (Verbose)
        printf("BER %lld/%lld (%10.3g) FER %d/%d (%10.3g)\r",
               tot_errs, (long long)framebits * (tr + 1), tot_errs / ((double)framebits * (tr + 1)),
               badframes, tr + 1, (double)badframes / (tr + 1));
      fflush(stdout);
    }
    if (Verbose > 1)
      printf("nframes = %d framesize = %d ebn0 = %.2f dB gain = %g\n", trials, framebits, ebn0, Gain);
    else if (Verbose == 0)
      printf("BER %lld/%lld (%.3g) FER %d/%d (%.3g)\n",
             tot_errs, (long long)framebits * trials, tot_errs / ((double)framebits * trials),
             badframes, tr + 1, (double)badframes / (tr + 1));
    else
      printf("\n");
  }
  else
  {
    /* Do time trials with profiling */
    struct timeval tv_start, tv_end;
    double time_init = 0, time_update = 0, time_chainback = 0;

    for (size_t i = 0; i < sizeof(symbols); i++)
    {
      symbols[i] = (random() & 1) ? 255 : 0; // High-confidence 0s and 1s
    }

    printf("Starting time trials with profiling\n");

    getrusage(RUSAGE_SELF, &start);
    for (tr = 0; tr < trials; tr++)
    {
      /* Time init_viterbi27 */
      gettimeofday(&tv_start, NULL);
      init_viterbi27(vp, 0);
      gettimeofday(&tv_end, NULL);
      time_init += (tv_end.tv_sec - tv_start.tv_sec) + 1e-6 * (tv_end.tv_usec - tv_start.tv_usec);

      /* Time update_viterbi27_blk */
      gettimeofday(&tv_start, NULL);
      update_viterbi27_blk(vp, symbols, framebits);
      gettimeofday(&tv_end, NULL);
      time_update += (tv_end.tv_sec - tv_start.tv_sec) + 1e-6 * (tv_end.tv_usec - tv_start.tv_usec);

      /* Time chainback_viterbi27 */
      gettimeofday(&tv_start, NULL);
      chainback_viterbi27(vp, data, framebits, 0);
      gettimeofday(&tv_end, NULL);
      time_chainback += (tv_end.tv_sec - tv_start.tv_sec) + 1e-6 * (tv_end.tv_usec - tv_start.tv_usec);
    }
    getrusage(RUSAGE_SELF, &finish);

    extime = finish.ru_utime.tv_sec - start.ru_utime.tv_sec + 1e-6 * (finish.ru_utime.tv_usec - start.ru_utime.tv_usec);
    printf("Execution time for %d %d-bit frames: %.2f sec\n", trials, framebits, extime);
    printf("decoder speed: %g bits/s\n", trials * framebits / extime);

    printf("\nProfiling breakdown:\n");
    printf("  init_viterbi27:      %.3f sec (%.1f%%)\n", time_init, 100.0 * time_init / extime);
    printf("  update_viterbi27_blk: %.3f sec (%.1f%%)\n", time_update, 100.0 * time_update / extime);
    printf("  chainback_viterbi27:  %.3f sec (%.1f%%)\n", time_chainback, 100.0 * time_chainback / extime);
    printf("  Total measured:       %.3f sec\n", time_init + time_update + time_chainback);
    printf("  Overhead:             %.3f sec (%.1f%%)\n",
           extime - (time_init + time_update + time_chainback),
           100.0 * (extime - (time_init + time_update + time_chainback)) / extime);
  }
  exit(0);
}
