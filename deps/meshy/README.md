# Meshy Network Engine

##Brief Introduction
A full-featured, high-performance and cross-platform event loop and network library that is based on C++, which also features REST service development kit.

##Installation
###Dependencies
Meshy supports to be built by Makefile (gmake) and Kakefile (Kake). Refer to the section "Build with Kake" for more details on how to build and install Meshy.


### Build Meshy using Makefile
For the sake of convenience of Linux users' usage, we provided Makefile to build Meshy.
It's very simple to build with Makefile, simply type the following command:

make

### Build Meshy using Kake
#### Brief Introduction to Kake
Kake is a building system following the the "convention over configuration" paradigm

#### Dependencies
Like some of build systems (e.g. scons), Kake build system is written in python3 and using PyYAML library, thus we need to install Python, libyaml and PyYAML.

Install the following 3rd-party packages if you are using Ubuntu:

    sudo apt-get install libyaml-0-2 libyaml-dev
    sudo apt-get install python3 libpython3-dev python3-pip

Then you may use pip to install PyYAML:

    sudo pip3 install PyYAML

#### Install Kake
It's very simple to install Kake build system into your environment, you just have to clone it:
Assume you are going to install Kake build system into ~/apps:

    cd ~/apps
    git clone https://git.oschina.net/kinuxroot/kake.git 

Then add one line in ~/.bashrc with your favorite text editor:

    export KAKE_HOME=~/apps/kake

Finally, add the path kake/bin right after PATH:

    export PATH="${PATH}:${KAKE_HOME}/bin"

#### Verify Installation
If you type in kake in a folder without Kakefile, it will output:

    Project file not exists.

This indicates that Kake build system is ready to use in your environment, if you want to use it, just simply type:

    kake
