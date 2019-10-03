#include <algorithm>
#include <chrono>
#include <fstream>
#include <iostream>
#include <random>
#include <sstream>
#include <memory>
#include <unordered_map>
#include <cassert>
#include <string>
#include "vocabulary.h"
#include "utils.h"
//#include "args.h"

bool operator<(const WordFreq &w1, const WordFreq &w2) {
	return w1.second < w2.second || w1.first < w2.first;
}

// Vocabulary
Vocabulary::Vocabulary(unsigned long min_count, unsigned long long max_size, bool keep_case, std::string input_file)
				: min_count(min_count), max_size(max_size), keep_case(keep_case), input_file(input_file) {

	build();
	sort("desc");
}

Vocabulary::Vocabulary() {}


void Vocabulary::build(const std::vector<WordFreq> &v) {
	clear();
	for (const auto &p : v) {
		add(p.first, p.second);
	}
}

void Vocabulary::build() {
	std::ifstream ifs;
	file::open(ifs, input_file);

	std::string line;
	std::unordered_map<std::string, size_t> counts;
	std::string key;

	// Statistics
	while (getline(ifs, line)) {
		std::vector<std::string> &&_words = split(line);

		for (const auto &word : _words) {
			key = keep_case ? word : lower(word);
			++counts[key];
		}
	}
	ifs.close();

	// Remove low frequencies
	std::vector<WordFreq> vec;
	std::copy_if( counts.begin(), counts.end(), std::back_inserter(vec), [&](const WordFreq &w) { return w.second >= this->min_count; });

	// Trim to max size
	std::sort(vec.begin(), vec.end(), [](const WordFreq &w1, const WordFreq &w2) { return w1.second > w2.second; });
	vec.erase(min(vec.begin() + max_size, vec.end()), vec.end());

	build(vec);
	return;
}

void Vocabulary::add(const std::string &word, long long freq) {
	std::string key = keep_case ? word : lower(word);
	if (has(key, true)) {
		this->freq[key] += freq;
	} else if (!full()) {
		std::uint32_t id = size();
		this->freq[key] += freq;
		this->atoi[key] = id;
		this->itoa[id] = key;
	}
	return;
}

bool Vocabulary::has(const std::string &word) const {
	return freq.count(keep_case ? word : lower(word)) > 0;
}

bool Vocabulary::has(const std::string &word, bool ignore_case) const {
	// If `ignore_case` is `true`, find directly, else respect the private
	// member `keep_case`
	return ignore_case ? freq.count(word) > 0 : has(word);
}

void Vocabulary::merge(const Vocabulary &other) {
	return merge(other.freq);
}

void Vocabulary::merge(const WordMap &freq) {
	for (const auto &it : freq) {
		add(it.first, it.second);
	}
	return;
}

void Vocabulary::sort(const std::string &order) {
	std::vector<WordFreq> vec(std::make_move_iterator(freq.begin()), std::make_move_iterator(freq.end()));

	if (order == "desc" || order == "asc") {
		auto compare = [&order](const WordFreq &w1, const WordFreq &w2) {
				return order == "desc" ? w1.second > w2.second : w1.second < w2.second; // TODO: by freq and alphabet//
				 };
				std::sort(vec.begin(), vec.end(), compare);

		} else if (order == "rand") {
			unsigned seed =
							std::chrono::system_clock::now().time_since_epoch().count();
			std::shuffle(vec.begin(), vec.end(), std::default_random_engine(seed));
		}

		return build(vec);
	}

	std::set<std::string> Vocabulary::words() const {
		std::set<std::string> keys;
		for (const auto &word : freq) {
			keys.insert(word.first);
		}
		return keys;
	}

	std::uint32_t Vocabulary::size() const {
		return itoa.size();
	}

	bool Vocabulary::full() const {
		return freq.size() >= max_size;
	}

	void Vocabulary::clear() {
		freq.clear();
		itoa.clear();
		atoi.clear();
	}

	void Vocabulary::to_txt(const std::string &file) const {
//    std::ofstream os(file);
//
//    //将unordered_map 中的数据进行有序排序
//    std::map<int, int> tmp;
//    std::transform(freq.begin(), freq.end(), std::inserter(tmp, tmp.begin()),
//                   [](std::pair<int, int> a) { return std::pair<int, int>(a.second, a.first); });
//
//    std::cout<<tmp.max_size()<<std::endl;
//    std::cout<<"==="<<std::endl;
//    for (auto m:tmp) {
//        std::cout << m.first << std::endl;
//    }
//
//    for (const auto &it : freq) {
//        os << it.first << " " << it.second << std::endl;
//    }
//    os.close();
//
//    std::ofstream os1, os2;
//    file::open(os1, file + ".itoa");
//    file::open(os2, file + ".atoi");
//    for (const auto &it : itoa) {
//        os1 << it.first << " " << it.second << std::endl;
//        os2 << it.second << " " << it.first << std::endl;
//    }
//    os1.close();
//    os2.close();
	}


	WordMap::iterator Vocabulary::begin() {
		return freq.begin();
	}

	WordMap::iterator Vocabulary::end() {
		return freq.end();
	}

	WordMap::const_iterator Vocabulary::cbegin() {
		return freq.cbegin();
	}

	WordMap::const_iterator Vocabulary::cend() {
		return freq.cend();
	}

	std::string Vocabulary::operator[](const std::uint32_t &i) const {
		return itoa.at(i);
	}

	std::uint32_t Vocabulary::operator[](const std::string &w) const {
		return atoi.at(w);
	}

	std::ostream &operator<<(std::ostream &os, const Vocabulary &vocab) {
		os << "freq:" << std::endl;
		for (const auto &it : vocab.freq) {
			os << it.first << " " << it.second << std::endl;
		}
		os << "itoa:" << std::endl;
		for (const auto &it : vocab.itoa) {
			os << it.first << " " << it.second << std::endl;
		}
		os << "atoi:" << std::endl;
		for (const auto &it : vocab.atoi) {
			os << it.first << " " << it.second << std::endl;
		}

		return os;
	}

	void Vocabulary::load() {
//    std::string vocab = file + ".voc";

		std::ifstream in("vocab.txt");
		assert(in);
		std::cout << "load from " << "vocab.txt" << "....." << std::endl;

		std::string temp;
		uint32_t id = 0;
		while (getline(in, temp)) {
			in >> temp;
			atoi[temp] = id;
			itoa[id] = temp;
			id++;
		}
		in.close();
	};