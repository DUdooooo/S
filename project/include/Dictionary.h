#ifndef __DICTIONARY_H__
#define __DICTIONARY_H__

#include <map>
#include <set>
#include <string>
#include <vector>

using std::map;
using std::pair;
using std::set;
using std::string;
using std::vector;

vector<pair<string, int>> &GetCnDict(); //中文
map<string, set<int>> &GetCnIndexTable();

vector<pair<string, int>> &GetEnDict(); //英文
map<string, set<int>> &GetEnIndexTable();
class Dictionary
{
public:
    class autorelease
    {
    public:
        autorelease()
        {
        }
        ~autorelease()
        {
            if (_pInstance)
            {
                delete _pInstance;
            }
        }
    };

public:
    static autorelease _ar;
    static Dictionary *getInstance();

    void initCnDict(const string &path);       //初始化中文词典
    void initCnIndexTable(const string &path); //初始化中文索引
    vector<pair<string, int>> &getCnDict();    //获取中文词典
    map<string, set<int>> &getCnIndexTable();  //获取中文索引

    void initEnDict(const string &path);       //初始化英文词典
    void initEnIndexTable(const string &path); //初始化英文索引
    vector<pair<string, int>> &getEnDict();    //获取英文词典
    map<string, set<int>> &getEnIndexTable();  //获取英文索引表

    string doQuery(string word); //执行查询

    /****test****/
    void showCnDict();
    void showCnIndexTable();
    /************/
private:
    Dictionary();
    static Dictionary *_pInstance;
    vector<pair<string, int>> _cnDict; //中文词典
    vector<pair<string, int>> _enDict; //英文词典

    map<string, set<int>> _cnIndexTable; //中文索引表
    map<string, set<int>> _enIndexTable; //英文索引表
};

inline vector<pair<string, int>> &GetCnDict()
{
    return Dictionary::getInstance()->getCnDict();
}
inline map<string, set<int>> &GetCnIndexTable()
{
    return Dictionary::getInstance()->getCnIndexTable();
}

inline vector<pair<string, int>> &GetEnDict()
{
    return Dictionary::getInstance()->getEnDict();
}
inline map<string, set<int>> &GetEnIndexTable()
{
    return Dictionary::getInstance()->getEnIndexTable();
}

#endif
