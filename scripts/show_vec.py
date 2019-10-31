import os
import numpy as np
from sklearn.manifold import TSNE
import matplotlib.pyplot as plt

show_words = ["one", "two", "three", "four", "five", "six", "seven", "eight", "nine",
              "happy", "sad",
              "rain", "snow", "windy", "rainy", "sunny", "storm", "mist",
              "monday", "tuesday", "wednesday", "thursday", "friday", "saturday", "sunday"]


# 找并集
def get_data(show_words):
    txt_name = "../../build/log/wordvec.txt"
    with open("../../build/log/vocab.txt") as f:
        words = f.readlines()

    words = [word.strip() for word in words]

    show_word_id = []
    words_fit = []
    for id, word in enumerate(words):
        if word in show_words:
            show_word_id.append(id)
            words_fit.append(word)


    np_arr = np.loadtxt(txt_name, dtype=np.float32)

    return words_fit, np_arr[show_word_id]


labels, final_embeddings = get_data(show_words)


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


show_word = 200
# print(labels[-100:])

tsne = TSNE(perplexity=30, n_components=2, init='pca', n_iter=500, method='exact')
low_dim_embs = tsne.fit_transform(final_embeddings[:show_word])
# low_dim_embs = tsne.fit_transform(final_embeddings)
low_dim_embs = np.around(low_dim_embs, decimals=2)

print(low_dim_embs)

np.savetxt('001', low_dim_embs, fmt="%f,%f")
# exit()

labels = labels[:show_word]
plot_with_labels(low_dim_embs, labels, os.path.join("./", 'tsne.png'))
plt.show()
