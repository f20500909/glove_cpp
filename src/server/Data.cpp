#include "Data.h"

Data::Data() {
	init();
}


void Data::init() {
	std::ifstream vocabFile("log/vocab.txt");
	std::ifstream wordvecFile("log/wordvec.txt");

	assert(vocabFile);
	assert(wordvecFile);

	std::string vocab;
	std::string wordvec;

	while (getline(vocabFile, vocab) && getline(wordvecFile, wordvec)) {
		m[vocab] = wordvec;
	}
}

std::string Data::getWordvec(std::string &str) {
    try{
		return m.at(str);
    }catch (std::out_of_range& error){
    	return "nullptr";
    }
}

Data::~Data() {

}

