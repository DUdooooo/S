#include "../../include/PageLibPreprocessor.h"
#include "../../include/DirScanner.h"
#include "../../include/Configuration.h"
#include"../../include/DictProducer.h"
#include <iostream>

using std::cout;
int main()
{
    mySimHash myh;


    string filename;
    string filename1;
    cout << "11" <<endl;

    filename = GetConfig("EN_YULIAO");

    int i = 1;
    DicProducer d(filename,myh,i);//英文

    filename1 = GetConfig("CN_YULIAO");
    DicProducer d1(filename1,myh);//中文

   cout << "111" << endl;
   PageLibPreprocessor pag(myh);
   cout << "222" << endl;
   
   pag.readInforFromFile();
   cout << "333" << endl;
   pag.bulidInvertIndexMap();
   cout << "444" << endl;
   pag.storeOnDisk();
   cout << "555" << endl;
    return 0;
}

