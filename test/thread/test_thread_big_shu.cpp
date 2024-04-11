#include <iostream>
#include <fstream>
#include <random>
#include <thread>
#include <vector>
#include <algorithm>
#include <sys/stat.h>

const int numThreads = 10;
const std::size_t fileSize = 1ULL * 1024 * 1024; // 1 GB

// 创建目录
void createDirectory(const std::string& directory)
{
    if (mkdir(directory.c_str(), 0777) == -1) {
        std::cerr << "Failed to create directory: " << directory << std::endl;
        exit(1);
    }
}

// 生成指定大小的随机数文件
void generateRandomFile(const std::string& filename)
{
    std::ofstream file(filename);
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<unsigned int> dis(0, std::numeric_limits<unsigned int>::max());

    std::size_t remainingSize = fileSize;
    while (remainingSize > 0) {
        std::size_t writeSize = std::min(remainingSize, static_cast<std::size_t>(1024));
        for (std::size_t i = 0; i < writeSize; ++i) {
            file << dis(gen) << '\n';
        }
        remainingSize -= writeSize;
    }
}

// 对文件中的随机数进行排序
void sortFile(const std::string& filename)
{
    std::vector<unsigned int> data;
    std::ifstream file(filename);
    unsigned int num;
    while (file >> num) {
        data.push_back(num);
    }
    std::sort(data.begin(), data.end());

    std::ofstream sortedFile(filename + ".sorted");
    for (unsigned int num : data) {
        sortedFile << num << '\n';
    }
}

int main()
{
    // 创建目录
    createDirectory("./tmp/");

    // 生成随机数文件
    std::vector<std::thread> generateThreads;
    for (int i = 0; i < numThreads; ++i) {
        std::string filename = "./tmp/random_" + std::to_string(i) + ".txt";
        generateThreads.emplace_back(generateRandomFile, filename);
    }

    for (std::thread& thread : generateThreads) {
        thread.join();
    }

    // 对文件进行排序
    std::vector<std::thread> sortThreads;
    for (int i = 0; i < numThreads; ++i) {
        std::string filename = "./tmp/random_" + std::to_string(i) + ".txt";
        sortThreads.emplace_back(sortFile, filename);
    }

    for (std::thread& thread : sortThreads) {
        thread.join();
    }

    return 0;
}