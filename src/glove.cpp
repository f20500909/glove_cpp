#include "glove.h"

Glove::Glove(std::string input_file,
               unsigned long long vocab_size, unsigned long long max_size,
             unsigned long min_count, unsigned long window, unsigned long embed_size, unsigned long epoch,
             int threads, double lr, bool keep_case) {
    this->input_file = input_file;
    this->vocab_size = vocab_size;
    this->max_size = max_size;
    this->min_count = min_count;
    this->window = window;
    this->embed_size = embed_size;
    this->epoch = epoch;
    this->threads = threads;
    this->lr = lr;
    this->keep_case = keep_case;
};

Glove::Glove(std::vector<std::string> args_cmd) {
    parseArgs(args_cmd);
    std::cout << "load cmd ..." << std::endl;
};


Glove::Glove() {

};

void Glove::train(std::string input_file) {
    this->input_file = input_file;

    std::cout << "Building vocabulary..." << std::endl;
    voca = Vocabulary(min_count, max_size, keep_case, input_file);
//    std::cout << "Vocab size: " << v.size() << std::endl;
    vocab_size = voca.size();

    CoMat builder(vocab_size,input_file,  window,  memory_limit);
    builder.build(voca);

//    std::cout << "Training..." << std::endl;
    model = Train(embed_size, vocab_size, threads,  epoch, lr);
    model.train();
//    std::cout << "Training down..." << std::endl;

};


AnalogyPairs Glove::most_similary(const std::string &word, unsigned long num) {
    return model.most_similary(word, num, voca);
}

void Glove::load( std::string wordvec_file, std::string meta_file) {

    voca.load();

    vocab_size = voca.atoi.size();
    model = Train(embed_size, vocab_size, threads, epoch, lr);
    model.load_model(wordvec_file, meta_file, voca);

}

void Glove::to_txt() {
//    std::cout << "save wordvec..." << std::endl;
    model.to_txt(voca);
//    std::cout << "save wordvec down..." << std::endl;
}
