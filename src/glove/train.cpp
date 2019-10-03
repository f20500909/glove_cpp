#include "train.h"


Train::Train(unsigned long embed_size, unsigned long long vocab_size, int threads,
             unsigned long epoch,  double lr
           ) : embed_size(embed_size),
                                      vocab_size(vocab_size),
                                      threads(threads),
                                      epoch(epoch),
                                      lr(lr)
                            {
    //W1随机生成
    double scale = 0.01;
    //词向量权重参数
    W1 = std::make_shared<DenseMatrix>(vocab_size, embed_size, scale);
    W2 = std::make_shared<DenseMatrix>(vocab_size, embed_size, scale);
    GW1 = std::make_shared<DenseMatrix>(vocab_size, embed_size);
    GW2 = std::make_shared<DenseMatrix>(vocab_size, embed_size);
    Gb1 = std::make_shared<Vector>(vocab_size);
    Gb2 = std::make_shared<Vector>(vocab_size);
    b1 = std::make_shared<Vector>(vocab_size);
    b2 = std::make_shared<Vector>(vocab_size);
    threadCount();
};


void Train::train() {
    std::vector<std::thread> vec_threads(threads);
    std::vector<double> partial_loss(threads);
    for (unsigned long _epoch = 0; _epoch != epoch; ++_epoch) {
        Timer timer;
        timer.start();
        vec_threads.clear();
        for (uint32_t threadId = 0; threadId != threads; ++threadId) {
            vec_threads.emplace_back(&Train::single_thread, this, threadId, std::ref(partial_loss[threadId]),
                                     lr);
        }

        std::for_each(vec_threads.begin(), vec_threads.end(), [](std::thread &t) { return t.join(); });
        double loss = std::accumulate(partial_loss.begin(), partial_loss.end(), 0.0);

        timer.stop();
        std::cout << std::fixed << "Epoch " << std::setw(3) << _epoch << " (took:  " << std::setprecision(3)
                  << timer.elapsed() << "s): Loss: " << std::setprecision(6) << loss << std::endl;
    }
    return;
}


double Train::single_thread( uint32_t threadId, double &loss, double lr) {
    std::ifstream in("log/cooccur.bin");
    assert(in);
    untils::seek(in, threadInfo[threadId]);

    loss = 0;
    Vector dw1(embed_size);
    Vector dw2(embed_size);

    //三元组
    uint32_t i;
    uint32_t j;
    double val;

    double weight;
    double sigma;

    double db1 = 0;
    double db2 = 0;

    while (in) {
        in.read((char *) &i, sizeof(uint32_t));
        in.read((char *) &j, sizeof(uint32_t));
        in.read((char *) &val, sizeof(double));

        weight = weighted(val);

        sigma = difference(W1->dotIds(i, j, *W2), (*b1)[i], (*b2)[j], std::log(val));
        loss += 0.5 * weight * std::pow(sigma, 2);
        sigma *= weight;
        dw1 = W2->getVector(j) * sigma;
        dw2 = W1->getVector(i) * sigma;

        db1 = sigma;
        db2 = sigma;

        Vector vec1 = dw1 * dw1;
        GW1->addVectorToRow(vec1, i, 1);
        Vector vec2 = dw2 * dw2;
        GW2->addVectorToRow(vec2, j, 1);

//        //累加bias
        (*Gb1)[i] += db1 * db1;
        (*Gb2)[j] += db2 * db2;

        (*W1).addVectorToRow(dw1 * lr / (((*GW1).getVector(i) + 1e-8).sqrt()), i, -1);
        (*W2).addVectorToRow(dw2 * lr / (((*GW2).getVector(j) + 1e-8).sqrt()), j, -1);


        (*b1)[i] -= lr * db1 / std::sqrt((*Gb1)[i] + 1e-8);
        (*b2)[j] -= lr * db2 / std::sqrt((*Gb2)[j] + 1e-8);


        if (in.tellg() == threadInfo[threadId + 1]) {
            break;
        }
    }
    in.close();
    return loss;
}


