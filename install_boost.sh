#!/bin/bash
# for further info check https://www.boost.org/doc/libs/1_61_0/doc/html/mpi/getting_started.html#mpi.using

# get lib_boost
wget -O ~/boost_1_71_0.tar.gz 'http://dl.bintray.com/boostorg/release/1.71.0/source/boost_1_71_0.tar.gz'
cd ~/
tar xzvf boost_1_71_0.tar.gz
cd boost_1_71_0/

# get required libraries
echo "Getting required libraries..."
sudo apt-get update
sudo apt-get install build-essential g++ python-dev autotools-dev libicu-dev libbz2-dev

# set install dir
installDir="/usr/local/boost-1.71.0"
./bootstrap.sh --prefix=$installDir

# pipe "using mpi ;" to the config file so that mpi is enabled
user_configFile=`find $PWD -name project-config.jam`
mpicc_exe=`which mpic++` # required incase the mpi path is not in root
echo "using mpi : $mpicc_exe ;" >> $user_configFile

# Build using maximum number of physical cores
n=`cat /proc/cpuinfo | grep "cpu cores" | uniq | awk '{print $NF}'`

# begin install
sudo ./b2 --with=all -j $n cxxflags="-std=c++11" --target=shared,static install

sudo echo "$installDir/lib" >> /etc/ld.so.conf.d/boost-1.71.0.conf
sudo ldconfig -v