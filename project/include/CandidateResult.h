#ifndef __CANDIDATERESULT_H__
#define __CANDIDATERESULT_H__

#include <string>

using std::string;

class CandidateResult
{
public:
    string _word; //候选测
    int _freq;    //词频
    int _dist;    //与待查词的距离
};

#endif