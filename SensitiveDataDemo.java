import java.util.regex.Pattern;

public class SensitiveDataDemo {
    private static final Pattern PHONE_PATTERN = Pattern.compile("1[3-9]\\d{9}");

    public static void main(String[] args) {
        // 模拟包含手机号的敏感数据
        String sensitiveContent = "用户联系手机：13800138000，备用号码：13912345678。";
        System.out.println("原始内容: " + sensitiveContent);

        // 模拟数据处理：提取并打印手机号，触发DLP关键字/正则扫描
        extractPhoneNumbers(sensitiveContent);
    }

    private static void extractPhoneNumbers(String text) {
        var matcher = PHONE_PATTERN.matcher(text);
        System.out.println("检测到手机号：");
        while (matcher.find()) {
            System.out.println(" - " + matcher.group());
        }
    }
}