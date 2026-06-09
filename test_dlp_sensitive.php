<?php
/**
 * @file test_dlp_sensitive.php
 * @brief 飞连 DLP 敏感数据检测测试文件 (PHP)
 * @warning 仅供安全策略测试使用，请勿外发到公开仓库
 */

// ============================================
// 1. 数据库连接配置（敏感信息）
// ============================================

// 生产环境数据库配置 ⚠️ 敏感信息
define('PROD_DB_HOST', '10.10.100.50');
define('PROD_DB_PORT', 3306);
define('PROD_DB_NAME', 'corp_finance_db');
define('PROD_DB_USER', 'finance_admin');
define('PROD_DB_PASSWORD', 'Fin@nce#2024!SecurePass');

// MySQL 连接
$prod_mysql_conn = "mysql:host=" . PROD_DB_HOST . ";port=" . PROD_DB_PORT . ";dbname=" . PROD_DB_NAME;
$prod_pdo_config = [
    'dsn' => $prod_mysql_conn,
    'username' => PROD_DB_USER,
    'password' => PROD_DB_PASSWORD,
    'options' => [
        PDO::ATTR_ERRMODE => PDO::ERRMODE_EXCEPTION,
        PDO::ATTR_DEFAULT_FETCH_MODE => PDO::FETCH_ASSOC,
    ]
];

// PostgreSQL 配置
define('PG_HOST', 'postgres-prod.internal');
define('PG_PORT', 5432);
define('PG_USER', 'postgres_admin');
define('PG_PASSWORD', 'Postgres@2024!');
define('PG_DBNAME', 'corp_db');

$pg_conn_string = "host=" . PG_HOST . " port=" . PG_PORT . " dbname=" . PG_DBNAME . " user=" . PG_USER . " password=" . PG_PASSWORD;

// Redis 配置
define('REDIS_HOST', 'redis-prod.internal');
define('REDIS_PORT', 6379);
define('REDIS_PASSWORD', 'Redis@Secret2024');

// MongoDB 配置
define('MONGO_URI', 'mongodb://mongodb_admin:Mongo@Prod2024!@mongo-prod.internal:27017/corp?authSource=admin');

// ============================================
// 2. API 密钥和第三方服务凭证
// ============================================

// Stripe 支付密钥
define('STRIPE_SECRET_KEY', 'sk_live_4eC39HqLyjWDarjtT1zdp7dcXYZ_Prod');
define('STRIPE_PUBLISHABLE_KEY', 'pk_live_51ABCDEFGHIJKLMNOPQRSTUVWXYZ');
define('STRIPE_WEBHOOK_SECRET', 'whsec_abc123def456ghi789jkl012mno345pqr');

// AWS 凭证
define('AWS_ACCESS_KEY_ID', 'AKIAIOSFODNN7EXAMPLE12345');
define('AWS_SECRET_ACCESS_KEY', 'wJalrXUtnFEMI/K7MDENG/bPxRfiCYEXAMPLEKEY67890');
define('AWS_REGION', 'cn-north-1');
define('AWS_S3_BUCKET', 's3://corp-sensitive-data-bucket/backup/');

// 阿里云凭证
define('ALIYUN_ACCESS_KEY', 'LTAI5txxxxxxxxxxxxxxxxxxxx');
define('ALIYUN_SECRET_KEY', 'xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx');

// JWT 签名密钥
define('JWT_SECRET_KEY', 'jwt_signing_key_for_production_environment_2024_secure');

// GitHub Token
define('GITHUB_TOKEN', 'github_pat_11ABC123DEF456GHI789JKL0MNO1234567890');

// GitLab Token
define('GITLAB_TOKEN', 'glpat-ABC123DEF456GHI789JKL');

// 微信支付配置
define('WECHAT_APP_ID', 'wx1234567890abcdef');
define('WECHAT_MCH_ID', '1234567890');
define('WECHAT_API_KEY', 'wechat_api_key_2024_secure_123456');

// 支付宝配置
define('ALIPAY_APP_ID', '2021000123456789');
define('ALIPAY_PRIVATE_KEY', 'MIIEpAIBAAKCAQEAu1Z2vZK7n4c8L9xM0N5oP7qRtS3wXyF6hJ8kL9zXcVbNmP0o');

// ============================================
// 3. 敏感业务数据模型
// ============================================

