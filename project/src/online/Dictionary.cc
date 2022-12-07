#include "../../include/Dictionary.h"
#include "../../include/Configuration.h"
#include <string>
#include <fstream>
#include <iostream>
#include <sstream>
using std::cerr;
using std::endl;
using std::ifstream;
using std::istringstream;
using std::string;
Dictionary *Dictionary::_pInstance = getInstance();
Dictionary::autorelease Dictionary::_ar;
Dictionary::Dictionary()
{
    initCnDict(GetConfig("CN_DIC"));
    initCnIndexTable(GetConfig("CN_INDE"));

    initEnDict(GetConfig("EN_DICT"));
    initEnIndexTable(GetConfig("EN_INDEX"));

} //默认构造函数

Dictionary *Dictionary::getInstance()
{
    if (nullptr == _pInstance)
    {
        _pInstance = new Dictionary();
    }
    return _pInstance;

} //静态函数

void Dictionary::initCnDict(const string &dict_path)
{
    ifstream ifs;
    cout << dict_path << endl;
    ifs.open(dict_path);
    if (!ifs.good())
    {
        perror("initCnDict");
        return;
    }
    string line;
    while (getline(ifs, line))
    {
        istringstream iss(line);
        string word;
        string frequency;
        iss >> word >> frequency;
        _cnDict.push_back({word, stoi(frequency)});
    }
    ifs.close();
} //通过词典文件路径初始化词典
void Dictionary::initEnDict(const string &dict_path)
{
    ifstream ifs;
    cout << dict_path << endl;
    ifs.open(dict_path);
    if (!ifs.good())
    {
        perror("initEnDict");
        return;
    }
    string line;
    while (getline(ifs, line))
    {
        istringstream iss(line);
        string word;
        string frequency;
        iss >> word >> frequency;
        _enDict.push_back({word, stoi(frequency)});
    }
    ifs.close();
} //通过词典文件路径初始化词典

void Dictionary::initCnIndexTable(const string &indextable_path)
{
    ifstream ifs;
    ifs.open(indextable_path);
    if (!ifs.good())
    {
        perror("initCnIndexTable");
        return;
    }
    string line;
    while (getline(ifs, line))
    {
        istringstream iss(line);
        string ch;
        string index;
        iss >> ch;
        while (iss >> index)
        {
            _cnIndexTable[ch].insert(stoi(index));
        }
    }
    ifs.close();

} //通过索引文件路径初始化索引表
void Dictionary::initEnIndexTable(const string &indextable_path)
{
    ifstream ifs;
    ifs.open(indextable_path);
    if (!ifs.good())
    {
        perror("initEnIndexTable");
        return;
    }
    string line;
    while (getline(ifs, line))
    {
        istringstream iss(line);
        string ch;
        string index;
        iss >> ch;
        while (iss >> index)
        {
            _enIndexTable[ch].insert(stoi(index));
        }
    }
    ifs.close();

} //通过索引文件路径初始化索引表

vector<pair<string, int>> &Dictionary::getCnDict()
{
    return _cnDict;
} //获取词典
vector<pair<string, int>> &Dictionary::getEnDict()
{
    return _enDict;
} //获取词典

map<string, set<int>> &Dictionary::getCnIndexTable()
{
    return _cnIndexTable;
} //获取索引表
map<string, set<int>> &Dictionary::getEnIndexTable()
{
    return _enIndexTable;
} //获取索引表

string Dictionary::doQuery(string word)
{

} //执行查询

/************TEST*************/
void Dictionary::showCnDict()
{
    for (int cnt = 0; cnt < 100; ++cnt)
    {
        cout << _cnDict[cnt].first << " " << _cnDict[cnt].second << endl;
    }
}
void Dictionary::showCnIndexTable()
{
    size_t cnt = 0;
    for (auto &i : _cnIndexTable)
    {
        ++cnt;
        if (100 == cnt)
        {
            break;
        }
        cout << i.first;
        for (auto &j : i.second)
        {
            cout << " " << j;
        }
        cout << endl;
    }
}
