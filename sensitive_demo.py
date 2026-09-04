import re

def main():
    # 模拟包含手机号、邮箱、身份证号的混合敏感数据
    content = "用户张三，手机13800138000，邮箱zhangsan@demo.com，身份证110101199003076666。"
    print(f"原始内容: {content}")

    # 检测手机号
    phones = re.findall(r'1[3-9]\d{9}', content)
    print("检测到手机号:", phones)

    # 检测邮箱
    emails = re.findall(r'[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\.[a-zA-Z]{2,}', content)
    print("检测到邮箱:", emails)

    # 检测身份证号
    ids = re.findall(r'\d{17}[\dXx]', content)
    print("检测到身份证号:", ids)

if __name__ == "__main__":
    main()