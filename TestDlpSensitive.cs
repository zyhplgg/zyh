/**
 * @file TestDlpSensitive.cs
 * @brief 飞连 DLP 敏感数据检测测试文件 (C#)
 * @warning 仅供安全策略测试使用，请勿外发到公开仓库
 */

using System;
using System.Collections.Generic;
using System.Data;
using System.Data.SqlClient;
using System.Security.Cryptography;
using System.Text;
using System.Text.Json;
using System.Threading.Tasks;

namespace Company.Production.Core
{
    // ============================================
    // 1. 数据库连接配置（敏感信息）
    // ============================================
    
    public class DatabaseConfig
    {
        // 硬编码的生产环境数据库配置 ⚠️ 敏感信息
        private const string ProdDbHost = "10.10.100.50";
        private const int ProdDbPort = 3306;
        private const string ProdDbName = "corp_finance_db";
        private const string ProdDbUser = "finance_admin";
        private const string ProdDbPassword = "Fin@nce#2024!SecurePass";
        
        // SQL Server 配置
        private const string MsSqlConnection = 
            "Server=prod-sql.internal,1433;Database=CorpDatabase;User Id=sa;Password=Sql@Admin2024!;TrustServerCertificate=True;";
        
        // PostgreSQL 配置
        private const string PostgresConnection = 
            "Host=postgres-prod.internal;Port=5432;Database=corp_db;Username=postgres_admin;Password=Postgres@2024!;";
        
        // MongoDB 配置
        private const string MongoConnection = 
            "mongodb://mongodb_admin:Mongo@Prod2024!@mongo-prod.internal:27017/corp?authSource=admin";
        
        // Oracle 配置
        private const string OracleConnection = 
            "Data Source=oracle-prod.internal:1521/ORCL;User Id=system;Password=Oracle@2024!;";
        
        // 连接池配置
        public string GetProductionConnectionString()
        {
            return $"Server={ProdDbHost};Database={ProdDbName};User Id={ProdDbUser};Password={ProdDbPassword};";
        }
    }
    
    // ============================================
    // 2. API 密钥和第三方服务凭证
    // ============================================
    
    public static class ApiCredentials
    {
        // Stripe 支付密钥
        public const string StripeSecretKey = "sk_live_4eC39HqLyjWDarjtT1zdp7dcXYZ_Prod";
        public const string StripePublishableKey = "pk_live_51ABCDEFGHIJKLMNOPQRSTUVWXYZ";
        public const string StripeWebhookSecret = "whsec_abc123def456ghi789jkl012mno345pqr";
        
        // AWS 凭证
        public const string AwsAccessKeyId = "AKIAIOSFODNN7EXAMPLE12345";
        public const string AwsSecretAccessKey = "wJalrXUtnFEMI/K7MDENG/bPxRfiCYEXAMPLEKEY67890";
        public const string AwsRegion = "cn-north-1";
        public const string AwsS3Bucket = "s3://corp-sensitive-data-bucket/backup/";
        
        // 阿里云凭证
        public const string AliyunAccessKey = "LTAI5txxxxxxxxxxxxxxxxxxxx";
        public const string AliyunSecretKey = "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx";
        
        // 腾讯云凭证
        public const string TencentSecretId = "AKIDxxxxxxxxxxxxxxxxxxxxxxxxxxxxx";
        public const string TencentSecretKey = "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx";
        
        // JWT 签名密钥
        public const string JwtSecretKey = "jwt_signing_key_for_production_environment_2024_secure";
        
        // GitHub Token（模拟）
        public const string GitHubToken = "github_pat_11ABC123DEF456GHI789JKL0MNO1234567890";
        
        // GitLab Token
        public const string GitLabToken = "glpat-ABC123DEF456GHI789JKL";
        
        // Azure 凭证
        public const string AzureConnectionString = 
            "DefaultEndpointsProtocol=https;AccountName=corpstorage;AccountKey=abc123def456ghi789jkl012mno345pqr678stu;EndpointSuffix=core.windows.net";
        
        // 内部 API Key
        private const string InternalApiKey = "internal_api_9f8e7d6c5b4a3210";
    }
    
    // ============================================
    // 3. 敏感业务数据模型
    // ============================================
    
    public class CustomerInfo
    {
        public int Id { get; set; }
        public string Name { get; set; }
        public string ContactPerson { get; set; }
        public string Phone { get; set; }
        public string Email { get; set; }
        public decimal ContractAmount { get; set; }
        public DateTime ContractExpiry { get; set; }
        public string ServiceLevel { get; set; }
    }
    
