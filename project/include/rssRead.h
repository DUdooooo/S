#include<iostream>
#include<fstream>
#include<vector>
#include<regex>
#include"../include/tinyxml2.h"

using std::string;
using std::vector;
using std::cout;
using std::cerr;
using std::endl;
using std::ofstream;
using namespace tinyxml2;

struct RssItem{
    string _title;
    string _link;
    string _description;
    string _content;
};

struct Rsstemp{
    string _title;
    string _link;
    string _content;
};


class RssReader
{

public:
    RssReader(){};
    ~RssReader(){};
    void read1(const string &);
    RssItem readOnline(const string &);
    void store(const string & );
    vector<RssItem> & getRss();
    int docidOnline();
    
private:
    vector<RssItem> _rss;
    int docid;
}; 

#if 0
void RssReader::read1(const string &rssfile){
    XMLDocument doc;
    doc.LoadFile(rssfile.c_str());

    if(doc.ErrorID()){
        cerr<<"LoadFile failed"<<" ErrorID:"<<doc.ErrorID()<<endl;
        exit(-1);
    }

    XMLElement * itemNode=
        doc.FirstChildElement("rss")
        ->FirstChildElement("channel")
        ->FirstChildElement("item");
    while(itemNode){

        RssItem RI;
        
        //如果有title，则将title的内容存入RI里面，下面几个if同理
        if(itemNode->FirstChildElement("title"))
            RI._title = itemNode->FirstChildElement("title")->GetText();

        if(itemNode->FirstChildElement("link"))
            RI._link = itemNode->FirstChildElement("link")->GetText();

        //regex_replace 将第一个参数中的呗reg匹配的字符使用""之间的内容替换。
        std::regex reg("<[^>]+>");
        std::regex reg2("&nbsp;");
        std::regex reg3("^[\\s]*\n");//删除空行
        std::regex reg4("                ");

        if(itemNode->FirstChildElement("content:encoded"))
        {
            RI._content	= regex_replace(RI._content,reg,"");
            RI._content	= regex_replace(RI._content,reg2,"");
            RI._content	= regex_replace(RI._content,reg3,"");
            RI._content	= regex_replace(RI._content,reg4,"");

            RI._content	= itemNode->FirstChildElement("content:encoded")->GetText();

        }


        if(itemNode->FirstChildElement("description"))
        {
            RI._description	= itemNode->FirstChildElement("description")->GetText();
            RI._description	= regex_replace(RI._description,reg,"");
            RI._description	= regex_replace(RI._description,reg2,"");
            RI._description	= regex_replace(RI._description,reg3,"");
            RI._description	= regex_replace(RI._description,reg4,"");
        }
        
        /* if(itemNode->FirstChildElement("content:encoded")) */
        /* RI._content		= regex_replace(RI._content,blankline,""); */

        _rss.push_back(RI);
        //继续下个"item"
        itemNode=itemNode->NextSiblingElement("item");
    }
}
#endif

#if 0 
void RssReader::store(const string & storefilename){
    ofstream out;
    out.open(storefilename);
    
    if(!out.good()){
        cerr<<"open file:"<<storefilename<<" failed"<<endl;
        exit(-1);
    }
    
    for(size_t idx=0;idx!=_rss.size();++idx){
        out <<"<doc>"<<endl
            <<"\t"<<"<docid>"<<idx+1<<"</docid>"<<endl
            <<"\t"<<"<title>"<<_rss[idx]._title<<"</title>"<<endl
            <<"\t"<<"<link>"<<_rss[idx]._link<<"</link>"<<endl
            <<"\t"<<"<description>"<<_rss[idx]._description<<"</description>"<<endl
            <<"\t"<<"<content>"<<_rss[idx]._content<<"</content>"<<endl
            <<"</doc>"
            <<endl
            <<endl;
    }
    
    out.close();

}

void test(){
    RssReader rss;
    rss.read("../data/html/auto.xml");
    rss.store("../data/ripepage.dat");
}

int main(){
    test();
    return 0;
}
#endif
