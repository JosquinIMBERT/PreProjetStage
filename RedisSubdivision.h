#include <iostream>
#include <unistd.h>
#include <vector>

#ifndef PREPROJETSTAGE_REDISSUBDIVISION_H
#define PREPROJETSTAGE_REDISSUBDIVISION_H

namespace solution_subdivision {
    const int MAX_SET_SIZE = 150;
    const std::string EXTENSION = "_key";
    const int EXTENSION_LENGTH = EXTENSION.size();

    std::vector<std::string> split(char delimiter, std::string src);

    void add_set(int set_id);
    int add_key_to_set(int set_id);
    void print_sets();

    int main(int argc, char **argv);
}

#endif //PREPROJETSTAGE_REDISSUBDIVISION_H
