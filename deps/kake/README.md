#kake

##简介
Kake是一个基于约定的构建工具

##安装
###依赖需求
Kake使用python3编写，使用了PyYAML库，因此需要安装Python、libyaml和Python的PyYAML。

如果使用ubuntu，请安装以下包：

    sudo apt-get install libyaml-0-2 libyaml-dev
    sudo apt-get install python3 libpython3-dev python3-pip

然后使用pip安装PyYAML

    sudo pip3 install PyYAML

###安装kake
Kake本身安装很简单，将kake的代码clone到某个用户目录下即可。
假设我们将kake安装到~/apps下，命令行如下：

    cd ~/apps
    git clone https://git.oschina.net/kinuxroot/kake.git 

然后编辑~/.bashrc，添加如下一行：

    export KAKE_HOME=~/apps/kake

最后，将kake/bin目录下的kake添加到PATH路径，或者在PATH路径对应的目录下建立一个符号链接，比如我将kake所在目录添加到PATH路径，编辑~/.bashrc，在文件末尾添加如下一行：

    export PATH="${PATH}:${KAKE_HOME}/bin"

###验证安装
验证kake有没有安装好很简单，在一个没有Kakefile的目录下执行kake，如果输出：

    Project file not exists.

说明kake已经安装完成
