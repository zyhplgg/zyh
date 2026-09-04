#include <iostream>
#include <regex>
#include <string>

int main() {
    // 模拟包含邮箱和IP地址的敏感数据
    std::string sensitiveContent = "员工邮箱：zhangsan@company.com，登录IP：192.168.1.100。";
    std::cout << "原始内容: " << sensitiveContent << std::endl;

    // 匹配邮箱地址（简单示例）
    std::regex emailRegex(R"([a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\.[a-zA-Z]{2,})");
    std::smatch matches;
    std::string::const_iterator start = sensitiveContent.cbegin();
    std::cout << "检测到邮箱：\n";
    while (std::regex_search(start, sensitiveContent.cend(), matches, emailRegex)) {
        std::cout << " - " << matches[0] << std::endl;
        start = matches.suffix().first;
    }
    return 0;
}