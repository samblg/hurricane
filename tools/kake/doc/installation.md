# Installation

## Install Node.js
Because Kake is developmented by using Node.js, you must install Node.js before use kake.
It is recommended to download newest stable version of Node.js from [Node.js homepage](https://nodejs.org/en/).

**Note** If you are chinese user, you can download Node.js from [Node.js Taobao mirror](https://npm.taobao.org/mirrors/node).

### Ubuntu installation
1. Download the tar.gz package of Node.js and copy it to installtion directory(I choose /opt because I prefer installing 3rtparty softwares here)

        cd /opt        
        sudo wget https://nodejs.org/dist/v6.9.2/node-v6.9.2-linux-x64.tar.gz
2. Use tar to unpack the package
   
        sudo tar xzf node-v6.9.2-linux-x64.tar.gz
3. Edit system shell configuration and append PATH environment variable.

        sudo echo 'export PATH="${PATH}:/opt/node-v6.9.2-linux/bin"' > /etc/profile
   or
        
        sudo vim /etc/profile
   and append `export PATH="${PATH}:/opt/node-v6.9.2-linux/bin"` to file.

4. Logout and login
5. Open terminal and test node

        node
   You can press Ctrl+C twice to leave node REPL.
### CentOS installation
1. Download the tar.gz package of Node.js and copy it to installtion directory(I choose /opt because I prefer installing 3rtparty softwares here)

        su root
        cd /opt        
        wget https://nodejs.org/dist/v6.9.2/node-v6.9.2-linux-x64.tar.gz
2. Use tar to unpack the package
   
        tar xzf node-v6.9.2-linux-x64.tar.gz
3. Edit system shell configuration and append PATH environment variable.

        echo 'export PATH="${PATH}:/opt/node-v6.9.2-linux/bin"' > /etc/profile
   or
        
        vim /etc/profile
   and append `export PATH="${PATH}:/opt/node-v6.9.2-linux/bin"` to file.

4. Logout and login
5. Open terminal and test node

        node
   You can press Ctrl+C twice to leave node REPL. 
### Windows installation
1. Download the MSI installer of Node.js.
2. Double click the installer and install Node.js.
3. Enter Node.js installation directory(specified in installer).
4. Enter bin directory
5. Copy bin full path of Node.js and add it to PATH enviroment of Windows.
6. Open cmd.exe and enter `node` to test

## Install cnpm
If you are chinese user, install Node.js packages by npm will be very slow. Fortunately, Taobao has a [mirror site of npm](https://registry.npm.taobao.org/). 

### Ubuntu installation

        sudo npm install -g cnpm --registry=https://registry.npm.taobao.org

### CentOS installation

        su root
        npm install -g cnpm --registry=https://registry.npm.taobao.org

### Windows installation
Open cmd.exe and execute

        npm install -g cnpm --registry=https://registry.npm.taobao.org

## Install kake
**Note** If you are chinese user you can install cnpm and use cnpm instead of npm. Please read section **Install cnpm** to install cnpm.
 
### Ubuntu installation
1. Install git

        sudo apt-get install git
2. Clone kake
        
        git clone https://git.coding.net/kinuxroot/kake.git
3. Enter kake and install dependencies
    
        cd kake
        npm install

4. Add kake path to `PATH` envrionment variable in /etc/profile

### CentOS installation
1. Install git

        su root
        yum install git
2. Clone kake
        
        git clone https://git.coding.net/kinuxroot/kake.git
3. Enter kake and install dependencies
    
        cd kake
        npm install

4. Add kake path to `PATH` envrionment variable in /etc/profile

### Windows installation
1. Install [Git for windows](https://git-for-windows.github.io/)
2. Open explorer and enter a directory.
3. Right click mouse and click `Git Bash Here` of menu.
4. Enter commands in bash:

        git clone https://git.coding.net/kinuxroot/kake.git
5. Enter kake directory and add full path to `PATH` environment variable of Windows.
6. Open cmd.exe, enter kake directory and execute command:

        npm install

## Resources
1. [Node.js 6.9.2 Linux x64(Official)](https://nodejs.org/dist/v6.9.2/node-v6.9.2-linux-x64.tar.gz)
2. [Node.js 6.9.2 Linux x64(Taobao)](https://npm.taobao.org/mirrors/node/latest-v6.x/node-v6.9.2-linux-x64.tar.gz)
3. [Node.js 6.9.2 Windows x86(Official)](https://nodejs.org/dist/v6.9.2/node-v6.9.2-x86.msi)
4. [Node.js 6.9.2 Windows x86(Taobao)](https://npm.taobao.org/mirrors/node/latest-v6.x/node-v6.9.2-x86.msi)
5. [Node.js 6.9.2 Windows x64(Official)](https://nodejs.org/dist/v6.9.2/node-v6.9.2-x64.msi)
6. [Node.js 6.9.2 Windows x64(Taobao)](https://npm.taobao.org/mirrors/node/latest-v6.x/node-v6.9.2-x64.msi)