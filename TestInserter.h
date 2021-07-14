//
// Created by josqu on 12/07/2021.
//

#include <iostream>
#include <queue>
#include <mutex>

#ifndef PREPROJETSTAGE_TESTINSERTER_H
#define PREPROJETSTAGE_TESTINSERTER_H

class TestInserter {
public:
    TestInserter(int inserter_id, std::mutex *mutex_stop);
    TestInserter(const TestInserter &inserter);
    void addElement(std::string value);
    int getId();
    void run();
    int operator()();
private:
    int inserter_id;
    std::queue<std::string> fifo;
    bool stop;
    std::mutex *mtx_id;
    std::mutex *mtx_fifo;
    std::mutex *mtx_stop;
};


#endif //PREPROJETSTAGE_TESTINSERTER_H
