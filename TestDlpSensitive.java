/**
 * @file TestDlpSensitive.java
 * @brief 飞连 DLP 敏感数据检测测试文件 (Java)
 * @warning 仅供安全策略测试使用，请勿外发到公开仓库
 */

package com.company.production.core;

import java.sql.*;
import java.util.*;
import javax.crypto.*;
import javax.crypto.spec.*;

// ============================================
// 1. 数据库连接配置（敏感信息）
// ============================================

class DatabaseConfig {
    // 硬编码的生产环境数据库配置 ⚠️ 敏感信息
    private static final String PROD_DB_HOST = "10.10.100.50";
    private static final int PROD_DB_PORT = 3306;
    private static final String PROD_DB_NAME = "corp_finance_db";
    private static final String PROD_DB_USER = "finance_admin";
    private static final String PROD_DB_PASSWORD = "Fin@nce#2024!SecurePass";
    
    // 备份数据库凭证
    private static final String BACKUP_DB_URL = "jdbc:mysql://192.168.10.200:3306/backup";
    private static final String BACKUP_DB_USER = "backup_user";
    private static final String BACKUP_DB_PASSWORD = "Backup@Pass123456";
    
    // 内网数据库连接池配置
    private static final String HIKARI_CONFIG = 
        "jdbc:mysql://prod-db-master.internal:3306/corp?" +
        "user=prod_admin&password=Prod@DM1n!2024&useSSL=false";
    
    public Connection getProductionConnection() throws SQLException {
        // 使用硬编码密码建立连接
        return DriverManager.getConnection(
            "jdbc:mysql://" + PROD_DB_HOST + ":" + PROD_DB_PORT + "/" + PROD_DB_NAME,
            PROD_DB_USER,
            PROD_DB_PASSWORD
        );
    }
}

// ============================================
// 2. API 密钥和第三方服务凭证
// ============================================

class ApiCredentials {
    // Stripe 支付密钥
    public static final String STRIPE_SECRET_KEY = "sk_live_4eC39HqLyjWDarjtT1zdp7dcXYZ_Prod";
    public static final String STRIPE_PUBLISHABLE_KEY = "pk_live_51ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    public static final String STRIPE_WEBHOOK_SECRET = "whsec_abc123def456ghi789jkl012mno345pqr";
    
    // AWS 凭证
    public static final String AWS_ACCESS_KEY_ID = "AKIAIOSFODNN7EXAMPLE12345";
    public static final String AWS_SECRET_ACCESS_KEY = "wJalrXUtnFEMI/K7MDENG/bPxRfiCYEXAMPLEKEY67890";
    public static final String AWS_REGION = "cn-north-1";
    public static final String AWS_S3_BUCKET = "s3://corp-sensitive-data-bucket/backup/";
    
    // 阿里云凭证
    public static final String ALIYUN_ACCESS_KEY = "LTAI5txxxxxxxxxxxxxxxxxxxx";
    public static final String ALIYUN_SECRET_KEY = "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx";
    
    // 腾讯云凭证
    public static final String TENCENT_SECRET_ID = "AKIDxxxxxxxxxxxxxxxxxxxxxxxxxxxxx";
    public static final String TENCENT_SECRET_KEY = "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx";
    
    // JWT 签名密钥
    public static final String JWT_SECRET_KEY = "jwt_signing_key_for_production_environment_2024_secure";
    
    // GitHub Token（模拟）
    public static final String GITHUB_TOKEN = "github_pat_11ABC123DEF456GHI789JKL0MNO1234567890";
    
    // GitLab Token
    public static final String GITLAB_TOKEN = "glpat-ABC123DEF456GHI789JKL";
    
    // 内部 API Key
    private static final String INTERNAL_API_KEY = "internal_api_9f8e7d6c5b4a3210";
}

// ============================================
// 3. 敏感业务数据模型
// ============================================

class CustomerInfo {
    private int id;
    private String name;
    private String contactPerson;
    private String phone;
    private String email;
    private BigDecimal contractAmount;
    private LocalDate contractExpiry;
    private String serviceLevel;
    
    // 构造函数和getter/setter省略
}

class EmployeeInfo {
    private int id;
    private String name;
    private String department;
    private String position;
    private BigDecimal salary;
    private String bankAccount;
    private String idCard;  // 身份证号
    private String phone;
}

// ============================================
// 4. 敏感数据管理类
// ============================================

