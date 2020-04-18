package com.nordicsemi.nrfUARTv2;

import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.TextView;

public class SavedPositionsSaving extends Activity implements View.OnClickListener{
    public static final String TAG = "nRFUART";
    private static final int REQUEST_MESSAGE= 11;
    private static final int REQUEST_BACK = 1;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_saved_positions);

        Button btnPosition1 = (Button) findViewById(R.id.saved_position_1);
        Button btnPosition2 = (Button) findViewById(R.id.saved_position_2);
        Button btnPosition3 = (Button) findViewById(R.id.saved_position_3);
        Button btnPosition4 = (Button) findViewById(R.id.saved_position_4);
        Button btnPosition5 = (Button) findViewById(R.id.saved_position_5);
        Button btnPosition6 = (Button) findViewById(R.id.saved_position_6);
        Button btnPosition7 = (Button) findViewById(R.id.saved_position_7);
        Button btnPosition8 = (Button) findViewById(R.id.saved_position_8);
        Button btnPosition9 = (Button) findViewById(R.id.saved_position_9);


        btnPosition1.setOnClickListener(this);
        btnPosition2.setOnClickListener(this);
        btnPosition3.setOnClickListener(this);
        btnPosition4.setOnClickListener(this);
        btnPosition5.setOnClickListener(this);
        btnPosition6.setOnClickListener(this);
        btnPosition7.setOnClickListener(this);
        btnPosition8.setOnClickListener(this);
        btnPosition9.setOnClickListener(this);

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
            case R.id.saved_position_1:
                intent.putExtra("MESSAGE", "<1>");
                break;
            case R.id.saved_position_2:
                intent.putExtra("MESSAGE", "<2>");
                break;
            case R.id.saved_position_3:
                intent.putExtra("MESSAGE", "<3>");
                break;
            case R.id.saved_position_4:
                intent.putExtra("MESSAGE", "<4>");
                break;
            case R.id.saved_position_5:
                intent.putExtra("MESSAGE", "<5>");
                break;
            case R.id.saved_position_6:
                intent.putExtra("MESSAGE", "<6>");
                break;
            case R.id.saved_position_7:
                intent.putExtra("MESSAGE", "<7>");
                break;
            case R.id.saved_position_8:
                intent.putExtra("MESSAGE", "<8>");
                break;
            case R.id.saved_position_9:
                intent.putExtra("MESSAGE", "<9>");
                break;

        }
        setResult(REQUEST_MESSAGE, intent);
        finish();
    }
}

