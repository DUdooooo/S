#ifndef __Redis_H__
#define __Redis_H__
#include"../lib/sw/redis++/redis.h"
#include<string>
#include<map>
#include<string>
using std::string;
using std::vector;
using std::map;
using namespace sw::redis;

struct singleWeb
{
    string title;
    string link;
    string summary;
};


inline vector<singleWeb> queryWebs(const string& word)
{
    auto redis = Redis("tcp://127.0.0.1:6379");
    vector<singleWeb> ret;

    std::vector<std::string>   recommandTitle;
    redis.lrange(word, 0, -1, std::back_inserter(recommandTitle));
    if (recommandTitle.size() == 0)
    {
        return  vector<singleWeb>();
    }
    else
    {
        for (auto& it : recommandTitle)
        {
            std::map<std::string, std::string> getLS;
            redis.hgetall(it, std::inserter(getLS, getLS.begin()));
            auto it2 = getLS.begin();
            singleWeb tmp;
            tmp.title = it;
            tmp.link = it2->first;
            tmp.summary = it2->second;
            ret.push_back(tmp);
        }
        return ret;
    }
}


inline string queryContent(const string& link)
{
    auto redis = Redis("tcp://127.0.0.1:6379");
    auto val = redis.get(link);
    if (val)
    {
        return *val;
    }
    else
    {
        return string();
    }
}

inline void pushWebs(const string& queryword, map<string, map<string, string>> lib_returned_info)
{
    auto redis = Redis("tcp://127.0.0.1:6379");

    for (auto& wordlist : lib_returned_info)
    {
        redis.rpush(queryword, wordlist.first);
    }

    for (auto& webinfo : lib_returned_info)
    {

        for (auto& elem : webinfo.second)
        {
            redis.hset(webinfo.first, elem.first, elem.second);
        }
    }
}
inline void pushContent(const string& link, const string& content)
{
    auto redis = Redis("tcp://127.0.0.1:6379");
    redis.set(link, content);
}




#endif
