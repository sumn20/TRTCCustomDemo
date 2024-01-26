package com.tencent.trtccustom;

import android.content.Context;
import android.media.AudioManager;
import android.os.Bundle;

import com.tencent.trtccustom.databinding.ActivitySimpleTransportBinding;

public class SimpleTransportActivity extends BaseActivity {
    ActivitySimpleTransportBinding binding;
    private AudioManager mAudioManager;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        binding = ActivitySimpleTransportBinding.inflate(getLayoutInflater());
        setContentView(binding.getRoot());
        bindToolbarWithBack(binding.toolbar);
        mAudioManager = (AudioManager) getSystemService(Context.AUDIO_SERVICE);
        if (mAudioManager != null) {
            mAudioManager.setMode(AudioManager.MODE_IN_CALL);
        }
        String userID = getIntent().getStringExtra("userID");
        String roomID = getIntent().getStringExtra("roomID");
        binding.toolbar.setTitle("房间号:" + roomID);
        JNIManager.getInstance().enterRoom(userID, Integer.parseInt(roomID));
        binding.logInfo.append("\n当前API:" + AudioApi.apiToString(JNIManager.getInstance().getCurrentAudioApi()));
        binding.nativeChange.setOnClickListener(v -> {
            binding.logInfo.append("\n切换api");
            JNIManager.getInstance().changeAudioApi(AudioApi.OpenSLES);
            binding.logInfo.append("\n当前API:" + AudioApi.apiToString(JNIManager.getInstance().getCurrentAudioApi()));
        });
        binding.nativeStartCapture.setOnClickListener(v -> {
            binding.logInfo.append("\n开始采集");
            JNIManager.getInstance().startCapture();
            if (mAudioManager != null) {
                mAudioManager.setMode(AudioManager.MODE_IN_CALL);
            }
        });
        binding.nativeStopCapture.setOnClickListener(v -> {
            binding.logInfo.append("\n停止采集");
            JNIManager.getInstance().stopCapture();
        });
    }


    @Override
    protected void onDestroy() {
        super.onDestroy();
        JNIManager.getInstance().exitRoom();
    }
}