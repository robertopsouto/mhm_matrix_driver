#!/bin/bash

# Get latest development version from github and run dev scripts
#git clone --depth=1 https://github.com/ralna/spral.git
#cd spral
#./autogen.sh

# Build and install library
#BUILDDIR=build; mkdir $BUILDDIR; cd $BUILDDIR
../configure  \
CPPFLAGS="-I/usr/local/cuda/include" \
 --prefix=/home/rpsouto/usr/local/spral/v2016.09.23 \
 --with-metis="-L/home/rpsouto/usr/local/metis/5.1.0/lib -lmetis" 
# --with-metis="-L/home/rpsouto/programming/libraries/sparse/fill-in/metis/4.0.3 -lmetis" \
# --with-blas="-L/opt/intel/mkl/lib/intel64 -lmkl_gf_lp64 -lmkl_gnu_thread -lmkl_core" 
#make
#sudo make install # Optional

# Link against library
#cd /path/to/your/code
#gfortran -o myprog myobj.o -lspral -lmetis -lblas

