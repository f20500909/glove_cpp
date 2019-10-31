#include "cooccur.h"
#include "priority_queue.h"


//生成共现三元组的主程序
//如果使用二维矩阵储存共现矩阵,则二维矩阵的左上角稠密(高频词共现次数较多),而右下角稀疏(低频词共现次数较少),二维矩阵总体来说太过稀疏
//如果使用三元组(id1,id2,value)来储存,左上角的共现三元组次数又太多,重复更新左上角的三元组效率也太低了
//因此采用混合储存然后合并的方式进行储存,左上角的高频词使用二维数组储存,其余的低频词使用三元组储存,全部语料处理完成后,二维数组转化为三元组,合并高频三元组和低频三元组即得共现矩阵


//将优先队列中的顶端节点写入文件,相加重复的元素值;
int merge_write(CRECID _new, CRECID &old, std::ofstream &fout) {
//    去重,将重复的权重相加
    if (_new == old) {
        old.val += _new.val;
        return 0;
    }
    fout.write(reinterpret_cast<char *>(&old), sizeof(CREC));
    old = _new;
    return 1;
}

void CoMat::build(const Vocabulary &vocab) {
    std::ifstream input(input_file);
    assert(input);
//    先读取一个词汇
    long long lowCount=0;

    input >> context;

    exists = false;
    ids.push_back(exists ? id : 0);
    flags.push_back(exists);

    std::ofstream foverflow(tempFileName + std::to_string(fidCounter));

    //调试时设小一点
//    overflow_length = 10000;

    while (!input.eof()) {
        //  将低频词汇分成多个文件写入
        if (lowCount >= overflow_length) {
//            如果文件满了,排序,重新打开一个文件写入,fidCounter自增以区分文件
            std::ofstream foverflow(tempFileName + std::to_string(fidCounter), std::ios::out | std::ios::binary);
            write_chunk(low_cooccur, foverflow);
            fidCounter++;
            lowCount = 0;
        }

        input >> center;

        //如果只有一个词,跳过
        if (center.empty()) {
            continue;
        }

//        此处id在looup时已经被修改过了,id是中心词的词id
        exists = lookup(center, id, vocab);
        if (exists) {
            for (uint32_t i = 0; i != ids.size(); ++i) {
                // If context word is OOV, skip
                if (!flags[i]) {
                    continue;
                }
                double weight = 1.0 / (ids.size() - i);
//                threshold的默认值是 5000* 5000   id是中心词的词序,ids[i]是当前词的词序
//                如果当前词的id比较小,则前一项大,则flag返回true,此时id对应的词的词频也越高
                bool isHigh = (max_product / (id + 1) >= (ids[i] + 1));
//                std::cout << "threshold :" << threshold / (id + 1) << "ids[i] + 1"<<ids[i] + 1<<std::endl;
                if (isHigh) {
                    //如果为共现次数较多的三元组
//                    std::cout<<"index: "<<index[id] + ids[i]<<std::endl;
                    bigram_table[index[id] + ids[i]] += weight;
                } else {
                    //如果为共现次数较少的三元组
                    low_cooccur.emplace_back(id, ids[i], weight);
                    lowCount++;
                }
                //去掉了镜像的选项
                //TODO :加上镜像提取共现矩阵的功能,最好对比一便加上镜像和不加上镜像效果差了多少
            }
        }
        //滑动窗口提词
        sliding_window(vocab, window);
    }
    //将没有写完的低频词写入文件块,此时写入的文件只有一个,是0到overflow_length之间的一个数字
    write_chunk(low_cooccur, foverflow);

    //将高频词写入文件,也只有一个
    //TODO 这块的逻辑好像有点问题，高频部分和低频部分好像公用的是一个文件，不知道有没有问题，先放着再说
    write_high_cooccur(bigram_table);

    //合并临时文件
    merge_files(fidCounter + 1 );

    return;
}

//写入一个临时文件
void CoMat::write_chunk(std::list<CoRec> &co, std::ofstream &out) {
    co.sort();
    while (!co.empty()) {
        CoRec cur = std::move(co.front());
        co.pop_front();
        //如果是重复的，将权重相加
        if (cur == co.back()) {
            co.back() += cur;
        } else {
            out.write((char *) &(cur.i), sizeof(uint32_t));
            out.write((char *) &(cur.j), sizeof(uint32_t));
            out.write((char *) &(cur.weight), sizeof(double));
        }
    }
    co.clear();
    out.close();
}

//写入左上角矩阵
void CoMat::write_high_cooccur(std::vector<double> &bigram_table) {
    //如果是共现矩阵左上角高频共现词
    for (uint32_t i = 0; i != vocab_size; ++i) {
        for (uint32_t j = 0; j != index[i + 1] - index[i]; ++j) {
//            std::cout << "bigram_table..." << bigram_table.size() << std::endl;
            double weight = bigram_table[index[i] + j];
            if (weight > 0) {
                high_cooccur.emplace_back(i, j, weight);
            }
        }
    }
//    INFO("bigram_table  is :" , bigram_table.size());
    TRACE("bigram_table  is :", bigram_table.size());
    std::ofstream out(tempFileName + std::to_string(fidCounter), std::ios::out | std::ios::binary);
    assert(out);
    for (const CoRec &co:high_cooccur) {
        out.write((char *) &(co.i), sizeof(uint32_t));
        out.write((char *) &(co.j), sizeof(uint32_t));
        out.write((char *) &(co.weight), sizeof(double));
    }
    out.close();
    high_cooccur.clear();
}

