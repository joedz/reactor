#include <iostream>
#include <fstream>
#include <sstream>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <dirent.h>
#include "zip2.h"

#define ALLOWED_IP "127.0.0.1"  // 你允许的客户端 IP 地址
#define PORT 8080

// g++ -o Server_file Server_file.cpp zip2.cpp -L/usr/local/lib/libarchive.so.20 -larchive
// g++ -o Client_file Client_file.cpp zip2.cpp -L/usr/local/lib/libarchive.so.20 -larchive

void listDirectories(int newSocket,const char** argv) {
    
    minitar(sizeof(argv), argv);
    std::cout << "Received file name: " << "ServerTmp.tar" << std::endl;
    std::ifstream inputFile("ServerTmp.tar", std::ios::in | std::ios::binary);
    if (!inputFile.is_open()) {
        std::cerr << "Error opening file." << std::endl;
        close(newSocket);
        exit(EXIT_FAILURE);
    }

    // 读取并发送文件内容
    char buffer[4096] = {0};
    int bytesRead;
    while ((bytesRead = inputFile.readsome(buffer, sizeof(buffer))) > 0) {
        // std::cout << "在里面" << std::endl;
        send(newSocket, buffer, bytesRead, 0);
    }

    std::cout << "File received successfully." << std::endl;
    inputFile.close();
}

int main() {
    // 创建套接字
    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    int newSocket;
     
    // 配置服务器地址
    struct sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(PORT);
    // serverAddress.sin_addr.s_addr = INADDR_ANY;
    serverAddress.sin_addr.s_addr = inet_addr("127.0.0.1");
    socklen_t addrSize = sizeof(serverAddress);

    // 绑定地址
    bind(serverSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress));

    // 监听连接
    listen(serverSocket, 5);
    std::cout << "Server listening on port " << PORT << "..." << std::endl;  

    // 接受连接
    if ((newSocket = accept(serverSocket, (struct sockaddr*)&serverAddress, &addrSize)) < 0) {
        perror("Accept failed");
        exit(EXIT_FAILURE);
    }

    // 检查连接是否来自允许的 IP 地址
    if (strcmp(inet_ntoa(serverAddress.sin_addr), ALLOWED_IP) != 0) {
        std::cerr << "Connection from unauthorized IP address. Closing connection." << std::endl;
        close(newSocket);
        close(serverSocket);
        exit(EXIT_FAILURE);
    }

    std::cout << "Connection accepted from " << inet_ntoa(serverAddress.sin_addr) << ":" << ntohs(serverAddress.sin_port) << std::endl;


    // 读取文件名
    char fileName[4096] = {0};
    read(newSocket, fileName, sizeof(fileName));
    std::cout << "Received file name: " << fileName << std::endl;

    std::string file_Path = std::string(fileName);
    size_t lastSlashPos = file_Path.find_last_of('/');
    std::string directory = file_Path.substr(lastSlashPos+1);
    size_t dirSize = directory.size() + 1;
    send(newSocket, &dirSize, sizeof(dirSize), 0);
    send(newSocket, directory.c_str(), dirSize, 0);

    
    //压缩
    const char *chis[] = { 
        "./zip2",
        "-czf",
        "ServerTmp.tar",
        fileName,
        NULL  // 参数数组的末尾必须为NULL
    };
    listDirectories(newSocket, chis);
   

    // 关闭套接字和文件
    close(newSocket);
    close(serverSocket);
    

    return 0;
}
