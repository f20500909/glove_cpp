#ifndef _SRC_UTIL_H_
#define _SRC_UTIL_H_

#include <algorithm>
#include <chrono>
#include <fstream>
#include <initializer_list>
#include <iostream>
#include <iterator>
#include <sstream>
#include <vector>

std::string lower(const std::string &str);

template <typename Container = std::vector<std::string>>
Container split(const std::string &str, const char &delimiter = ' ') {
    Container conts;

    std::size_t start = str.find_first_not_of(delimiter);
    std::size_t end = str.find_first_of(delimiter, start);

    while (start != std::string::npos && end != std::string::npos) {
        std::string sub = str.substr(start, end - start);
        if (!sub.empty()) {
            conts.emplace_back(std::move(sub));
        }
        start = str.find_first_not_of(delimiter, end);
        end = str.find_first_of(delimiter, start);
    }

    if (start != std::string::npos) {
        conts.emplace_back(std::move(str.substr(start)));
    }

    return conts;
}

template <typename Arg, typename... Args>
std::string join(const char &delimiter, const Arg &arg, const Args &... args) {
    if (!sizeof...(args)) {
        return arg;
    }

    std::ostringstream os;
    os << arg;
    static_cast<void>(
        std::initializer_list<int>{(os << delimiter << args, 0)...});

    return os.str();
}

std::string trim( const std::string &s, const char &delimiter = ' ', bool left = true, bool right = true);
std::string trim_left(const std::string &s, const char &delimiter = ' ');
std::string trim_right(const std::string &s, const char &delimiter = ' ');



namespace file {

template <class Stream>
Stream &open(Stream &stream, const std::string &file) {
    auto old_state = stream.exceptions();
    try {
        stream.exceptions(stream.badbit | stream.failbit);
        stream.open(file);
    } catch (const std::ios_base::failure &e) {
        throw std::runtime_error("failed to open file: " + file);
    }
    stream.exceptions(old_state);

    return stream;
}

}  // namespace file


namespace untils{
    void seek(std::ifstream &ifs, int pos) ;
    int size(std::ifstream &ifs) ;
    void seek(std::ifstream&, int64_t);
}

class Timer {
public:
    Timer() = default;
    std::chrono::steady_clock::time_point now() const {
        return std::chrono::steady_clock::now();
    }
    std::chrono::steady_clock::time_point start() {
        return start_time = std::chrono::steady_clock::now();
    }
    std::chrono::steady_clock::time_point stop() {
        return end_time = std::chrono::steady_clock::now();
    }

    template <class T = double>
    T elapsed() const {
        return std::chrono::duration_cast<std::chrono::duration<T>>(
                   end_time - start_time)
            .count();
    }

    template <typename F, typename... Args>
    static double profile(F func, Args &&... args) {
        Timer timer;
        timer.start();
        func(std::forward<Args>(args)...);
        timer.stop();
        return timer.elapsed();
    };

private:
    std::chrono::steady_clock::time_point start_time;
    std::chrono::steady_clock::time_point end_time;
};

#endif /* _SRC_UTIL_H_ */
