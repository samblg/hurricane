#Kake Build System

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