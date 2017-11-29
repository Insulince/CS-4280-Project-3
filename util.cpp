#include "util.h"

using namespace std;

const string trim(const string &toBeTrimmed) {
    string trimmed;

    for (char charInToBeTrimmedString : toBeTrimmed) {
        if (charInToBeTrimmedString != ' ' && charInToBeTrimmedString != '\n') {
            trimmed += charInToBeTrimmedString;
        }
    }

    return trimmed;
}