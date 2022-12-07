#include "../../include/LRUCache.h"
#include "../../include/KeyRecommander.h"

LRUCache::LRUCache(int num)
    : _capacity(num)
{
    readFromFile();
} //构造函数

void LRUCache::addElement(string &queryWord, vector<string> &Result)
{
    //先去删除哈希表和队列中最后的结点
    if (_capacity == _nodes.size())
    {
        auto &deleteNode = _nodes.back();
        _hashMap.erase(deleteNode._key);
        _nodes.pop_back();
    }

    //再加入新的结点
    CacheNode newNode;
    newNode._key = queryWord;
    newNode._CacheNode = Result;
    _nodes.push_front(newNode);
    _hashMap.insert(make_pair(queryWord, _nodes.begin()));

} //往缓存中添加数据

void LRUCache::readFromFile()
{

} //从文件中读取缓存信息

void LRUCache::writeToFile()
{

} //将缓存信息写入到文件中

vector<string> LRUCache::Query(string &QueryWord)
{
    auto it = _hashMap.find(QueryWord);
    if (it == _hashMap.end())
    {
        //不在哈希表里说明缓存中没有
        //这时候去KeyRecommander中找
        KeyRecommander Query(QueryWord, 5);

        //查到了后更新一下缓存
        addElement(QueryWord, Query._topResult);

        //更新缓存后将缓存写入文件中
        writeToFile();

        //返回查询结果
        return Query._topResult;
    }
    else
    {
        //找到就把他拉到队头
        _nodes.splice(_nodes.begin(), _nodes, it->second);
        return it->second->_CacheNode;
    }

} //在缓存内进行查询