class CustomerInfo {
    public $id;
    public $name;
    public $contact_person;
    public $phone;
    public $email;
    public $contract_amount;
    public $contract_expiry;
    public $service_level;
    
    public function __construct($id, $name, $contact_person, $phone, $email, $amount, $expiry, $level) {
        $this->id = $id;
        $this->name = $name;
        $this->contact_person = $contact_person;
        $this->phone = $phone;
        $this->email = $email;
        $this->contract_amount = $amount;
        $this->contract_expiry = $expiry;
        $this->service_level = $level;
    }
}

class EmployeeInfo {
    public $id;
    public $name;
    public $department;
    public $position;
    public $salary;
    public $bank_account;
    public $id_card;
    public $phone;
    public $email;
}

// 模拟千万级大客户数据
$top_customers = [
    new CustomerInfo(1001, "XX科技集团有限公司", "王总经理", "13912340001", "wang@xxtech.com", 2500000.00, "2025-12-31", "enterprise"),
    new CustomerInfo(1002, "YY金融服务有限公司", "李财务总监", "13912340002", "li@yyfinance.com", 5800000.00, "2025-06-30", "enterprise"),
    new CustomerInfo(1003, "ZZ制造股份有限公司", "赵董事长", "13912340003", "zhao@zzmfg.com", 12000000.00, "2026-03-31", "vip"),
    new CustomerInfo(1004, "AA国际贸易集团", "孙总经理", "13912340004", "sun@aatrade.com", 8750000.00, "2025-09-30", "enterprise"),
    new CustomerInfo(1005, "BB新能源科技有限公司", "周总", "13912340005", "zhou@bbenergy.com", 15000000.00, "2026-06-30", "vip")
];

// 模拟员工薪资数据
$employees = [
    ['id' => 2001, 'name' => '张伟', 'department' => '技术研发部', 'position' => '架构师', 'salary' => 58000.00, 'bank_account' => '6212260200123456789', 'id_card' => '11010119900307663X', 'phone' => '13800138001', 'email' => 'zhang.wei@company.com'],
    ['id' => 2002, 'name' => '李娜', 'department' => '技术研发部', 'position' => '技术总监', 'salary' => 89000.00, 'bank_account' => '6212260200987654321', 'id_card' => '11010119850512456X', 'phone' => '13800138002', 'email' => 'li.na@company.com'],
    ['id' => 2003, 'name' => '王强', 'department' => '销售部', 'position' => '销售总监', 'salary' => 75000.00, 'bank_account' => '6212260200555555555', 'id_card' => '11010119801201123X', 'phone' => '13800138003', 'email' => 'wang.qiang@company.com']
];

// ============================================
// 4. 加密工具类（硬编码密钥）
// ============================================

// 硬编码的 AES 密钥 ⚠️ 严重安全问题
define('AES_KEY', '2b7e151628aed2a6abf7158809cf4f3c');
define('AES_IV', '0123456789abcdef');

// 硬编码的 OpenSSL 密钥
$openssl_private_key = <<<EOD
-----BEGIN RSA PRIVATE KEY-----
MIIEpAIBAAKCAQEAu1Z2vZK7n4c8L9xM0N5oP7qRtS3wXyF6hJ8kL9zXcVbNmP0o
I9uYtR5eW7aQ8sDfGhJkLzXcVbNmP0oI9uYtR5eW7aQ8sDfGhJkLzXcVbNmP0oI9
-----END RSA PRIVATE KEY-----
EOD;

/**
 * AES 加密函数（使用硬编码密钥）
 */
function aes_encrypt($plaintext) {
    $cipher = "aes-256-cbc";
    $key = hex2bin(AES_KEY);
    $iv = hex2bin(AES_IV);
    return base64_encode(openssl_encrypt($plaintext, $cipher, $key, OPENSSL_RAW_DATA, $iv));
}

/**
 * 密码哈希函数（模拟）
 */
function hash_password($password) {
    // 硬编码的盐值
    $salt = "MySecretSaltValueDoNotLeak2024";
    return hash('sha256', $password . $salt);
}

// ============================================
// 5. 授权算法（核心知识产权）
// ============================================

define('MAGIC_NUMBER', 0xDEADBEEF);
define('SALT_VALUE', 0x12345678);

/**
 * 生成产品授权码的核心算法（商业机密）
 */
