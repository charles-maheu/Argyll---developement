package com.nordicsemi.nrfUARTv2;

import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.TextView;

public class Function extends Activity implements View.OnClickListener{
    public static final String TAG = "nRFUART";
    private static final int REQUEST_TIROIRS = 21;
    private static final int REQUEST_CLAW = 22;
    private static final int REQUEST_ICECUBE = 23;
    private static final int REQUEST_BACK = 999;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_functions);

        Button btnTiroir = (Button) findViewById(R.id.tiroir_button);
        Button btnClaw = (Button) findViewById((R.id.claw_button));
        Button btnIcecube = (Button) findViewById((R.id.icecube_button));

        btnTiroir.setOnClickListener(this);
        btnClaw.setOnClickListener(this);
        btnIcecube.setOnClickListener(this);

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
            case R.id.tiroir_button:
                intent.putExtra("MESSAGE", "<drawer>");
                resultCode = REQUEST_TIROIRS;
                break;
            case R.id.claw_button:
                intent.putExtra("MESSAGE", "<gripper>");
                resultCode = REQUEST_CLAW;
                break;
            case R.id.icecube_button:
                intent.putExtra("MESSAGE", "<icecube>");
                resultCode = REQUEST_ICECUBE;
                break;
        }
        setResult(resultCode, intent);
        finish();
    }
}

