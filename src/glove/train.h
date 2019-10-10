#ifndef TRAIN_H
#define TRAIN_H
#include <thread>
#include <memory>
#include <queue>

#include "utils.h"
#include "vector.h"
#include "args.h"
#include "densematrix.h"
#include "cooccur.h"
#include "vocabulary.h"

class Train {
public:


    Train() = default;

    Train(unsigned long embed_size, unsigned long long vocab_size, int threads,
          unsigned long epoch,  double lr
       );

    void train();

    inline double weighted(double cooccur) const;

    inline double difference(double dotValue, double b1, double b2, double gold) const;

    void to_txt(const Vocabulary &v) const;

    double single_thread(uint32_t i, double &loss, double lr);

    void threadCount();

    AnalogyPairs most_similary(const std::string &word, unsigned long num, const Vocabulary &vocab);

    std::vector<int> getTop(Vector vec, uint32_t num);

    void load_model(std::string wordvec_file, std::string meta_file, Vocabulary &v);


private:

    uint32_t vocab_size;
    unsigned long embed_size;
    double threshold;
    std::shared_ptr<DenseMatrix> W1;
    std::shared_ptr<DenseMatrix> W2;
    std::shared_ptr<Vector> b1;
    std::shared_ptr<Vector> b2;
    std::shared_ptr<DenseMatrix> GW1;
    std::shared_ptr<DenseMatrix> GW2;
    std::shared_ptr<Vector> Gb1;
    std::shared_ptr<Vector> Gb2;

    std::vector<int> threadInfo;

    int threads;
    unsigned long epoch;
    double lr;
};


#endif
