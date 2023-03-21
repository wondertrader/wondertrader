from wondertrader/base

# set prefix, compile_path, etc.
ARG PREFIX="/usr/local"
ARG COMPILE_PATH="/tmp"

# setup sudo
RUN set -e && apt-get update && apt-get install sudo

# setup nanomsg.
ADD nanomsg.tar.gz $COMPILE_PATH
RUN set -e \
  && cd $COMPILE_PATH/nanomsg && mkdir build && cd build && cmake .. && cmake --build . \
  && ctest . && sudo cmake --build . --target install && ldconfig

# setup wondertrader
WORKDIR /home/wondertrader/
RUN apt update &&  apt install --reinstall -y ca-certificates && rm -rf /var/lib/apt/lists/*

