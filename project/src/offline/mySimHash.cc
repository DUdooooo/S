#include"../../include/mySimHash.h"
#include <iostream>

//this define can avoid some logs which you don't need to care about.
#define LOGGER_LEVEL LL_WARN 

using namespace simhash;

const char * const DICT_PATH = "../../lib/simhash/dict/jieba.dict.utf8";;//最大概率法(MPSegment: Max Probability)分词所使用的词典路径
const char * const HMM_PATH = "../../lib/simhash/dict/hmm_model.utf8";//隐式马尔科夫模型(HMMSegment: Hidden Markov Model)分词所使用的词典路径
/* const char * const USER_DICT_PATH = "../lib/simhash/dict/user.dict.utf8";//用户自定义词典路径 */
const char* const IDF_PATH = "../../lib/simhash/dict/idf.utf8";//IDF路径
const char* const STOP_WORD_PATH = "../../lib/simhash/dict/stop_words.utf8";//停用词路径

mySimHash::mySimHash()
:_simhasher(DICT_PATH, HMM_PATH, IDF_PATH, STOP_WORD_PATH)
, _jieba(DICT_PATH, HMM_PATH, "")
{
    
}

mySimHash::~mySimHash(){}

uint64_t mySimHash::getSimhash(const string & filename)
{
    /* string s("我是蓝翔技工拖拉机学院手扶拖拉机专业的。不用多久，我就会升职加薪，当上总经理，出任CEO，走上人生巅峰。"); */
    size_t topN = 5;
    uint64_t u64 = 0;
    /* vector<pair<string ,double> > res; */
    /* _simhasher.extract(filename, res, topN); */
    _simhasher.make(filename, topN, u64);
    return u64;
}
unordered_map<string, int> mySimHash::Splick(const string &doContent)
{
    /* cout << "enter WebPage::Splick" << endl; */

    //1.找到停用词
    unordered_set<string> stopWord;
    string path = GetConfig("CN_STOP");
    /* path = "../data/stopWord/stop_words_eng.txt"; */

    ifstream ifs(path,ifstream::in);//以只读的方式打开
    /* if(!ifs.good()) */
    /* { */
    /*     perror("getStopWord ifs"); */
    /*     return ; */
    /* } */

    string lines,word1;

    while(getline(ifs,lines))
    {
        istringstream str(lines);
        str >> word1;
        stopWord.insert(word1);
    }

    //2.进行分词   
    
    vector<string> vec;
    //进行分词,然后存入vec
    _jieba.Cut(doContent,vec,true);

    /* unordered_map<string,int> map; */
    
    string text;
    string word;
    
    //去掉停用词,并统计词频，将词和词频插入到map里面
    /* int cnt =0; */
    unordered_map<string,int> _wordsMap;
    for(auto & elem :vec )
    {
#if 1
            if(stopWord.count(" ")==1)
            {
                continue;
            }
            else if(stopWord.find(elem)!=stopWord.end() )//是stopword
            {
                /* text.clear(); */ 
                continue;
            }
            else//不是stopword
            {
                //判断是第一次插入，还是已经插入过
                if(_wordsMap.count(elem)>0)//已经插入过
                {
                    ++_wordsMap[elem];
                }
                else//第一次插入
                {
                    _wordsMap.insert(make_pair(elem,1));
                }

            }
#endif
                /* cnt++; */
    }
    /* cout << "cnt :" << cnt << endl; */
    ifs.close();
    return _wordsMap;
}
