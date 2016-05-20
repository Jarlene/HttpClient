package com.baidu.music.httpclientlib;

/**
 * Created by Jarlene on 2016/5/20.
 */
public class HttpClient {
    private static volatile HttpClient ourInstance = null;

    public static HttpClient getInstance() {
        if (ourInstance == null) {
            synchronized (HttpClient.class) {
                if (ourInstance == null) {
                    ourInstance = new HttpClient();
                }
            }
        }
        return ourInstance;
    }

    private HttpClient() {
    }
}
