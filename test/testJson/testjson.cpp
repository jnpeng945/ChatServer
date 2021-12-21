#include "json.hpp"
#include <iostream>
#include <vector>
#include <map>
#include <string>

using json = nlohmann::json;
using namespace std;

void func1()
{
    json js;        // 实例化一个 json 对象
    js["msgType"] = 2;
    js["from"] = "Alice";
    js["to"] = "Bob";
    js["msg"] = "Hello, I am sending a plot to you";
    
    // 输出 json 对象序列化后的结果
    cout << js << endl;
    
    // 转换为字符串进行发送
    string sendBuf = js.dump();
    cout << sendBuf.c_str() << endl;
}

void func2()
{
    json js;
    // 添加数组
    js["id"] = {1, 2, 3, 4, 5};
    // 添加 key - value
    js["name"] = "Alice";

    js["msg"]["Alice"] = "hello Bob";
    js["msg"]["Bob"] = "hello Alice";
    
    // 上面两行等效以下代码
    js["msg"] = {{"Alice", "hello Bob"}, {"Bob", "hello Alice"}};
    
    cout << js << endl;
    
    // 反序列化
    string jsonstr = js.dump();
    cout << "jsonstr:" << jsonstr << endl;
    json js2 = json::parse(jsonstr);
    string name = js2["name"];
    cout << "name:" << name << endl;
}

void func3()
{
    json js;
    // 序列化一个 vector 容器
    vector<int> vec{1, 2, 5};
    js["list"] = vec;

    // 序列化一个 map 容器
    map<int, string> m;
    m.insert({1, "learning"});
    m.insert({2, "playing"});
    m.insert({3, "sleeping"});

    js["path"] = m;

    cout << js << endl;

    // 反序列化
    string jsonstr = js.dump();
    cout << "jsonstr:" << jsonstr << endl;

    // 模拟从网络中接收到的 json 字符串，通过 json::parse 函数将 json 字符串转成 json 对象
    json js2 = json::parse(jsonstr);

    // 直接反序列化 vector 容器
    vector<int> v = js2["list"];
    for (int val : v) {
        cout << val << ' ';
    }
    cout << endl;
    // 反序列化 map 容器
    map<int, string> m2 = js2["path"];
    for (auto p : m2) {
        cout << p.first << ' ' << p.second << endl;
    }
    cout << endl;
}

int main() 
{
    func1();

    func2();

    func3();

    return 0;
}
