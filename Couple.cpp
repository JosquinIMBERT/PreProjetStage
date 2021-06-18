#include "Couple.h"

using namespace std;

Couple::Couple(int age, string label) {
    this->age = age;
    this->label = label;
}

int Couple::getAge() {
    return age;
}

string Couple::getLabel() {
    return label;
}

void Couple::setAge(int age) {
    this->age = age;
}

void Couple::setLabel(string label) {
    this->label = label;
}

bool Couple::operator<(Couple& other) {
    return this->age < other.getAge();
}

