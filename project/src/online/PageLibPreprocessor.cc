/**
 * Project SearchEngine
 */


#include"../../include/PageLibPreprocessor.h"
#include"../../include/DirScanner.h"
#include"../../include/Configuration.h"

/* #include "../include/RssRead.h" */
PageLibPreprocessor::PageLibPreprocessor(mySimHash & myh)
:_myh(myh)
{

}

PageLibPreprocessor::~PageLibPreprocessor(){}

//创建网页库和偏移库
void PageLibPreprocessor::readInforFromFile()
{
    //1.读取xml
    /* cout << "readInforFromFile" << endl; */
    string path = GetConfig("CN_XML");
    DirScanner scanner;
    scanner.traverse(path);
    vector<string> dir = scanner.getFiles();

    /* cout << "scanner 1" << endl; */
    for(auto & elem : dir)
    {
        cout <<"file:"<< elem << endl;
    }

    string doc;//一篇文章
    int docid = 1;//文章id
    int allpage = 0;
    int emptypage = 0;
    int cnt =0;
    string docSummary;//文档摘要

    int startAddress = 0;//文章的起始地址 
    int offsetSize;

    for(auto & elem : dir)
    {
        /* cout << "111" << endl; */
        doc = elem.substr(path.size() + 1);
        cout << "doc = " << doc << endl; 
        RssReader rss;
        /* cout << "112" << endl; */

        rss.read1(elem);
        /* cout << "113" << endl; */
        for(auto & page : rss.getRss())
        {
        /* cout << "121" << endl; */
            ++allpage;
            /* ++docidtmp; */
            ++cnt;

            size_t size = page._content.size()+page._description.size();

            //如果文章内容为空，则跳过
            if(0 == size)
            {
                /* cout << "kong" << endl; */
                ++emptypage;
                continue;
            }

            //记录文章摘要,文章前一百个字
            //将文章的content内容和description内容放到s里
            string s = page._content + page._description;

            //将s中的字符从0开始，到100结束，复制给docSunmmary
            docSummary = s.substr(0,300);
            /* cout << "docSummary:" << docSummary << endl; */

            /* cout << "1211" << endl; */

            WebPage web(doc,docid,page,_myh,docSummary);
        /* cout << "122" << endl; */

            //插入网页库
            //因为用的set容器，set会自动进行排序
            //WebPage里面重载了<符号，set在排序的时候，会进行判重，如果重复会返回false

        std::pair<set<WebPage>::iterator, bool> ret = _pageList.insert(web);
        
#if 1

            //如果插入成功,则创建文件偏移库
            if(ret.second)
            {
        /* cout << "211" << endl; */
                //文章的长度 
                //76是一个文章id+输出网页库时一些前缀，详情看下面的输出网页库代码，一看就能懂
                
                string t = std::to_string(docid);
                offsetSize= 71+t.size()+page._title.size()+
                    +page._link.size()+size;  
                //进行插入
                _offsetLib.insert({docid,{startAddress,offsetSize}});            

        /* cout << "221" << endl; */


        /* cout << "231" << endl; */
                //下一篇文章的起始地址
                startAddress = startAddress + offsetSize +1;

        /* cout << "241" << endl; */
                //下一篇文章的id
                ++docid;
            }
            /* ++docid; */
#endif
        }
    }
    cout << "cnt::" << cnt << endl;
    cout << "共有文章：" << allpage << endl;
    cout << "去重后的文章：" << docid -1 << endl;
    cout << "空文章：" << emptypage << endl;
}

//创建倒排索引
void PageLibPreprocessor::bulidInvertIndexMap() 
{
    cout << "buildInverIndexMap" << endl;
    //找出单词在所有文章的词频,存在map1里面
    map<string ,int > map1;

    //遍历网页库
    for(auto & page : _pageList)
    {
        //遍历网页库中记录每篇文档词语词频的容器
        for (auto & elem :page._wordsMap)
        {
            string word = elem.first;//单词
            int i = elem.second;//词频

            if(0==i)//词频为零
            {
                map1[word]=elem.second;
            }
            else//不为零
            {
                map1[word]+=elem.second;
            }
        }
    }

    //计算权重
    
    //权重库
    unordered_map<string,vector<pair<int,double>>> weight;
    
    double N = _pageList.size();
    for(auto & page : _pageList)
    {
        double sum = 0;
        //记录每个单词的w
        unordered_map<string,double> w;
        

        for(auto &elem : page._wordsMap)
        {
            string word = elem.first;
            double TF = elem.second;
            double DF = map1[word];
            double IDF = log2(N/(DF+1));
            w[word] = TF*IDF;
            sum += TF*IDF*2;
        }


        //生成倒排索引库
        for(auto & elem : w)
        {
            /* weight[elem.first].push_back({page._docid,elem.second}); */
            double total = elem.second/sqrt(sum);
            _invertIndexLib[elem.first].push_back({page._docid,total});
        }

        /* for(auto & elem :_invertIndexLib) */
        /* { */
        /*     cout << elem << endl; */
        /* } */
    }
        //生成权重库
        string weightPath = GetConfig("XML_WEIGHT");
        ofstream offset(weightPath);
        for(auto & elem : weight)
        {
            offset << elem.first << " ";
                for(auto & elem1 : elem.second)
                {
                  offset <<" "<< elem1.first << " "
                           << elem1.second << " ";
                }
            offset << "\n";
        }
    return;
}

/**
 * @return vkid
 *
 **/

//将生成的三个库写入磁盘
void PageLibPreprocessor::storeOnDisk() 
{
    cout << "storeOnDish" << endl;
    //输出网页库
    string pagePath = GetConfig("XML_PAGE");
    ofstream page(pagePath);

    for(auto & elem : _pageList)
    {
        page<< "<doc><docid>" << elem._docid 
            << "</docid><url>" << elem._docUrl 
            << "</url><title>" << elem._docTitle 
            << "</title><content>" << elem._doContent 
            << "</content></doc>\n";
    }

    /* for(auto &elem : _offsetLib) */
    /* { */
    /*     cout << elem << endl; */
    /* } */

    //输出偏移库
    string offsetPath = GetConfig("XML_OFFSET");
    ofstream offset(offsetPath);
    for(auto & elem : _offsetLib)
    {
       offset << elem.first << " "
              << elem.second.first << " "
              << elem.second.second << "\n";
    }

    //输出倒排索引库
    string invertPath = GetConfig("XML_INVERT");
    ofstream inver(invertPath);
    if(!inver.good())
    {
        perror("ofstream invert");
    }
    for(auto & elem : _invertIndexLib)
    {
        inver << elem.first << " ";
            for(auto & elem1 : elem.second)
            {
                inver <<" "<< elem1.first << " "
                       << elem1.second << " ";
            }
        inver << "\n";
    }

    
    return;
}
