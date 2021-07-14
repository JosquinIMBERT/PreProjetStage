#include <iostream>
#include <thread>
#include <signal.h>

#include "RedisUsingEvents.h"
#include "RedisSubdivision.h"
#include "RedisZSet.h"
#include "TestInserter.h"

#include "RedisSizeManager.h"
#include "RedisInserter.h"

using namespace std;

static vector<TestInserter> inserters;
std::mutex mtx_stop;

void interrupt(int signum) {
    cout << "Ending program (" << inserters.size() << " thread(s))" << endl;
    mtx_stop.unlock();
}

void threadLauncher(int i) {
    inserters.at(i).run();
}

void thread_test() {
    cout << "Début du thread test" << endl;
    sleep(2);
    cout << "Fin du thread test" << endl;
}

int main(int argc, char **argv) {
    int nb_inserters = 3;
    vector<thread> threads;
    mtx_stop.lock();
    for(int i=0; i<nb_inserters; i++) {
        inserters.push_back(TestInserter(i, &mtx_stop));
        //threads.push_back(thread(threadLauncher, i));
        threads.push_back(thread(threadLauncher, i));
    }

    signal(SIGINT, interrupt);

    inserters.at(0).addElement("élément_0");
    /*if(inserters.size()>0) {
        int i = 0;
        while (i < 200) {
            char str[20];
            strcpy(str,"élément_");
            strcat(str,to_string(i).c_str());
            inserters.at(i % inserters.size()).addElement(str);
            cout << "On insère élément_" << i << " dans l'inserter " << i % inserters.size() << endl;
            i++;
        }
    }*/

    for(int i=0; i<threads.size(); i++) {
        threads.at(i).join();
    }

    return 0;
}


    /*vector<string> sets = {"set1", "set2"};
    RSM.setSets(sets);
    RI.setSets(sets);

    thread sizeManagerThread = thread(sizeManager);
    thread inserterThread = thread(dataInserter);

    sizeManagerThread.join();
    inserterThread.join();

    return 0;//solution_zset::main(argc, argv);*/

