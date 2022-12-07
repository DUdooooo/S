#include "../../include/DirScanner.h"


vector<string>& DirScanner::getFiles() 
{
    return _files;
}

void DirScanner::traverse(const string& dir) 
{
    DIR * pdir = opendir(dir.c_str());
    if(pdir == nullptr)
    {
        perror("opendir error");
        return; 
    }
    struct dirent * pdirent;
    while((pdirent = readdir(pdir))!=NULL)
    {
        if('.'==pdirent->d_name[0])
        {
            continue;
        }
        if(DT_DIR==pdirent->d_type)//是一个目录
        {
            traverse(dir+"/"+pdirent->d_name);//递归扫描目录
        }
        else{
            string path;
            path = dir + "/" + pdirent->d_name;
            _files.push_back(path);
        }
    }
    closedir(pdir);
}

void DirScanner::showFiles()
{
    for(auto & elem:_files){
        cout<<elem<<endl;
    }

}


