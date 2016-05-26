package com.baidu.music.httpclient;

import android.app.Activity;
import android.os.Bundle;
import android.widget.TextView;

import com.baidu.music.httpclientlib.HttpClient;

public class MainActivity extends Activity {

    TextView textView;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        textView = (TextView) findViewById(R.id.text);
        textView.setText(HttpClient.getInstance().getBaiduUrl());
    }
}
