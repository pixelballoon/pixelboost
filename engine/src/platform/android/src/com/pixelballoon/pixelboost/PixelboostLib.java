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

// Wrapper for native library

public class PixelboostLib {

    static {
        System.loadLibrary("pixelboost");
    }

    /**
     * @param width the current view width
     * @param height the current view height
     */
    public static native void init(int width, int height);
    public static native boolean allowFrameskip();
    public static native void update(float delta);
    public static native void render();

    public static native void onSurfaceCreated();

    public static native void onPause(boolean quit);
    public static native void onResume();

    public static native void onBackButton();
    public static native void onMenuButton();

    public static native void onPointerDown(int index, int width, int height);
    public static native void onPointerMove(int index, int width, int height);
    public static native void onPointerUp(int index, int width, int height);

    public static native void onJoystickButtonDown(int joystick, int button);
    public static native void onJoystickButtonUp(int joystick, int button);
    public static native void onJoystickAxis(int joystick, int stick, int axis, float value);
}
