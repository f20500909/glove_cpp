#include "args.h"

void Args::parseArgs(const std::vector<std::string> &args) {
    std::string command(args[1]);
    for (int ai = 1; ai < args.size(); ai += 2) {
//        cout<<"args.at(ai + 1):"<<args.at(ai + 1)<<endl;
        if (args[ai][0] != '-') {
            std::cerr << "Provided argument without a dash! Usage:" << std::endl;
            printHelp();
            exit(EXIT_FAILURE);
        }
        try {
            if (args[ai] == "-h") {
                std::cerr << "Here is the help! Usage:" << std::endl;
                printHelp();
                exit(EXIT_FAILURE);
            } else if (args[ai] == "-vocab_size") { vocab_size = std::stoi(args.at(ai + 1)); }
            else if (args[ai] == "-min_count") { min_count = std::stoi(args.at(ai + 1)); }
            else if (args[ai] == "-window") { window = std::stoi(args.at(ai + 1)); }
            else if (args[ai] == "-embed_size") { embed_size = std::stoi(args.at(ai + 1)); }
            else if (args[ai] == "-max_size") { max_size = std::stoi(args.at(ai + 1)); }
            else if (args[ai] == "-epoch") { epoch = std::stoi(args.at(ai + 1)); }
            else if (args[ai] == "-threads") { threads = std::stoi(args.at(ai + 1)); }
            else if (args[ai] == "-lr") { lr = atof(args.at(ai + 1).c_str()); }


            else if (args[ai] == "-input_file") { input_file = args.at(ai + 1).c_str(); }
            else {
                std::cerr << "Unknown argument: " << args[ai] << std::endl;
                printHelp();
                exit(EXIT_FAILURE);
            }
        }
        catch (std::out_of_range) {
            std::cerr << args[ai] << " is missing an argument" << std::endl;
            printHelp();
            exit(EXIT_FAILURE);
        }
    }
}

void Args::printHelp() {
    std::cerr << "\nhelp info......\n"
              << "  -input_file               input_file [" << input_file << "]\n"
              << "  -vocab_size               vocabulary size to train  [" << vocab_size << "]\n"
              << "  -max_size                 maximum size to keep in courpus [" << max_size << "]\n"
              << "  -min_count                minimum times  [" << min_count << "]\n"
              << "  -window                   the window size to get common occurrence [" << window << "]\n"
              << "  -embed_size               embedded size  [" << embed_size << "]\n"
              << "  -epoch                    train epoch [" << epoch << "]\n"
              << "  -threads                  max length of char ngram [" << threads << "]\n"
              << "  -memory_limit             max length of char ngram [" << memory_limit << "]\n"
              << "  -lr                       max length of char ngram [" << lr << "]\n";
}



