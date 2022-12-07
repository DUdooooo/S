#include "../../include/DictProducer.h"
/* #include "../../include/SplitTool.h" */
#include"../../include/mySimHash.h"
#include <string.h>
#include <sys/stat.h>
#include<dirent.h>
#include<iostream>
#include<algorithm>
#include<vector>
#include<fstream>
#include<sstream>

using std::vector;
using std::cout;
using std::endl;
using std::ifstream;
using std::istringstream;
using std::ostringstream;
using std::transform;
using std::ofstream;

DicProducer::DicProducer(const string &dir,mySimHash & myh,const int i)
    :_filepath(dir)
     ,_i(i)
     ,_myh(myh)
{
    cout << "enter english DicProducer" << endl; 
    getFiles();
    /* showFiles(); */
    cout << "000" << endl;
    getStopWord();

    cout << "111" << endl;
    buildEnDdict();
    cout << "aaaa" << endl;
}

//构造函数
DicProducer::DicProducer(const string &dir,mySimHash & myh )
    :_filepath(dir)
     ,_myh(myh)
{
    cout << "1" << endl;
    getFiles();
    buildCnDict();
    storeDict();
}
//处理中文的构造函数

//获取stopword
void DicProducer::getStopWord()
{
    cout << "enter getStopWord" << endl;
    string path = GetConfig("EN_STOP");
    /* path = "../data/stopWord/stop_words_eng.txt"; */

    ifstream ifs(path,ifstream::in);//以只读的方式打开
    if(!ifs.good())
    {
        perror("getStopWord ifs");
        return;
    }

    string lines,word;

    while(getline(ifs,lines))
    {
        istringstream str(lines);
        str >> word;
        _stopword.insert(word);
    }

    /* for(auto &elem : _stopword) */
    /* { */
    /*     cout << elem <<endl; */
    /* } */

}

//创建英文字典和索引
void DicProducer::buildEnDdict()
{
    for(auto & elem : _files)
    {
        buildEnDdictProcess(elem);
    }

    string dictPath=GetConfig("EN_DICT");
    /* dictPath = "../data/en_dict.dat"; */
    enStoreDict(dictPath);

    string indexPath=GetConfig("EN_INDEX");
    /* indexPath = "../data/en_index.dat"; */
    storeIndex(indexPath);
}

void DicProducer::buildEnDdictProcess(const string & file)
{
    cout << "buildEnDdictProcess" << endl;

    ifstream ifs(file);
    if(!ifs)
    {
        perror("buildEnDdictProcess open file error");
        return;
    }

    string text;
    string word;


    while(getline(ifs,text))
    {

        for(auto &elem : text)
        {
            //判断是不是字母
            if(!isalpha(elem))//不是字母
                elem = ' ';//将之改成空格
            else if(isupper(elem))//是字母
                elem = tolower(elem);//转换成小写
        }

        istringstream line(text);

        while(line >> word){

            //判断是否为stopword
            if(_stopword.find(word)!=_stopword.end())//是stopword
            {
                /* text.clear(); */ 
                continue;
            }
            else//不是stopword
            {
                //判断是第一次插入，还是已经插入过
                if(_en_dict.count(word)>0)//已经插入过
                {
                    ++_en_dict[word];
                }
                else//第一次插入
                {
                    _en_dict[word] = 1;
                }
            }
            /* text.clear(); */
        }

    }
    ifs.close();

    //创建索引

    int index=1;
    for(auto & elem :_en_dict)
    {
        for(size_t i=0;i<elem.first.length();++i)
        {
            _index[elem.first.substr(i,1)].insert(index);
        }
        ++index;
    }

    /* for(auto i = _en_dict.begin();i!=_en_dict.end();i++,index++) */
    /* { */
    /*     string temp = i->first; */
    /*     for(auto &elem :temp) */
    /*     { */
    /*         string ch(1,elem); */
    /*         _index[ch].insert(index); */
    /*     } */
    /* } */
}

void DicProducer::buildCnDict()//创建中文词典库和索引库
{

    string content;
    string line;

    // 遍历每篇中文语料文章存储所有的内容
    for (size_t i = 0; i < _files.size(); i++)
    {
        ifstream file(_files[i]);
        while (getline(file, line))
        {
            content += line;
        }
    }

    // 切分出所有的单词，建立中文词典
    /* _dict = _myh.Splick(content); */
    _dict = _myh.Splick(content);

    // 遍历中文词典建立中文索引，每个中文字占三个字符
    int lineNum = 1;
    for (auto it = _dict.begin(); it != _dict.end(); it++, lineNum++)
    {
        string temp = it->first;
        for (auto it = temp.begin(); it != temp.end(); it += 3)
        {
            // 后面剩的不足一个汉字跳过
            if ((it == temp.end()) || (it + 1) == temp.end() || (it + 2) == temp.end())
            {
                break;
            }
            ostringstream tmp;
            tmp << *it << *(it + 1) << *(it + 2);
            _cnIndexTable[tmp.str()].insert(lineNum);
        }
    }
}

