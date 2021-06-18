#include <iostream>
#include <unistd.h>

#ifndef PREPROJETSTAGE_COUPLE_H
#define PREPROJETSTAGE_COUPLE_H

class Couple {
public:
    Couple(int age, std::string label);
    int getAge();
    std::string getLabel();
    void setAge(int age);
    void setLabel(std::string label);
    bool operator<(Couple& other);

private:
    int age;
    std::string label;
};

#endif //PREPROJETSTAGE_COUPLE_H
