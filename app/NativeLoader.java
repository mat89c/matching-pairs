package com.mat89c.matching_pairs;

import android.content.Context;
import android.view.inputmethod.InputMethodManager;
import android.view.inputmethod.InputConnection;
import android.view.inputmethod.EditorInfo;
import android.view.KeyEvent;
import android.view.View;

public class NativeLoader extends android.app.NativeActivity {
    static {
        System.loadLibrary("main");
    }

    private View decorView;

    @Override
    protected void onCreate(android.os.Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        decorView = getWindow().getDecorView();
    }

    public NativeLoader getFeatures() {
        return this;
    }

    public void showKeyboard() {
        InputMethodManager imm = (InputMethodManager) getSystemService(Context.INPUT_METHOD_SERVICE);
        if (imm != null && decorView != null) {
            imm.showSoftInput(decorView, InputMethodManager.SHOW_FORCED);
        }
    }

    public void hideKeyboard() {
        InputMethodManager imm = (InputMethodManager) getSystemService(Context.INPUT_METHOD_SERVICE);
        if (imm != null && decorView != null) {
            imm.hideSoftInputFromWindow(decorView.getWindowToken(), 0);
        }
    }

    @Override
    public boolean onKeyDown(int keyCode, KeyEvent event) {
        if (keyCode == KeyEvent.KEYCODE_DEL) {
            nativeOnTextInput("\b");
            return true;
        }
        
        char label = event.getDisplayLabel();
        if (label != 0) {
            nativeOnTextInput(String.valueOf(label));
            return true;
        }
        return super.onKeyDown(keyCode, event);
    }  

    private native void nativeOnTextInput(String text);
}
