#include <iostream>
#include <boost/asio.hpp>
#include "tsqueue.h"
#include <thread>

template<typename T>
void foo(tsqueue<T> &dq){
    std::cout << std::this_thread::get_id() << std::endl;
    try{
        std::cout << dq.pop_back() << std::endl;
    }
    catch(std::error_code &ec){
        std::cout << ec.message() << std::endl;
    }

}

int main(int, char**) {
    tsqueue<int> t;
    t.push_back(10);
    t.push_back(11);
    t.push_back(12);
    std::cout << std::this_thread::get_id() << std::endl;
    std::thread thr1(foo<int>, std::ref(t));

    std::thread thr2(foo<int>, std::ref(t));

    thr1.join();
    thr2.join();

    return 0;
}
