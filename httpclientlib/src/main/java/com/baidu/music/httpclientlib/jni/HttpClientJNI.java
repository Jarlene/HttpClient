package com.baidu.music.httpclientlib.jni;

/**
 * Created by Jarlene on 2016/5/20.
 */
public class HttpClientJNI {

    static {
        try {
            System.loadLibrary("HttpClient");
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    public static native String doHttpRequest(String url);
}
