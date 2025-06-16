# Automake file for libfec
ACLOCAL_AMFLAGS = -I m4

# Library version (current:revision:age)
# See: https://www.gnu.org/software/libtool/manual/html_node/Updating-version-info.html
lib_LTLIBRARIES = libfec.la
libfec_la_LDFLAGS = -version-info 3:0:0

# Include directories
AM_CPPFLAGS = -I$(srcdir) $(VOLK_CFLAGS)

# Compiler flags
AM_CFLAGS = -Wall -O3 -fPIC \
            -march=native -mtune=native \
            -ffast-math -funroll-loops

# Library sources
libfec_la_SOURCES = \
    fec.c \
    sim.c \
    viterbi27_port.c \
    viterbi29_port.c \
    viterbi39_port.c \
    viterbi615_port.c \
    encode_rs_char.c \
    encode_rs_int.c \
    encode_rs_8.c \
    decode_rs_char.c \
    decode_rs_int.c \
    decode_rs_8.c \
    init_rs_char.c \
    init_rs_int.c \
    encode_rs_ccsds.c \
    decode_rs_ccsds.c \
    dotprod_port.c \
    peakval_port.c \
    sumsq_port.c

# Add VOLK libraries if available
libfec_la_LIBADD = $(VOLK_LIBS)

# Headers to install
include_HEADERS = fec.h

# Man pages
man3_MANS = simd-viterbi.3 rs.3 dsp.3

# Test programs
check_PROGRAMS = vtest27 vtest29 vtest39 vtest615 rstest rs_speedtest
TESTS = $(check_PROGRAMS)

vtest27_SOURCES = vtest27.c
vtest27_LDADD = libfec.la -lm $(VOLK_LIBS)

vtest29_SOURCES = vtest29.c
vtest29_LDADD = libfec.la -lm $(VOLK_LIBS)

vtest39_SOURCES = vtest39.c
vtest39_LDADD = libfec.la -lm $(VOLK_LIBS)

vtest615_SOURCES = vtest615.c
vtest615_LDADD = libfec.la -lm $(VOLK_LIBS)

rstest_SOURCES = rstest.c exercise_rs_char.c exercise_rs_int.c \
                 exercise_rs_8.c exercise_rs_ccsds.c
rstest_LDADD = libfec.la $(VOLK_LIBS)

rs_speedtest_SOURCES = rs_speedtest.c
rs_speedtest_LDADD = libfec.la $(VOLK_LIBS)

# Extra programs (not part of check)
noinst_PROGRAMS = profile_viterbi27 gen_ccsds gen_ccsds_tal

profile_viterbi27_SOURCES = profile_viterbi27.c
profile_viterbi27_LDADD = libfec.la -lm $(VOLK_LIBS)

gen_ccsds_SOURCES = gen_ccsds.c init_rs_char_local.c
gen_ccsds_tal_SOURCES = gen_ccsds_tal.c

# Generated sources
nodist_libfec_la_SOURCES = ccsds_tab.c ccsds_tal.c

# Build generated sources
ccsds_tab.c: gen_ccsds
	./gen_ccsds > $@

ccsds_tal.c: gen_ccsds_tal
	./gen_ccsds_tal > $@

# Clean generated files
CLEANFILES = ccsds_tab.c ccsds_tal.c

# Distribution extras
EXTRA_DIST = \
    README \
    LICENSE \
    exercise_rs.h \
    exercise_rs_bc.h \
    exercise.h \
    fixed.h \
    $(man3_MANS)

# pkg-config file
pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = libfec.pc
