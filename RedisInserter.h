#include <signal.h>
#include "ConnectedTool.h"

#ifndef PREPROJETSTAGE_REDISINSERTER_H
#define PREPROJETSTAGE_REDISINSERTER_H

class RedisInserter : public ConnectedTool {
public:
    void start();
    void start(int argc, char **argv);
    void setSets(std::vector<std::string> sets);
    void interrupt();

private:
    std::vector<std::string> sets;
    volatile sig_atomic_t stop;

};

#endif //PREPROJETSTAGE_REDISINSERTER_H
