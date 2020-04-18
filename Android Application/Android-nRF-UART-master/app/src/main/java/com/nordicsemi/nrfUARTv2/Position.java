package com.nordicsemi.nrfUARTv2;

import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.TextView;

public class Position extends Activity implements View.OnClickListener{
    public static final String TAG = "nRFUART";
    private static final int REQUEST_MOTORS= 11;
    private static final int REQUEST_SAVED_POSITION= 12;
    private static final int REQUEST_BACK = 999;
    private UartService mService = null;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_position);

        Button btnMotors = (Button) findViewById(R.id.motor_button);
        Button btnPosition = (Button) findViewById((R.id.saved_positions_button));

        btnMotors.setOnClickListener(this);
        btnPosition.setOnClickListener(this);

        ((TextView) findViewById(R.id.deviceName)).setText(getIntent().getStringExtra("DEVICE_STATE"));
        mService = (UartService) getIntent().getSerializableExtra("BLE");
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
            case R.id.motor_button:
                intent.putExtra("MESSAGE", "<motors>");
                resultCode = REQUEST_MOTORS;
                break;
            case R.id.saved_positions_button:
                intent.putExtra("MESSAGE", "<saved_positions>");
                resultCode = REQUEST_SAVED_POSITION;
                break;
        }
        setResult(resultCode, intent);
        finish();
    }
}

