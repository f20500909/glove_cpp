#ifndef MAIN_DATA_H
#define MAIN_DATA_H

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cassert>
#include <unordered_map>
#include <vector>
#include <stdlib.h>
#include <stdio.h>


class Data {
public:
	Data();;

	~Data();

	std::string getWordvec(std::string &str);

private:
	std::unordered_map<std::string, std::string> m;

	void init();

};


#endif
