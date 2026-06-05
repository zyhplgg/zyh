/**
 * @file test_dlp_sensitive.c
 * @brief 飞连 DLP 敏感数据检测测试文件
 * @warning 仅供安全策略测试使用，请勿外发到公开仓库
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// ============================================
// 1. 数据库连接凭证（模拟生产环境敏感信息）
// ============================================

#define PROD_DB_HOST     "10.10.100.50"
#define PROD_DB_PORT     3306
#define PROD_DB_NAME      "corp_finance_db"
#define PROD_DB_USER      "finance_admin"
#define PROD_DB_PASSWORD  "Fin@nce#2024!SecurePass"

// 备份数据库凭证
#define BACKUP_DB_HOST    "192.168.10.200"
#define BACKUP_DB_USER    "backup_user"
#define BACKUP_DB_PASSWORD  "Backup@Pass123"

// ============================================
// 2. API 密钥和 Token（模拟第三方服务凭证）
// ============================================

// Stripe 支付密钥格式
static const char* STRIPE_SECRET_KEY = "sk_live_4eC39HqLyjWDarjtT1zdp7dcXYZ_Prod";
static const char* STRIPE_PUBLISHABLE_KEY = "pk_live_51ABCDEFGHIJKLMNOPQRSTUVWXYZ";

// AWS 凭证
static const char* AWS_ACCESS_KEY_ID = "AKIAIOSFODNN7EXAMPLE";
static const char* AWS_SECRET_ACCESS_KEY = "wJalrXUtnFEMI/K7MDENG/bPxRfiCYEXAMPLEKEY";

// JWT 签名密钥
static const char* JWT_SECRET_KEY = "jwt_signing_key_for_production_environment_2024";

// API 访问令牌
static const char* GITHUB_TOKEN = "ghp_abcdefghijklmnopqrstuvwxyz1234567890";
static const char* GITLAB_TOKEN = "glpat-ABC123DEF456GHI789JKL";

// ============================================
// 3. 敏感业务数据（模拟客户/财务信息）
// ============================================

// 客户信息结构体
typedef struct {
    int customer_id;
    char name[100];
    char contact[50];
    char phone[20];
    double contract_amount;
} Customer;

// 模拟的客户数据（千万级合同）
Customer g_top_customers[] = {
    {1001, "XX科技集团", "王总经理", "13912340001", 2500000.00},
    {1002, "YY金融服务有限公司", "李财务总监", "13912340002", 5800000.00},
    {1003, "ZZ制造股份", "赵董事长", "13912340003", 12000000.00},
    {1004, "AA国际贸易", "孙总", "13912340004", 8750000.00},
};

// ============================================
// 4. 加密解密函数（模拟核心算法）
// ============================================

// 硬编码的 AES 密钥（严重安全问题）
static const unsigned char AES_ENCRYPTION_KEY[] = {
    0x2b, 0x7e, 0x15, 0x16, 0x28, 0xae, 0xd2, 0xa6,
    0xab, 0xf7, 0x15, 0x88, 0x09, 0xcf, 0x4f, 0x3c,
    0x2b, 0x7e, 0x15, 0x16, 0x28, 0xae, 0xd2, 0xa6,
    0xab, 0xf7, 0x15, 0x88, 0x09, 0xcf, 0x4f, 0x3c
};

// 用户数据解密函数
void decrypt_user_sensitive_data(const unsigned char* encrypted_data, size_t data_len) {
    printf("[WARNING] Decrypting sensitive user data using hardcoded key\n");
    printf("Encrypted data length: %zu bytes\n", data_len);
    // 实际解密逻辑（省略）
}

// ============================================
// 5. License 授权算法（核心知识产权）
// ============================================

/**
 * @brief 生成产品授权码的核心算法
 * @param company_id 公司ID
 * @param product_code 产品代码
 * @param expiry_days 有效期天数
 * @return 生成的授权码
 */
unsigned long generate_license_key(int company_id, const char* product_code, int expiry_days) {
    unsigned long hash = 0x12345678;
    
    // 核心算法逻辑（商业机密）
    hash ^= (company_id << 16);
    hash ^= (company_id >> 16);
    
    for (int i = 0; i < strlen(product_code); i++) {
        hash = hash * 31 + product_code[i];
    }
    
    hash ^= (expiry_days * 86400);
    
    // 使用硬编码的盐值（安全问题）
    const char* SALT = "MySecretSaltValueDoNotLeak2024";
    for (int i = 0; i < strlen(SALT); i++) {
        hash = hash * 37 + SALT[i];
    }
    
    return hash;
}

// ============================================
// 6. 内网敏感路径信息
// ============================================

#define INTERNAL_GITLAB_URL   "http://gitlab.company.internal/corp/core-project"
#define INTERNAL_JENKINS_URL  "https://jenkins.company.internal/job/deploy"
#define NAS_SENSITIVE_PATH    "\\\\nas-01\\finance\\quarterly_reports\\2024\\Q4\\"
#define SMB_SHARE_PATH        "smb://storage.internal/confidential/board_meeting/"

// ============================================
// 7. 主函数
// ============================================

int main(int argc, char* argv[]) {
    printf("=== DLP Sensitivity Test File ===\n");
    printf("This file contains simulated sensitive data for DLP testing.\n\n");
    
    // 显示数据库配置（敏感信息）
    printf("[Database Configuration]\n");
    printf("  Host: %s\n", PROD_DB_HOST);
    printf("  Database: %s\n", PROD_DB_NAME);
    printf("  User: %s\n", PROD_DB_USER);
    printf("  Password: %s\n", PROD_DB_PASSWORD);
    
    // 显示客户信息
    printf("\n[Top Customer List]\n");
    for (int i = 0; i < 4; i++) {
        printf("  Customer: %s, Contact: %s, Amount: %.2f\n",
               g_top_customers[i].name,
               g_top_customers[i].contact,
               g_top_customers[i].contract_amount);
    }
    
    // 生成授权码
    unsigned long license = generate_license_key(10086, "ENTERPRISE", 365);
    printf("\n[License Generated]\n");
    printf("  License Code: %lu\n", license);
    
    printf("\n=== End of Test File ===\n");
    return 0;
}