    public class EmployeeInfo
    {
        public int Id { get; set; }
        public string Name { get; set; }
        public string Department { get; set; }
        public string Position { get; set; }
        public decimal Salary { get; set; }
        public string BankAccount { get; set; }
        public string IdCard { get; set; }  // 身份证号
        public string Phone { get; set; }
        public string Email { get; set; }
    }
    
    // ============================================
    // 4. 敏感数据管理类
    // ============================================
    
    public class SensitiveDataManager
    {
        // 模拟千万级大客户数据
        private static readonly List<CustomerInfo> TopCustomers = new List<CustomerInfo>
        {
            new CustomerInfo { Id = 1001, Name = "XX科技集团有限公司", ContactPerson = "王总经理", 
                Phone = "13912340001", Email = "wang@xxtech.com", ContractAmount = 2500000.00m, 
                ContractExpiry = DateTime.Parse("2025-12-31"), ServiceLevel = "enterprise" },
            new CustomerInfo { Id = 1002, Name = "YY金融服务有限公司", ContactPerson = "李财务总监", 
                Phone = "13912340002", Email = "li@yyfinance.com", ContractAmount = 5800000.00m, 
                ContractExpiry = DateTime.Parse("2025-06-30"), ServiceLevel = "enterprise" },
            new CustomerInfo { Id = 1003, Name = "ZZ制造股份有限公司", ContactPerson = "赵董事长", 
                Phone = "13912340003", Email = "zhao@zzmfg.com", ContractAmount = 12000000.00m, 
                ContractExpiry = DateTime.Parse("2026-03-31"), ServiceLevel = "vip" },
            new CustomerInfo { Id = 1004, Name = "AA国际贸易集团", ContactPerson = "孙总经理", 
                Phone = "13912340004", Email = "sun@aatrade.com", ContractAmount = 8750000.00m, 
                ContractExpiry = DateTime.Parse("2025-09-30"), ServiceLevel = "enterprise" },
            new CustomerInfo { Id = 1005, Name = "BB新能源科技有限公司", ContactPerson = "周总", 
                Phone = "13912340005", Email = "zhou@bbenergy.com", ContractAmount = 15000000.00m, 
                ContractExpiry = DateTime.Parse("2026-06-30"), ServiceLevel = "vip" }
        };
        
        // 模拟员工薪资数据
        private static readonly List<EmployeeInfo> Employees = new List<EmployeeInfo>
        {
            new EmployeeInfo { Id = 2001, Name = "张伟", Department = "技术研发部", Position = "架构师", 
                Salary = 58000.00m, BankAccount = "6212260200123456789", IdCard = "11010119900307663X", 
                Phone = "13800138001", Email = "zhang.wei@company.com" },
            new EmployeeInfo { Id = 2002, Name = "李娜", Department = "技术研发部", Position = "技术总监", 
                Salary = 89000.00m, BankAccount = "6212260200987654321", IdCard = "11010119850512456X", 
                Phone = "13800138002", Email = "li.na@company.com" },
            new EmployeeInfo { Id = 2003, Name = "王强", Department = "销售部", Position = "销售总监", 
                Salary = 75000.00m, BankAccount = "6212260200555555555", IdCard = "11010119801201123X", 
                Phone = "13800138003", Email = "wang.qiang@company.com" }
        };
        
        public void PrintCustomerList()
        {
            Console.WriteLine("=== 大客户名单（机密）===");
            foreach (var customer in TopCustomers)
            {
                Console.WriteLine($"客户: {customer.Name}, 联系人: {customer.ContactPerson}, 合同金额: {customer.ContractAmount:C} 元");
            }
        }
        
        public void PrintSalaryList()
        {
            Console.WriteLine("=== 薪资列表（机密）===");
            foreach (var emp in Employees)
            {
                Console.WriteLine($"员工: {emp.Name}, 部门: {emp.Department}, 薪资: {emp.Salary:C} 元, 银行账号: {emp.BankAccount}");
            }
        }
        
        public List<CustomerInfo> GetTopCustomers() => TopCustomers;
        public List<EmployeeInfo> GetEmployees() => Employees;
    }
    
    // ============================================
    // 5. 加密工具类（硬编码密钥）
    // ============================================
    
    public static class EncryptionUtils
    {
        // 硬编码的 AES 密钥 ⚠️ 严重安全问题
        private static readonly byte[] AesKey = Encoding.UTF8.GetBytes("2b7e151628aed2a6abf7158809cf4f3c");
        private static readonly byte[] AesIV = Encoding.UTF8.GetBytes("0123456789abcdef");
        
