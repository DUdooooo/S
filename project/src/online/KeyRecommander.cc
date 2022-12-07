#include "../../include/KeyRecommander.h"
#include "../../include/EditDistance.h"
#include "../../include/Dictionary.h"

#include <iostream>
#include <fstream>
#include <algorithm>

using std::cout;
using std::endl;
using std::transform;

std::ofstream ofsL("textLine.txt");
std::ofstream ofsW("textWord.txt");
std::ofstream ofsR("textResult.txt");

KeyRecommander::KeyRecommander(string &query, int num)
    : _queryWord(query), _num(num)
{
    ToLower();

    cout << _queryWord << endl;

    execute();
} //构造函数

void KeyRecommander::ToLower()
{
    string LowQuery;
    int num = _queryWord.size();

    int i = 0;
    while (i < num)
    {
        int size = 1;
        if (_queryWord[i] & 0x80)
        {
            char temp = _queryWord[i];
            temp <<= 1;
            do
            {
                temp <<= 1;
                ++size;
            } while (temp & 0x80);

            i += size;
        }
        else
        {
            for (int j = i; j < i + size; j++)
            {
                _queryWord[i] = tolower(_queryWord[i]);
            }
            i += size;
        }
    }
}

void KeyRecommander::splitWord()
{
    int num = _queryWord.size();

    int i = 0;
    while (i < num)
    {
        int size = 1;
        if (_queryWord[i] & 0x80)
        {
            char temp = _queryWord[i];
            temp <<= 1;
            do
            {
                temp <<= 1;
                ++size;
            } while (temp & 0x80);
            _queryCnWord.push_back(_queryWord.substr(i, size));
            i += size;
        }
        else
        {
            _queryEnWord.push_back(_queryWord.substr(i, size));
            i += size;
        }
    }
} //将混合词分为中文和英文部分

void KeyRecommander::queryIndexTable()
{
    splitWord(); //先分词再查

    for (auto &it : _queryEnWord)
    {
        cout << it << endl;
    }
    for (auto &it : _queryCnWord)
    {
        cout << it << endl;
    }

    //英文查询
    map<string, set<int>> EnIndexTable = GetEnIndexTable(); //读入英文索引表

    //在英文索引表内查到行号
    for (auto &it : _queryEnWord)
    {
        //将行号存入索引图,每次出现就让其value++
        for (auto &itInt : EnIndexTable[it])
        {
            _EnIndexLine[itInt]++;

            ofsL << itInt << endl;
        }
    }

    //中文部分同上
    map<string, set<int>> CnIndexTable = GetCnIndexTable();

    for (auto &it : _queryCnWord)
    {
        for (auto &itInt : CnIndexTable[it])
        {
            _CnIndexLine[itInt]++;

            ofsL << itInt << endl;
        }
    }

} //查询索引

void KeyRecommander::execute()
{
    queryIndexTable();
    CandidateResult result;

    //执行英文查询
    vector<pair<string, int>> EnDict = GetEnDict();

    for (auto &elem : _EnIndexLine)
    {
        ofsW << elem.first << ":"
             << " " << EnDict[elem.first - 1].first << endl;

        result._word = EnDict[elem.first - 1].first;
        result._freq = EnDict[elem.first - 1].second;
        result._dist = distance(result._word);
        _resultQue.emplace(result);

        _resulToVector.emplace(result);
    }

    //执行中文查询
    vector<pair<string, int>> CnDict = GetCnDict();

    for (auto &elem : _CnIndexLine)
    {
        ofsW << CnDict[elem.first].first << endl;

        result._word = CnDict[elem.first].first;
        result._freq = CnDict[elem.first].second;
        result._dist = distance(result._word);
        _resultQue.emplace(result);

        _resulToVector.emplace(result);
    }

    while ((_num != 0) && (!_resulToVector.empty()))
    {
        _topResult.push_back(_resulToVector.top()._word);
        _resulToVector.pop();
        _num--;
    }

    while (!_resultQue.empty())
    {
        CandidateResult Result = _resultQue.top();
        ofsR << Result._word << " " << Result._dist << " " << Result._freq << endl;
        _resultQue.pop();
    }

} //执行查询

int KeyRecommander::distance(const string &rhs)
{
    return editDistance(_queryWord, rhs);
} //计算最小编辑距离

void KeyRecommander::response()
{
    _resultQue;

} //响应客户端的请求
