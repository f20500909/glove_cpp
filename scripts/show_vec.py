import os
import numpy as np
from sklearn.manifold import TSNE
import matplotlib.pyplot as plt


def get_data():
    txt_name = "./log/wordvec.txt"
    np_arr = np.loadtxt(txt_name, dtype=np.float32)
    with open("./log/vocab.txt") as f:
        word = f.readlines()
    return word, np_arr


word, np_arr = get_data()

labels = word

final_embeddings = np_arr


def plot_with_labels(low_dim_embs, labels, filename):
    assert low_dim_embs.shape[0] >= len(labels), 'More labels than embeddings'
    plt.figure(figsize=(18, 18))  # in inches
    for i, label in enumerate(labels):
        x, y = low_dim_embs[i, :]
        plt.scatter(x, y)
        plt.annotate(label,
                     xy=(x, y),
                     xytext=(5, 2),
                     textcoords='offset points',
                     ha='right',
                     va='bottom')

    plt.savefig(filename)


print(word)
print(np_arr.shape)
show_word = 200
# print(labels[-100:])

tsne = TSNE(perplexity=30, n_components=2, init='pca', n_iter=5000, method='exact')
low_dim_embs = tsne.fit_transform(final_embeddings[:show_word])
labels = labels[:show_word]
plot_with_labels(low_dim_embs, labels, os.path.join("./", 'tsne.png'))
plt.show()
