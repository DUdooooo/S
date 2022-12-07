#ifndef _CPPJIEBA_H_
#define _CPPJIEBA_H_

#include "../lib/cppjieba/include/cppjieba/Jieba.hpp"
#include "SplitTool.h"

#include <iostream>
#include <string>
#include <vector>
#include<sstream> 

using std::cout;
using std::endl;
using std::string;
using std::vector;
using std::istringstream;
using std::ifstream;

const char * const DICT_PATH = "../lib/cppjieba/dict/jieba.dict.utf8";;//最大概率法(MPSegment: Max Probability)分词所使用的词典路径
const char * const HMM_PATH = "../lib/cppjieba/dict/hmm_model.utf8";//隐式马尔科夫模型(HMMSegment: Hidden Markov Model)分词所使用的词典路径
const char * const USER_DICT_PATH = "../lib/cppjieba/dict/user.dict.utf8";//用户自定义词典路径
const char* const IDF_PATH = "../lib/cppjieba/dict/idf.utf8";//IDF路径
const char* const STOP_WORD_PATH = "../lib/cppjieba/dict/stop_words.utf8";//停用词路径

class CppJieba//使用结巴分词库进行分词
:public SplitTool
{
public:
    CppJieba()
        : _jieba(DICT_PATH, HMM_PATH, USER_DICT_PATH,IDF_PATH,STOP_WORD_PATH)
    {
        cout << "cppjieba init!" << endl;
    }
    virtual vector<string> cut(const string&sentence)override
    {
         vector<string> results;
        _jieba.CutAll(sentence, results);//FullSegment
        ifstream file;
        file.open(STOP_WORD_PATH);

            if (!file.is_open())
            {
                cout << "打开文件失败";
            }
            vector<string> stopword;
            string s;
            while (getline(file, s))
            {
               istringstream istring(s);
               string word;
               while(istring>>word)
               {
                    stopword.push_back(word);
               }
            }
            for(auto e:stopword)
            {
                // for(auto e1=results.begin();e1!=results.end();e1++)
                // {
                //     if((*e1)==e)
                //     {
                //         // cout<<*e1<<" "<<e<<endl;
                //         results.erase(e1);
                //     }
                // }
                auto e1=results.begin();
                while(e1!=results.end())
                {
                    if((*e1)==e)
                    {
                        // cout<<*e1<<" "<<e<<endl;
                        results.erase(e1);
                    }else{
                            e1++;
                    }
                }
            }
            // for(auto e:results)
            // {
            //     cout<<e<<endl;
            // }
        return results;
    }
   
private:
    cppjieba::Jieba _jieba;
};

#endif