void DicProducer::storeDict()//将中文词典写入文件
{
    //存词典
    string filepath = GetConfig("CN_DICT");
    ofstream ofs;
    ofs.open(filepath);
    if(!ofs)
    {
        perror("storDict ofstram filepath error");
        return;
    }

    for(auto & elem : _dict)
    {
        ostringstream oss;
        ofs << elem.first << " " << elem.second << endl;
        string str = oss.str();
        ofs << str;
    }
    ofs.close();

    //存索引
    string filename1 = GetConfig("CN_INDEX");
    ofstream ofs1(filename1,std::ios::out);
    if(!ofs1)
    {
        perror("storeIndex ofstream error");
        return;
    }

    for(auto & word : _cnIndexTable)
    {
        ostringstream oss;//防止缓冲区过小，造成字符串无法全部存储
        oss << word.first << " ";//写入字母
        for(auto & num : word.second)
        {
            oss << num << " ";//写入该字母分别出现的
        }
        oss << endl;

        string str = oss.str();

        ofs1 << str;
    }

    ofs1.close();
}
// void DicProducer::showFiles() const                               //查看文件路径，作为测试用
void DicProducer::showDict() const 
{
    for(auto e:_index)
    {
        cout<<e.first<<" ";
        for(auto e1:e.second)
        {
            cout<<e1<<" ";
        }
        cout<<endl;
    }
}


//将词典写入文件
void DicProducer::enStoreDict(const string &filepath)
{
    ofstream ofs;
    ofs.open(filepath);
    if(!ofs)
    {
        perror("storDict ofstram filepath error");
        return;
    }

    for(auto & elem : _en_dict)
    {
        ostringstream oss;
        ofs << elem.first << " " << elem.second << endl;
        string str = oss.str();
        ofs << str;
    }
    ofs.close();
}

//将索引写入文件
void DicProducer::storeIndex(const string & filename)
{
    ofstream ofs(filename,std::ios::out);
    if(!ofs)
    {
        perror("storeIndex ofstream error");
        return;
    }

    for(auto & word : _index)
    {
        ostringstream oss;//防止缓冲区过小，造成字符串无法全部存储
        oss << word.first << " ";//写入字母
        for(auto & num : word.second)
        {
            oss << num << " ";//写入该字母分别出现的
        }
        oss << endl;

        string str = oss.str();

        ofs << str;
    }

    ofs.close();
}

//获取文件的绝对路径
void DicProducer::getFiles()
{
    cout << "enter getFile" << endl;
    cout << "_filepath" << _filepath << endl;
    DIR * dir = opendir(_filepath.c_str());

    if(dir == nullptr)
    {
        perror("getFile error");
        return; 
    }

    struct dirent * pdirent;

    //返回参数dir 目录流的下个目录进入点
    while((pdirent = readdir(dir))!=NULL)
    {
        cout << "111" << endl;
        if('.'==pdirent->d_name[0])
        {
            continue;
        }

        string path;
        path = _filepath + "/" + pdirent->d_name;
        cout << "getFile path:" << path << endl;

        _files.push_back(path);
    }
    closedir(dir);
}

//存储某个单词
void DicProducer::pushDict(const string &word)
{

}

void DicProducer::showFiles() const
{
    cout << "enter showFile" << endl;
    for(auto &elem : _files)
    {
        cout << elem <<endl;
    }
}
//查看文件路径，作为测试用

//测试
#if 0
int main()
{
    CppJieba jieba; 

    string filename;
    string filename1;
    string filename2;

    filename = GetConfig("EN_YULIAO");
    /* filename = "../data/en"; */
    cout << "filename:"<<filename << endl;
    filename1 = GetConfig("CN_YULIAO");
    /* filename1 = "../data/cn"; */
    cout << "filename1:"<<filename1 << endl;


    DicProducer d(filename);//英文

    DicProducer d1(filename1,&jieba);//中文
    d1.buildCnDict();

    filename2 = GetConfig("CN_DICT");
    cout << "filename2:"<<filename2 << endl;
    d1.storeDict(filename2.c_str());

}
#endif
