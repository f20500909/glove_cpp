

**glove-py is an implementation of the GloVe algorithm for learning word vectors from a corpus.** 


## Installation


using pip:

```
pip install glove-py
```

install with github code :

```
git clone git@github.com:f20500909/glove_py.git
python3 setup.py install
```


using in shell



### Run

with python:
```python
from glove import *

model = Glove(params)
model.train(input_file)
model.to_txt()
words = model.most_similary("one", 10)
print(words)

```

List of available `params` and their default value:

```
help info......
  -input_file               input_file [small_text]
  -vocab_size               vocabulary size to train  [0]
  -max_size                 maximum size to keep in courpus [10000000]
  -min_count                minimum times  [1]
  -window                   the window size to get common occurrence [10]
  -embed_size               embedded size  [80]
  -epoch                    train epoch [1]
  -threads                  train thread [40]
  -memory_limit             memory used to tain (GB) [1]
  -lr                       learning rate [0.05]
```

with shell:
./scripts/demo.sh


我去掉了很过我看上去用处不大的功能(只是我认为的用处打大的功能,有些细节并不理解也移除了),如提取共现矩阵的symmetric功能

忽略了大小写,统一改大写为小写


TODO:

修复有时会发生的错误:   read err...: Connection reset by peer

在合并共现词时，高频和低频矩阵好像写在了一个文件里面，感觉有点问题.....




在编译pybind_11时不能让对应文件被include 必须用cmake加载


![word](https://github.com/f20500909/glove_py/blob/master/scripts/tsne.png)
### Reference

- [GloVe ](https://github.com/stanfordnlp/GloVe)
- [GloVe-cpp](https://github.com/Yevgnen/GloVe-cpp)
- Jeffrey Pennington, Richard Socher, and Christopher D. Manning. 2014. GloVe: Global Vectors for Word Representation.
