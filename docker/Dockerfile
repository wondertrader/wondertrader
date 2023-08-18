# 这个dockerfile直接拉取dockerhub上提交的wondertrader镜像，镜像大小约500M
# wondertrader镜像，基于ubuntu18.04进行构建，安装了gcc8等编译工具
# 同时内置了linux编译所需要的依赖库，位置在/home/mydeps
# 基于该文件构建镜像可以使用命令： docker build -t yourimagename -f Dockerfile .
# 构建完成以后，要启动容器，可以使用命令： docker run -it yourimagename /bin/bash

FROM wondertrader/wondertrader:latest
MAINTAINER wondertrader@126.com

# 添加预编译依赖库，并解压
ADD mydeps_gcc8.4.0.7z /home
RUN cd /home && 7za x mydeps_gcc8.4.0.7z

# 拉取wondertrader代码
# 如果在宿主机上已经拉取了wondertrader代码，可以注释掉下一行指令，并在启动容器的时候使用-v参数将宿主机上的代码目录挂载到容器中
# docker run -it yourimagename -v /home/wondertrader:/home/wondertrader /bin/bash
RUN cd /home && git clone https://gitee.com/wondertrader/wondertrader.git

# 设置工作目录
WORKDIR /home/wondertrader/

