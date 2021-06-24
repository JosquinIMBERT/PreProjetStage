#include <iostream>
#include <thread>
#include <signal.h>

#include "RedisUsingEvents.h"
#include "RedisSubdivision.h"
#include "RedisZSet.h"

#include "RedisSizeManager.h"
#include "RedisInserter.h"

using namespace std;

RedisSizeManager RSM;
RedisInserter RI;

void sizeManager() {
    RSM.start();
}

void dataInserter() {
    RI.start();
}

void interrupt(int signum) {
    RSM.interrupt();
    RI.interrupt();
}

int main(int argc, char **argv) {
    signal(SIGINT, interrupt);
    vector<string> sets = {"set1", "set2"};
    RSM.setSets(sets);
    RI.setSets(sets);

    thread sizeManagerThread = thread(sizeManager);
    thread inserterThread = thread(dataInserter);

    sizeManagerThread.join();
    inserterThread.join();

    return 0;//solution_zset::main(argc, argv);
}
