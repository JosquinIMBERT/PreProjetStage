//
// Created by josqu on 12/07/2021.
//

#include <iostream>
#include <queue>
#include <unistd.h>
#include <thread>

#include "TestInserter.h"

using namespace std;

TestInserter::TestInserter(int inserter_id, mutex *mutex_stop) {
    cout << "Création inserter " << inserter_id << endl;
    this->inserter_id = inserter_id;
    this->fifo = queue<string>();
    this->stop = false;
    mutex mutex_fifo;
    this->mtx_fifo = &mutex_fifo;
    mutex mutex_id;
    this->mtx_id = &mutex_id;
    this->mtx_stop = mutex_stop;
}

TestInserter::TestInserter(const TestInserter &inserter) {
    this->inserter_id = inserter.inserter_id;
    this->fifo = inserter.fifo;
    this->stop = inserter.stop;
    this->mtx_fifo = inserter.mtx_fifo;
    this->mtx_id = inserter.mtx_id;
    this->mtx_stop = inserter.mtx_stop;
}

void TestInserter::addElement(string value) {
    mtx_fifo->lock();
    this->fifo.push(value);
    mtx_fifo->unlock();
}

int TestInserter::getId() {
    return this->inserter_id;
}

void TestInserter::run() {
    cout << "Lancement du thread " << inserter_id << endl;
    while(!mtx_stop->try_lock()) {
        if(this->fifo.empty()) {
            sleep(1);
        } else {
            mtx_fifo->lock();
            string element = this->fifo.front();
            this->fifo.pop();
            mtx_fifo->unlock();
            cout << "Thread " << inserter_id << " : " << element << endl;
        }
    }
    mtx_stop->unlock();
    cout << "Fin du thread " << inserter_id << endl;
}

int TestInserter::operator()() {
    cout << "Running my object with id " << inserter_id << endl;
    return 0;
}