#include"../../include/EchoServer.h"
#include"../../include/LRUCache.h"
#include"../../include/Logger.h"
/* #include"../../include/WebPageQuery.h" */

#include<iostream>
#include <algorithm>
#include  <fstream>
#include <sstream>
#include <unordered_map>

using std::set_intersection;
using std::ifstream;
using std::istringstream;
using std::unordered_map;

//返回给客户端5个关键字
void LRU(const string & word,TcpConnectionPtr & _con)
{
    LRUCache Cache(10);

    /* int i = 100; */

    /* string QueryWord = "hello卧室好看又给你"; */
    string QueryWord = word;

    vector<string> Result = Cache.Query(QueryWord);

    _con->sendInLoop(Result);
    //2.返回五个相近的关键字
#if 0
    for (auto &it : Result)
    {

        /* cout << it << endl; */
        _con->sendInLoop(it);
        _con->sendInLoop("\n");
        /* it1+=it; */
    }
#endif

}

size_t Nbytes(const char ch){
    /* if(ch & (1 << 7)) */
    /* { */
    /*     int nBytes = 1; */
    /*     for(int idx = 0; idx != 6; ++idx) */
    /*     { */
    /*         if(ch & (1 << (6 - idx))) */
    /*         { */
    /*             ++nBytes; */	
    /*         } */
    /*         else */
    /*             break; */
    /*     } */
    /*     return nBytes; */
    /* } */
    if((ch>>5&1)&&(ch>>6&1)&&(ch>>7&1)){
        return 3;    
    }
    return 1;

}

