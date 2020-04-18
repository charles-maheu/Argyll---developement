package com.nordicsemi.nrfUARTv2;

import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.TextView;

public class Motors extends Activity implements View.OnClickListener{
    public static final String TAG = "nRFUART";
    private static final int REQUEST_MOTOR1 = 111;
    private static final int REQUEST_MOTOR2 = 112;
    private static final int REQUEST_MOTOR3 = 113;
    private static final int REQUEST_MOTOR4 = 114;
    private static final int REQUEST_SAVE = 115;
    private static final int REQUEST_BACK = 1;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_motors);

        Button btnBase = (Button) findViewById(R.id.base_motor_button);
        Button btnSoulder = (Button) findViewById((R.id.shoulder_motor_button));
        Button btnElbow = (Button) findViewById(R.id.elbow_motor_button);
        Button btnWrist = (Button) findViewById((R.id.wrist_motor_button));
        Button btnSave = (Button) findViewById(R.id.save_button);

        btnBase.setOnClickListener(this);
        btnSoulder.setOnClickListener(this);
        btnElbow.setOnClickListener(this);
        btnWrist.setOnClickListener(this);
        btnSave.setOnClickListener(this);

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
        int resultCode = -1;
        switch (v.getId()) {
            case R.id.base_motor_button:
                intent.putExtra("MESSAGE", "<1>");
                resultCode = REQUEST_MOTOR1;
                break;
            case R.id.shoulder_motor_button:
                intent.putExtra("MESSAGE", "<2>");
                resultCode = REQUEST_MOTOR2;
                break;
            case R.id.elbow_motor_button:
                intent.putExtra("MESSAGE", "<3>");
                resultCode = REQUEST_MOTOR3;
                break;
            case R.id.wrist_motor_button:
                intent.putExtra("MESSAGE", "<4>");
                resultCode = REQUEST_MOTOR4;
                break;
            case R.id.save_button:
                intent.putExtra("MESSAGE", "<save>");
                resultCode = REQUEST_SAVE;
        }
        setResult(resultCode, intent);
        finish();
    }
}

