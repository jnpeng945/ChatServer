#include <muduo/net/TcpServer.h>
#include <muduo/net/EventLoop.h>
#include <iostream>
#include <string>
#include <functional>

using namespace std;
using namespace muduo;
using namespace muduo::net;
using namespace placeholders;

/* 基于 muduo 网络库开发服务器程序
1. 组合 TcpServer 对象
2. 创建 EventLoop 事件循环对象的指针
3. 明确 TcpServer 构造函数需要什么参数，输出 ChatServer 的构造函数
4. 在当前服务器类的构造函数当中，注册处理连接的回调函数和处理读写事件的回调函数
5. 设置合适的服务端线程数量， muduo 库会自动分配 I/O 线程和 worker 线程

*/
class ChatServer
{
public:
    // TcpServer 没有默认构造函数
    ChatServer(EventLoop* loop,                  // 事件循环
            const InetAddress& listenAddr,      // IP + 端口
            const string& nameArg)              // 服务器的名字
            :_server(loop, listenAddr, nameArg)
            , _loop(loop)
    {
        // 给服务器注册用户连接的创建和断开回调
        _server.setConnectionCallback(std::bind(&ChatServer::onConnection, this, _1));

        // 给服务器注册用户读写事件回调
        _server.setMessageCallback(std::bind(&ChatServer::onMessage, this, _1, _2, _3));  // 参数占位符

        // 设置服务器端的线程数量 1 个 I/O 线程，3 个 worker 线程
        _server.setThreadNum(4);
    }

    // 开启事件循环
    void start()
    {
        _server.start();
    }
private:
    // 专门处理用户的连接创建和断开 epoll listenfd accept
    void onConnection(const TcpConnectionPtr &conn)
    {
        if (conn->connected())
        {
            cout << conn->peerAddress().toIpPort() << " -> " << conn->localAddress().toIpPort() << "state:online" << endl;
        }
        else
        {
            cout << conn->peerAddress().toIpPort() << " -> " << conn->localAddress().toIpPort() << "state:offline" << endl;
            conn->shutdown();       // close(fd)
            // _loop->quit();
        }


    }
    // 专门处理用户的读写事件
    void onMessage(const TcpConnectionPtr &conn,    // 连接
                    Buffer *buffer,        // 缓冲区
                    Timestamp time)     // 接收到数据的时间信息
    {
        string buf = buffer->retrieveAllAsString();
        cout << "recv data:" << buf << "time:" << time.toString() << endl;
        conn->send(buf);
    }


    TcpServer _server;      // #1
    EventLoop *_loop;       // #2 epoll


};

int main() 
{
    EventLoop loop;     // epoll
    InetAddress addr("127.0.0.1", 9000);        // 设置自己云服务器名称和端口号
    ChatServer server(&loop, addr, "ChatServer");

    server.start();     // listenfd epoll_ctl => epoll

    loop.loop();        // epoll_wait  以阻塞方式等待新用户连接，已连接用户的读写时间等

    return 0;
}
