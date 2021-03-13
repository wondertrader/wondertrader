### 前言
**WonderTrader**开源以来，虽然没有正式做过推广，但是逐渐有了第一批吃螃蟹的用户。在此**WonderTrader**团队对这些用户深表感谢，希望**WonderTrader**能够获得大家更多的认可。
在用户的反馈中，我们逐渐发现，除了直接使用`wtpy`进行回测和交易的用户，还有一批对源码有直接需求的用户。他们在使用过程中遇到的最大问题是如何编译源码。**WonderTrader**的官方文档中，对开发环境做了简单的介绍，但是并不深入，所以这些用户在搭建开发环境的过程中遇到了不少问题。

**本文的主要目的就是帮助用户解决WonderTrader的源码的编译问题**。

鉴于一般用户对于`windows`平台下搭建开发环境都比较熟悉，所以**本文将重点介绍linux下的开发环境的搭建**。

### 共享资源
因为笔者长期在开发一线，平时在工作中会用到很多方便易用的工具，**WonderTrader**的开发中也有涉及。为了方便大家下载使用，本文中涉及到的一些好用的工具，笔者都统一放到百度云盘里共享出来，大家可以根据需要自行下载。文中会有多出涉及到从共享资源中下载文件，后面就不再赘述，统一用**共享资源**代称。

下载连接:<https://pan.baidu.com/s/1Bdxh_PgjqHMzuGjl9ernhg>

 提取码: **d6bh**
 
 ![image.png](https://segmentfault.com/img/bVbZLhk)

### `Linux`开发环境搭建
操作系统：**Ubuntu Server 18.04.3 LTS**
`Ubuntu 20`以后的秘钥加密算法做了调整，`SecureCRT 7`以前的版本都无法连接，所以本文以`Ubuntu 18.04.3 LTS`版本作为基本环境。笔者将使用`vmware`安装一个全新环境，从零开始演示开发环境如何搭建。

#### 1、使用`vmware`安装`ubuntu server 18.04.3 LTS`镜像
镜像可以从**共享资源**中直接下载
![image.png](https://segmentfault.com/img/bVbZLhw)
![image.png](https://segmentfault.com/img/bVbZLhx)

#### 2、虚拟机创建成功以后，自动开启，进入安装界面，然后一路默认
![image.png](https://segmentfault.com/img/bVbZLhM)

#### 3、到了源配置界面，一定要把镜像改为国内镜像！
这个很重要！不然安装特别慢！
![image.png](https://segmentfault.com/img/bVbZLhO)
**推荐使用阿里云的源http://mirrors.aliyun.com/ubuntu**
![image.png](https://segmentfault.com/img/bVbZLhP)

#### 4、到了SSH配置界面，一定要选择`SSH server`，不然会无法使用`putty`或`SecureCRT`
![image.png](https://segmentfault.com/img/bVbZLhR)

#### 5、到了组件选择界面，不用安装多余的组件，直接`done`
![image.png](https://segmentfault.com/img/bVbZLhI)

#### 6、进入最后流程
这里会有安全升级，可能耗时较长（如果前面没有配置国内镜像，这里的时间更是数倍）,也可以跳过安全升级
![image.png](https://segmentfault.com/img/bVbZLhV)

#### 7、重启之后，进入控制台
如果使用`SecureCRT`连接，在`vmware`中可以看一下虚拟机的ip地址，然后配置连接地址和账号密码进行连接
![image.png](https://segmentfault.com/img/bVbZLh5)

#### 8、安装开发环境
```shell
$ sudo apt-get install build-essential
```
![image.png](https://segmentfault.com/img/bVbZLil) 

使用`gcc --version`查看`gcc`的版本号
![image.png](https://segmentfault.com/img/bVbZLin)

安装`cmake`
![image.png](https://segmentfault.com/img/bVbZLio)

使用`cmake --version`查看`cmake`的版本号
![image.png](https://segmentfault.com/img/bVbZLip)

安装7z解压工具
```shell
$ sudo apt-get install p7zip-full
```
![image.png](https://segmentfault.com/img/bVbZLiq)

#### 9、从`共享资源`中下载`mydes_linux.7z`并上传到虚拟机
可以使用使用`SecureFX`（**共享资源**中的`SecureCRT`自带`SecureFX`）上传到虚拟机，如下图：
![image.png](https://segmentfault.com/img/bVbZLir)

#### 10、将`mydeps_linux.7z`解压到`/home`下
```shell
$ sudo 7z x mydeps_linux.7z /home
```

#### 11、拉取`WonderTrader`的源码
```shell
$ sudo git clone https://github.com/wondertrader/wondertrader.git
```
![image.png](https://segmentfault.com/img/bVbZLiz)

#### 12、进入`src`目录，执行编译脚本
```shell
$ sudo ./build_release.sh
```
编译完成以后，使用`SecureFX`就可以直接从`src`目录下的`build/bin`中下载需要的文件即可
![image.png](https://segmentfault.com/img/bVbZLiA)


### `Windows`开发环境搭建
#### 1、安装`vs2017`社区版
首先下载`vs2017`的安装器，下载地址如下：
[https://visualstudio.microsoft.com/zh-hans/vs/older-downloads/](https://visualstudio.microsoft.com/zh-hans/vs/older-downloads/)
![image.png](https://segmentfault.com/img/bVbZLiR)
![image.png](https://segmentfault.com/img/bVbZLiT)

或者直接从**共享资源**中直接下载该安装器

#### 2、从`共享资源`中下载`vc141deps_win.7z`，并解压到`E:/vc141depends`目录
![image.png](https://segmentfault.com/img/bVbZLiY)

#### 3、配置环境变量

从**共享资源**中下载环境变量编辑器`RapidEE`到本地，解压以后运行。
新建一个名为`MyDepends141`的环境变量，并将值设置为依赖库解压目录`E:/vc141depends`
![image.png](https://segmentfault.com/img/bVbZLi0)
设置好了以后，保存环境变量，即可生效，如果没有生效，可以重启电脑。`win10`下要注意一下使用**管理员身份运行**`RapidEE`，不然保存可能会失败！

#### 4、安装`git`

下载git并安装：[https://git-scm.com/download/win](https://git-scm.com/download/win)
![image.png](https://segmentfault.com/img/bVbZLi4)
下载tortoisegit并安装：[https://tortoisegit.org/download/](https://tortoisegit.org/download/)
![image.png](https://segmentfault.com/img/bVbZLi5)

#### 5、下载代码
选择一个目录，然后右键点击该目录，选择“**Git克隆**”菜单
![image.png](https://segmentfault.com/img/bVbZLi8)
URL设置为:<https://github.com/wondertrader/wondertrader.git>
![image.png](https://segmentfault.com/img/bVbZLjc)

#### 6、编译代码
进入到`wondertrader`的`src`目录下
![image.png](https://segmentfault.com/img/bVbZLjd)
双击运行`wondertrader.sln`，打开以后在解决方案上右键点击，选择“**生成解决方案**”就进入了编译过程
![image.png](https://segmentfault.com/img/bVbZLje)
![image.png](https://segmentfault.com/img/bVbZLjf)