#include "glove.h"

Glove::Glove(std::string input_file,
               unsigned long long vocab_size, unsigned long long max_size,
             unsigned long min_count, unsigned long window, unsigned long embed_size, unsigned long epoch,
             int threads, double lr) {
    this->input_file = input_file;
    this->vocab_size = vocab_size;
    this->max_size = max_size;
    this->min_count = min_count;
    this->window = window;
    this->embed_size = embed_size;
    this->epoch = epoch;
    this->threads = threads;
    this->lr = lr;
};

Glove::Glove(std::vector<std::string> args_cmd) {
    parseArgs(args_cmd);
    std::cout << "load cmd ..." << std::endl;
};


Glove::Glove() {

};

void Glove::train(std::string input_file) {
    this->input_file = input_file;

    TRACE("====================================================== ","\n");
    TRACE("building vocabulary","...");
    voca = Vocabulary(min_count, max_size,  input_file);
    vocab_size = voca.size();
    TRACE("vocabulary size is ",vocab_size);
    TRACE("Build vocabulary ","down");



    TRACE("====================================================== ","\n");
    TRACE("building cooccur mat","...");
    CoMat builder(vocab_size,input_file,  window,  memory_limit);
    builder.build(voca);
    TRACE("build cooccur mat  ","down");



    TRACE("====================================================== ","\n");
    model = Train(embed_size, vocab_size, threads,  epoch, lr);
    model.train();
    TRACE("====================================================== ","\n");
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
