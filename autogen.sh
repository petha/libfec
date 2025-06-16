#!/bin/sh
# Bootstrap script for libfec

set -e

echo "Bootstrapping libfec build system..."

# Create m4 directory if it doesn't exist
mkdir -p m4

# Run autotools
echo "Running autoreconf..."
autoreconf --force --install --verbose

echo "Bootstrap complete. You can now run:"
echo "  ./configure"
echo "  make"
echo "  make check"
echo "  sudo make install"
