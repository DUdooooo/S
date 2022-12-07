#ifndef _CONFIGURATION_H
#define _CONFIGURATION_H
#include<string>
#include<map>
#include<iostream>
using std::string;
using std::map;
using std::endl;
using std::cout;
string GetConfig(const string & key);

class Configuration {
public:
    class autorelease{
    public:
        autorelease()
        {
        }
        ~autorelease()
        {
            if(_pInstance)
            {
                delete _pInstance;
                _pInstance=nullptr;
            }
        }
    };
public:

    static Configuration * getInstance();
    void getConfigInfo();
    string getFilePath(const string &key) ;
    map<string,string> getConfigMap() const;
    void processLine(string& line);
    /****test*****/
    void show();
private:
    static autorelease _ar;
    Configuration(string configpath="../../conf/conf.conf");
    static Configuration * _pInstance;
    string _configFilePath;
    map<string,string> _configs;
};

inline string GetConfig(const string & key)
{
    return Configuration::getInstance()->getFilePath(key);
}

#endif //_CONFIGURATION_H
