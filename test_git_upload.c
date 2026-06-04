/**
 * @file test_git_upload.c
 * @brief Git 仓库上传测试用例 - 用于验证飞连源码外发监控
 * @warning 仅供安全策略测试使用，请勿推送到公开仓库
 */

#include <stdio.h>
#include <string.h>

// ============================================
// 模拟的敏感配置信息（触发检测用）
// ============================================

// 数据库凭证（模拟）
#define DB_HOST     "10.10.10.100"
#define DB_PORT     3306
#define DB_USER     "prod_admin"
#define DB_PASSWORD "Pr0d@DM1n!2024#Secure"

// API 密钥（模拟）
static const char* API_SECRET_KEY = "sk_live_4eC39HqLyjWDarjtT1zdp7dcXYZ";
static const char* JWT_SECRET = "jwt_signing_key_for_production_2024";

// 云服务凭证（模拟）
static const char* AWS_ACCESS_KEY = "AKIAIOSFODNN7EXAMPLE";
static const char* AWS_SECRET_KEY = "wJalrXUtnFEMI/K7MDENG/bPxRfiCYEXAMPLEKEY";

// ============================================
// 核心业务逻辑（模拟敏感算法）
// ============================================

// 用户授权算法
int generate_license(const char* user_id, const char* machine_id) {
    if (user_id == NULL || machine_id == NULL) {
        return -1;
    }
    
    // 模拟授权码生成逻辑
    int hash = 0;
    for (int i = 0; i < strlen(user_id); i++) {
        hash = (hash * 31 + user_id[i]) % 1000000;
    }
    for (int i = 0; i < strlen(machine_id); i++) {
        hash = (hash * 37 + machine_id[i]) % 1000000;
    }
    
    return hash * 86421;
}

// 数据解密函数
void decrypt_sensitive_data(const unsigned char* encrypted, size_t len) {
    // 模拟敏感数据解密逻辑
    printf("Decrypting %zu bytes of protected data...\n", len);
    // 实际业务中的解密实现
}

// ============================================
// 主函数
// ============================================

int main(int argc, char* argv[]) {
    printf("=== Git Upload Test File ===\n");
    printf("This file is used to test source code exfiltration detection.\n\n");
    
    char user_id[] = "admin_001";
    char machine_id[] = "PC-PROD-SERVER-01";
    
    int license = generate_license(user_id, machine_id);
    printf("Generated License: %d\n", license);
    
    printf("\n=== End of Test File ===\n");
    return 0;
}