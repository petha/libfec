# libfec - Forward Error Correction Library

A high-performance forward error correction library with VOLK acceleration support.

## Features

- Viterbi decoders (K=7, K=9, constraint lengths)
- Reed-Solomon encoder/decoder
- SIMD acceleration via VOLK library
- Portable C fallback implementations
- Optimized for modern processors

## Building from Source

### Prerequisites

- C compiler (GCC or Clang)
- autoconf
- pkg-config
- VOLK library (optional, for SIMD acceleration)

### Quick Build

```bash
./build.sh
make test
sudo make install
```

### Manual Build

```bash
autoconf
./configure
make
make test
sudo make install
```

### Configure Options

- `--prefix=/path` - Installation prefix (default: /usr/local)
- `--enable-shared` - Build shared library
- `--disable-static` - Don't build static library

## Performance

With VOLK acceleration, the Viterbi decoder achieves:
- K=7, r=1/2: ~57 Mbps on M1 Max

## License

GNU Lesser General Public License (LGPL)
