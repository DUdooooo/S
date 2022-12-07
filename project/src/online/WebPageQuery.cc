#include"../../include/WebPageQuery.h"
/* #include"rssRead.cc" */
/* #include"../include/WebPage.h" */

#include <algorithm>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include<set>

using std::set_intersection;
using std::ifstream;
using std::istringstream;
using std::unordered_map;
using std::istringstream;
using std::set;


WebPageQuery::WebPageQuery(const string & str)
    :_summary(str)
{
    //加载库文件
    loadLibary();

    //打印测试
    /* for(auto & elem : _pageLib) */
    /* { */
    /*     cout<< "<doc><docid>" << elem.getDocId() */ 
    /*         << "</docid><url>" << elem.getUrl() */ 
    /*         << "</url><title>" << elem.getTile() */ 
    /*         << "</title><content>" << elem.getContent() */ 
    /*         << "</content></doc>" << endl; */

    /* } */

    //计算权重
    keyWeight(_summary);

    //计算余弦值
    Cos(_summary);
    /* cout << "WebPageQuery"<< endl; */

    //测试能否拿到这个容器
    //
    getHashString(); 
}

WebPageQuery::~WebPageQuery(){}

void WebPageQuery::loadLibary()
{
    //加载倒排索引库
    string invert = GetConfig("XML_INVERT");
    Libary(invert);
    cout << "22" << endl;

    /* //加载权重库 */
    /* string wei = GetConfig("XML_WEIGHT"); */
    /* Libary(wei,_weightTable); */
    /* cout << "11" << endl; */

    //加载偏移库
    //和网页库
    offsetLibary();

}

#if 1
//加载倒排索引库
void WebPageQuery::Libary(const string &str)
{


    ifstream ifs1(str);

    if(!ifs1.good())
    {
        cout<<"ifs1 is error"<<endl;
    }
    string s;
    string stemp;
    int itemp;
    double dtemp;
    set<pair<int,double>> settemp;
    while(getline(ifs1,s))
    {

        istringstream iss(s);
        iss>>stemp;
        //cout<<stemp<<" ";
        while(iss>>itemp>>dtemp)
        {

            pair<int,double> pairtemp{itemp,dtemp};
            _invertIndexTable[stemp].insert(pairtemp);
        }
    }
}
#endif

//加载偏移库
void WebPageQuery::offsetLibary()
{
    cout << "offsetLibary" << endl;

    //读取偏移库
    string offse = GetConfig("XML_OFFSET");
    ifstream offsetIfs(offse);

    //读取网页库
    string web = GetConfig("XML_PAGE");
    ifstream webIfs(web);

    /* unordered_map<int,pair<int,int>> off; */

    string line;
    while(getline(offsetIfs,line))
    {
        char buff[100000] = {0};
        /* memset(buff,0,sizeof(buff)); */

        istringstream offset(line);
        int id,startAddress,pageSize;

        offset >> id >> startAddress >> pageSize;
        /* cout << "id:" << id<<endl; */
        /* cout << "startAddress:"<<startAddress <<endl; */
        /* cout << "pageSize:" <<pageSize << endl; */

        _offsetLib[id]=make_pair(startAddress,pageSize);

        //定位流,定位到该片文章的起始地址
        webIfs.seekg(startAddress);
        //读取文章的偏移大小的字节，也就是把该文章读取进去
        webIfs.read(buff,pageSize);

        //加载网页库
        creatPageLibary(string(buff));
        /* string(buff); */
        /* cout << "buff:" << string(buff) <<endl; */
    }

    /* for(auto & elem : _offsetLib) */
    /* { */
    /*     cout << elem << endl; */
    /* } */

}

