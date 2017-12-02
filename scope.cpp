#include "scope.h"

using namespace std;

Scope::Scope() = default;

Scope::~Scope() = default;

const vector<string> &Scope::getVariableNames() const {
    return variableNames;
}

void Scope::setVariableNames(const vector<string> &variableNames) {
    Scope::variableNames = variableNames;
}

const vector<Scope> &Scope::getChildScopes() const {
    return childScopes;
}

void Scope::setChildScopes(const vector<Scope> &childScopes) {
    Scope::childScopes = childScopes;
}
