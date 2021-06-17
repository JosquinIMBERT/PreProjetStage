#include <iostream>
#include <unistd.h>

#ifndef PREPROJETSTAGE_REDISUSINGEVENTS_H
#define PREPROJETSTAGE_REDISUSINGEVENTS_H

namespace solution_events {
    const std::string EXTENSION = "_key";
    const int EXTENSION_LENGTH = EXTENSION.size();

    std::string extract_label(std::string msg);
    void listenKeyspaceEvents();

    int main(int argc, char **argv);
}

#endif //PREPROJETSTAGE_REDISUSINGEVENTS_H
