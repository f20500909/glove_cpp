# 当模块安装好了之后,进行测试
from glove import *

model = Glove(epoch=2)
model.train("small_text")
model.to_txt()
words = model.most_similary("one", 10)
print(words)
