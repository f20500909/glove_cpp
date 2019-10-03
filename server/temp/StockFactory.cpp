//#include <boost/bind.hpp>
//#include <boost/enable_shared_from_this.hpp>
//#include <boost/shared_ptr.hpp>
//#include <boost/weak_ptr.hpp>

#include "Mutex.h"

#include <assert.h>
#include <stdio.h>
#include <string>
#include <memory>
#include <map>

using std::string;
using namespace std::placeholders;


class Stock  {
public:
    Stock(const string& name) : name_(name) {
        printf(" Stock[%p] %s\n", this, name_.c_str());
    }
    ~Stock() {
        printf("~Stock[%p] %s\n", this, name_.c_str());
    }

    const string& key() const { return name_;}

private:
    string name_;
};

// 对象工场

class StockFactory : public std::enable_shared_from_this<StockFactory> {
public:
    std::shared_ptr<Stock> get(const string& key){
        std::shared_ptr<Stock> pStock;
        MutexLockGuard lock(mutex_); //加锁
        std::weak_ptr<Stock>& wkStock = stocks_[key]; // 注意，wkStock是引用
        pStock = wkStock.lock(); //提升

        if(!pStock){ //提升失败，对象还不存在
            pStock.reset(new Stock(key), 
                         std::bind(&StockFactory::weakDeleteCallback,
                         std::weak_ptr<StockFactory>(shared_from_this()), _1) );
            //上面必须强制吧shared_from_this转型为weak_ptr, 才不会延长生命期
            // 因为boost::bind拷贝的实参类型，而不是形参类型
            wkStock = pStock; //因为wkStock是一个引用，所以这一步操作会在stocks_中插入新的key-value
        }
        return pStock;
    }


private:
    static void weakDeleteCallback(const std::weak_ptr<StockFactory>& wkFactory, Stock* stock) {
        std::shared_ptr<StockFactory> factory(wkFactory.lock()); //尝试提升
        if(factory){ //如果factory还在，那就清理stocks_
            factory->removeStock(stock);
        }
        delete stock; //sorry, here is a delete...
    }

    void removeStock(Stock* stock){
        if(stock) {
            MutexLockGuard lock(mutex_);
            stocks_.erase(stock->key());
        }
    }

    mutable MutexLock mutex_;
    std::map<string, std::weak_ptr<Stock> > stocks_;
};


void testLongLifeFactory() {
    std::shared_ptr<StockFactory> factory(new StockFactory);
    
    {
        std::shared_ptr<Stock> stock = factory->get("NYSE:IBM");
        std::shared_ptr<Stock> stock2 = factory->get("NYSE:IBM");
        assert(stock == stock2);
        //stock destructs here
    }
    //factory destructs here
}

void testShortLifeFactory() {
    std::shared_ptr<Stock> stock;

    {
        std::shared_ptr<StockFactory> factory(new StockFactory);
        stock = factory->get("NYSE:IBM");
        std::shared_ptr<Stock> stock2 =factory->get("NYSE:IBM");
        assert(stock == stock2);
        // factory destucts here
    }
    // stock destructs here
}


int main4() {

    testLongLifeFactory();

    testShortLifeFactory();

    return 0;
}