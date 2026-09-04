package main

import (
	"fmt"
	"regexp"
)

func main() {
	// 模拟包含身份证号的敏感数据
	sensitiveContent := "身份证号：110101199003076666，另一身份证：320102198805094444。"
	fmt.Println("原始内容:", sensitiveContent)

	// 模拟身份证号正则匹配（简单示例）
	re := regexp.MustCompile(`\d{17}[\dXx]`)
	matches := re.FindAllString(sensitiveContent, -1)
	fmt.Println("检测到身份证号：")
	for _, id := range matches {
		fmt.Println(" -", id)
	}
}