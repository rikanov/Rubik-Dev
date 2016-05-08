#ifndef EDITOR_BUFFER_H
#define EDITOR_BUFFER_H

#include "globals.h"

class Buffer
{
public:
    Buffer();

    std::vector<String> lines;

    void insertLine(String, int);
    void appendLine(String);
    void removeLine(int);

    std::string remTabs(String);
};

#endif
