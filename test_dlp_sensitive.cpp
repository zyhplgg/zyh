/**
 * @file test_dlp_sensitive.cpp
 * @brief 飞连 DLP 敏感数据检测测试文件 (C++)
 * @warning 仅供安全策略测试使用，请勿外发到公开仓库
 */

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <fstream>
#include <sstream>

// ============================================
// 1. 数据库连接配置（敏感信息）
// ============================================

class DatabaseConfig {
private:
    std::string host;
    int port;
    std::string username;
    std::string password;
    
public:
    DatabaseConfig() {
        // 硬编码的生产环境数据库配置 ⚠️ 敏感信息
        host = "10.10.100.50";
        port = 3306;
        username = "prod_admin";
        password = "Pr0d@DM1n!2024#SecurePassword";
    }
    
    // 内网数据库地址
    std::string getConnectionString() {
        return "jdbc:mysql://" + host + ":" + std::to_string(port) + 
               "/corp_finance?user=" + username + "&password=" + password;
    }
    
    // 备份数据库配置
    std::string getBackupConfig() {
        return "mysql://backup_host:3306?user=backup&password=Backup@Pass123456";
    }
};

// ============================================
// 2. API 密钥和第三方服务凭证
// ============================================

// Stripe 支付密钥（模拟）
const std::string STRIPE_SECRET_KEY = "sk_live_4eC39HqLyjWDarjtT1zdp7dcProdKey2024";
const std::string STRIPE_WEBHOOK_SECRET = "whsec_abc123def456ghi789jkl012mno345pqr";

// AWS 凭证（模拟）
const std::string AWS_ACCESS_KEY = "AKIAIOSFODNN7EXAMPLE12345";
const std::string AWS_SECRET_KEY = "wJalrXUtnFEMI/K7MDENG/bPxRfiCYEXAMPLEKEY67890";
const std::string AWS_REGION = "cn-north-1";

// 阿里云凭证（模拟）
const std::string ALIYUN_ACCESS_KEY = "LTAI5txxxxxxxxxxxxxxxxxxxx";
const std::string ALIYUN_SECRET_KEY = "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx";

// JWT 签名密钥
const std::string JWT_SECRET = "jwt_signing_key_for_production_environment_2024_secure";

// GitHub Token（模拟）
const std::string GITHUB_TOKEN = "github_pat_11ABC123DEF456GHI789JKL0MNO1234567890";

// ============================================
// 3. 敏感业务数据模型
// ============================================

struct CustomerInfo {
    int id;
    std::string name;
    std::string contact_person;
    std::string phone;
    std::string email;
    double contract_amount;
    std::string contract_expiry;
    std::string service_level;
};

struct EmployeeInfo {
    int id;
    std::string name;
    std::string department;
    std::string position;
    double salary;
    std::string bank_account;
    std::string id_card;  // 身份证号模拟
};

class SensitiveDataManager {
private:
    std::vector<CustomerInfo> customers;
    std::vector<EmployeeInfo> employees;
    
    // 硬编码的加密密钥 ⚠️ 严重安全问题
    const char* AES_KEY = "2b7e151628aed2a6abf7158809cf4f3c";
    const char* RSA_PRIVATE_KEY = R"(
-----BEGIN RSA PRIVATE KEY-----
MIIEpAIBAAKCAQEAu1Z2vZK7n4c8L9xM0N5oP7qRtS3wXyF6hJ8kL9zXcVbNmP0o
I9uYtR5eW7aQ8sDfGhJkLzXcVbNmP0oI9uYtR5eW7aQ8sDfGhJkLzXcVbNmP0oI9
-----END RSA PRIVATE KEY-----
)";
    