class SensitiveDataManager {
    // 模拟千万级大客户数据
    private static final List<Map<String, Object>> TOP_CUSTOMERS = Arrays.asList(
        new HashMap<String, Object>() {{
            put("id", 1001);
            put("name", "XX科技集团有限公司");
            put("contact", "王总经理");
            put("phone", "13912340001");
            put("amount", 2500000.00);
            put("level", "enterprise");
        }},
        new HashMap<String, Object>() {{
            put("id", 1002);
            put("name", "YY金融服务有限公司");
            put("contact", "李财务总监");
            put("phone", "13912340002");
            put("amount", 5800000.00);
            put("level", "enterprise");
        }},
        new HashMap<String, Object>() {{
            put("id", 1003);
            put("name", "ZZ制造股份有限公司");
            put("contact", "赵董事长");
            put("phone", "13912340003");
            put("amount", 12000000.00);
            put("level", "vip");
        }},
        new HashMap<String, Object>() {{
            put("id", 1004);
            put("name", "AA国际贸易集团");
            put("contact", "孙总经理");
            put("phone", "13912340004");
            put("amount", 8750000.00);
            put("level", "enterprise");
        }},
        new HashMap<String, Object>() {{
            put("id", 1005);
            put("name", "BB新能源科技有限公司");
            put("contact", "周总");
            put("phone", "13912340005");
            put("amount", 15000000.00);
            put("level", "vip");
        }}
    );
    
    // 模拟员工薪资数据
    private static final List<Map<String, Object>> EMPLOYEES = Arrays.asList(
        new HashMap<String, Object>() {{
            put("name", "张伟");
            put("department", "技术研发部");
            put("position", "架构师");
            put("salary", 58000.00);
            put("bankAccount", "6212260200123456789");
            put("idCard", "11010119900307663X");
        }},
        new HashMap<String, Object>() {{
            put("name", "李娜");
            put("department", "技术研发部");
            put("position", "技术总监");
            put("salary", 89000.00);
            put("bankAccount", "6212260200987654321");
            put("idCard", "11010119850512456X");
        }},
        new HashMap<String, Object>() {{
            put("name", "王强");
            put("department", "销售部");
            put("position", "销售总监");
            put("salary", 75000.00);
            put("bankAccount", "6212260200555555555");
            put("idCard", "11010119801201123X");
        }}
    );
    
    public void printCustomerList() {
        System.out.println("=== 大客户名单（机密）===");
        for (Map<String, Object> customer : TOP_CUSTOMERS) {
            System.out.printf("客户: %s, 联系人: %s, 合同金额: %.2f 元%n",
                customer.get("name"), customer.get("contact"), customer.get("amount"));
        }
    }
    
    public void printSalaryList() {
        System.out.println("=== 薪资列表（机密）===");
        for (Map<String, Object> emp : EMPLOYEES) {
            System.out.printf("员工: %s, 部门: %s, 薪资: %.2f 元, 银行账号: %s%n",
                emp.get("name"), emp.get("department"), emp.get("salary"), emp.get("bankAccount"));
        }
    }
}

// ============================================
// 5. 加密工具类（硬编码密钥）
// ============================================

class EncryptionUtils {
    // 硬编码的 AES 密钥 ⚠️ 严重安全问题
    private static final String AES_KEY = "2b7e151628aed2a6abf7158809cf4f3c";
    private static final String AES_IV = "0123456789abcdef";
    
    // 硬编码的 RSA 私钥
    private static final String RSA_PRIVATE_KEY = 
        "-----BEGIN RSA PRIVATE KEY-----\n" +
        "MIIEpAIBAAKCAQEAu1Z2vZK7n4c8L9xM0N5oP7qRtS3wXyF6hJ8kL9zXcVbNmP0o\n" +
        "I9uYtR5eW7aQ8sDfGhJkLzXcVbNmP0oI9uYtR5eW7aQ8sDfGhJkLzXcVbNmP0oI9\n" +
        "-----END RSA PRIVATE KEY-----";
    
    // 使用硬编码密钥进行加密
    public static String encryptData(String plainText) throws Exception {
        // 实际加密逻辑（使用硬编码的密钥）
        SecretKeySpec keySpec = new SecretKeySpec(AES_KEY.getBytes(), "AES");
        // ... 加密代码
        return "encrypted_data";
    }
}

// ============================================
// 6. 授权算法（核心知识产权）
// ============================================

class LicenseGenerator {
    // 硬编码的算法常量（商业机密）
    private static final long MAGIC_NUMBER = 0xDEADBEEFL;
    private static final int SALT_VALUE = 0x12345678;
    
    /**
     * @brief 生成产品授权码的核心算法
     * @param companyName 公司名称
     * @param productId 产品ID
     * @param expiryDays 有效期天数
     * @return 授权码
     */
    public static String generateLicense(String companyName, int productId, int expiryDays) {
        // 核心算法逻辑（商业机密 - 请勿外泄）
        String seed = companyName + productId + expiryDays;
        long hash1 = computeHash(seed);
        
        // 第二层加密
        long hash2 = hash1 ^ MAGIC_NUMBER;
        for (int i = 0; i < expiryDays; i++) {
            hash2 = (hash2 * 31) + (i % 97);
        }
        
        // 生成最终授权码
        return String.format("LIC-%08X-%08X-%08X", 
            (hash1 >> 16) & 0xFFFFFFFFL, 
            hash2 & 0xFFFFL, 
            (hash1 ^ hash2) % 1000000L);
    }
    
