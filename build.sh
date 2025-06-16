#!/bin/sh
# Simple build script for libfec

set -e

echo "Building libfec..."

# Generate configure if needed
if [ ! -f configure ]; then
    echo "Running autoreconf..."
    autoreconf -fiv || {
        echo "autoreconf failed. Trying with simpler configure.ac..."
        # Fall back to simple autoconf if autoreconf fails
        autoconf
    }
fi

# Configure
echo "Configuring..."
./configure "$@"

# Build
echo "Building..."
make -j$(nproc 2>/dev/null || sysctl -n hw.ncpu 2>/dev/null || echo 1)

echo "Build complete. Run 'make test' to test, 'sudo make install' to install."
