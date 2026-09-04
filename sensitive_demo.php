<?php
// 模拟包含银行卡号的敏感数据
$sensitiveContent = "银行卡号：6222021234567890123，另一卡号：6228480012345678912。";
echo "原始内容: " . $sensitiveContent . "\n";

// 模拟银行卡号（Luhn算法基本格式）匹配
preg_match_all('/\d{16,19}/', $sensitiveContent, $matches);
echo "检测到银行卡号：\n";
foreach ($matches[0] as $card) {
    echo " - " . $card . "\n";
}
?>