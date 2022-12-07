#ifndef __KEYRECOMMANDER_H__
#define __KEYRECOMMANDER_H__

#include "CandidateResult.h"
#include <queue>
#include <string>
#include <vector>
#include <set>
#include <memory>
#include <unordered_map>
#include <map>

using std::greater;
using std::map;
using std::priority_queue;
using std::set;
using std::shared_ptr;
using std::string;
using std::unordered_map;
using std::vector;

//候选词之间的比较方法
struct CRcompare
{
    bool operator()(CandidateResult &lhs, CandidateResult &rhs)
    {
        bool flag;
        if (lhs._dist == rhs._dist) //距离相同的情况下
        {
            flag = (lhs._freq < rhs._freq); // 词频大的放前面
        }
        else if (lhs._dist < rhs._dist) // 距离大的放后面
        {
            flag = false;
        }
        else
        {
            flag = true;
        }

        return flag;
    }
};

class KeyRecommander
{
public:
    KeyRecommander(string &query, int num); //构造函数
    void ToLower();                         //将待查词转为小写
    void execute();                         //执行查询
    void queryIndexTable();                 //查询索引
    int distance(const string &rhs);        //计算最小编辑距离
    void response();                        //响应客户端的请求
    void splitWord();                       //将混合词分为中文和英文部分

    vector<string> _topResult; //供给其他类的关键字
    int _num;                  //需要前几个关键字，自定义
private:
    string _queryWord;                                                              //等查询的单词
    vector<string> _queryCnWord;                                                    //待查单词中分出来的中文单词
    vector<string> _queryEnWord;                                                    //待查单词中分出来的英文单词
    unordered_map<int, int> _EnIndexLine;                                           //保存英文索引结果，key是行号，value是频率
    unordered_map<int, int> _CnIndexLine;                                           //保存中文索引结果，key是行号，value是频率                                                       //与客户端进行连接的文件描述符
    priority_queue<CandidateResult, vector<CandidateResult>, CRcompare> _resultQue; //保存候选结果集的优先级队列
    priority_queue<CandidateResult, vector<CandidateResult>, CRcompare> _resulToVector;
};

#endif
