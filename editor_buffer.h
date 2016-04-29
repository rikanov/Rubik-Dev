#ifndef EDITOR_BUFFER_H
#define EDITOR_BUFFER_H

#include "globals.h"

using namespace std;

class Buffer
{
public:
    Buffer();

    vector<String> lines;

    void insertLine(String, int);
    void appendLine(String);
    void removeLine(int);

    string remTabs(String);
};

#endif
