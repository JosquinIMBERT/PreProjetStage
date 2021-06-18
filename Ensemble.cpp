#include <iostream>
#include <unistd.h>
#include <set>

#include "Ensemble.h"

using namespace std;

Ensemble::Ensemble() {
    this->cpt = 0;
    this->ensemble = set<pair<int, string>>();
}
void Ensemble::add(std::string label) {
    //TODO regarder si le label est déjà dans les données
    pair<int, string> c(cpt, label);
    this->ensemble.insert(c);
    this->cpt++;
}
void Ensemble::del(int id) {
    //this->ensemble.erase();
}
void Ensemble::del(std::string label) {

}

int Ensemble::size() {
    return this->ensemble.size();
}

string Ensemble::get(int id) {
    for(auto item : this->ensemble) {
        if(item.first==id)
            return item.second;
    }
    return "";
}

pair<int, string> Ensemble::getOlderCouple() {
    return pair<int, string>();
}

