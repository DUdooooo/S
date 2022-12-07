#ifndef _MYSIMHASH_H_
#define _MYSIMHASH_H_

#include"../lib/simhash/Simhasher.hpp"
#include"../lib/simhash/cppjieba/KeywordExtractor.hpp"
#include"../lib/simhash/jenkins.h"
#include"Configuration.h"
#include<string>
#include <iostream>                                                                                                                    
#include <fstream>

using simhash::Simhasher;
using std::string;

//this define can avoid some logs which you don't need to care about.
#define LOGGER_LEVEL LL_WARN 

using namespace simhash;

class mySimHash
{
public:
    mySimHash() ;
    ~mySimHash() ;

    uint64_t getSimhash(const string & filename);
    /* unordered_map<string, int> cut(const string &text); */
    unordered_map<string, int>Splick(const string &doContent);
    
private:
    const static int TOPK_NUMBER;
    unordered_set<string> _stopWordList;
    Simhasher _simhasher;

    Jieba _jieba;
};

#endif
