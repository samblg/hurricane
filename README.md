#Hurricane

##Introduction
Hurricane is a distributed computing framework developing by C++.

##Installation
###Dependencies
Hurricane use the library called Meshy which is including in deps directory, you need to build it firstly.
You can use gmake and kake to build Hurricane, please reference to the manual to learn how to build Hurricane

### Build Hurricane by Makefile
We provide the Makefile for convinience of compiling.
You can use make to build Hurricane

make

### Build Hurricane by kake
#### Introduction to kake
Kake is a building tool.

#### Dependencies
Kake is written in python3. It depends on PyYAML, so you need to install Python, libyaml and PyYAML for Python.

If you use Ubuntu, you can use apt-get to install all packages.

    sudo apt-get install libyaml-0-2 libyaml-dev
    sudo apt-get install python3 libpython3-dev python3-pip

Then use pip to install PyYAML.

    sudo pip3 install PyYAML

#### Install kake
It is easy to install kake.

You can use git clone to clone the git code from online repository.
If we install kake in ~/apps, you can execute following commands.

    cd ~/apps
    git clone https://git.oschina.net/kinuxroot/kake.git 

Then edit the ~/.bashrc to append following code.

    export KAKE_HOME=~/apps/kake

At last, don't forget to append the kake/bin to PATH variable, or create a symbolic link to kake under one path in PATH variable.

If you want to append kake/bin to PATH variable, you can append following to ~/.bashrc

    export PATH="${PATH}:${KAKE_HOME}/bin"

#### Verify installation
You can execute kake under one directory without Kakefile, if it print following contents, then the kake is installed successfully.

    Project file not exists.

You can execute `kake` to build Hurricane.

    kake
