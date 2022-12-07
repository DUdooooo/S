#include "../../include/Configuration.h"
#include<fstream>
#include<sstream>
#include<iostream>
using std::ifstream;
using std::istringstream;


Configuration::Configuration(string configpath)
:_configFilePath(configpath)
{
    getConfigInfo();
}

//读入配置信息
void Configuration::getConfigInfo(){
    ifstream ifs;
    ifs.open(_configFilePath);
    cout << ":::" << _configFilePath << endl;
    if(!ifs.good())
    {
        perror("open configinfo failed\n");
        exit(-1);
    }
    string line;
    string key;
    string value;
    while(getline(ifs,line))
    {
        processLine(line);
        istringstream iss(line);
        iss>>key;
        iss>>value;
        _configs[key]=value;
    }
    ifs.close();
}


string Configuration::getFilePath(const string& key)  
{
   if(_configs.count(key))
    return _configs[key];
   else
       return key+":"+"Invalid Configuration Information";
    /* return _configs.at(key); */
}


map<string,string> Configuration::getConfigMap() const
{
    return _configs; 
}


Configuration * Configuration::_pInstance =getInstance();
Configuration::autorelease Configuration::_ar;


Configuration * Configuration::getInstance()
{
    if(nullptr==_pInstance) 
    {
        _pInstance = new Configuration();
    }
    return _pInstance;
}

void Configuration::processLine(string& line)
{
    for(auto& ch:line)
    {
        if('='== ch) 
        {
            ch = ' ';
            break;
        }
    }
}

/*test*/
void Configuration::show()
{
    for(auto & i:_configs)
    {
        std::cout<<i.first<<"="<<i.second<<std::endl;
    }
}
