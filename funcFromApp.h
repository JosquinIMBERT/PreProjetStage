//
// Created by josqu on 24/06/2021.
//

#ifndef PREPROJETSTAGE_FUNCFROMAPP_H
#define PREPROJETSTAGE_FUNCFROMAPP_H

#include <iostream>
#include <unistd.h>
#include <unordered_map>
#include <bgpstream.h>
#include <vector>
#include <boost/algorithm/string.hpp>

class PrefixPath {
public:
    unsigned int hash=0;
    char pathLength;
    char shortPathLength;
    unsigned int *shortPath;
    unsigned int prefNum = 0;
    short int collector;
    bool active = true;
    unsigned int lastChange = 0;
    double globalRisk=0.0;
    double meanUp=0.0, meanDown=0.0;

    PrefixPath(std::string str);
    PrefixPath(std::vector<unsigned int> &pathVect);
    void update(std::string str);
    PrefixPath();
    ~PrefixPath();
    void setHash(unsigned int h);
    double getScore() const;
    std::string str() const;
    std::string sstr() const;
    int size_of();
    unsigned int getPeer() const;
    unsigned int getDest() const;
    bool addPrefix(unsigned int time);
    bool erasePrefix(unsigned int time);
    bool checkPrefix(bgpstream_pfx_t *pfx);
    void setPathActive(unsigned int time);
    void setPathNonActive(unsigned int time);
    void fromRedis(std::string str);
    void saveToRedis(unsigned int timestamp);
};

#endif //PREPROJETSTAGE_FUNCFROMAPP_H
