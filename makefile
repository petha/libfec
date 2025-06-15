# Makefile prototype for configure
# Copyright 2004 Phil Karn, KA9Q
# May be used under the terms of the GNU Lesser General Public License (LGPL)

# makefile.  Generated from makefile.in by configure.
srcdir = .
prefix = /usr/local
exec_prefix = ${prefix}
datarootdir = ${prefix}/share

CC=clang
AR = ar
RANLIB = ranlib
LDFLAGS = 

LIBS =  fec.o sim.o \
	viterbi27_port.o viterbi29_port.o viterbi39_port.o viterbi615_port.o \
	encode_rs_char.o encode_rs_int.o encode_rs_8.o \
	decode_rs_char.o decode_rs_int.o decode_rs_8.o \
	init_rs_char.o init_rs_int.o \
	ccsds_tab.o encode_rs_ccsds.o decode_rs_ccsds.o ccsds_tal.o \
	dotprod_port.o peakval_port.o sumsq_port.o

CFLAGS=-g -O2 -I. -fPIC -Wall -O3 -march=native -mtune=native -ffast-math -funroll-loops

SHARED_LIB=libfec.dylib

all: libfec.a $(SHARED_LIB)

test: vtest27 vtest29 vtest39 vtest615 rstest  rs_speedtest
	@echo "Running tests..."
	./vtest27
	./vtest29
	./vtest39
	./vtest615
	./rstest
	./rs_speedtest
	@echo "Tests finished."

install: all
	mkdir -p $(DESTDIR)${exec_prefix}/lib
	install -m 644 -p $(SHARED_LIB) libfec.a $(DESTDIR)${exec_prefix}/lib
	
	mkdir -p $(DESTDIR)${prefix}/include
	install -m 644 -p fec.h $(DESTDIR)${prefix}/include
	mkdir -p $(DESTDIR)${datarootdir}/man/man3
	install -m 644 -p simd-viterbi.3 rs.3 dsp.3 $(DESTDIR)${datarootdir}/man/man3

libfec.a: $(LIBS)
	$(AR) rv $@ $^
	$(RANLIB) $@

# For Darwin (macOS)
libfec.dylib: $(LIBS)
	$(CC) $(LDFLAGS) -dynamiclib -install_name ${exec_prefix}/lib/$@ -o $@ $^

# For Linux et al
libfec.so: $(LIBS)
	$(CC) $(LDFLAGS) -shared -Xlinker -soname=$@ -o $@ -Wl,-whole-archive $^ -Wl,-no-whole-archive -lc

# CCSDS table generation
ccsds_tab.o: ccsds_tab.c fec.h
	$(CC) $(CFLAGS) -c -o $@ $<

ccsds_tab.c: gen_ccsds
	./gen_ccsds > $@

gen_ccsds: gen_ccsds.o init_rs_char_local.o
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^

ccsds_tal.o: ccsds_tal.c fec.h
	$(CC) $(CFLAGS) -c -o $@ $<

ccsds_tal.c: gen_ccsds_tal
	./gen_ccsds_tal > $@

gen_ccsds_tal: gen_ccsds_tal.o
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^

# Test executables
vtest27: vtest27.o libfec.a
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^ -lm

vtest29: vtest29.o libfec.a
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^ -lm

vtest39: vtest39.o libfec.a
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^ -lm

vtest615: vtest615.o libfec.a
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^ -lm

rstest: rstest.o exercise_rs_char.o exercise_rs_int.o exercise_rs_8.o exercise_rs_ccsds.o libfec.a
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^


rs_speedtest: rs_speedtest.o libfec.a
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^

# Generic rule for compiling .c to .o
# Assumes most .c files include fec.h or their necessary headers.
# For files with more specific dependencies (like RS exercise files or init_rs_char_local),
# you might need more specific rules if this generic one doesn't trigger rebuilds correctly.
%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

# Specific rule for init_rs_char_local.o if it doesn't depend on fec.h in the same way
init_rs_char_local.o: init_rs_char_local.c
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f *.o *.a $(SHARED_LIB) \
	vtest27 vtest29 vtest39 vtest615 rstest rs_speedtest \
	gen_ccsds gen_ccsds_tal ccsds_tab.c ccsds_tal.c core
	rm -rf autom4te.cache

distclean: clean
	rm -f config.log config.cache config.status config.h makefile TAGS tags