//加载网页库
void WebPageQuery::creatPageLibary(const string & pageString)
{
    /* cout << "creatPageLibary" << endl; */
    RssReader rss;
    string doc = "";
    /* cout << "112" << endl; */

    /* rss.readOnline(pageString); */
    RssItem item;
    item = rss.readOnline(pageString);
    /* cout << "title: " << item._title; */


    /* string docSummary = pageString.substr(sizeof(item._title+item._link)+24, */
    /*                                       300+sizeof(item._link+item._title)+24); */
    /* string docSummary = 0; */

    string docSummary = (item._content+item._description).substr(0,300);
    WebPage web(doc,rss.docidOnline(),item,_simhash,docSummary);
    std::pair<set<WebPage>::iterator, bool> ret = _pageLib.insert(web);
    _pageLib.insert(web);

    /* cout << "docid" << rss.docidOnline() << endl; */
    /* cout << "title:"<< item._title << endl; */
    /* cout << "url:"<< item._link << endl; */
    /* cout << "content:"<< item._content << endl; */


    /* cout << "113" << endl; */
}

//计算余弦相似度
double WebPageQuery::Cos(const string & content)
{
    /* cout << "Cos" << endl; */
#if 1
    //执行读文件库
    //执行查询，返回结果
    //将关键词分割，存入容器temp...............................................................
    unordered_map<string, int> temp=_simhash.Splick(content);
    /* _keywords = temp; */

    for(auto & elem : _keywords)
    {
        cout << elem.first << endl;
        cout << elem.second << endl;
    }
    //执行查询，返回结果
    //将关键词分割，存入容器keywords...............................................................
    set<int> docid_s;
    int cnt=0;

    for(auto e:temp)
    {
        //查询关键词是否存在
        //临时存放文章号set
        auto i=_invertIndexTable.count(e.first.c_str())  ;
        /* cout<<"遍历的关键词:"<<e.first.c_str()<<endl; */
        //如果存在
        //将排索引库对应内容读出存入set自动去重
        if(cnt==0)
        {
            /* cout<<"循环第一次"<<endl; */
            cnt++;
            for(auto e1:_invertIndexTable[e.first.c_str()])
            {       
                // cout<<e1.first<<endl;
                docid_s.insert(e1.first);
            }
        }else{
            /* cout<<"循环一次"<<endl; */
            set<int> settemp;
            for(auto e1:_invertIndexTable[e.first.c_str()])
            {      
                cnt++; 
                // cout<<e1.first<<endl;
                settemp.insert(e1.first);
            }
            set<int> result_temp;
            set_intersection(settemp.begin(),settemp.end(),docid_s.begin(),docid_s.end(),inserter(result_temp,result_temp.begin()));
            docid_s.clear();
            docid_s=result_temp;
            settemp.clear();
            result_temp.clear();
        }

        /* if(docid_s.size()) */
        /* { */
        /*     //有查询结果 */
        /*     for(auto e3:docid_s) */
        /*     { */   
        /*             cout<<e3<<endl; */
        /*     } */
        /* } */
        /* else */
        /* { */
        /*     cout<<"暂无结果"<<endl; */
        /* } */

    }
    /* for(auto & elem :docid_s) */
    /* { */
    /*     cout << elem << endl; */
    /* } */
#endif

    //存储权重系数
    /* unordered_map<int,vector<pair<string,double>>> weightX; */
    unordered_map<int,map<string,double>> weightX;

    //存储权重
    /* map<int,double> weight; */

#if 1
    for(auto & str:temp)
    {
        for(auto & elem : docid_s)
        {
            /* weightX.insert(_invertIndexTable[str].second); */
            set<pair<int,double>> s = _invertIndexTable[str.first.c_str()];

            /* set<pair<int,double>> s1 = _weightTable[str.first.c_str()]; */

            //通过id找到对应的权重系数，放到容器里
            for(auto & type:s)
            {
                if(type.first == elem)
                {
                    weightX[type.first].insert({str.first,type.second});
                }else{
                    continue;
                }
            }

            /* //通过id找到对应的权重，放到容器里 */
            /* for(auto & type:s1) */
            /* { */
            /*     if(type.first == elem) */
            /*     { */
            /*         weight.insert({type.first,type.second}); */
            /*     }else{ */
            /*         continue; */
            /*     } */
            /* } */

        }
    }

    //打印测试
    for(auto  &elem: weightX)
    {
        cout << "id:" << elem.first << " ";
        for(auto & ss : elem.second)
        {
            cout << "string:" << ss.first << " "
                << "double:" << ss.second<< " ";
        }
        cout << endl;

    }

    double XY = 0;
    double x = 0;//X向量的模没开平方
    double y = 0;//Y向量的模
    //计算余弦值
    for(auto &num : docid_s)
    {
        map<string,double> vec = weightX[num];
        for(auto &elem : temp)
        {
            double X = keymap[elem.first];
            cout << "X:" << X <<endl;
            double Y = vec[elem.first];
            cout << "Y:" << Y <<endl;
            XY += X*Y; 
            x += X*X;
            y += Y*Y;
        }
        double value = XY/sqrt(x*y);
        cosValue.insert({value,num});
    }

    for(auto & elem : cosValue)
    {
        cout << "cosQ:" << elem.first << " "
            << "docid:"<< elem.second << endl;
    }
#endif

#if 0
    for(auto & elem :keywords )
    {
        for(auto & num:docid_s)
        {
            vector<pair<int,double>> vec = weightX[elem.first.c_str()];
            for(auto & v:vec)
            {
                XY+=keymap[elem.first] * v.second;  
            }
        }
    }
#endif    
}

