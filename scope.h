#ifndef P3_SCOPE_H
#define P3_SCOPE_H

#include <string>
#include <vector>

class Scope {
private:
    const int depth;
    std::vector<std::string> *variableNames = new std::vector<std::string>();
    std::vector<std::string> *throwAwayVariableNames = new std::vector<std::string>();
public:
    Scope(const int depth);

    ~Scope();

    std::vector<std::string> *getVariableNames() const;

    void setVariableNames(std::vector<std::string> *variableNames);

    const int getDepth() const;

    std::vector<std::string> *getThrowAwayVariableNames() const;

    void setThrowAwayVariableNames(std::vector<std::string> *throwAwayVariableNames);

    const std::string toString() const;
};

#endif //P3_SCOPE_H
