#ifndef _SPLITTOOL_H
#define _SPLITTOOL_H

#include<vector>
#include<string>

using std::vector;
using std::string;

class SplitTool
{
    public:
     SplitTool(){};
    virtual vector<string> cut(const string&sentence)=0;

    private:
    string  _sentence;
};

#endif
