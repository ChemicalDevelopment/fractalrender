#!/bin/sh
###
#
# compile_static.sh -- compile a static/distributable version of fractalrender
#
###


FDIR=$PWD
TARFILE_BN= FILEHERE
TARFILE="$PWD/$TARFILE_BN.tar.gz"
BDIR=/tmp/builddir

PREFIX="$BDIR/out"
mkdir -p $PREFIX


ZLIB_TARFILE_BN=zlib-1.2.11
ZLIB_TARFILE=https://zlib.net/$ZLIB_TARFILE_BN.tar.gz

PNG_TARFILE_BN=libpng-
PNG_TARFILE=https://zlib.net/$ZLIB_TARFILE_BN.tar.gz

CARGS_TARFILE_BN_VERSION=0.2.1
CARGS_TARFILE_BN=cargs-$CARGS_TARFILE_BN_VERSION
CARGS_TARFILE=https://github.com/ChemicalDevelopment/cargs/releases/download/$CARGS_TARFILE_BN_VERSION/$CARGS_TARFILE_BN.tar.gz

GMP_TARFILE_BN=gmp-6.1.2
GMP_TARFILE="https://gmplib.org/download/gmp/$GMP_TARFILE_BN.tar.xz"


LIBMPI=/usr/local/lib/libmpi.a


###



mkdir -p $BDIR

cd $BDIR


curl -L $CARGS_TARFILE > $CARGS_TARFILE_BN.tar.gz

tar xfv $CARGS_TARFILE_BN.tar.gz
cd $CARGS_TARFILE_BN

./configure --prefix=$PREFIX
make -j8
make install

cd ..

curl -L $ZLIB_TARFILE > $ZLIB_TARFILE_BN.tar.gz

tar xfv $ZLIB_TARFILE_BN.tar.gz
cd $ZLIB_TARFILE_BN

./configure --prefix=$PREFIX --static --archs="-arch i386 -arch x86_64"
make -j8
make install




curl -L $GMP_TARFILE > $GMP_TARFILE_BN.tar.gz

tar xfv $GMP_TARFILE_BN.tar.gz
cd $GMP_TARFILE_BN

./configure --prefix=$PREFIX --disable-assembly --enable-static --disable-shared
make -j8
make install





tar xfv $TARFILE

cd $TARFILE_BN

./configure CLFAGS="-I$PREFIX/include" LDFLAGS="-L$PREFIX/lib  $LIBMPI" LIBS="-lz" --with-cargs --with-gmp --with-png --with-opencl --with-mpi
