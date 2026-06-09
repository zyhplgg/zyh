/**
 * @file test_dlp_sensitive.go
 * @brief 飞连 DLP 敏感数据检测测试文件 (Go)
 * @warning 仅供安全策略测试使用，请勿外发到公开仓库
 */

package main

import (
	"context"
	"crypto/aes"
	"crypto/cipher"
	"crypto/rsa"
	"crypto/x509"
	"database/sql"
	"encoding/base64"
	"encoding/json"
	"encoding/pem"
	"fmt"
	"log"
	"net/http"
	"os"
	"time"

	_ "github.com/go-sql-driver/mysql"
	"github.com/redis/go-redis/v9"
)

// ============================================
// 1. 数据库连接配置（敏感信息）
// ============================================

// 生产环境数据库配置 ⚠️ 敏感信息
const (
	ProdDBHost     = "10.10.100.50"
	ProdDBPort     = 3306
	ProdDBName     = "corp_finance_db"
	ProdDBUser     = "finance_admin"
	ProdDBPassword = "Fin@nce#2024!SecurePass"
)

// MySQL 连接字符串
var prodDBConnStr = fmt.Sprintf("%s:%s@tcp(%s:%d)/%s?charset=utf8mb4&parseTime=True&loc=Local",
	ProdDBUser, ProdDBPassword, ProdDBHost, ProdDBPort, ProdDBName)

// PostgreSQL 配置
const (
	PgHost     = "postgres-prod.internal"
	PgPort     = 5432
	PgUser     = "postgres_admin"
	PgPassword = "Postgres@2024!"
	PgDBName   = "corp_db"
)

var pgConnStr = fmt.Sprintf("host=%s port=%d user=%s password=%s dbname=%s sslmode=disable",
	PgHost, PgPort, PgUser, PgPassword, PgDBName)

// Redis 配置
const (
	RedisHost     = "redis-prod.internal"
	RedisPort     = 6379
	RedisPassword = "Redis@Secret2024"
)

// MongoDB 配置（模拟）
const MongoURI = "mongodb://mongodb_admin:Mongo@Prod2024!@mongo-prod.internal:27017/corp?authSource=admin"

// ============================================
// 2. API 密钥和第三方服务凭证
// ============================================

// Stripe 支付密钥
const (
	StripeSecretKey      = "sk_live_4eC39HqLyjWDarjtT1zdp7dcXYZ_Prod"
	StripePublishableKey = "pk_live_51ABCDEFGHIJKLMNOPQRSTUVWXYZ"
	StripeWebhookSecret  = "whsec_abc123def456ghi789jkl012mno345pqr"
)

// AWS 凭证
const (
	AwsAccessKeyID     = "AKIAIOSFODNN7EXAMPLE12345"
	AwsSecretAccessKey = "wJalrXUtnFEMI/K7MDENG/bPxRfiCYEXAMPLEKEY67890"
	AwsRegion          = "cn-north-1"
	AwsS3Bucket        = "s3://corp-sensitive-data-bucket/backup/"
)

// 阿里云凭证
const (
	AliyunAccessKey = "LTAI5txxxxxxxxxxxxxxxxxxxx"
	AliyunSecretKey = "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx"
)

// JWT 签名密钥
const JwtSecretKey = "jwt_signing_key_for_production_environment_2024_secure"

// GitHub Token（模拟）
const GitHubToken = "github_pat_11ABC123DEF456GHI789JKL0MNO1234567890"

// GitLab Token
const GitLabToken = "glpat-ABC123DEF456GHI789JKL"

// ============================================
// 3. 敏感业务数据模型
// ============================================

// CustomerInfo 客户信息
type CustomerInfo struct {
	ID             int       `json:"id"`
	Name           string    `json:"name"`
	ContactPerson  string    `json:"contact_person"`
	Phone          string    `json:"phone"`
	Email          string    `json:"email"`
	ContractAmount float64   `json:"contract_amount"`
	ContractExpiry time.Time `json:"contract_expiry"`
	ServiceLevel   string    `json:"service_level"`
}

// EmployeeInfo 员工信息
type EmployeeInfo struct {
	ID           int     `json:"id"`
	Name         string  `json:"name"`
	Department   string  `json:"department"`
	Position     string  `json:"position"`
	Salary       float64 `json:"salary"`
	BankAccount  string  `json:"bank_account"`
	IDCard       string  `json:"id_card"` // 身份证号
	Phone        string  `json:"phone"`
	Email        string  `json:"email"`
}

