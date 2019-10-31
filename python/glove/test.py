from glove_pybind import glove

model = glove()
model.train("../../data/small_text")


# model.load("log/vocab.txt", "log/wordvec.txt", "log/wordvec.txt.meta")
words = model.most_similary("one", 10)
print(words)
