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
import android.view.WindowManager;

import java.io.File;
import java.io.IOException;

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
}
