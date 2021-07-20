//
// Created by josqu on 19/07/2021.
//

#include <iostream>
#include <unistd.h>
#include <mutex>

#include "TestThread.h"

using namespace std;

std::mutex* TestThread::mtx = new mutex();
std::mutex* TestThread::print_mtx = new mutex();

void TestThread::run() {
    while(!mtx->try_lock()) {
        print_mtx->lock();
        cout << "Running" << endl;
        print_mtx->unlock();
        sleep(1);
    }
    mtx->unlock();
    print_mtx->lock();
    cout << "Stopped" << endl;
    print_mtx->unlock();
}