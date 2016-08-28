package com.baidu.music.httpclientlib.jni;

import android.text.TextUtils;

/**
 * Created by Jarlene on 2016/5/20.
 */
public class HttpClientJNI {

    static {
        try {
            System.loadLibrary("event");
            System.loadLibrary("event_core");
            System.loadLibrary("event_extra");
            System.loadLibrary("event_extra");
            System.loadLibrary("event_pthreads");
            System.loadLibrary("HttpClient");
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    public static String httpRequest(String url) {
        if (TextUtils.isEmpty(url)) {
            return null;
        }
        return doHttpRequest(url);
    }

    private static native String doHttpRequest(String url);
}