public:
    SensitiveDataManager() {
        // 模拟千万级大客户数据
        customers.push_back({1001, "XX科技集团有限公司", "王总经理", "13912340001", "wang@xxtech.com", 2500000.00, "2025-12-31", "enterprise"});
        customers.push_back({1002, "YY金融服务有限公司", "李财务总监", "13912340002", "li@yyfinance.com", 5800000.00, "2025-06-30", "enterprise"});
        customers.push_back({1003, "ZZ制造股份有限公司", "赵董事长", "13912340003", "zhao@zzmfg.com", 12000000.00, "2026-03-31", "vip"});
        customers.push_back({1004, "AA国际贸易集团", "孙总经理", "13912340004", "sun@aatrade.com", 8750000.00, "2025-09-30", "enterprise"});
        customers.push_back({1005, "BB新能源科技有限公司", "周总", "13912340005", "zhou@bbenergy.com", 15000000.00, "2026-06-30", "vip"});
        
        // 模拟员工薪资数据
        employees.push_back({2001, "张伟", "技术研发部", "架构师", 58000.00, "6212260200123456789", "11010119900307663X"});
        employees.push_back({2002, "李娜", "技术研发部", "技术总监", 89000.00, "6212260200987654321", "11010119850512456X"});
        employees.push_back({2003, "王强", "销售部", "销售总监", 75000.00, "6212260200555555555", "11010119801201123X"});
        employees.push_back({2004, "陈杰", "财务部", "财务总监", 82000.00, "6212260200111111111", "11010119780304234X"});
    }
    
    void printCustomerList() {
        std::cout << "=== 大客户名单（机密）===" << std::endl;
        for (const auto& c : customers) {
            std::cout << "客户: " << c.name 
                      << ", 联系人: " << c.contact_person
                      << ", 合同金额: " << c.contract_amount << " 元"
                      << std::endl;
        }
    }
    
    void printSalaryList() {
        std::cout << "=== 薪资列表（机密）===" << std::endl;
        for (const auto& e : employees) {
            std::cout << "员工: " << e.name 
                      << ", 部门: " << e.department
                      << ", 薪资: " << e.salary << " 元"
                      << ", 银行账号: " << e.bank_account
                      << std::endl;
        }
    }
};

// ============================================
// 4. 授权算法（核心知识产权）
// ============================================

class LicenseGenerator {
private:
    // 硬编码的算法常量
    const unsigned long MAGIC_NUMBER = 0xDEADBEEF;
    const int SALT_VALUE = 0x12345678;
    
    // 内部算法函数（商业机密）
    unsigned long computeHash(const std::string& input) {
        unsigned long hash = MAGIC_NUMBER;
        for (char c : input) {
            hash = ((hash << 5) + hash) + static_cast<unsigned long>(c);
            hash ^= SALT_VALUE;
        }
        return hash;
    }
    
public:
    /**
     * @brief 生成软件授权码的核心算法
     * @param company_name 公司名称
     * @param product_id 产品ID
     * @param expiry_days 有效期天数
     * @return 授权码
     */
    std::string generateLicense(const std::string& company_name, int product_id, int expiry_days) {
        // 核心算法逻辑（商业机密 - 请勿外泄）
        std::string seed = company_name + std::to_string(product_id) + std::to_string(expiry_days);
        unsigned long hash1 = computeHash(seed);
        
        // 第二层加密
        unsigned long hash2 = hash1 ^ MAGIC_NUMBER;
        for (int i = 0; i < expiry_days; i++) {
            hash2 = (hash2 * 31) + (i % 97);
        }
        
        // 生成最终授权码
        char license[32];
        snprintf(license, sizeof(license), "LIC-%08X-%08X-%08X", 
                 (unsigned int)(hash1 >> 16), 
                 (unsigned int)(hash2 & 0xFFFF), 
                 (unsigned int)((hash1 ^ hash2) % 1000000));
        
        return std::string(license);
    }
};

// ============================================
// 5. 敏感路径配置
// ============================================

