#!/bin/bash
set -e

input_file="small_text"

vocab_size=0
max_size=1e7
min_count=1
window=10
embed_size=80
epoch=2
threads=20
memory_limit=1

lr=0.05
keep_case=false

#正常显示

#../build/glove -input_file $input_file  -log_dir $log_dir -vocab_size $vocab_size -min_count $min_count \
#-window $window -vocab_size $vocab_size  -epoch $epoch -threads $threads  -lr $lr

#输出错误信息

#../build/glove r $lr