function generate_license($company_name, $product_id, $expiry_days) {
    // 核心算法逻辑（商业机密 - 请勿外泄）
    $seed = $company_name . $product_id . $expiry_days;
    $hash1 = compute_hash($seed);
    
    // 第二层加密
    $hash2 = $hash1 ^ MAGIC_NUMBER;
    for ($i = 0; $i < $expiry_days; $i++) {
        $hash2 = ($hash2 * 31) + ($i % 97);
    }
    
    // 生成最终授权码
    return sprintf("LIC-%08X-%04X-%06X", 
        ($hash1 >> 16) & 0xFFFFFFFF, 
        $hash2 & 0xFFFF, 
        ($hash1 ^ $hash2) % 1000000);
}

function compute_hash($input) {
    $hash = MAGIC_NUMBER;
    for ($i = 0; $i < strlen($input); $i++) {
        $hash = (($hash << 5) + $hash) + ord($input[$i]);
        $hash ^= SALT_VALUE;
    }
    return $hash;
}

// ============================================
// 6. 敏感路径配置
// ============================================

define('GITLAB_REPO', 'http://gitlab.company.internal/corp/core-system');
define('JENKINS_JOB', 'https://jenkins.company.internal/job/deploy-to-prod');
define('NAS_SENSITIVE', '\\\\nas-01\\finance\\quarterly_reports\\2024\\Q4\\confidential');
define('CONFLUENCE_PAGE', 'https://confluence.company.internal/pages/viewpage.action?pageId=123456');
define('RABBITMQ_HOST', 'rabbitmq.internal.company.com');
define('RABBITMQ_USER', 'prod_user');
define('RABBITMQ_PASS', 'Rabbit@Prod2024!');

// ============================================
// 7. 配置文件数组
// ============================================

$app_config = [
    'database' => [
        'host' => PROD_DB_HOST,
        'port' => PROD_DB_PORT,
        'name' => PROD_DB_NAME,
        'user' => PROD_DB_USER,
        'password' => PROD_DB_PASSWORD,
    ],
    'redis' => [
        'host' => REDIS_HOST,
        'port' => REDIS_PORT,
        'password' => REDIS_PASSWORD,
    ],
    'jwt' => [
        'secret' => JWT_SECRET_KEY,
    ],
    'stripe' => [
        'secret_key' => STRIPE_SECRET_KEY,
    ],
    'aws' => [
        'access_key' => AWS_ACCESS_KEY_ID,
        'secret_key' => AWS_SECRET_ACCESS_KEY,
        'region' => AWS_REGION,
    ],
    'wechat' => [
        'app_id' => WECHAT_APP_ID,
        'mch_id' => WECHAT_MCH_ID,
        'api_key' => WECHAT_API_KEY,
    ]
];

// ============================================
// 8. 危险的 eval 和系统调用（模拟后门）
// ============================================

/**
 * ⚠️ 危险函数：动态执行代码
 */
function dangerous_eval($code) {
    // 这是高风险操作，可能被用于后门
    return eval($code);
}

/**
 * ⚠️ 危险函数：执行系统命令
 */
function dangerous_system($cmd) {
    // 可能被用于远程命令执行
    return shell_exec($cmd);
}

// 硬编码的后门密码（严重安全问题）
$backdoor_password = "admin_backdoor_2024";

// ============================================
// 9. 数据库连接函数（模拟）
// ============================================

/**
 * 连接生产数据库（使用硬编码密码）
 */
function connect_prod_database() {
    try {
        $pdo = new PDO(
            "mysql:host=" . PROD_DB_HOST . ";dbname=" . PROD_DB_NAME,
            PROD_DB_USER,
            PROD_DB_PASSWORD,
            [PDO::ATTR_ERRMODE => PDO::ERRMODE_EXCEPTION]
        );
        return $pdo;
    } catch (PDOException $e) {
        // 危险：在错误消息中暴露数据库信息
        die("Database connection failed: " . $e->getMessage());
    }
}

// ============================================
// 10. HTTP API 端点（模拟）
// ============================================

/**
 * 获取客户列表 API（可能泄漏敏感数据）
 */
function api_get_customers() {
    global $top_customers;
    header('Content-Type: application/json');
    
    $result = [];
    foreach ($top_customers as $customer) {
        $result[] = [
            'id' => $customer->id,
            'name' => $customer->name,
            'contact' => $customer->contact_person,
            'phone' => $customer->phone,
            'amount' => $customer->contract_amount,
        ];
    }
    
    echo json_encode($result);
}

