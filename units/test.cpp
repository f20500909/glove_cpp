#include <iostream>
#include <memory>
#include <unordered_map>
#include <fstream>
#include <iomanip>
#include <vector>
#include<algorithm>
#include<utility>
#include <vector>
#include "vector.h"
#include "densematrix.h"
#include "utils.h"
#include <unistd.h>
#include <sstream>
#include <armadillo>

using namespace std;
typedef struct cooccur_rec {
    int word1;
    int word2;
    double val;
} CREC;

int main() {
    int memory_limit = 1.0;
    double rlimit, n = 1e5;
    long long max_product;
    long long overflow_length;

    //rlimit是1G的内存约能装下多少个CREC
    rlimit = 0.85 * (double) memory_limit * 1073741824 / (sizeof(CREC));
    while (fabs(rlimit - n * (log(n) + 0.1544313298)) > 1e-3) {
        n = rlimit / (log(n) + 0.1544313298);
    }
    max_product = (long long) n;
    overflow_length = (long long) rlimit / 6;

    cout << "rlimit :" << rlimit << endl;
    cout << "n :" << n << endl;
    cout << "max_product :" << max_product << endl;
    cout << "overflow_length :" << overflow_length << endl;

    return 0;
}