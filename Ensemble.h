#include <iostream>
#include <unistd.h>
#include <set>

#include "Couple.h"

#ifndef PREPROJETSTAGE_ENSEMBLE_H
#define PREPROJETSTAGE_ENSEMBLE_H

class Ensemble {
public:
    Ensemble();
    void add(std::string label);
    void del(int id);
    void del(std::string label);
    int size();
    std::string get(int id);
    std::pair<int, std::string> getOlderCouple();

private:
    int cpt;
    std::set<std::pair<int, std::string>> ensemble;
    std::set<int> e_ages;
    std::set<std::string> e_labels;
};

#endif //PREPROJETSTAGE_ENSEMBLE_H