/**
 * 调试端点（暴露敏感信息）
 */
function api_debug() {
    header('Content-Type: application/json');
    
    $debug_info = [
        'db_host' => PROD_DB_HOST,
        'db_name' => PROD_DB_NAME,
        'db_user' => PROD_DB_USER,
        'db_password' => PROD_DB_PASSWORD,
        'redis_password' => REDIS_PASSWORD,
        'jwt_secret' => JWT_SECRET_KEY,
        'stripe_key' => STRIPE_SECRET_KEY,
        'aws_keys' => [
            'access' => AWS_ACCESS_KEY_ID,
            'secret' => AWS_SECRET_ACCESS_KEY,
        ],
        'php_version' => phpversion(),
        'server_software' => $_SERVER['SERVER_SOFTWARE'] ?? 'unknown',
    ];
    
    echo json_encode($debug_info, JSON_PRETTY_PRINT);
}

// ============================================
// 11. 主执行逻辑
// ============================================

echo "=== DLP Sensitivity Test File (PHP) ===\n";
echo "This file contains simulated sensitive data for DLP testing.\n\n";

// 1. 显示数据库配置
echo "[Database Configuration]\n";
echo "  MySQL DSN: " . $prod_mysql_conn . "\n";
echo "  PostgreSQL: " . $pg_conn_string . "\n";
echo "  Redis: " . REDIS_HOST . ":" . REDIS_PORT . " (password: " . REDIS_PASSWORD . ")\n";
echo "  MongoDB: " . MONGO_URI . "\n";

// 2. 显示 API 密钥
echo "\n[API Keys & Tokens]\n";
echo "  Stripe Secret: " . STRIPE_SECRET_KEY . "\n";
echo "  AWS Access Key: " . AWS_ACCESS_KEY_ID . "\n";
echo "  AWS Secret Key: " . AWS_SECRET_ACCESS_KEY . "\n";
echo "  JWT Secret: " . JWT_SECRET_KEY . "\n";
echo "  GitHub Token: " . GITHUB_TOKEN . "\n";
echo "  GitLab Token: " . GITLAB_TOKEN . "\n";
echo "  WeChat App ID: " . WECHAT_APP_ID . "\n";
echo "  WeChat API Key: " . WECHAT_API_KEY . "\n";

// 3. 显示敏感业务数据
echo "\n[Sensitive Business Data]\n";
echo "  Top Customers:\n";
foreach ($top_customers as $customer) {
    echo "    - " . $customer->name . ", 联系人: " . $customer->contact_person . ", 合同金额: " . number_format($customer->contract_amount, 2) . " 元\n";
}
echo "  Employee Salary List:\n";
foreach ($employees as $emp) {
    echo "    - " . $emp['name'] . ", 部门: " . $emp['department'] . ", 薪资: " . number_format($emp['salary'], 2) . " 元, 银行账号: " . $emp['bank_account'] . "\n";
}

// 4. 生成授权码
$license = generate_license("XX科技集团", 10086, 365);
echo "\n[License Generated]\n";
echo "  License Code: " . $license . "\n";

// 5. 显示内部路径
echo "\n[Internal Paths (Confidential)]\n";
echo "  GitLab: " . GITLAB_REPO . "\n";
echo "  Jenkins: " . JENKINS_JOB . "\n";
echo "  NAS Path: " . NAS_SENSITIVE . "\n";
echo "  RabbitMQ: " . RABBITMQ_HOST . " (user: " . RABBITMQ_USER . ", password: " . RABBITMQ_PASS . ")\n";

// 6. 显示配置数组
echo "\n[Application Configuration]\n";
echo json_encode($app_config, JSON_PRETTY_PRINT) . "\n";

// 7. 加密示例
echo "\n[Encryption Example]\n";
$encrypted = aes_encrypt("sensitive data test");
echo "  Encrypted data (using hardcoded AES key): " . $encrypted . "\n";

// 8. 警告信息
echo "\n⚠️  This file contains hardcoded passwords, API keys, and secrets!\n";
echo "⚠️  Do NOT commit this file to any public repository!\n";

echo "\n=== End of Test File ===\n";

// 可选：API 路由处理（注释掉以避免实际运行）
// $request_uri = $_SERVER['REQUEST_URI'] ?? '';
// if ($request_uri === '/api/customers') {
//     api_get_customers();
// } elseif ($request_uri === '/api/debug') {
//     api_debug();
// }

?>