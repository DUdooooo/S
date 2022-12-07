/**
 * Project SearchEngine
 */


#ifndef _PAGELIBPREPROCESSOR_H
#define _PAGELIBPREPROCESSOR_H
#include"WebPage.h"
#include<iostream>
#include<set>
#include<unordered_map>

using std::unordered_map;
using std::set;

class PageLibPreprocessor {

public: 
    PageLibPreprocessor(mySimHash & myh);
    ~PageLibPreprocessor();
    
/* void cutRedundantPage(); */
    
    /* void doProcess(); */
    void readInforFromFile();//读取xml文件，并且生成网页库和偏移库
    void bulidInvertIndexMap();//创建倒排索引
    
    void storeOnDisk();//将网页库、偏移库、倒排索引库写入磁盘
private: 
    mySimHash &_myh;

    set<WebPage> _pageList;//网页库对象
    unordered_map<int ,pair<int,int>> _offsetLib;//网页偏移库
    unordered_map<string,vector<pair<int,double>>>  _invertIndexLib;//倒排索引库:w

    /* SplitTool* _wordCutter; */
};

#endif //_PAGELIBPREPROCESSOR_H