        // 硬编码的 RSA 私钥
        private const string RsaPrivateKey = @"
-----BEGIN RSA PRIVATE KEY-----
MIIEpAIBAAKCAQEAu1Z2vZK7n4c8L9xM0N5oP7qRtS3wXyF6hJ8kL9zXcVbNmP0o
I9uYtR5eW7aQ8sDfGhJkLzXcVbNmP0oI9uYtR5eW7aQ8sDfGhJkLzXcVbNmP0oI9
-----END RSA PRIVATE KEY-----";
        
        // 使用硬编码密钥进行加密
        public static string EncryptData(string plainText)
        {
            using (var aes = Aes.Create())
            {
                aes.Key = AesKey;
                aes.IV = AesIV;
                aes.Mode = CipherMode.CBC;
                aes.Padding = PaddingMode.PKCS7;
                
                var encryptor = aes.CreateEncryptor();
                var plainBytes = Encoding.UTF8.GetBytes(plainText);
                var cipherBytes = encryptor.TransformFinalBlock(plainBytes, 0, plainBytes.Length);
                return Convert.ToBase64String(cipherBytes);
            }
        }
        
        // 硬编码的 Base64 解码密钥
        private const string Base64Key = "dGhpc0lzQVNlY3JldEtleUZvckRlcGxveW1lbnQ=";
    }
    
    // ============================================
    // 6. 授权算法（核心知识产权）
    // ============================================
    
    public class LicenseGenerator
    {
        // 硬编码的算法常量（商业机密）
        private const long MagicNumber = 0xDEADBEEFL;
        private const int SaltValue = 0x12345678;
        
        /// <summary>
        /// 生成产品授权码的核心算法
        /// </summary>
        /// <param name="companyName">公司名称</param>
        /// <param name="productId">产品ID</param>
        /// <param name="expiryDays">有效期天数</param>
        /// <returns>授权码</returns>
        public static string GenerateLicense(string companyName, int productId, int expiryDays)
        {
            // 核心算法逻辑（商业机密 - 请勿外泄）
            string seed = $"{companyName}{productId}{expiryDays}";
            long hash1 = ComputeHash(seed);
            
            // 第二层加密
            long hash2 = hash1 ^ MagicNumber;
            for (int i = 0; i < expiryDays; i++)
            {
                hash2 = (hash2 * 31) + (i % 97);
            }
            
            // 生成最终授权码
            return $"LIC-{(hash1 >> 16):X8}-{(hash2 & 0xFFFF):X4}-{((hash1 ^ hash2) % 1000000):X6}";
        }
        
        private static long ComputeHash(string input)
        {
            long hash = MagicNumber;
            foreach (char c in input)
            {
                hash = ((hash << 5) + hash) + (long)c;
                hash ^= SaltValue;
            }
            return hash;
        }
    }
    
    // ============================================
    // 7. 敏感路径配置
    // ============================================
    
    public static class InternalConfig
    {
        // 内部服务地址
        public const string GitLabRepo = "http://gitlab.company.internal/corp/core-system";
        public const string JenkinsJob = "https://jenkins.company.internal/job/deploy-to-prod";
        public const string NasSensitive = @"\\nas-01\finance\quarterly_reports\2024\Q4\confidential";
        public const string ConfluencePage = "https://confluence.company.internal/pages/viewpage.action?pageId=123456";
        public const string RabbitMqHost = "rabbitmq.internal.company.com";
        public const string RabbitMqUser = "prod_user";
        public const string RabbitMqPassword = "Rabbit@Prod2024!";
        
        // Redis 配置
        public const string RedisHost = "redis-prod.internal";
        public const int RedisPort = 6379;
        public const string RedisPassword = "Redis@Secret2024";
        
        // Consul 配置
        public const string ConsulUrl = "http://consul.internal:8500";
        public const string ConsulAclToken = "consul_acl_token_9f8e7d6c5b4a3210";
        
        // Kubernetes 配置
        public const string KubeConfig = @"
apiVersion: v1
clusters:
- cluster:
    certificate-authority-data: LS0tLS1CRUdJTiBDRV...
    server: https://k8s-api.internal:6443
  name: prod-cluster
users:
- name: admin
  user:
    client-certificate-data: LS0tLS1CRUdJTiBDRV...
    client-key-data: LS0tLS1CRUdJTiBSU0EgUFJJVkFURSBLRVktLS0tLQo=";
    }
    
    // ============================================
    // 8. appsettings.json 配置模拟
    // ============================================
    
