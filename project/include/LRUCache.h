#ifndef __LRUCACHE_H__
#define __LRUCACHE_H__

#include <string>
#include <list>
#include <unordered_map>
#include <vector>

using std::list;
using std::pair;
using std::string;
using std::unordered_map;
using std::vector;

class CacheNode
{
public:
    string _key;
    vector<string> _CacheNode;
};

class LRUCache
{
public:
    LRUCache(int num);                                          //构造函数
    void addElement(string &queryWord, vector<string> &Result); //往缓存中添加数据
    void readFromFile();                                        //从文件中读取缓存信息
    void writeToFile();                                         //将缓存信息写入到文件中
    vector<string> Query(string &QueryWord);                    //在缓存内进行查询

private:
    int _capacity;                                             //缓存节点的容量
    unordered_map<string, list<CacheNode>::iterator> _hashMap; //用hash进行查找，结点在链表中的位置
    list<CacheNode> _nodes;                                    //结点存在链表中
};

#endif