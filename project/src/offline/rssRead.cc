#include"../../include/rssRead.h"
#include<sstream>
using std::istringstream;
vector<RssItem> & RssReader::getRss()
{
    return _rss;
}
void RssReader::read1(const string &rssfile){
    XMLDocument doc;
    cout << "rssfile" << rssfile << endl;
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
        /* std::regex reg4("                "); */
        std::regex reg4("\n");

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
int RssReader::docidOnline()
{
    return docid;
}
RssItem RssReader::readOnline(const string &rssfile){

    RssItem RI;
    size_t begin,end;
    
    //找到docid
    begin =rssfile.find("<docid>");
    end = rssfile.find("</docid>");

    //为什么药-1，+2，我也不知道，一个一个试出来的
    istringstream isid(rssfile.substr(begin+sizeof("<docid>")-1,
                                         end-begin-sizeof("</docid>")+2));

    isid >> docid;

    //找到title
    begin =rssfile.find("<title>");
    end = rssfile.find("</title>");
    RI._title = rssfile.substr(begin+sizeof("<title>")-1,
                               end-begin-sizeof("</title>")+2);
    
    //找到url
    begin =rssfile.find("<url>");
    end = rssfile.find("</url>");
    RI._link = rssfile.substr(begin+sizeof("<url>")-1,
                               end-begin-sizeof("</url>")+2);

    //找到content
    begin =rssfile.find("<content>");
    end = rssfile.find("</content>");
    RI._content = rssfile.substr(begin+sizeof("<content>")-1,
                               end-begin-sizeof("</content>")+2);

    return RI;
}
