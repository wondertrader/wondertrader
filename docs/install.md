### 如何安装wtpy
***
* 安装python3.6以上的版本（32位、64位），安装完成以后输入以下命令，可以检查python的版本号
    ```
    $ python
    ```

* 打开命令行，输入以下指令直接安装
    ```
    $ pip install wtpy
    ```
    或者直接下载whl文件到本地进行安装
    阿里云镜像地址：<https://mirrors.aliyun.com/pypi/simple/wtpy/>
    pipy地址：<https://pypi.org/project/wtpy/#files>

* 安装完成以后，输入以下命令，可以查看wtpy的版本号
    ```
    $ pip show wtpy
    ```

### 如何编译WonderTrader
***
* WonderTrader用到的三方库中，只有3个库没有直接放在代码中
    * [boost 1.72](https://dl.bintray.com/boostorg/release/1.72.0/source/)
    * [rapidjson](https://github.com/Tencent/rapidjson/)
    * [spdlog](https://github.com/gabime/spdlog)
    * [curl 7.70](https://curl.haxx.se/)
    * [mysql connector 6.11](https://conan.io/center/mysql-connector-c/6.1.11/?revision=&os=Windows&tab=config)
    
    其中mysql是数据组件（`WtDataWriter`,`WtDataReader`）和回测框架(`WtBtCore`)的依赖库；curl是易盛极星交易模块的依赖库，主要用于获取二次验证码。

    为了方便大家使用，依赖库统一发布到百度云盘，下载就可以直接使用：链接：<https://pan.baidu.com/s/1Bdxh_PgjqHMzuGjl9ernhg> 提取码:d6bh

* 配置依赖库环境
    `WonderTrader`在`windows`下的依赖文件根目录，采用了环境变量存储。将云盘的依赖库下载到本地以后，解压到本地目录，如`E:\vc141depends`。然后设置一个环境变量，如下：
    ```
    $ MyDepends141=E:\vc141depends
    ```
    最终如下图所示
    ![alt 依赖库目录结构](http://wt.f-sailors.cn/snapshots/deps_snap.png)
    linux下，WonderTrader将依赖库文件统一放到/home/mydeps目录下，目录结构同上

* `Windows`下编译`WonderTrader`
    `WonderTrader`在`Windows`下的开发环境是`Win7` + `vs2017`，`WonderTrader`直接提供了`VisualStudio 2017`的解决方案文件。
    解决方案结构如图
    ![alt 解决方案结构图](http://wt.f-sailors.cn/snapshots/wt_vs_snap.png)

* Linux下编译WonderTrader
    Linux下编译通过一个shell脚本直接执行
    * Release版本的编译
    ```shell
    $ sudo ./build_release.sh
    ```
    * Debug版本的编译
    ```shell
    $ sudo ./build_debug.sh
    ```
    * 如果提示shell脚本不可执行，修改一下属性即可
    ```shell
    $ sudo chmod 777 build_xxx.sh
    ```
* 使用Docker作为开发环境。
    docker环境已经将将Linux下的依赖提前安装。
    进入到docker目录下，
    ```
    $ docker build -t wondertrader .
    ```
    镜像构建完成后，在宿主机上运行
    ```
    docker run -it --rm  wondertrader /bin/bash
    ```
    之后可以使用vscode 的远程开发模式连接到docker容器进行开发。
