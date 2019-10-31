import os
import numpy as np
from sklearn.manifold import TSNE
import matplotlib.pyplot as plt


def clear_words(show_words):
    res1 = []
    for words in show_words:
        temp = []
        for word in words:
            if word != "":
                temp.append(word)
        res1.append(temp)
    res2 = []
    color = []
    for n, list in enumerate(res1):
        for word in list:
            color.append(n)
            res2.append(word)
    return color, res2


color = ["black", "blue", "green", "purple", "yellow", "red", "pink", "orange", "", "", "", "", "", "", "", "", "", "",
         "", "", "", "", "", "", "", "", "", "", "", ""]
emotion = ["joyful", "angry", "sad", "happy", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "",
           "", "", "", "", "", "", "", ""]

sport = ["instructor", "badminton", "Tennis", "Baseball", "Walking", "Sprint", "Marathon", "Boating", "Boxing",
         "swimming", "fencing", "cycling", "run", "soccer", "diving", "", "", "", "", "", "", "", "", "", "", "", "",
         "", "", ""]
number = ["one", "two", "three", "four", "five", "six", "seven", "eight", "nine", "", "", "", ""]
time = ["monday", "tuesday", "wednesday", "thursday", "friday", "saturday", "sunday", "", ""]
weather = ["rain", "snow", "windy", "rainy", "sunny", "storm", "mist", ""]
emotion = ["dog", "cat", "bird", "monkey", "panda", "fisnh", " pig", "whale", "shark", "rabbit", "tiger", "giraffe",
           "lion", "deer", "monkey", "horse", "sparrow", "armadillo", "butterfly", "", "", "",
           "", "", "", "", "", "", "", ""]
fruit = ["peach", "pineapple", "orange", "banana", "grape", "berry", "durian", "lemon", "mango", "", "", "", "", "", "",
         "", "", "", "", "", "", "",
         "", "", "", "", "", "", "", ""]
food = [" meat", "beef", "veal", "lamb", "sirloin", "steak", "chop", "cutlet", "stew", "pork", "chicken", "broth",
        "egg", "oil", "salt", "", "", "", "", "", "", "",
        "", "", "", "", "", "", "", ""]

character = ["active", "able", "aggressive", "ambitious", "amiable", "active", "capable", "careful", "candid",
             "energetic", "expressivity", "frank", "objective", "precise", "cheerful", "aspiring", "", "", "", "", "",
             "",
             "", "", "", "", "", "", "", ""]
school_supplies = ["pen", "pencil", "ballpen", "pencilbox", "schoolbag", "rubber", "eraser", "ruler", "lamp", "paper",
                   "dictionary", "tube", "notebook", "comicbook", "staples", "glue", "mathbook", "Englishbook", "", "",
                   "", "",
                   "", "", "", "", "", "", "", ""]

city = ["city", "capital", "centre", "district", "urban", "slums", "village", "house", "church", "school", "university",
        "museum", "station", "", "", "", "", "", "", "", "", "",
        "", "", "", "", "", "", "", ""]

country = ["China", "Brazil", "Russia", "Canada", "Maldives", "Japan", "USA", "Korea", "Mexico", "Iceland", "Sweden",
           "", "", "", "", "", "", "", "", "", "", "",
           "", "", "", "", "", "", "", ""]
weapon = ["weapon", "arrow", "spear", "gun", "cannon", "missile", "tank", "sword", "stick", "pistol", "rifle", "ball",
          "bow", "", "", "", "", "", "", "", "", "", "",
          "", "", "", "", "", "", "", ""]
emotion = ["joyful", "angry", "sad", "happy", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "",
           "", "", "", "", "", "", "", ""]

woman = ["female", "woman", "girl", "lady", "lady", "goddess", "empress", "queen", "princess", "actress", "waitress",
         "mistress", "hostess", "", "",
         "", "", "", "", "", "", "", "", "", "", "", "", "", "", ""]
man = ["male", "man", "boy", "sir", "gentleman", "god", "emperor", "king", "prince", "actor", "waiter", "master",
       "host", "", "",
       "", "", "", "", "", "", "", "", "", "", "", "", "", "", ""]
show_words = []
show_words.append(color)
show_words.append(emotion)
show_words.append(sport)
show_words.append(number)
show_words.append(time)
show_words.append(weather)
show_words.append(emotion)
show_words.append(fruit)
show_words.append(food)
show_words.append(character)
show_words.append(school_supplies)
show_words.append(city)
show_words.append(country)
show_words.append(weapon)
show_words.append(woman)
show_words.append(man)

colors, show_words = clear_words(show_words)


# 找并集
def get_data(colors, show_words):
    txt_name = "../build/log/wordvec.txt"
    with open("../build/log/vocab.txt") as f:
        words = f.readlines()
    words = [word.strip() for word in words]
    show_word_id = []
    labels = []
    colors_fit = []
    for id, word in enumerate(words):
        if word in show_words:
            index = show_words.index(word)
            colors_fit.append(colors[index])
            show_word_id.append(id)
            labels.append(word)
    np_arr = np.loadtxt(txt_name, dtype=np.float32)
    return colors_fit, labels, np_arr[show_word_id]


colors, labels, final_embeddings = get_data(colors, show_words)
print(colors)
print(labels)
print(final_embeddings)


def plot_with_labels(low_dim_embs, labels):
    assert low_dim_embs.shape[0] >= len(labels), 'More labels than embeddings'
    plt.figure(figsize=(18, 18))  # in inches

    print(low_dim_embs.shape)
    plt.scatter(low_dim_embs[:, 0], low_dim_embs[:, 1], c=colors)

    for i, label in enumerate(labels):
        x, y = low_dim_embs[i, :]
        plt.annotate(label,
                     xy=(x, y),
                     xytext=(5, 2),
                     textcoords='offset points',
                     ha='right',
                     va='bottom')

    plt.savefig('./tsne.png')


show_word = 200
# print(labels[-100:])

tsne = TSNE(perplexity=30, n_components=2, init='pca', n_iter=500, method='exact')
low_dim_embs = tsne.fit_transform(final_embeddings[:show_word])
# low_dim_embs = tsne.fit_transform(final_embeddings)
low_dim_embs = np.around(low_dim_embs, decimals=2)

print(low_dim_embs)

np.savetxt('001', low_dim_embs, fmt="%f,%f")

labels = labels[:show_word]
plot_with_labels(low_dim_embs, labels)
plt.show()
