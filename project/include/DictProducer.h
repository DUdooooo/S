#ifndef __DICTPRODUCER_H__
#define __DICTPRODUCER_H__
#include"Configuration.h"
#include"mySimHash.h"
#include <map>
#include <set>
#include <string>
#include <vector>
#include<unordered_set>
#include<unordered_map>

using std::map;
using std::pair;
using std::set;
using std::string;
using std::vector;
using std::unordered_set;
using std::unordered_map;

class SplitTool; //分词工具类

class DicProducer //词典创建类
{
public:
    DicProducer(const string &dir,mySimHash & myh,int i);                       //构造函数
    DicProducer(const string &dir,mySimHash &myh ); //处理中文的构造函数

    void buildEnDdict();                                  //创建英文字典
    void buildCnDict();                                   //创建中文字典

    void storeDict();                 //将中文词典写入文件

    void enStoreDict(const string &filepath);                 //将英文词典写入文件
    void storeIndex(const string &filepath);                //将索引写入文件

    void showFiles() const;                               //查看文件路径，作为测试用
    void showDict() const;                                //查看词典，作为测试用

    void getFiles();                                      //获取文件的绝对路径
    void pushDict(const string &word);                    //存储某个单词

    void getStopWord();                                   //获取stopword
    void buildEnDdictProcess(const string &);             //分词  

    virtual ~DicProducer() //析构函数_没有不写
    {
    }

private:
    mySimHash & _myh;
    int _i;
    unordered_set<string> _stopword; 
    string _filepath;//用来保存输入的文件路径

    vector<string> _files;           //语料库文件的绝对路径集合

    /* vector<pair<string, int>> _dict; //词典 */

   unordered_map <string,int> _dict; //中文词典

    unordered_map<string,int> _en_dict;//英文词典    

    map<string, set<int>> _index;    //词典索引
    map<string, set<int>> _cnIndexTable;    //词典索引
    SplitTool *_splitTool;           //分词工具
};

#endif
