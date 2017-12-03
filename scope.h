#ifndef P3_SCOPE_H
#define P3_SCOPE_H

#include <string>
#include <vector>

class Scope {
private:
    std::vector<std::string> variableNames;

    std::vector<Scope> childScopes;
public:
    Scope();

    ~Scope();

    const std::vector<std::string> &getVariableNames() const;

    void setVariableNames(const std::vector<std::string> &variableNames);

    const std::vector<Scope> &getChildScopes() const;

    void setChildScopes(const std::vector<Scope> &childScopes);
};

#endif //P3_SCOPE_H
