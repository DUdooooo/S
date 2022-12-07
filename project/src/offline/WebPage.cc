/** * Project SearchEngine
*/

#include "../../include/WebPage.h"
#include "../../include/mySimHash.h"

/* const char * const DICT_PATH = "../lib/simhash/dict/jieba.dict.utf8";;//最大概率法(MPSegment: Max Probability)分词所使用的词典路径 */
/* const char * const HMM_PATH = "../lib/simhash/dict/hmm_model.utf8";//隐式马尔科夫模型(HMMSegment: Hidden Markov Model)分词所使用的词典路径 */
/* /1* const char * const USER_DICT_PATH = "../lib/simhash/dict/user.dict.utf8";//用户自定义词典路径 *1/ */
/* const char* const IDF_PATH = "../lib/simhash/dict/idf.utf8";//IDF路径 */
/* const char* const STOP_WORD_PATH = "../lib/simhash/dict/stop_words.utf8";//停用词路径 */


bool operator==(const WebPage & lhs,const WebPage & rhs)
{
    return Simhasher::isEqual(lhs._simhash,rhs._simhash);
}

bool operator<(const WebPage & lhs, const WebPage & rhs)
{
    if(lhs == rhs)
    {
        return false;
    }

    return lhs._docid < rhs._docid;
}

WebPage::WebPage(string & doc,int docid,RssItem & rss,mySimHash & mysimhash,string docSummary)     
    :_doc(doc)
    ,_docid(docid)
    ,_docTitle(rss._title)
    ,_docUrl(rss._link)
    ,_doContent(rss._content+rss._description)
    ,_docSummary(docSummary)
     ,_mysimhash(mysimhash)
{
    /* cout << "_docSummary:" << docSummary << endl; */
    /* cout << "WebPage " << endl; */
    /* cout << "docid" << _docid << endl; */
    processDoc(); 
    /* cout << "WebPage end" << endl; */
} 
WebPage::~WebPage(){}
#if 0
void WebPage::Splick(const string doContent)
{
    cout << "enter WebPage::Splick" << endl;

    //1.找到停用词
    unordered_set<string> stopWord;
    string path = GetConfig("CN_STOP");
    /* path = "../data/stopWord/stop_words_eng.txt"; */

    ifstream ifs(path,ifstream::in);//以只读的方式打开
    if(!ifs.good())
    {
        perror("getStopWord ifs");
        return ;
    }

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
    int cnt =0;
    for(auto & elem :vec )
    {
#if 1
        if(stopWord.find(elem)!=stopWord.end())//是stopword
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
        cnt++;
    }
    cout << "cnt :" << cnt << endl;
    ifs.close();
}
#endif

int WebPage::getDocId() const
{
    return _docid;
}
string WebPage::getTile() const
{
    return _docTitle;
}
string WebPage::getUrl() const
{
    return _docUrl;
}
string WebPage::getContent() const
{
    return _doContent;
}
string WebPage::getSummary() const
{
    return _docSummary;
}


string  WebPage::getDoc()

{
    return _doc;
}
unordered_map<string,int> &  WebPage::getWordsMap()
{
    return _wordsMap;
}

void  WebPage::processDoc()
{
    /* cout << "processDoc" << endl; */
    //分词 
    _wordsMap = _mysimhash.Splick(_doContent);   

    //创建simhash值
    _simhash = _mysimhash.getSimhash(_doContent);
}

void  WebPage::calcTopK(vector<string> & wordsmap,int K,set<string> & stopWordList)
{

}


/**
 * WebPage implementation
 */