//计算单词权重
void WebPageQuery::keyWeight(const string & content)
{
    cout << "keyWeight" << endl;
    unordered_map<string, int> _keywords=_simhash.Splick(content);
    int N = _offsetLib.size();
    cout << "N:" << N <<endl;

    double sum = 0;
    //记录每个单词的w
    unordered_map<string,double> w;

    for(auto &elem : _keywords)
    {
        string word = elem.first;
        double TF = elem.second;
        cout << "TF:" << TF << endl;
        int DF = 0;
        if(_invertIndexTable.find(elem.first)!=_invertIndexTable.end())
        {
            DF =  _invertIndexTable[elem.first].size();
            cout << "DF:" << DF << endl;

        }

        double IDF = log2(N/(DF+1));
        cout << "IDF:" << IDF << endl;


        w[word] = TF*IDF;
        sum += TF*IDF*TF*IDF;
    }


    //生成权重库
    for(auto & elem : w)
    {
        double total = elem.second/sqrt(sum);
        keymap.insert({elem.first,total});
    }

    cout << "auto" <<w.size()<< endl;

    for(auto & elem : w)
    {

        cout << "string" << elem.first
            << "权重" << elem.second << endl;
    }
}
size_t WebPageQuery::nBytesCode(const char ch){
    if(ch & (1 << 7))
    {
        int nBytes = 1;
        for(int idx = 0; idx != 6; ++idx)
        {
            if(ch & (1 << (6 - idx)))
            {
                ++nBytes;	
            }
            else
                break;
        }
        return nBytes;
    }
    return 1;
}
map<string,map<string,string>> WebPageQuery::getHashString()
{
    for(auto &num : cosValue)
    {
        for(auto & elem :_pageLib)
        {
            if(elem.getDocId() == num.second)
            {
                hashString[elem.getTile()].insert({elem.getUrl(),elem.getSummary()});
                _RecPage[elem.getTile()].insert({elem.getUrl(),elem.getSummary()});
                _content[elem.getUrl()] = elem.getContent();
            }else
            {
                continue;
            }
        }
    }

    /* for(auto & elem :hashString) */
    /* { */
    /*     cout << "getTile:" << elem.first << " "; */
    /*         for(auto & tmp : elem.second) */
    /*         { */
    /*          cout << "getLink:" << tmp.first<< " " */
    /*          << "getSummary:" << tmp.second ; */

    /*         } */
    /*     cout << endl; */
    /* } */
    return hashString;
}
/* bool cmp(map<string,map<string,string>> &m1,map<string,map<string,string>> &m2){ */
/*         size_t s1 = m1.begin()->first.size()+m1.begin()->second.begin()->first.size()+m1.begin()->second.begin()->first.size(); */
/*         size_t s2 = m2.begin()->first.size()+m2.begin()->second.begin()->first.size()+m2.begin()->second.begin()->first.size(); */
/*         return s1<s2; */
/* } */
/* void WebPageQuery::sortRecPage(){ */
/*     sort(_RecPage.begin(),_RecPage.end(),cmp); */
/* } */
