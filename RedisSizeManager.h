#include <vector>

#include "ConnectedTool.h"

#ifndef PREPROJETSTAGE_REDISSIZEMANAGER_H
#define PREPROJETSTAGE_REDISSIZEMANAGER_H

class RedisSizeManager : public ConnectedTool {
public:
    void start();
    void start(int argc, char **argv);
    void interrupt();
    void setSets(std::vector<std::string> sets);

private:
    const int MAX_SET_SIZE = 150;
    std::vector<std::string> sets;
    volatile sig_atomic_t stop;

};

#endif //PREPROJETSTAGE_REDISSIZEMANAGER_H
