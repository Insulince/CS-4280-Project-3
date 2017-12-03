#include "scope.h"

using namespace std;

Scope::Scope(const int depth) : depth(depth) {
}

Scope::~Scope() = default;

const int Scope::getDepth() const {
    return depth;
}

vector<string> *Scope::getVariableNames() const {
    return variableNames;
}

void Scope::setVariableNames(vector<string> *variableNames) {
    Scope::variableNames = variableNames;
}

vector<string> *Scope::getThrowAwayVariableNames() const {
    return throwAwayVariableNames;
}

void Scope::setThrowAwayVariableNames(vector<string> *throwAwayVariableNames) {
    Scope::throwAwayVariableNames = throwAwayVariableNames;
}

const std::string Scope::toString() const {
    string output = to_string(depth) + "\n";

    for (unsigned int i = 0; i < variableNames->size(); i++) {
        output += "--- " + variableNames->at(i) + ": " + throwAwayVariableNames->at(i) + "\n";
    }

    return output;
}
