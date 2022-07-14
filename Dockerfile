FROM quay.io/centos/centos:centos7

LABEL name="wtbase"
LABEL maintainer="puyuan<github@puyuan.tech>"

ENV TZ=Asia/Shanghai

RUN mkdir -p /tmp/resource && \
    echo -e "\\033[45;37m ############### GCC 9 ############### \033[0m" && \
    yum update -y --nogpgcheck && \
    yum install -y --nogpgcheck centos-release-scl && \
    yum install -y --nogpgcheck devtoolset-9-gcc-* && \
    echo "source /opt/rh/devtoolset-9/enable" >> /etc/bashrc
SHELL ["/bin/bash", "--login", "-c"]

RUN cd /tmp/resource/ && \
    yum update -y && \
    yum install -y --nogpgcheck make vim which wget git openssl-devel && \ 
    yum install -y --nogpgcheck epel-release && \
    echo -e "\\033[45;37m ############### PYTHON3.7 ############### \033[0m" && \
    yum install -y --nogpgcheck libffi-devel zlib-devel  bzip2-devel xz-devel ncurses-devel sqlite-devel readline-devel tk-devel gdbm-devel db4-devel libpcap-devel  && \
    cd /tmp/resource/ && wget https://www.python.org/ftp/python/3.7.4/Python-3.7.4.tgz && tar -xzf Python-3.7.4.tgz && cd Python-3.7.4 && ./configure --prefix=/usr/local --enable-shared LDFLAGS="-Wl,-rpath /usr/local/lib" && make -j2 && make install && \
    pip3 install pip --upgrade && ln -snf /usr/local/bin/python3 /usr/bin/python3 && ln -snf /usr/bin/python2 /usr/bin/python
    
RUN  pip3 install -i https://pypi.tuna.tsinghua.edu.cn/simple numpy pandas xlsxwriter flask flask_socketio==4.3.1 pyquery flask-compress psutil chardet pyyaml matplotlib

RUN echo -e "\\033[45;37m ############### cmake 3.18 ############### \033[0m" && \
    cd /tmp/resource/ && wget https://github.com/Kitware/CMake/releases/download/v3.18.4/cmake-3.18.4.tar.gz && tar -xvf cmake-3.18.4.tar.gz && cd cmake-3.18.4 && ./bootstrap && make install -j4 

RUN echo -e "\\033[45;37m ############### rapidjson ############### \033[0m" && \
    yum install -y --nogpgcheck rapidjson-devel 

RUN echo -e "\\033[45;37m ############### spdlog ############### \033[0m" && \
    cd /tmp/resource/ && git clone https://hub.fastgit.xyz/gabime/spdlog.git && cd spdlog && mkdir build && cd build && cmake .. && make -j4 && make install

RUN echo -e "\\033[45;37m ############### boost 1.76 ############### \033[0m" && \
    yum install -y --nogpgcheck python-devel && \
    cd /tmp/resource/ && wget https://boostorg.jfrog.io/artifactory/main/release/1.76.0/source/boost_1_76_0.tar.gz && tar -xvf boost_1_76_0.tar.gz && cd boost_1_76_0 && ./bootstrap.sh && ./b2 install -j4 
    
RUN echo -e "\\033[45;37m ############### curl 7.70 ############### \033[0m" && \
    cd /tmp/resource/ && wget https://curl.se/download/curl-7.70.0.tar.gz && tar -xzvf curl-7.70.0.tar.gz && cd curl-7.70.0 && ./configure && make -j4 && make install && curl --version

RUN echo -e "\\033[45;37m ############### nanomsg 1.2 ############### \033[0m" && \
    cd /tmp/resource/ && git clone https://hub.fastgit.xyz/nanomsg/nanomsg.git && cd nanomsg && mkdir build && cd build && cmake .. && cmake --build . && ctest . && cmake --build . --target install && ldconfig

RUN echo -e "\\033[45;37m ############### clean temp files ############### \033[0m" && \
    yum -y clean all && rm -rf /tmp/resource

WORKDIR /home/wondertrader