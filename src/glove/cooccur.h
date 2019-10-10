#ifndef COOCCUR_H
#define COOCCUR_H

#include <list>
#include <memory>
#include <fstream>
#include <deque>
#include <iostream>
#include "utils.h"
#include "vocabulary.h"
#include "args.h"


class CoMat {
private:
	std::deque<unsigned long> ids;
	std::deque<bool> flags;
	std::string center;
	std::string context;
	uint32_t vocab_size;
	bool exists;
	unsigned long id=0;
	std::list<CoRec> low_cooccur;
	std::ifstream input;
	std::list<CoRec> high_cooccur;

	std::vector<unsigned long> index;
	std::vector<double> bigram_table;//    bigram_table存储高频的共现矩阵，用一维数组模拟二维数组
	int fidCounter = 0;
	int memory_limit;

	std::string input_file;

	unsigned long window = 10;

	long long max_product;
	long long overflow_length;

	std::string tempFileName="log/temp.bin_";


public:
	CoMat(unsigned long long vocab_size, std::string input_file
		, unsigned long long window,  int memory_limit
		 );

	void write_chunk(std::list<CoRec> &co, std::ofstream &out);

	void merge_files(int fileAllNums);

	void build(const Vocabulary &vocab);

	bool lookup(const std::string &key, unsigned long &id, const Vocabulary &vocab);

	void sliding_window(const Vocabulary &vocab, unsigned long &window);

	void write_high_cooccur(std::vector<double> &bigram_table);

	void initParas();

};

#endif /* _SRC_COOCCUR_H_ */
