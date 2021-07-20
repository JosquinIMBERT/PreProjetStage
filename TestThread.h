//
// Created by josqu on 19/07/2021.
//

#include <iostream>
#include <mutex>

#ifndef PREPROJETSTAGE_TESTTHREAD_H
#define PREPROJETSTAGE_TESTTHREAD_H


class TestThread {
public:
    static std::mutex *mtx;
    static std::mutex *print_mtx;
    void run();
};


#endif //PREPROJETSTAGE_TESTTHREAD_H
