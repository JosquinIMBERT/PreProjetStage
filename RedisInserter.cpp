#include <cassandra.h>
#include <iostream>
#include <sw/redis++/redis++.h>
#include <unistd.h>
#include <vector>

#include "RedisInserter.h"

using namespace sw::redis;
using namespace std;


void RedisInserter::setSets(vector<string> sets) {
    this->sets = sets;
}

void RedisInserter::interrupt() {
    this->stop = 1;
}

void RedisInserter::start() {
    start(0, NULL);
}

void RedisInserter::start(int argc, char **argv) {
    init_connections(argc, argv);

    //TODO, insérer des données
    int i=0;
    while(!stop) {
        cout << "Adding Couple " << i << ", val" << i << endl;
        redis.set(to_string(i), "val"+to_string(i));
        redis.zadd(sets.at(i%sets.size()), to_string(i), i);
        i++;
        sleep(1);
    }

    end_connections();
}