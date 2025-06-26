// ...（保持 import 不变）
package com.android.camerastresstest;

import android.Manifest;
import android.content.Context;
import android.content.pm.PackageManager;
import android.graphics.SurfaceTexture;
import android.hardware.camera2.*;
import android.os.Bundle;
import android.os.Handler;
import android.os.HandlerThread;
import android.os.SystemClock;
import android.view.Surface;
import android.view.TextureView;
import android.view.View;
import android.widget.*;

import androidx.annotation.NonNull;
import androidx.appcompat.app.AppCompatActivity;
import androidx.core.app.ActivityCompat;

import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.text.SimpleDateFormat;
import java.util.Collections;
import java.util.Date;
import java.util.LinkedList;
import java.util.Locale;

public class MainActivity extends AppCompatActivity {

    private static final int PERMISSION_REQUEST_CAMERA = 1;
    private static final int TEST_DELAY_MS = 500;
    private static final int PREVIEW_TIMEOUT_MS = 2000;
    private static final int SESSION_TIMEOUT_MS = 2000;
    private static final int IGNORE_FRAME_COUNT = 3;
    private static final int MAX_LOG_LINES = 1000;

    private Button mStartButton, mStopButton;
    private EditText mCameraIdEditText;
    private TextView mLogTextView, mTotalRunsTextView, mSuccessTextView, mFailureTextView;
    private ScrollView mLogScrollView;
    private TextureView mPreviewTextureView;

    private CameraManager mCameraManager;
    private HandlerThread mCameraThread;
    private Handler mCameraHandler;

    private volatile boolean mIsTesting = false;
    private int mTotalRuns = 0;
    private int mSuccessCount = 0;
    private int mFailureCount = 0;
    private String mTargetCameraId = "0";

    private long mOpenStartTimeMs;
    private long mCloseStartTimeMs;
    private long mPreviewStartTimeMs;

    private CameraDevice mOpenedCameraDevice;
    private CameraCaptureSession mPreviewSession;

    private boolean mSessionConfigured = false;
    private int mFrameCounter = 0;

    private final LinkedList<String> mLogLines = new LinkedList<>();
    private FileOutputStream mLogFileOutput;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        mStartButton = findViewById(R.id.start_button);
        mStopButton = findViewById(R.id.stop_button);
        mCameraIdEditText = findViewById(R.id.camera_id_edit_text);
        mLogTextView = findViewById(R.id.log_text_view);
        mLogScrollView = findViewById(R.id.log_scroll_view);
        mTotalRunsTextView = findViewById(R.id.total_runs_text);
        mSuccessTextView = findViewById(R.id.success_text);
        mFailureTextView = findViewById(R.id.failure_text);
        mPreviewTextureView = findViewById(R.id.preview_texture_view);

        mStartButton.setOnClickListener(v -> startTest());
        mStopButton.setOnClickListener(v -> stopTest());
        mStopButton.setEnabled(false);

        mCameraManager = (CameraManager) getSystemService(Context.CAMERA_SERVICE);

        if (ActivityCompat.checkSelfPermission(this, Manifest.permission.CAMERA) != PackageManager.PERMISSION_GRANTED) {
            ActivityCompat.requestPermissions(this, new String[]{Manifest.permission.CAMERA}, PERMISSION_REQUEST_CAMERA);
        }

