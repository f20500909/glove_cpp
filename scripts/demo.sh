#!/usr/bin/env bash

#如果data目录下面没有text8的训练文件,则下载文件
if [ ! -e ../data/text8 ]; then
  if hash wget 2>/dev/null; then
    wget http://mattmahoney.net/dc/text8.zip
  else
    curl -O http://mattmahoney.net/dc/text8.zip
  fi
  unzip text8.zip
#  rm text8.zip
fi

mkdir -p "../build"

#可执行文件不存在,则进行编译
if [ ! -f  "../build/glove" ]; then
    ./build.sh
fi



input_file="small_text"
#input_file="text8"

vocab_size=0
max_size=1e7
min_count=1
window=10
embed_size=2
epoch=30
threads=20
memory_limit=1

lr=0.05
keep_case=false

#正常显示#运行
../build/glove -input_file $input_file   -vocab_size $vocab_size -min_count $min_count \
-window $window -vocab_size $vocab_size  -epoch $epoch -threads $threads  -lr $lr -embed_size $embed_size

sudo python3 show_vec.py
