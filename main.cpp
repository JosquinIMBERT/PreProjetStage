#include <iostream>
#include <thread>
#include <signal.h>
#include <unistd.h>
#include <vector>

#include "TestThread.h"

using namespace std;

//Variales globales
vector<TestThread> objects;
vector<thread> threads;

//Handler pour l'interruption du programme
void sigint_handler(int signum) {
    TestThread::mtx->unlock();
}

int main(int argc, char **argv) {
    int nb_threads = 50;

    //Mutex bloqué par défaut, à débloquer pour terminer le programme
    TestThread::mtx->lock();

    //Handler pour la terminaison du programme
    signal(SIGINT, sigint_handler);

    //Création des threads
    for(int i=0; i<nb_threads; i++) {
        objects.push_back(TestThread());
        threads.push_back(thread(&TestThread::run, ref(objects.at(i))));
    }

    //Attente des threads
    for(int i=0; i<nb_threads; i++) {
        threads.at(i).join();
    }

    return 0;
}