namespace InternalPaths {
    const std::string GITLAB_REPO = "http://gitlab.company.internal/corp/core-system";
    const std::string JENKINS_JOB = "https://jenkins.company.internal/job/deploy-to-prod";
    const std::string NAS_SENSITIVE = "\\\\nas-01\\finance\\quarterly_reports\\2024\\Q4\\confidential";
    const std::string CONFLUENCE_PAGE = "https://confluence.company.internal/pages/viewpage.action?pageId=123456";
    const std::string S3_BUCKET = "s3://corp-sensitive-data-bucket/backup/2024/";
}

// ============================================
// 6. 配置文件生成器（模拟配置泄露）
// ============================================

class ConfigFileGenerator {
public:
    std::string generateApplicationYaml() {
        std::stringstream yaml;
        yaml << "# application.yml - 生产环境配置（敏感）\n";
        yaml << "spring:\n";
        yaml << "  datasource:\n";
        yaml << "    url: jdbc:mysql://prod-db.internal:3306/corp?useSSL=false\n";
        yaml << "    username: prod_user\n";
        yaml << "    password: Prod@Pass2024!\n";
        yaml << "  redis:\n";
        yaml << "    host: redis-prod.internal\n";
        yaml << "    port: 6379\n";
        yaml << "    password: Redis@Secret2024\n";
        yaml << "api:\n";
        yaml << "  stripe:\n";
        yaml << "    secret-key: sk_live_4eC39HqLyjWDarjtT1zdp7dc\n";
        yaml << "  aws:\n";
        yaml << "    access-key: AKIAIOSFODNN7EXAMPLE\n";
        yaml << "    secret-key: wJalrXUtnFEMI/K7MDENG/bPxRfiCYEXAMPLEKEY\n";
        return yaml.str();
    }
};

// ============================================
// 7. 主函数
// ============================================

int main(int argc, char* argv[]) {
    std::cout << "=== DLP Sensitivity Test File (C++) ===" << std::endl;
    std::cout << "This file contains simulated sensitive data for DLP testing.\n" << std::endl;
    
    // 1. 显示数据库配置
    DatabaseConfig dbConfig;
    std::cout << "[Database Configuration]" << std::endl;
    std::cout << "  Connection String: " << dbConfig.getConnectionString() << std::endl;
    std::cout << "  Backup Config: " << dbConfig.getBackupConfig() << std::endl;
    
    // 2. 显示 API 密钥
    std::cout << "\n[API Keys & Tokens]" << std::endl;
    std::cout << "  Stripe Secret: " << STRIPE_SECRET_KEY << std::endl;
    std::cout << "  AWS Access Key: " << AWS_ACCESS_KEY << std::endl;
    std::cout << "  JWT Secret: " << JWT_SECRET << std::endl;
    
    // 3. 显示敏感业务数据
    SensitiveDataManager dataManager;
    std::cout << "\n[Sensitive Business Data]" << std::endl;
    dataManager.printCustomerList();
    dataManager.printSalaryList();
    
    // 4. 生成授权码
    LicenseGenerator licenseGen;
    std::string license = licenseGen.generateLicense("XX科技集团", 10086, 365);
    std::cout << "\n[License Generated]" << std::endl;
    std::cout << "  License Code: " << license << std::endl;
    
    // 5. 显示内部路径
    std::cout << "\n[Internal Paths (Confidential)]" << std::endl;
    std::cout << "  GitLab: " << InternalPaths::GITLAB_REPO << std::endl;
    std::cout << "  NAS Path: " << InternalPaths::NAS_SENSITIVE << std::endl;
    std::cout << "  S3 Bucket: " << InternalPaths::S3_BUCKET << std::endl;
    
    // 6. 生成配置文件内容
    ConfigFileGenerator configGen;
    std::cout << "\n[Application Configuration (YAML)]" << std::endl;
    std::cout << configGen.generateApplicationYaml();
    
    std::cout << "\n=== End of Test File ===" << std::endl;
    return 0;
}