// 模拟千万级大客户数据
var topCustomers = []CustomerInfo{
	{ID: 1001, Name: "XX科技集团有限公司", ContactPerson: "王总经理", Phone: "13912340001", Email: "wang@xxtech.com", ContractAmount: 2500000.00, ContractExpiry: time.Date(2025, 12, 31, 0, 0, 0, 0, time.UTC), ServiceLevel: "enterprise"},
	{ID: 1002, Name: "YY金融服务有限公司", ContactPerson: "李财务总监", Phone: "13912340002", Email: "li@yyfinance.com", ContractAmount: 5800000.00, ContractExpiry: time.Date(2025, 6, 30, 0, 0, 0, 0, time.UTC), ServiceLevel: "enterprise"},
	{ID: 1003, Name: "ZZ制造股份有限公司", ContactPerson: "赵董事长", Phone: "13912340003", Email: "zhao@zzmfg.com", ContractAmount: 12000000.00, ContractExpiry: time.Date(2026, 3, 31, 0, 0, 0, 0, time.UTC), ServiceLevel: "vip"},
	{ID: 1004, Name: "AA国际贸易集团", ContactPerson: "孙总经理", Phone: "13912340004", Email: "sun@aatrade.com", ContractAmount: 8750000.00, ContractExpiry: time.Date(2025, 9, 30, 0, 0, 0, 0, time.UTC), ServiceLevel: "enterprise"},
	{ID: 1005, Name: "BB新能源科技有限公司", ContactPerson: "周总", Phone: "13912340005", Email: "zhou@bbenergy.com", ContractAmount: 15000000.00, ContractExpiry: time.Date(2026, 6, 30, 0, 0, 0, 0, time.UTC), ServiceLevel: "vip"},
}

// 模拟员工薪资数据
var employees = []EmployeeInfo{
	{ID: 2001, Name: "张伟", Department: "技术研发部", Position: "架构师", Salary: 58000.00, BankAccount: "6212260200123456789", IDCard: "11010119900307663X", Phone: "13800138001", Email: "zhang.wei@company.com"},
	{ID: 2002, Name: "李娜", Department: "技术研发部", Position: "技术总监", Salary: 89000.00, BankAccount: "6212260200987654321", IDCard: "11010119850512456X", Phone: "13800138002", Email: "li.na@company.com"},
	{ID: 2003, Name: "王强", Department: "销售部", Position: "销售总监", Salary: 75000.00, BankAccount: "6212260200555555555", IDCard: "11010119801201123X", Phone: "13800138003", Email: "wang.qiang@company.com"},
}

// ============================================
// 4. 加密工具类（硬编码密钥）
// ============================================

// 硬编码的 AES 密钥 ⚠️ 严重安全问题
var aesKey = []byte("2b7e151628aed2a6abf7158809cf4f3c")
var aesIV = []byte("0123456789abcdef")

// 硬编码的 RSA 私钥
const rsaPrivateKeyPEM = `
-----BEGIN RSA PRIVATE KEY-----
MIIEpAIBAAKCAQEAu1Z2vZK7n4c8L9xM0N5oP7qRtS3wXyF6hJ8kL9zXcVbNmP0o
I9uYtR5eW7aQ8sDfGhJkLzXcVbNmP0oI9uYtR5eW7aQ8sDfGhJkLzXcVbNmP0oI9
-----END RSA PRIVATE KEY-----`

// AES 加密函数
func encryptAES(plaintext string) (string, error) {
	block, err := aes.NewCipher(aesKey)
	if err != nil {
		return "", err
	}
	
	ciphertext := make([]byte, len(plaintext))
	stream := cipher.NewCFBEncrypter(block, aesIV)
	stream.XORKeyStream(ciphertext, []byte(plaintext))
	
	return base64.StdEncoding.EncodeToString(ciphertext), nil
}

// ============================================
// 5. 授权算法（核心知识产权）
// ============================================

const (
	magicNumber = 0xDEADBEEF
	saltValue   = 0x12345678
)

// generateLicense 生成产品授权码的核心算法（商业机密）
func generateLicense(companyName string, productID int, expiryDays int) string {
	// 核心算法逻辑（商业机密 - 请勿外泄）
	seed := fmt.Sprintf("%s%d%d", companyName, productID, expiryDays)
	hash1 := computeHash(seed)
	
	// 第二层加密
	hash2 := hash1 ^ magicNumber
	for i := 0; i < expiryDays; i++ {
		hash2 = (hash2 * 31) + (i % 97)
	}
	
	// 生成最终授权码
	return fmt.Sprintf("LIC-%08X-%04X-%06X", 
		(hash1>>16)&0xFFFFFFFF, 
		hash2&0xFFFF, 
		(hash1^hash2)%1000000)
}

