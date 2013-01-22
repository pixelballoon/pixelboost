/*
 * Copyright (C) 2007 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

package com.pixelballoon.pixelboost;

import com.pixelballoon.pixelboost.PixelboostHelpers;

import android.app.Activity;
import android.media.AudioManager;
import android.media.MediaPlayer;
import android.media.SoundPool;
import android.os.Bundle;
import android.util.Log;
import android.view.KeyEvent;
import android.view.MotionEvent;
import android.view.WindowManager;

import java.io.File;
import java.io.IOException;

import tv.ouya.console.api.OuyaController;

public class PixelboostActivity extends Activity {

    PixelboostView mView;

    MediaPlayer music;
    SoundPool sounds;

    @Override protected void onCreate(Bundle icicle) {
        super.onCreate(icicle);
        mView = new PixelboostView(getApplication());
        setContentView(mView);

        sounds = new SoundPool(8, AudioManager.STREAM_MUSIC, 0);

        PixelboostHelpers.mainActivity = this;
    }

    @Override protected void onPause() {
        super.onPause();
        mView.onPause();

        if (music != null)
        {
            music.stop();
            if (isFinishing())
            {
                music.stop();
                music.release();
            }
        }

        PixelboostLib.onPause(isFinishing());
    }

    @Override protected void onResume() {
        super.onResume();
        mView.onResume();

        PixelboostLib.onResume();
    }

    @Override public boolean onKeyDown(int keyCode, KeyEvent event) {
        int player = OuyaController.getPlayerNumByDeviceId(event.getDeviceId());       
        boolean handled = false;

        switch(keyCode){
            case OuyaController.BUTTON_O:
                handled = true;
                PixelboostLib.onJoystickButtonDown(player, 0);
                break;
            case OuyaController.BUTTON_U:
                handled = true;
                PixelboostLib.onJoystickButtonDown(player, 3);
                break;
            case OuyaController.BUTTON_Y:
                handled = true;
                PixelboostLib.onJoystickButtonDown(player, 2);
                break;
            case OuyaController.BUTTON_A:
                handled = true;
                PixelboostLib.onJoystickButtonDown(player, 1);
                break;
            case OuyaController.BUTTON_DPAD_DOWN:
                handled = true;
                PixelboostLib.onJoystickButtonDown(player, 12);
                break;
            case OuyaController.BUTTON_DPAD_LEFT:
                handled = true;
                PixelboostLib.onJoystickButtonDown(player, 13);
                break;
            case OuyaController.BUTTON_DPAD_UP:
                handled = true;
                PixelboostLib.onJoystickButtonDown(player, 11);
                break;
            case OuyaController.BUTTON_DPAD_RIGHT:
                handled = true;
                PixelboostLib.onJoystickButtonDown(player, 14);
                break;
            case OuyaController.BUTTON_R1:
                handled = true;
                PixelboostLib.onJoystickButtonDown(player, 5);
                break;
            case OuyaController.BUTTON_L1:
                handled = true;
                PixelboostLib.onJoystickButtonDown(player, 4);
                break;
        }

        return handled || super.onKeyDown(keyCode, event);
    }

    @Override public boolean onKeyUp(int keyCode, KeyEvent event) {
        int player = OuyaController.getPlayerNumByDeviceId(event.getDeviceId());       
        boolean handled = false;

        switch(keyCode){
            case OuyaController.BUTTON_O:
                handled = true;
                PixelboostLib.onJoystickButtonUp(player, 0);
                break;
            case OuyaController.BUTTON_U:
                handled = true;
                PixelboostLib.onJoystickButtonUp(player, 3);
                break;
            case OuyaController.BUTTON_Y:
                handled = true;
                PixelboostLib.onJoystickButtonUp(player, 2);
                break;
            case OuyaController.BUTTON_A:
                handled = true;
                PixelboostLib.onJoystickButtonUp(player, 1);
                break;
            case OuyaController.BUTTON_DPAD_DOWN:
                handled = true;
                PixelboostLib.onJoystickButtonUp(player, 12);
                break;
            case OuyaController.BUTTON_DPAD_LEFT:
                handled = true;
                PixelboostLib.onJoystickButtonUp(player, 13);
                break;
            case OuyaController.BUTTON_DPAD_UP:
                handled = true;
                PixelboostLib.onJoystickButtonUp(player, 11);
                break;
            case OuyaController.BUTTON_DPAD_RIGHT:
                handled = true;
                PixelboostLib.onJoystickButtonUp(player, 14);
                break;
            case OuyaController.BUTTON_R1:
                handled = true;
                PixelboostLib.onJoystickButtonUp(player, 5);
                break;
            case OuyaController.BUTTON_L1:
                handled = true;
                PixelboostLib.onJoystickButtonUp(player, 4);
                break;
        }

        return handled || super.onKeyDown(keyCode, event);
    }

    @Override public boolean onGenericMotionEvent(MotionEvent event) {
        int player = OuyaController.getPlayerNumByDeviceId(event.getDeviceId());    

        float LS_X = event.getAxisValue(OuyaController.AXIS_LS_X);
        float LS_Y = event.getAxisValue(OuyaController.AXIS_LS_Y);
        float RS_X = event.getAxisValue(OuyaController.AXIS_RS_X);
        float RS_Y = event.getAxisValue(OuyaController.AXIS_RS_Y);
        float L2 = event.getAxisValue(OuyaController.AXIS_L2);
        float R2 = event.getAxisValue(OuyaController.AXIS_R2);

        PixelboostLib.onJoystickAxis(player, 0, 0, LS_X);
        PixelboostLib.onJoystickAxis(player, 0, 1, LS_Y);
        PixelboostLib.onJoystickAxis(player, 1, 0, RS_X);
        PixelboostLib.onJoystickAxis(player, 1, 1, RS_Y);
        PixelboostLib.onJoystickAxis(player, 2, 0, L2);
        PixelboostLib.onJoystickAxis(player, 2, 1, R2);

        return true;
    }
}
