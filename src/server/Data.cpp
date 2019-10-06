#include "Data.h"

Data::Data() {
	init();

}


void Data::init() {
	std::ifstream vocabFile("../data/vocab.txt");
	std::ifstream wordvecFile("../data/wordvec.txt");

	assert(vocabFile);
	assert(wordvecFile);

	std::string vocab;
	std::string wordvec;

	while (getline(vocabFile, vocab) && getline(wordvecFile, wordvec)) {
		m[vocab] = wordvec;
	}
}

std::string Data::getWordvec(std::string &str) {
	return m[str];
}

Data::~Data() {

}
