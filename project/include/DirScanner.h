#ifndef _DIRSCANNER_H
#define _DIRSCANNER_H
#include<dirent.h>
#include<sys/types.h>
#include<vector>
#include<string>
#include<iostream>
using std::endl;
using std::cout;
using std::vector;
using std::string;


class DirScanner {
public: 
    vector<string>& getFiles();//此方法将返回一个存放了文件路径的vector<string>
    void traverse(const string& dir);//递归扫描目录，存放目录中所有的文件的路径
    void operator()();
    /*test*/
    void showFiles();
private: 
    vector<string> _files;
};

#endif //_DIRSCANNER_H
