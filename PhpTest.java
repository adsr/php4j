import com.github.adsr.php4j.Php;

public class PhpTest {
    public static void main(String[] args) {
        System.out.println("Initializing PHP...");
        Php.init();
        System.out.println("Requiring test.php...");
        String result = Php.execString("require 'test.php';");
        System.out.printf("Result was: %s\n", result);
        System.out.println("Shutting down PHP...");
        Php.shutdown();
        System.out.println("Fin");
    }
}
