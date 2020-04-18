package com.nordicsemi.nrfUARTv2;

import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.TextView;

public class Motor2 extends Activity implements View.OnClickListener{
    public static final String TAG = "nRFUART";
    private static final int REQUEST_MOVEMENT = 112;
    private static final int REQUEST_BACK = 11;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_motor1);

        Button btnLeft = (Button) findViewById(R.id.left_button);
        Button btnRigth = (Button) findViewById((R.id.right_button));
        Button btnStop = (Button) findViewById(R.id.stop_button);

        btnLeft.setOnClickListener(this);
        btnRigth.setOnClickListener(this);
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
        switch (v.getId()) {
            case R.id.left_button:
                intent.putExtra("MESSAGE", "<left>");
                break;
            case R.id.right_button:
                intent.putExtra("MESSAGE", "<right>");
                break;
            case R.id.stop_button:
                intent.putExtra("MESSAGE", "<stop>");
                break;
        }
        setResult(REQUEST_MOVEMENT, intent);
        finish();
    }
}

