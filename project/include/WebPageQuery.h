#ifndef __WEBPAGEQUERY_H_
#define __WEBPAGEQUERY_H_
#include"mySimHash.h"
#include"WebPage.h"

class WebPageQuery
{
public:
    WebPageQuery(const string & str); 
    ~WebPageQuery();

    void loadLibary();//加载库文件

    //用来实现加载倒排索引库
    void Libary(const string & str);

    void offsetLibary();//加载偏移库 
    void creatPageLibary(const string &);//加载网页库

    double Cos(const string &);//计算余弦相似度
    
    void keyWeight(const string&);//计算每个关键词的权重
    
    map<string,map<string,string>> getHashString(); 
    void sortRecPage();
    map<string,map<string,string>> _RecPage;
    map<string,string> _content;
    size_t nBytesCode(const char ch);
private:
    string _summary;//传入的关键字


    mySimHash _simhash;

    set<WebPage> _pageLib;//网页库

    unordered_map<int,pair<int,int>> _offsetLib;//偏移库
    unordered_map<string,set<pair<int,double>>> _invertIndexTable;//倒排索引库

    /* unordered_map<string,set<pair<int,double>>> _weightTable;//权重库 */

    unordered_map<string,double> keymap;//每个关键词的权重系数

    unordered_map<string, int> _keywords;//关键词+词频

    map<double,int> cosValue;//将网页按余弦相似度进行排序

    map<string,map<string,string>> hashString; //存在Redis里面的数据
};



#endif
