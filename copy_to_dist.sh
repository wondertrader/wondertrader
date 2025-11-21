#!/usr/bin/env bash

# 源目录（build_all 的 bin 目录）
root="./src/build_all/build_x64/Release/bin"

# 目标目录
des_trader="dist/TestTrader"
des_parser="dist/TestParser"

# 创建目标目录
mkdir -p "$des_trader"
mkdir -p "$des_parser"

echo "Copying TestTrader bin & Trader libs to $des_trader"
echo "Copying TestParser bin & Parser libs to $des_parser"

#######################################
# 复制 TestTrader 可执行文件
#######################################

# 常见两种布局：bin/TestTrader 或 bin/TestTrader/TestTrader
if [ -f "$root/TestTrader" ]; then
    cp -vf "$root/TestTrader" "$des_trader"
elif [ -f "$root/TestTrader/TestTrader" ]; then
    cp -vf "$root/TestTrader/TestTrader" "$des_trader"
else
    echo "Warning: TestTrader executable not found in $root"
fi

#######################################
# 复制 Trader 相关 so
#######################################

trader_libs=("libTraderCTP.so" "libTraderCTPOpt.so" "libTraderCTPMini.so")

for lib in "${trader_libs[@]}"; do
    if [ -f "$root/$lib" ]; then
        cp -vf "$root/$lib" "$des_trader/$lib"
    else
        echo "Warning: $lib not found in $root"
    fi
done

#######################################
# 复制 TestParser 可执行文件
#######################################

# 同样处理两种布局：bin/TestParser 或 bin/TestParser/TestParser
if [ -f "$root/TestParser" ]; then
    cp -vf "$root/TestParser" "$des_parser"
elif [ -f "$root/TestParser/TestParser" ]; then
    cp -vf "$root/TestParser/TestParser" "$des_parser"
else
    echo "Warning: TestParser executable not found in $root"
fi

#######################################
# 复制 Parser 相关 so
#######################################

parser_libs=("libParserCTP.so" "libParserCTPOpt.so" "libParserCTPMini.so")

for lib in "${parser_libs[@]}"; do
    if [ -f "$root/$lib" ]; then
        cp -vf "$root/$lib" "$des_parser/$lib"
    else
        echo "Warning: $lib not found in $root"
    fi
done

echo "Copy finished."