    public static class AppSettingsGenerator
    {
        public static string Generate()
        {
            return @"
{
  ""ConnectionStrings"": {
    ""DefaultConnection"": ""Server=prod-sql.internal;Database=CorpDatabase;User Id=sa;Password=Sql@Admin2024!;"",
    ""RedisConnection"": ""redis-prod.internal:6379,password=Redis@Secret2024"",
    ""RabbitMQConnection"": ""amqp://prod_user:Rabbit@Prod2024!@rabbitmq.internal:5672""
  },
  ""Jwt"": {
    ""Secret"": ""jwt_signing_key_for_production_environment_2024_secure"",
    ""Issuer"": ""https://auth.company.internal"",
    ""Audience"": ""https://api.company.internal""
  },
  ""Stripe"": {
    ""SecretKey"": ""sk_live_4eC39HqLyjWDarjtT1zdp7dc"",
    ""WebhookSecret"": ""whsec_abc123def456ghi789jkl""
  },
  ""AWS"": {
    ""AccessKey"": ""AKIAIOSFODNN7EXAMPLE"",
    ""SecretKey"": ""wJalrXUtnFEMI/K7MDENG/bPxRfiCYEXAMPLEKEY"",
    ""Region"": ""cn-north-1""
  },
  ""Azure"": {
    ""StorageConnectionString"": ""DefaultEndpointsProtocol=https;AccountName=corpstorage;AccountKey=abc123def456ghi789jkl012mno345pqr678stu;EndpointSuffix=core.windows.net""
  },
  ""Serilog"": {
    ""WriteTo"": [
      {
        ""Name"": ""File"",
        ""Args"": {
          ""path"": ""C:\\Logs\\corp-application\\log-.txt""
        }
      }
    ]
  }
}";
        }
    }
    
    // ============================================
    // 9. 主类
    // ============================================
    
    public class TestDlpSensitive
    {
        public static void Main(string[] args)
        {
            Console.WriteLine("=== DLP Sensitivity Test File (C#) ===");
            Console.WriteLine("This file contains simulated sensitive data for DLP testing.\n");
            
            // 1. 显示数据库配置
            Console.WriteLine("[Database Configuration]");
            var dbConfig = new DatabaseConfig();
            Console.WriteLine($"  SQL Server: {dbConfig.GetProductionConnectionString()}");
            
            // 2. 显示 API 密钥
            Console.WriteLine("\n[API Keys & Tokens]");
            Console.WriteLine($"  Stripe Secret: {ApiCredentials.StripeSecretKey}");
            Console.WriteLine($"  AWS Access Key: {ApiCredentials.AwsAccessKeyId}");
            Console.WriteLine($"  AWS Secret Key: {ApiCredentials.AwsSecretAccessKey}");
            Console.WriteLine($"  JWT Secret: {ApiCredentials.JwtSecretKey}");
            Console.WriteLine($"  GitHub Token: {ApiCredentials.GitHubToken}");
            Console.WriteLine($"  Azure Connection: {ApiCredentials.AzureConnectionString}");
            
            // 3. 显示敏感业务数据
            var dataManager = new SensitiveDataManager();
            Console.WriteLine("\n[Sensitive Business Data]");
            dataManager.PrintCustomerList();
            dataManager.PrintSalaryList();
            
            // 4. 生成授权码
            string license = LicenseGenerator.GenerateLicense("XX科技集团", 10086, 365);
            Console.WriteLine("\n[License Generated]");
            Console.WriteLine($"  License Code: {license}");
            
            // 5. 显示内部路径
            Console.WriteLine("\n[Internal Paths (Confidential)]");
            Console.WriteLine($"  GitLab: {InternalConfig.GitLabRepo}");
            Console.WriteLine($"  Jenkins: {InternalConfig.JenkinsJob}");
            Console.WriteLine($"  NAS Path: {InternalConfig.NasSensitive}");
            Console.WriteLine($"  RabbitMQ: {InternalConfig.RabbitMqHost}");
            Console.WriteLine($"  Redis Password: {InternalConfig.RedisPassword}");
            Console.WriteLine($"  Consul Token: {InternalConfig.ConsulAclToken}");
            
            // 6. 显示配置文件
            Console.WriteLine("\n[AppSettings Configuration (JSON)]");
            Console.WriteLine(AppSettingsGenerator.Generate());
            
            // 7. 加密示例
            Console.WriteLine("\n[Encryption Example]");
            Console.WriteLine($"  Using hardcoded AES key: {Encoding.UTF8.GetString(EncryptionUtils.AesKey)}");
            
            Console.WriteLine("\n=== End of Test File ===");
        }
    }
}