func computeHash(input string) int64 {
	var hash int64 = magicNumber
	for _, c := range input {
		hash = ((hash << 5) + hash) + int64(c)
		hash ^= saltValue
	}
	return hash
}

// ============================================
// 6. 敏感路径配置
// ============================================

const (
	GitLabRepo      = "http://gitlab.company.internal/corp/core-system"
	JenkinsJob      = "https://jenkins.company.internal/job/deploy-to-prod"
	NasSensitive    = "\\\\nas-01\\finance\\quarterly_reports\\2024\\Q4\\confidential"
	ConfluencePage  = "https://confluence.company.internal/pages/viewpage.action?pageId=123456"
	RabbitMQHost    = "rabbitmq.internal.company.com"
	RabbitMQUser    = "prod_user"
	RabbitMQPass    = "Rabbit@Prod2024!"
)

// ============================================
// 7. 配置文件结构
// ============================================

type AppConfig struct {
	Database struct {
		Host     string `json:"host"`
		Port     int    `json:"port"`
		User     string `json:"user"`
		Password string `json:"password"`
		DBName   string `json:"dbname"`
	} `json:"database"`
	Redis struct {
		Host     string `json:"host"`
		Port     int    `json:"port"`
		Password string `json:"password"`
	} `json:"redis"`
	JWT struct {
		Secret string `json:"secret"`
	} `json:"jwt"`
	Stripe struct {
		SecretKey string `json:"secret_key"`
	} `json:"stripe"`
	AWS struct {
		AccessKey string `json:"access_key"`
		SecretKey string `json:"secret_key"`
		Region    string `json:"region"`
	} `json:"aws"`
}

// 生成模拟的配置文件内容
func generateAppConfig() *AppConfig {
	return &AppConfig{
		Database: struct {
			Host     string `json:"host"`
			Port     int    `json:"port"`
			User     string `json:"user"`
			Password string `json:"password"`
			DBName   string `json:"dbname"`
		}{
			Host:     ProdDBHost,
			Port:     ProdDBPort,
			User:     ProdDBUser,
			Password: ProdDBPassword,
			DBName:   ProdDBName,
		},
		Redis: struct {
			Host     string `json:"host"`
			Port     int    `json:"port"`
			Password string `json:"password"`
		}{
			Host:     RedisHost,
			Port:     RedisPort,
			Password: RedisPassword,
		},
		JWT: struct {
			Secret string `json:"secret"`
		}{
			Secret: JwtSecretKey,
		},
		Stripe: struct {
			SecretKey string `json:"secret_key"`
		}{
			SecretKey: StripeSecretKey,
		},
		AWS: struct {
			AccessKey string `json:"access_key"`
			SecretKey string `json:"secret_key"`
			Region    string `json:"region"`
		}{
			AccessKey: AwsAccessKeyID,
			SecretKey: AwsSecretAccessKey,
			Region:    AwsRegion,
		},
	}
}

// ============================================
// 8. HTTP 处理器（可能泄漏敏感信息）
// ============================================

func healthHandler(w http.ResponseWriter, r *http.Request) {
	response := map[string]string{
		"status": "ok",
		"version": "1.0.0",
		"env": "production",
	}
	w.Header().Set("Content-Type", "application/json")
	json.NewEncoder(w).Encode(response)
}

// 错误的调试端点，可能暴露内部信息
func debugHandler(w http.ResponseWriter, r *http.Request) {
	// ⚠️ 危险：暴露数据库连接信息
	w.Write([]byte(fmt.Sprintf("DB Connection: %s\n", prodDBConnStr)))
	w.Write([]byte(fmt.Sprintf("Redis: %s:%d\n", RedisHost, RedisPort)))
}

// ============================================
// 9. 主函数
// ============================================

