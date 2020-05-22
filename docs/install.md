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
    
    因为rapidjson和spdlog是header-only的，所以只需要预编译boost1.72即可
    boost的编译方法，不管是win还是linux，网络上都有很多教程，这里就不赘述了。

* 配置依赖库环境
    WonderTrader在windows下的依赖文件根目录，采用了环境变量存储，如32位依赖文件根目录为
    ```
    $ MyDepends12=E:\vc12depends
    ```
    64位依赖文件根目录
    ```
    $ MyX64Deps12=E:\vc12depsX64
    ```
    在依赖库文件根目录下，建立两个子目录`include`、`lib`，将头文件全部放到`include`里，将预编译的lib全部放到`lib`里，如下图所示
    ![alt 依赖库目录结构](http://wt.f-sailors.cn/snapshots/deps_snap.png)
    linux下，WonderTrader将依赖库文件统一放到/home/mydeps目录下，目录结构同上

* Windows下编译WonderTrader
    WonderTrader在Windows下的开发环境是`Win7` + `vs2013`，WonderTrader直接提供了vs的解决方案文件，当然也可以用CMakelists生成解决方案。
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