inline double Train::weighted(double cooccur) const {
    return std::min(std::pow(cooccur / threshold, alpha), 1.0);
}

inline double Train::difference(double dotValue, double b1, double b2, double gold) const {
    return dotValue + b1 + b2 - gold;
}

void Train::to_txt(const Vocabulary &v) const {
    std::ofstream os;
    file::open(os, "log/wordvec.txt.meta");
    os << embed_size << std::endl;
    os.close();

    file::open(os, "log/wordvec.txt");
    for (std::uint32_t i = 0; i != W1->rows(); ++i) {
        for (std::uint32_t j = 0; j != W1->cols(); ++j) {
            if (j >= 0) os << " ";
            os << W1->at(i, j);
        }
        os << std::endl;
    }
    os.close();


    file::open(os, "log/vocab.txt");
    for (std::uint32_t i = 0; i != W1->rows(); ++i) {
        os << v[i] << std::endl;
    }
    os.close();

}

void Train::threadCount() {

    std::ifstream in("log/cooccur.bin");
    assert(in);
    int size = untils::size(in);

    int size_single = sizeof(CREC);
    int singleNum = size / (threads * size_single);

    for (int n = 0; n < threads; n++) {
        threadInfo.push_back(n * singleNum * size_single);
    }
    threadInfo.push_back(size);
    in.close();

    std::cout << "线程统计: " << std::endl;
    for (auto info:threadInfo) {
        std::cout << info << "  ";
    }
    std::cout << std::endl;
}

AnalogyPairs Train::most_similary(const std::string &word, unsigned long num, const Vocabulary &vocab) {
    num = std::min(std::uint32_t(num), vocab.size());
    Vector temp(W1->rows());

    Vector key = W1->getVector(vocab[word]);
    for (int row = 0; row < W1->rows(); row++) {
        for (int col = 0; col < W1->cols(); col++) {
            temp[row] += std::pow(W1->at(row, col), 2);
        }
        temp[row] = std::sqrt(temp[row]);
    }

    Vector topk = (*W1) * key / temp;


    std::vector<int> top = getTop(topk, num);

    AnalogyPairs words;


    std::vector<std::string> _string;
    std::vector<double> _val;

    for (auto id:top) {
//        words.emplace_back(make_pair(vocab[id], topk[id]));
        _string.push_back(vocab[id]);
        _val.push_back(topk[id]);
    }

//    for (auto w:_string) {
//        std::cout << w << "  "  << std::endl;
//    }
//
//    for (auto w:_val) {
//        std::cout << w << "  "  << std::endl;
//    }

    words.first = _string;
    words.second = _val;

    return words;
}


std::vector<int> Train::getTop(Vector vec, uint32_t num) {
    assert(num < vec.size());
    std::vector<int> res;
    std::priority_queue<WID> wid;
    for (uint32_t i = 0; i < num; i++) {
        wid.push({i, vec[i]});
    }

    for (uint32_t i = num; i < vec.size(); i++) {
        WID temp = {i, vec[i]};
        if (temp > wid.top()) {
            wid.pop();
            wid.push({i, vec[i]});
        }
    }
    while (!wid.empty()) {
        res.push_back(wid.top().id);
        wid.pop();
    }
    return res;
}


void Train::load_model(std::string wordvec_file, std::string meta_file, Vocabulary &voca) {

    std::ifstream meta(meta_file);
    assert(meta);
    meta >> embed_size;
    meta.close();

    std::ifstream mat(wordvec_file);
    assert(mat);
    double temp2 = 0;
    int count_2 = 0;

//    std::cout << "v.itoa.size() * a->embed_size   " << v.itoa.size() * a->embed_size << std::endl;
    while (!mat.eof()) {
        mat >> temp2;
        if (count_2 < voca.itoa.size() * embed_size) {
            (*W1)[count_2] = temp2;
        }
        count_2++;
    }
    mat.close();


}




