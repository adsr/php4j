package com.github.adsr.php4j;

public class Php {
    static {
        System.loadLibrary("php4j");
    }
    public static native void init();
    public static native void shutdown();
    public static native String execString(String code);
}
