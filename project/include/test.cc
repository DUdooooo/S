#include"json.hpp"
#include <iostream>
#include <string>
#include <vector>
using std::cout;
using std::endl;
using std::string;
using std::vector;
using json = nlohmann::json;

///这里是json的test用例
//
////关于头文件包含：如果你要使用josn，有两种方法
////第一种：需要去源文件里把源文件的single-include 移动到你的当前工作目录
////然后 头文件包含格式为"xxx/single-inlcude/nlohmann/json.hpp"即可
////第二种： #include<nlohmann/json.hpp>
//
////json的语法规则：
///* 数组（Array）用方括号(“[]”)表示。 */
///* 对象（0bject）用大括号(“{}”)表示。 */
///* 名称/值对(name/value）组合成数组和对象。 */
///* 名称(name）置于双引号中，值（value）有字符串、数值、布尔值、null、对象和并列的数据之间用逗号(“,”）分隔 */       
//
void test3()
{
    auto j3 = json::parse(R"({"happy": true, "pi": 3.141})");
    cout << "j3 = " << j3 << endl;

    //string --->json----->string
    string t1;
    json j_string = t1;
    auto cpp_string = j_string.get<std::string>();
    cout << "cpp_string = " << cpp_string << endl;

}

void test4()
{
    //json转化为string的三种方法
    json j_string = "this is a string";



   // ​    //第一种
        auto cpp_string = j_string.get<std::string>();
   // ​    //第二种
        std::string cpp_string2;
    j_string.get_to(cpp_string2);
    //第三种
    std::string serialized_string = j_string.dump();

    std::cout << cpp_string
        << " == " << cpp_string2
        << " == " << j_string.get<std::string>() << '\n';
    std::cout << j_string
        << " == " << serialized_string << std::endl;

}



struct Player
{
    string name;
    int credits;
    int ranking;
};
void to_json(json &j, const Player &p)
{
    j= json{ {"name", p.name},
        {"credits", p.credits},
        {"ranking", p.ranking} };
} 
void from_json(const json &j, Player &p)
{
    j.at("name").get_to(p.name);
    j.at("credits").get_to(p.credits);
    j.at("ranking").get_to(p.ranking);
} 

//test0 和上面的from_json与to_json还有struct Player相关联
//但是仅供参考，我还没有搞懂实操怎么套用
void test0()
{
    auto j = R"([
                                                                                             {"name": "Judd Trump","credits": 1754499,"ranking": 1},
                                                                                                             {"name": "Neil Robertson","credits": 1040500,"ranking": 2},
                                                                                                                             {"name": "Ronnie O'Sullivan","credits": 954500,"ranking": 3}
                                                                                                                                         ])"_json;

    vector<Player> players = j.get<vector<Player>>();
    /* vector<Player> players; */
    /* j.get_to(players); */
    cout<< "name:" << players[2].name << endl;
    cout<< "credits:" << players[2].credits << endl;
    cout<< "ranking:" << players[2].ranking << endl;

    for(auto &it:players)
    {
        cout<<"name: "<<it.name
            <<" c: "<<it.credits
            <<" r "<<it.ranking<<endl;
    }
}

int main(){
    test4();
    test3();
    test0();
        return 0;

}