void MyTask::process()
{
    //_msg
    //处理_msg,进行相应的编解码、计算
    //处理完成之后，消息是需要发出去的,需要一个TCP连接
    //消息的接收与发送此时都是Reactor/EventLoop
    //
    //涉及到计算线程（线程池）与IO线程（EventLoop/Reactor）进行通信
    cout << "--msg:" << _msg.getData() << endl;
    int fd = _con->_sock.fd(); 

    //if 1
    //1.客户端第一次发关键字过来
    //返回五个关键字
    /* LRU(_msg.getData(),_con); */


    if(1 == _msg.getDataid() )
    {
        LRU(_msg.getData(),_con);

    }else if(2 == _msg.getDataid())
    {

        //发送网页(title+link+summary)
        cout<<"客户端选择的推荐关键字："<<_msg.getData()<<endl;

        string word1 = _msg.getData().substr(0,_msg.getData().size()-1);

        vector<singleWeb> redisReturn = queryWebs(word1);
        if(redisReturn.size()!=0)
        {
        int z = 1;
        map<int,string> tmp;
        tmp.clear();
        int count = redisReturn.size();
        if(count == 0)
        {
            cout << "未检索到文章" << endl;
            send(fd,&count,4,0);
            return ;
        }
        if(count>=10){

            count = 10;

            send(fd,&count,4,0);
            cout<<"count"<<count<<endl;
        }
        else{
            send(fd,&count,4,0);
            cout<<"count"<<count<<endl;
        }
            vector<string> v;
            v.clear();
            for(auto &i :redisReturn)
            {
                v.push_back(i.title);
                v.push_back(i.link);
                tmp[z] = i.link;
                int j = 0;
                string docSummary = i.summary;
                string a ;
                for(size_t i = 0;j<95;i++,j++)
                {
                    size_t c = Nbytes(docSummary[i]);
                    a += docSummary.substr(i,c);

                    i+=c-1;
                }
                v.push_back(a);
                _con->send(v);
                z++;
                if(z == count+1){
                    break;
                }
            }
            int choiceNum = 0;
            string _title;

            recv(fd,&choiceNum,sizeof(choiceNum),0);

            cout<<"客户选择了第 "<<choiceNum<<" 篇文章"<<endl;

            cout<<tmp[choiceNum]<<endl;

            string _content = queryContent(tmp[choiceNum]);
            vector<string> v3;
            v3.clear();
            v3.push_back(_content);
            /* Json sendFinalePage(fd,300,_content); */
            _con->send(v3);

        }
        else
        {



        int z = 1;
        map<int,string> tmp;
        tmp.clear();
            WebPageQuery q(word1);


            int count = q._RecPage.size(); 
            pushWebs(word1,q.getHashString());
            if(count == 0)
            {
                cout << "未检索到文章" << endl;
                send(fd,&count,4,0);
                return ;
            }
            if(count>=10){

                count = 10;

                send(fd,&count,4,0);
                cout<<"count"<<count<<endl;
            }
            else{
                send(fd,&count,4,0);
                cout<<"count"<<count<<endl;
            }
            for(auto &it :q._RecPage){
                vector<string> v;
                v.clear();
                v.push_back(it.first);
                for(auto &sec:it.second){
                    v.push_back(sec.first);
                    tmp[z] = sec.first;
                    int j = 0;
                    string docSummary = sec.second;
                    string a ;
                    for(size_t i = 0;j<95;i++,j++)
                    {
                        size_t c = Nbytes(docSummary[i]);
                        a += docSummary.substr(i,c);

                        i+=c-1;
                    }
                    v.push_back(a);
                }
                /* break; */
                /* Json sendRecPage(fd,200,v); */
                _con->send(v);
                z++;
                if(z == count+1){
                    break;
                }
            }
            int choiceNum = 0;
            string _title;
            for(int i = 0;i<count;++i){
                pushContent(tmp[i],q._content[tmp[i]]);
            }
            recv(fd,&choiceNum,sizeof(choiceNum),0);

            cout<<"客户选择了第 "<<choiceNum<<" 篇文章"<<endl;

            cout<<tmp[choiceNum]<<endl;

            string _content = q._content[tmp[choiceNum]];
            vector<string> v3;
            v3.clear();
            v3.push_back(_content);
            /* Json sendFinalePage(fd,300,_content); */
            _con->send(v3);
        }
    }
    /* string QueryWord = "hello卧室好看又给你"; */


#if 0
    string it1 ;
    for (auto &it : Result)
    {
        /* cout << it << endl; */
        it1 = it;
        cout << it1 << endl;
        /* Json J(_fd,100,Result); */
        /* _con->sendInLoop("\n"); */
        it1+=it;
    }
#endif
    /* cout << "it1:" <<  it1<< endl; */

    cout << "1_msg:"<<_msg.getData() << endl;
    /* string a = "hahahhaha"; */
    /* _con->sendInLoop(a); */

    /* WebPageQuery web(_msg.getData()); */

    /* if(_msg == "1\n") */
    /* { */
    /*     web.loadLibary(); */    
    /* }else */
    /* { */
    /*     web.Cos(_msg); */
    /*     cout << "111" << endl; */
    /* web.getHashString(); */
    /* } */

    //if 2
    //2.客户端选择要查询的网页关键字


#if 0
    string aa = "a\n";
    string bb = "b\n";

    if(_msg == aa)
    {
        LRUCache Cache(10);

        int i = 100;

        string QueryWord = "hello卧室好看又给你";

        vector<string> Result = Cache.Query(QueryWord);

        string it1 ;
        for (auto &it : Result)
        {
            /* cout << it << endl; */
            it1 = it;
            cout << it1 << endl;
            _con->sendInLoop(it1);
            _con->sendInLoop("\n");

            /* it1+=it; */
        }
        /* cout << "it1:" <<  it1<< endl; */

        cout << "1_msg:"<<_msg << endl;
        /* string a = "hahahhaha"; */
        /* _con->sendInLoop(a); */
    }else if(_msg == bb)
    {
        cout << "2_msg:"<<_msg << endl;
        cout << "2" << endl;
        string b = "uuuuuuuu";
        _con->sendInLoop(b);

    }else
    {
        cout << "3_msg:"<<_msg << endl;
        cout << "3" << endl;
        string c = "error num";
        _con->sendInLoop(c);
    }
#endif
    /* string a = " num1"; */
    /* string a1 = " num2"; */
    /* _con->sendInLoop(a); */
    /* _con->sendInLoop(a1); */
    //消息要从TcpConnection对象发送给EventLoop进行发送
    //此时TcpConnection必须要知道EventLoop的存在(在TcpConnection中添加
    //EventLoop的引用或者指针)
    //

}

void EchoServer::onMessage(const TcpConnectionPtr &con)
{
    //实现了回显服务
    ProtocolParser  msg =  con->receive();

    cout << "recv from client msg data : " << msg.getData() << endl;
    cout << "recv from client msg id : " << msg.getDataid() << endl;
    //msg是应该做处理的,就是业务逻辑的处理
    //将msg的处理交给线程池
    /* TcpServer fd; */
    MyTask task(msg, con);
    _pool.addTask(std::bind(&MyTask::process, task));
}