func main() {
	fmt.Println("=== DLP Sensitivity Test File (Go) ===")
	fmt.Println("This file contains simulated sensitive data for DLP testing.\n")
	
	// 1. 显示数据库配置
	fmt.Println("[Database Configuration]")
	fmt.Printf("  MySQL: %s\n", prodDBConnStr)
	fmt.Printf("  PostgreSQL: %s\n", pgConnStr)
	fmt.Printf("  Redis: %s:%d (password: %s)\n", RedisHost, RedisPort, RedisPassword)
	fmt.Printf("  MongoDB: %s\n", MongoURI)
	
	// 2. 显示 API 密钥
	fmt.Println("\n[API Keys & Tokens]")
	fmt.Printf("  Stripe Secret: %s\n", StripeSecretKey)
	fmt.Printf("  AWS Access Key: %s\n", AwsAccessKeyID)
	fmt.Printf("  AWS Secret Key: %s\n", AwsSecretAccessKey)
	fmt.Printf("  JWT Secret: %s\n", JwtSecretKey)
	fmt.Printf("  GitHub Token: %s\n", GitHubToken)
	fmt.Printf("  GitLab Token: %s\n", GitLabToken)
	
	// 3. 显示敏感业务数据
	fmt.Println("\n[Sensitive Business Data]")
	fmt.Println("  Top Customers:")
	for _, c := range topCustomers {
		fmt.Printf("    - %s, 联系人: %s, 合同金额: %.2f 元\n", c.Name, c.ContactPerson, c.ContractAmount)
	}
	fmt.Println("  Employee Salary List:")
	for _, e := range employees {
		fmt.Printf("    - %s, 部门: %s, 薪资: %.2f 元, 银行账号: %s\n", e.Name, e.Department, e.Salary, e.BankAccount)
	}
	
	// 4. 生成授权码
	license := generateLicense("XX科技集团", 10086, 365)
	fmt.Printf("\n[License Generated]\n")
	fmt.Printf("  License Code: %s\n", license)
	
	// 5. 显示内部路径
	fmt.Println("\n[Internal Paths (Confidential)]")
	fmt.Printf("  GitLab: %s\n", GitLabRepo)
	fmt.Printf("  Jenkins: %s\n", JenkinsJob)
	fmt.Printf("  NAS Path: %s\n", NasSensitive)
	fmt.Printf("  RabbitMQ: %s (user: %s, password: %s)\n", RabbitMQHost, RabbitMQUser, RabbitMQPass)
	
	// 6. 显示配置文件
	fmt.Println("\n[Application Configuration]")
	config := generateAppConfig()
	configJSON, _ := json.MarshalIndent(config, "  ", "  ")
	fmt.Printf("  %s\n", string(configJSON))
	
	// 7. 加密示例
	fmt.Println("\n[Encryption Example]")
	encrypted, _ := encryptAES("sensitive data test")
	fmt.Printf("  Encrypted data (using hardcoded AES key): %s\n", encrypted)
	
	// 8. 模拟 HTTP 服务（仅演示）
	fmt.Println("\n[HTTP Handlers]")
	fmt.Println("  Health endpoint: /health")
	fmt.Println("  Debug endpoint: /debug (⚠️ exposes internal info)")
	
	fmt.Println("\n=== End of Test File ===")
	
	// 可选：启动 HTTP 服务（注释掉以避免实际运行）
	// http.HandleFunc("/health", healthHandler)
	// http.HandleFunc("/debug", debugHandler)
	// log.Fatal(http.ListenAndServe(":8080", nil))
}

// ============================================
// 10. 数据库连接示例（模拟）
// ============================================

// connectToProdDB 连接生产数据库（使用硬编码密码）
func connectToProdDB() (*sql.DB, error) {
	// ⚠️ 危险：在代码中硬编码生产环境密码
	db, err := sql.Open("mysql", prodDBConnStr)
	if err != nil {
		return nil, err
	}
	
	db.SetMaxOpenConns(25)
	db.SetMaxIdleConns(5)
	db.SetConnMaxLifetime(5 * time.Minute)
	
	return db, nil
}

// connectToRedis 连接 Redis
func connectToRedis() *redis.Client {
	client := redis.NewClient(&redis.Options{
		Addr:     fmt.Sprintf("%s:%d", RedisHost, RedisPort),
		Password: RedisPassword,
		DB:       0,
	})
	return client
}

// init 函数在 main 之前执行，可能包含敏感初始化逻辑
func init() {
	// 模拟初始化日志
	log.Println("Initializing production system...")
	
	// 设置环境变量（模拟）
	os.Setenv("APP_ENV", "production")
	os.Setenv("DB_PASSWORD", ProdDBPassword)
	os.Setenv("API_SECRET", StripeSecretKey)
}