        initLogFile();
    }

    @Override
    protected void onResume() {
        super.onResume();
        mCameraThread = new HandlerThread("CameraTestThread");
        mCameraThread.start();
        mCameraHandler = new Handler(mCameraThread.getLooper());
    }

    @Override
    protected void onPause() {
        super.onPause();
        stopTest();
        if (mCameraThread != null) {
            mCameraThread.quitSafely();
            try {
                mCameraThread.join();
            } catch (InterruptedException e) {
                logError("停止线程失败", e);
            }
            mCameraThread = null;
            mCameraHandler = null;
        }
        closeLogFile();
    }

    private void startTest() {
        mTargetCameraId = mCameraIdEditText.getText().toString();
        if (mTargetCameraId.isEmpty()) {
            Toast.makeText(this, "请输入有效的 Camera ID", Toast.LENGTH_SHORT).show();
            return;
        }

        mIsTesting = true;
        mStartButton.setEnabled(false);
        mStopButton.setEnabled(true);
        mCameraIdEditText.setEnabled(false);

        mTotalRuns = mSuccessCount = mFailureCount = 0;
        updateCounters();

        logMessage("========== 开始测试 ==========");
        logMessage("目标 Camera ID: " + mTargetCameraId);

        mCameraHandler.post(mTestRunnable);
    }

    private void stopTest() {
        mIsTesting = false;
        if (mCameraHandler != null) {
            mCameraHandler.removeCallbacks(mTestRunnable);
        }
        runOnUiThread(() -> {
            mStartButton.setEnabled(true);
            mStopButton.setEnabled(false);
            mCameraIdEditText.setEnabled(true);
            logMessage("========== 停止测试 ==========");
        });

        if (mOpenedCameraDevice != null) {
            try {
                mOpenedCameraDevice.close();
            } catch (Exception e) {
                logError("关闭相机失败", e);
            }
        }
    }

    private final Runnable mTestRunnable = new Runnable() {
        @Override
        public void run() {
            if (!mIsTesting) return;

            mTotalRuns++;
            mSessionConfigured = false;
            updateCounters();
            logMessage("\n--- 第 " + mTotalRuns + " 次测试 ---");

            try {
                if (ActivityCompat.checkSelfPermission(MainActivity.this, Manifest.permission.CAMERA) != PackageManager.PERMISSION_GRANTED) {
                    logError("缺少相机权限", null);
                    stopTest();
                    return;
                }

                mOpenStartTimeMs = SystemClock.elapsedRealtime();
                mCameraManager.openCamera(mTargetCameraId, mStateCallback, mCameraHandler);
            } catch (CameraAccessException | IllegalArgumentException e) {
                logError("openCamera 异常", e);
                mFailureCount++;
                updateCounters();
                scheduleNextRun();
            }
        }
    };

    private final CameraDevice.StateCallback mStateCallback = new CameraDevice.StateCallback() {
        @Override
        public void onOpened(@NonNull CameraDevice camera) {
            long openTime = SystemClock.elapsedRealtime() - mOpenStartTimeMs;
            logMessage(String.format(Locale.US, "第 %d 次: 打开成功 (耗时 %d ms)", mTotalRuns, openTime));

            mSuccessCount++;
            updateCounters();

            mOpenedCameraDevice = camera;
            if (!mPreviewTextureView.isAvailable()) {
                logError("TextureView 不可用", null);
                mFailureCount++;
                updateCounters();
                camera.close();
                return;
            }

            SurfaceTexture texture = mPreviewTextureView.getSurfaceTexture();
            if (texture == null) {
                logError("SurfaceTexture 为空", null);
                mFailureCount++;
                updateCounters();
                camera.close();
                return;
            }

            texture.setDefaultBufferSize(640, 480);
            Surface surface = new Surface(texture);

            try {
                logMessage("创建预览会话...");
                camera.createCaptureSession(Collections.singletonList(surface),
                        new CameraCaptureSession.StateCallback() {
                            @Override
                            public void onConfigured(@NonNull CameraCaptureSession session) {
                                mSessionConfigured = true;
                                mPreviewSession = session;

                                logMessage("预览会话配置成功，准备延迟启动预览 500ms");

                                CaptureRequest.Builder builder;
                                try {
                                    builder = camera.createCaptureRequest(CameraDevice.TEMPLATE_PREVIEW);
                                    builder.addTarget(surface);
                                } catch (CameraAccessException e) {
                                    logError("创建预览请求失败", e);
                                    mFailureCount++;
                                    updateCounters();
                                    camera.close();
                                    return;
                                }

                                mCameraHandler.postDelayed(() -> {
                                    try {
                                        logMessage("延迟启动预览中...");
                                        session.setRepeatingRequest(builder.build(), new CameraCaptureSession.CaptureCallback() {
                                            @Override
                                            public void onCaptureCompleted(@NonNull CameraCaptureSession session,
                                                                           @NonNull CaptureRequest request,
                                                                           @NonNull TotalCaptureResult result) {
                                                mFrameCounter++;
                                                if (mFrameCounter <= IGNORE_FRAME_COUNT) {
                                                    logMessage("忽略前帧: " + mFrameCounter);
                                                }
                                            }
                                        }, mCameraHandler);

                                        mPreviewStartTimeMs = SystemClock.elapsedRealtime();
                                        logMessage("预览启动成功");
                                    } catch (CameraAccessException e) {
                                        logError("延迟启动预览失败", e);
                                        mFailureCount++;
                                        updateCounters();
                                    }

                                    mCameraHandler.postDelayed(() -> {
                                        logMessage("预览超时或正常结束，关闭相机");
                                        mCloseStartTimeMs = SystemClock.elapsedRealtime();
                                        camera.close();
                                    }, PREVIEW_TIMEOUT_MS);

                                }, 500); // 延迟 500ms 启动预览
                            }

                            @Override
                            public void onConfigureFailed(@NonNull CameraCaptureSession session) {
                                logError("配置预览失败", null);
                                mFailureCount++;
                                updateCounters();
                                mCloseStartTimeMs = SystemClock.elapsedRealtime();
                                camera.close();
                            }
                        }, mCameraHandler);

                mCameraHandler.postDelayed(() -> {
                    if (!mSessionConfigured) {
                        logError("创建预览会话超时", null);
                        mFailureCount++;
                        updateCounters();
                        mCloseStartTimeMs = SystemClock.elapsedRealtime();
                        camera.close();
                    }
                }, SESSION_TIMEOUT_MS);
            } catch (CameraAccessException e) {
                logError("创建会话失败", e);
                mFailureCount++;
                updateCounters();
                camera.close();
            }
        }

        @Override
        public void onClosed(@NonNull CameraDevice camera) {
            long closeTime = SystemClock.elapsedRealtime() - mCloseStartTimeMs;
            logMessage(String.format(Locale.US, "第 %d 次: 关闭成功 (耗时 %d ms)", mTotalRuns, closeTime));
            mOpenedCameraDevice = null;
            mPreviewSession = null;
            mFrameCounter = 0;
            scheduleNextRun();
        }

        @Override
        public void onDisconnected(@NonNull CameraDevice camera) {
            logError("相机断开连接", null);
            mFailureCount++;
            updateCounters();
            camera.close();
            scheduleNextRun();
        }

        @Override
        public void onError(@NonNull CameraDevice camera, int error) {
            long openTime = SystemClock.elapsedRealtime() - mOpenStartTimeMs;
            logError(String.format(Locale.US, "第 %d 次: 打开失败 (%d ms)，错误码: %d", mTotalRuns, openTime, error), null);
            mFailureCount++;
            updateCounters();
            camera.close();
            scheduleNextRun();
        }
    };

    private void scheduleNextRun() {
        if (mIsTesting && mCameraHandler != null) {
            mCameraHandler.postDelayed(mTestRunnable, TEST_DELAY_MS);
        }
    }

    private void updateCounters() {
        runOnUiThread(() -> {
            mTotalRunsTextView.setText("总次数: " + mTotalRuns);
            mSuccessTextView.setText("成功: " + mSuccessCount);
            mFailureTextView.setText("失败: " + mFailureCount);
        });
    }

    private void logMessage(String msg) {
        String timestamp = new SimpleDateFormat("HH:mm:ss.SSS", Locale.US).format(new Date());
        String log = timestamp + " | " + msg;

        writeLogToFile(log);

        runOnUiThread(() -> {
            mLogLines.addLast(log);
            if (mLogLines.size() > MAX_LOG_LINES) {
                mLogLines.removeFirst();
            }

            StringBuilder logBuilder = new StringBuilder();
            for (String line : mLogLines) {
                logBuilder.append(line).append("\n");
            }
            mLogTextView.setText(logBuilder.toString());
            mLogScrollView.post(() -> mLogScrollView.fullScroll(View.FOCUS_DOWN));
        });
    }

    private void logError(String msg, Exception e) {
        if (e != null) msg += " | 异常: " + e.getMessage();
        logMessage("错误: " + msg);
    }

    private void initLogFile() {
        try {
            File logDir = new File(getExternalFilesDir(null), "logs");
            if (!logDir.exists()) logDir.mkdirs();
            String filename = "camera_test_log_" + new SimpleDateFormat("yyyyMMdd_HHmmss", Locale.US).format(new Date()) + ".txt";
            File logFile = new File(logDir, filename);
            mLogFileOutput = new FileOutputStream(logFile, true);
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    private void writeLogToFile(String msg) {
        if (mLogFileOutput != null) {
            try {
                mLogFileOutput.write((msg + "\n").getBytes());
                mLogFileOutput.flush();
            } catch (IOException e) {
                e.printStackTrace();
            }
        }
    }

    private void closeLogFile() {
        try {
            if (mLogFileOutput != null) {
                mLogFileOutput.close();
                mLogFileOutput = null;
            }
        } catch (IOException e) {
            e.printStackTrace();
        }
    }
}

