# Meshy

##简介
Meshy是使用C++开发的网络通信框架，跨平台，支持不同平台下的高性能异步通信。同时提供最基本的REST服务开发支持。

##安装
###依赖需求
Meshy支持gmake的Makefile和kake。kake的安装过程参见使用kake编译一节

### 使用Makefile编译Hurricane
为了方便广大Linux用户，我们提供了传统的Makefile。
使用Makefile编译Meshy是很简单的事情，只需要使用make即可。

make

### 使用kake编译
#### kake简介
Kake是一个基于约定的构建工具

#### 依赖需求
Kake使用python3编写，使用了PyYAML库，因此需要安装Python、libyaml和Python的PyYAML。

如果使用ubuntu，请安装以下包：

    sudo apt-get install libyaml-0-2 libyaml-dev
    sudo apt-get install python3 libpython3-dev python3-pip

然后使用pip安装PyYAML

    sudo pip3 install PyYAML

#### 安装kake
Kake本身安装很简单，将kake的代码clone到某个用户目录下即可。
假设我们将kake安装到~/apps下，命令行如下：

    cd ~/apps
    git clone https://git.oschina.net/kinuxroot/kake.git 

然后编辑~/.bashrc，添加如下一行：

    export KAKE_HOME=~/apps/kake

最后，将kake/bin目录下的kake添加到PATH路径，或者在PATH路径对应的目录下建立一个符号链接，比如我将kake所在目录添加到PATH路径，编辑~/.bashrc，在文件末尾添加如下一行：

    export PATH="${PATH}:${KAKE_HOME}/bin"

#### 验证安装
验证kake有没有安装好很简单，在一个没有Kakefile的目录下执行kake，如果输出：

    Project file not exists.

说明kake已经安装完成
使用kake编译也非常简单，只需要在目录下执行kake即可。

    kake
