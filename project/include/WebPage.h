/**
 * Project SearchEngine
 */
#include"../lib/simhash/cppjieba/Jieba.hpp"
#include"Configuration.h"
#include"rssRead.h"
#include"mySimHash.h"
/* #include"Logger.h" */
#include<string>
#include<vector>
#include<map>
#include<set>

using std::string;
using std::map;
using std::set;
using std::vector;

#ifndef _WEBPAGE_H
#define _WEBPAGE_H

/* class RssItem; */
/* class Configuration; */
/* class WordSegmentation; */
using namespace cppjieba;

class WebPage {
    friend bool operator==(const WebPage &lhs,  const WebPage & rhs);
    friend bool operator<(const WebPage &lhs,  const WebPage & rhs);
    friend class PageLibPreprocessor;
public:
    WebPage(string & doc,int docid,RssItem & rss,mySimHash &mysimhash,string docSummary);//rss 用来赋值文章id，content、title、link
    ~WebPage();

    //获取数据成员
    int getDocId() const;
    string getTile() const;
    string getUrl() const;
    string getContent() const;
    string getSummary() const;

    string getDoc();

    unordered_map<string,int> & getWordsMap();

    void processDoc();

    void Splick(const string text);//分词+词频
    
    

    void calcTopK(vector<string> & wordsVec,int K,set<string> & stopWordList);


private:
    const static int TOPK_NUMBER=20;

    string _doc;
    int _docid;
    string _docTitle;
    string _docUrl;
    string _doContent;
    string _docSummary;

    mySimHash &_mysimhash;

    vector<string> _topWords;

    unordered_map<string,int> _wordsMap;//保存每篇文档的所有词和词频

    uint64_t _simhash;//保存simhash值
    /* Jieba _jieba; */

};

#endif //_WEBPAGE_H
