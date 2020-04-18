package com.nordicsemi.nrfUARTv2;

import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.TextView;

public class Icecube extends Activity implements View.OnClickListener{
    public static final String TAG = "nRFUART";
    private static final int REQUEST_MOVEMENT = 23;
    private static final int REQUEST_BACK = 2;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_function);

        Button btnClose = (Button) findViewById(R.id.close_button);
        Button btnOpen = (Button) findViewById((R.id.open_button));
        Button btnStop = (Button) findViewById(R.id.stop_button);

        btnClose.setOnClickListener(this);
        btnOpen.setOnClickListener(this);
        btnStop.setOnClickListener(this);

        ((TextView) findViewById(R.id.deviceName)).setText(getIntent().getStringExtra("DEVICE_STATE"));
    }

    @Override
    public void onBackPressed() {
        Intent intent= new Intent();
        intent.putExtra("MESSAGE", "<back>");
        setResult(REQUEST_BACK, intent);
        finish();
    }

    @Override
    public void onClick(View v) {
        Intent intent = new Intent();
        int requestCode = -1;
        switch (v.getId()) {
            case R.id.close_button:
                intent.putExtra("MESSAGE", "<close>");
                requestCode = REQUEST_MOVEMENT;
                break;
            case R.id.open_button:
                intent.putExtra("MESSAGE", "<open>");
                requestCode = REQUEST_MOVEMENT;
                break;
            case R.id.stop_button:
                intent.putExtra("MESSAGE", "<stop>");
                requestCode = REQUEST_MOVEMENT;
                break;
        }
        setResult(requestCode, intent);
        finish();
    }
}

