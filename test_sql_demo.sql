-- ============================================
-- 测试数据库文件 - 用于验证飞连文件外发监控
-- 文件类型: MySQL/PostgreSQL 数据库导出
-- 用途: 安全策略功能测试
-- 警告: 仅供内部安全测试使用，请勿外发
-- ============================================

-- --------------------------------------------------
-- 1. 数据库结构 (模拟生产环境)
-- --------------------------------------------------

CREATE DATABASE IF NOT EXISTS `corp_production_db`
    CHARACTER SET utf8mb4
    COLLATE utf8mb4_unicode_ci;

USE `corp_production_db`;

-- --------------------------------------------------
-- 2. 用户表 (包含敏感个人信息)
-- --------------------------------------------------

CREATE TABLE `users` (
    `id` INT PRIMARY KEY AUTO_INCREMENT,
    `username` VARCHAR(50) NOT NULL,
    `email` VARCHAR(100) NOT NULL,
    `phone` VARCHAR(20),
    `department` VARCHAR(100),
    `role` VARCHAR(50),
    `created_at` TIMESTAMP DEFAULT CURRENT_TIMESTAMP
);

-- 模拟的员工数据
INSERT INTO `users` (`username`, `email`, `phone`, `department`, `role`) VALUES
('zhang.wei', 'zhang.wei@company.com', '13800138001', '技术研发部', '后端工程师'),
('li.na', 'li.na@company.com', '13800138002', '技术研发部', '前端工程师'),
('wang.qiang', 'wang.qiang@company.com', '13800138003', '产品部', '产品经理'),
('chen.jie', 'chen.jie@company.com', '13800138004', '销售部', '销售总监'),
('liu.yang', 'liu.yang@company.com', '13800138005', '技术研发部', '架构师');

-- --------------------------------------------------
-- 3. 客户表 (包含商业敏感信息)
-- --------------------------------------------------

CREATE TABLE `customers` (
    `id` INT PRIMARY KEY AUTO_INCREMENT,
    `customer_name` VARCHAR(100) NOT NULL,
    `contact_person` VARCHAR(50),
    `contact_phone` VARCHAR(20),
    `contract_amount` DECIMAL(12,2),
    `contract_expiry` DATE,
    `service_level` ENUM('basic','premium','enterprise')
);

INSERT INTO `customers` (`customer_name`, `contact_person`, `contact_phone`, `contract_amount`, `contract_expiry`, `service_level`) VALUES
('XX科技集团', '王经理', '13912345678', 2500000.00, '2025-12-31', 'enterprise'),
('YY金融服务', '李总监', '13912345679', 5800000.00, '2025-06-30', 'enterprise'),
('ZZ制造有限', '赵总', '13912345680', 1200000.00, '2025-09-30', 'premium');

-- --------------------------------------------------
-- 4. 配置表 (包含数据库连接凭证 - 模拟)
-- --------------------------------------------------

CREATE TABLE `system_config` (
    `config_key` VARCHAR(100) PRIMARY KEY,
    `config_value` TEXT,
    `description` VARCHAR(200),
    `is_sensitive` BOOLEAN DEFAULT FALSE
);

INSERT INTO `system_config` (`config_key`, `config_value`, `description`, `is_sensitive`) VALUES
('db.master.host', '10.10.10.100', '主数据库地址', TRUE),
('db.master.port', '3306', '主数据库端口', TRUE),
('db.master.user', 'prod_admin', '数据库管理员账号', TRUE),
('db.master.password', 'Pr0d@DM1n!2024#Secure', '数据库密码 - 敏感信息', TRUE),
('api.payment.secret', 'pay_sk_live_4eC39HqLyjWDarjtT1zdp7dcXYZ', '支付网关密钥', TRUE),
('jwt.secret.key', 'jwt_signing_key_for_production_environment_2024', 'JWT签名密钥', TRUE);

-- --------------------------------------------------
-- 5. 财务数据表 (敏感财务信息)
-- --------------------------------------------------

CREATE TABLE `financial_records` (
    `id` INT PRIMARY KEY AUTO_INCREMENT,
    `transaction_id` VARCHAR(50) UNIQUE,
    `amount` DECIMAL(10,2),
    `transaction_type` VARCHAR(50),
    `transaction_date` DATE,
    `customer_id` INT,
    `status` VARCHAR(20)
);

INSERT INTO `financial_records` (`transaction_id`, `amount`, `transaction_type`, `transaction_date`, `customer_id`, `status`) VALUES
('TXN2024000001', 1250000.00, '合同首付款', '2024-01-15', 1, 'completed'),
('TXN2024000002', 2900000.00, '年度服务费', '2024-02-20', 2, 'completed'),
('TXN2024000003', 600000.00, '二期项目款', '2024-03-10', 1, 'processing');

-- --------------------------------------------------
-- 6. 敏感存储过程 (核心业务逻辑)
-- --------------------------------------------------

DELIMITER //

CREATE PROCEDURE `calculate_commission`(
    IN employee_id INT,
    IN sale_amount DECIMAL(12,2),
    OUT commission DECIMAL(10,2)
)
BEGIN
    -- 核心提成计算逻辑 - 商业机密
    DECLARE base_rate DECIMAL(5,2);
    DECLARE bonus_rate DECIMAL(5,2);
    
    SET base_rate = 0.05;
    SET bonus_rate = 0.02;
    
    IF sale_amount > 1000000 THEN
        SET commission = sale_amount * (base_rate + bonus_rate);
    ELSE
        SET commission = sale_amount * base_rate;
    END IF;
    
    -- 插入审计记录
    INSERT INTO commission_log (employee_id, amount, calculated_at)
    VALUES (employee_id, commission, NOW());
END //

DELIMITER ;

-- --------------------------------------------------
-- 7. 索引和约束
-- --------------------------------------------------

CREATE INDEX idx_user_email ON users(email);
CREATE INDEX idx_customer_name ON customers(customer_name);
CREATE INDEX idx_financial_date ON financial_records(transaction_date);

ALTER TABLE financial_records
    ADD FOREIGN KEY (customer_id) REFERENCES customers(id);

-- --------------------------------------------------
-- 8. 视图 (用于报表生成)
-- --------------------------------------------------

CREATE VIEW customer_financial_summary AS
SELECT 
    c.customer_name,
    COUNT(f.transaction_id) AS transaction_count,
    SUM(f.amount) AS total_amount,
    MAX(f.transaction_date) AS last_transaction_date
FROM customers c
LEFT JOIN financial_records f ON c.id = f.customer_id
GROUP BY c.id;

-- ============================================
-- 导出信息
-- 导出时间: 2024-01-15 10:30:00
-- 数据库版本: MySQL 8.0.32
-- 数据大小: ~15 KB
-- ============================================
-- 测试说明:
-- 1. 此文件包含数据库结构、配置凭证、财务数据等敏感内容
-- 2. 用于验证飞连对 .sql 文件的敏感数据识别能力
-- 3. 触发策略后可验证飞书预警通知是否正常发送
-- ============================================