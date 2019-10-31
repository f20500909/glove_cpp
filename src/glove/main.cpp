#include <args.hxx>
#include <iomanip>
#include <iostream>
#include <thread>
#include <vector>
#include "utils.h"
#include "vocabulary.h"
#include "cooccur.h"
#include "train.h"
#include "args.h"
#include "glove.h"

int main(int argc, char **argv) {

    std::vector<std::string> args_cmd(argv, argv + argc);

    Glove glove = Glove();

    if (args_cmd.size() > 1) glove.parseArgs(args_cmd);
    glove.train(glove.input_file);

//    glove.load("log/vocab.txt", "log/wordvec.txt", "log/wordvec.txt.meta");
    glove.to_txt();
    glove.most_similary("eight", 10);

    return 0;
}

