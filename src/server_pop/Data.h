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

	std::string getRespone(std::vector<std::string>& words){
		std::string res = "";
		for (int i = 0; i < words.size(); i++) {
			res += getWordvec(words[i]) ;
			if(i!=words.size()){
				res+=" ";
			}
		}
		return res;
	}

private:
	std::unordered_map<std::string, std::string> m;

	void init();

};


#endif
