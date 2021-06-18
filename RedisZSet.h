#include <cassandra.h>

#ifndef PREPROJETSTAGE_REDISZSET_H
#define PREPROJETSTAGE_REDISZSET_H

namespace solution_zset {
    void add_key_value(std::string set, std::string key, std::string value, int timestamp);
    void cass_statement_bind_int(CassStatement_ *statement, size_t index, int integer);
    int main(int argc, char **argv);
}

#endif //PREPROJETSTAGE_REDISZSET_H