//合并所有的临时文件
void CoMat::merge_files(int fileAllNums) {
    TRACE("merge.... file fileAllNumss is :  ", fileAllNums);
    //nums是现有临时文件的数量,i是临时变量
    int i, nums = fileAllNums;
    long long counter = 0;
    CRECID _new, old;
//    优先队列,小根堆
    priority_queue que(fileAllNums);
    std::ofstream fout("log/cooccur.bin", std::ios::out | std::ios::binary);

//打开所有文件,保存到fid中
    std::ifstream fin[fileAllNums];
    for (i = 0; i < fileAllNums; i++) {
        fin[i].open(tempFileName + std::to_string(i), std::ios::out | std::ios::binary);
    }

//将所以文件的第一个词汇加入到优先队列中去
    for (i = 0; i < fileAllNums; i++) {
        fin[i].read(reinterpret_cast<char *>(&_new), sizeof(CREC));
        _new.fileId = i;
        //注意此处插入的i需要加1,i是从0开始的
        que.insert(_new, i + 1);
    }

//弹出堆的顶部节点,保存它,以判断下一个是否是重复的
    old = que.top();
//    i记录了文件id
    i = que.top().fileId;
    que.remove(nums-1);
    fin[i].read(reinterpret_cast<char *>(&_new), sizeof(CREC));
    //文件写完了，将nums减一，最后再统一关闭文件
    if (!(fin[i])) nums--;
    else {
        _new.fileId = i;
        //加入优先队列，此处会调整堆
        que.insert(_new, nums);
    }

    //重复操作，直到所有文件都被写完
    while (nums > 0) {
        //counter计数,计算一共有多少共现词汇的数目
        counter += merge_write(que.top(), old, fout);
        i = que.top().fileId;
        que.remove(nums-1);
        fin[i].read(reinterpret_cast<char *>(&_new), sizeof(CREC));
        if (!(fin[i])) nums--;
        else {
            _new.fileId = i;
            que.insert(_new, nums);
        }
    }

//    写最后一个数据
    fout.write(reinterpret_cast<char *>(&old), sizeof(CREC));
    fout.close();

//    合并完毕,关闭所有文件
    for (i = 0; i < fileAllNums; i++) fin[i].close();

    TRACE("merge  down all cooccur number is", counter);
    return;
}


CoMat::CoMat(unsigned long long vocab_size, std::string input_file,
             unsigned long long window, int memory_limit
) : vocab_size(vocab_size),
    index(vocab_size + 1, 0),
    input_file(input_file),
    window(window),
    memory_limit(memory_limit)
 {
    initParas();
    TRACE("index.size() :", index.size());
};


bool CoMat::lookup(const std::string &key, unsigned long &id, const Vocabulary &vocab) {
    try {
        id = vocab[key];
//        printf("id :%ld,\n",id);
    } catch (const std::out_of_range &e) {
//        printf("not in...");
        return 0;
    }
    return 1;
};

void CoMat::sliding_window(const Vocabulary &vocab, unsigned long &window) {
    // If line breaks, separate context between paragraphs
    //这一行结束,清空ids和flags,重新读取一个词到ids里面
//        peek函数用于读取并返回下一个字符，但并不移动流指针
    if (input.peek() == '\n') {
        ids.clear();
        flags.clear();
        input >> context;
        exists = lookup(center, id, vocab);
        ids.push_back(exists ? id : 0);
        flags.push_back(exists);
        center = "";
    } else {
        // Remove the oldest history
//            如果超出滑动窗口的上限 window 则移除顶部数据
        if (ids.size() >= window) {
            ids.pop_front();
            flags.pop_front();
        }
        // Current center word becomes history
//            如果没有到换行,也没有达到滑动窗口的上限,则将id压如ids中;
        ids.push_back(id);
        flags.push_back(exists);
    }
}


void CoMat::initParas() {
//    TODO  以后搞清楚怎么计算,这段代码从源代码中直接copy的
    double rlimit, n = 1e5;

    rlimit = 0.85 * (double) memory_limit * 1073741824 / (sizeof(CREC));
    while (fabs(rlimit - n * (log(n) + 0.1544313298)) > 1e-3) {
        n = rlimit / (log(n) + 0.1544313298);
    }
    max_product = (long long) n;
    overflow_length = (long long) rlimit / 6;


    //首位置零,从1开始
    index[0] = 0;
    for (uint32_t i = 1; i != vocab_size + 1; ++i) {
        index[i] = index[i - 1] + std::min((int) max_product / i, vocab_size);
    }

    bigram_table = std::vector<double>(index[vocab_size], 0);
    std::cout << "bigram_table..." << bigram_table.size() << std::endl;
    return;
};