    private static long computeHash(String input) {
        long hash = MAGIC_NUMBER;
        for (char c : input.toCharArray()) {
            hash = ((hash << 5) + hash) + (long) c;
            hash ^= SALT_VALUE;
        }
        return hash;
    }
}

// ============================================
// 7. 敏感路径配置
// ============================================

class InternalConfig {
    // 内部服务地址
    public static final String GITLAB_REPO = "http://gitlab.company.internal/corp/core-system";
    public static final String JENKINS_JOB = "https://jenkins.company.internal/job/deploy-to-prod";
    public static final String NAS_SENSITIVE = "\\\\nas-01\\finance\\quarterly_reports\\2024\\Q4\\confidential";
    public static final String CONFLUENCE_PAGE = "https://confluence.company.internal/pages/viewpage.action?pageId=123456";
    public static final String RABBITMQ_HOST = "rabbitmq.internal.company.com";
    public static final String RABBITMQ_USER = "prod_user";
    public static final String RABBITMQ_PASSWORD = "Rabbit@Prod2024!";
    
    // Redis 配置
    public static final String REDIS_HOST = "redis-prod.internal";
    public static final int REDIS_PORT = 6379;
    public static final String REDIS_PASSWORD = "Redis@Secret2024";
}

// ============================================
// 8. Spring Boot 配置文件模拟
// ============================================

class ApplicationYamlGenerator {
    public static String generate() {
        return """
            # application-prod.yml - 生产环境配置（敏感）
            spring:
              datasource:
                url: jdbc:mysql://prod-db.internal:3306/corp?useSSL=false
                username: prod_user
                password: Prod@Pass2024!
              redis:
                host: redis-prod.internal
                port: 6379
                password: Redis@Secret2024
              rabbitmq:
                host: rabbitmq.internal
                username: prod_user
                password: Rabbit@Prod2024!
            
            api:
              stripe:
                secret-key: sk_live_4eC39HqLyjWDarjtT1zdp7dc
              aws:
                access-key: AKIAIOSFODNN7EXAMPLE
                secret-key: wJalrXUtnFEMI/K7MDENG/bPxRfiCYEXAMPLEKEY
            
            jwt:
              secret: jwt_signing_key_for_production_environment_2024
            
            logging:
              file:
                path: /var/log/corp-application
            """;
    }
}

// ============================================
// 9. 主类
// ============================================

public class TestDlpSensitive {
    
    public static void main(String[] args) {
        System.out.println("=== DLP Sensitivity Test File (Java) ===");
        System.out.println("This file contains simulated sensitive data for DLP testing.\n");
        
        // 1. 显示数据库配置
        System.out.println("[Database Configuration]");
        DatabaseConfig dbConfig = new DatabaseConfig();
        try {
            System.out.println("  Production DB Config ready");
        } catch (Exception e) {
            System.out.println("  DB error: " + e.getMessage());
        }
        
        // 2. 显示 API 密钥
        System.out.println("\n[API Keys & Tokens]");
        System.out.println("  Stripe Secret: " + ApiCredentials.STRIPE_SECRET_KEY);
        System.out.println("  AWS Access Key: " + ApiCredentials.AWS_ACCESS_KEY_ID);
        System.out.println("  AWS Secret Key: " + ApiCredentials.AWS_SECRET_ACCESS_KEY);
        System.out.println("  JWT Secret: " + ApiCredentials.JWT_SECRET_KEY);
        System.out.println("  GitHub Token: " + ApiCredentials.GITHUB_TOKEN);
        
        // 3. 显示敏感业务数据
        SensitiveDataManager dataManager = new SensitiveDataManager();
        System.out.println("\n[Sensitive Business Data]");
        dataManager.printCustomerList();
        dataManager.printSalaryList();
        
        // 4. 生成授权码
        String license = LicenseGenerator.generateLicense("XX科技集团", 10086, 365);
        System.out.println("\n[License Generated]");
        System.out.println("  License Code: " + license);
        
        // 5. 显示内部路径
        System.out.println("\n[Internal Paths (Confidential)]");
        System.out.println("  GitLab: " + InternalConfig.GITLAB_REPO);
        System.out.println("  Jenkins: " + InternalConfig.JENKINS_JOB);
        System.out.println("  NAS Path: " + InternalConfig.NAS_SENSITIVE);
        System.out.println("  RabbitMQ: " + InternalConfig.RABBITMQ_HOST);
        System.out.println("  Redis Password: " + InternalConfig.REDIS_PASSWORD);
        
        // 6. 显示配置文件
        System.out.println("\n[Application Configuration (YAML)]");
        System.out.println(ApplicationYamlGenerator.generate());
        
        // 7. 加密示例
        System.out.println("\n[Encryption Example]");
        System.out.println("  Using hardcoded AES key: " + EncryptionUtils.AES_KEY);
        
        System.out.println("\n=== End of Test File ===");
    }
}