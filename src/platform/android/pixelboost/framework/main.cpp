#include <android/log.h>
#include <jni.h>
#include <pthread.h>

#include "pixelboost/audio/soundManager.h"
#include "pixelboost/debug/log.h"
#include "pixelboost/file/fileSystem.h"
#include "pixelboost/framework/engine.h"
#include "pixelboost/graphics/device/device.h"
#include "pixelboost/input/joystickManager.h"
#include "pixelboost/input/touchManager.h"

extern JavaVM* g_JavaVM;
pb::Engine* g_Game = 0;

struct TouchInfo
{
    int type;
    int index;
    int x;
    int y;
};

struct JoystickButtonInfo
{
    int joystick;
    int button;
    bool isDown;
};

struct JoystickAxisInfo
{
    int joystick;
    int stick;
    int axis;
    float value;
};

typedef std::vector<TouchInfo> TouchList;
typedef std::vector<JoystickAxisInfo> JoystickAxisList;
typedef std::vector<JoystickButtonInfo> JoystickButtonList;

pthread_mutex_t g_InputMutex = PTHREAD_MUTEX_INITIALIZER;
TouchList g_Touches;
JoystickAxisList g_JoystickAxis;
JoystickButtonList g_JoystickButton;

extern "C" {
    JNIEXPORT void JNICALL Java_com_pixelballoon_pixelboost_PixelboostLib_init(JNIEnv * env, jobject obj,  jint width, jint height);
    JNIEXPORT jboolean JNICALL Java_com_pixelballoon_pixelboost_PixelboostLib_allowFrameskip(JNIEnv * env, jobject obj);
    JNIEXPORT void JNICALL Java_com_pixelballoon_pixelboost_PixelboostLib_update(JNIEnv * env, jobject obj, jfloat delta);
    JNIEXPORT void JNICALL Java_com_pixelballoon_pixelboost_PixelboostLib_render(JNIEnv * env, jobject obj);
    JNIEXPORT void JNICALL Java_com_pixelballoon_pixelboost_PixelboostLib_onSurfaceCreated(JNIEnv * env, jobject obj);
    JNIEXPORT void JNICALL Java_com_pixelballoon_pixelboost_PixelboostLib_onPause(JNIEnv * env, jobject obj, jboolean quit);
    JNIEXPORT void JNICALL Java_com_pixelballoon_pixelboost_PixelboostLib_onResume(JNIEnv * env, jobject obj);
    JNIEXPORT void JNICALL Java_com_pixelballoon_pixelboost_PixelboostLib_onPointerDown(JNIEnv * env, jobject obj, jint touchIndex, jint x, jint y);
    JNIEXPORT void JNICALL Java_com_pixelballoon_pixelboost_PixelboostLib_onPointerMove(JNIEnv * env, jobject obj, jint touchIndex, jint x, jint y);
    JNIEXPORT void JNICALL Java_com_pixelballoon_pixelboost_PixelboostLib_onPointerUp(JNIEnv * env, jobject obj, jint touchIndex, jint x, jint y);
    JNIEXPORT void JNICALL Java_com_pixelballoon_pixelboost_PixelboostLib_onJoystickButtonDown(JNIEnv * env, jobject obj, jint joystick, jint button);
    JNIEXPORT void JNICALL Java_com_pixelballoon_pixelboost_PixelboostLib_onJoystickButtonUp(JNIEnv * env, jobject obj, jint joystick, jint button);
    JNIEXPORT void JNICALL Java_com_pixelballoon_pixelboost_PixelboostLib_onJoystickAxis(JNIEnv * env, jobject obj, jint joystick, jint stick, jint axis, jfloat value);
};

JNIEXPORT void JNICALL Java_com_pixelballoon_pixelboost_PixelboostLib_init(JNIEnv * env, jobject obj,  jint width, jint height)
{
    env->GetJavaVM(&g_JavaVM);

    float scale = 1.f;

    float displayDensity = 16.f;

    if (width > 480 || height > 320)
        displayDensity = 32.f;  // TODO: Use correct density

    PbLogDebug("pixelboost.init", "Display created %d %d\n", width, height);
    
    pb::GraphicsDevice::Instance()->SetDisplayResolution(glm::vec2(width, height));
    pb::GraphicsDevice::Instance()->SetDisplayDensity(displayDensity);
    
    if (!g_Game)
    {
        g_Game = pb::Engine::Create(0, std::vector<std::string>());
        g_Game->Initialise();
    }
}

JNIEXPORT jboolean JNICALL Java_com_pixelballoon_pixelboost_PixelboostLib_allowFrameskip(JNIEnv * env, jobject obj)
{
    return g_Game->AllowFrameskip();
}


JNIEXPORT void JNICALL Java_com_pixelballoon_pixelboost_PixelboostLib_update(JNIEnv * env, jobject obj, jfloat delta)
{
    pthread_mutex_lock(&g_InputMutex);

    for (TouchList::iterator it = g_Touches.begin(); it != g_Touches.end(); ++it)
    {
        if (it->type == 0)
        {
            g_Game->GetTouchManager()->OnTouchDown(it->index, glm::vec2(it->x, it->y));
        } else if (it->type == 1)
        {
            g_Game->GetTouchManager()->OnTouchMove(it->index, glm::vec2(it->x, it->y));
        } else if (it->type == 2)
        {
            g_Game->GetTouchManager()->OnTouchUp(it->index, glm::vec2(it->x, it->y));
        }
    }

    for (JoystickAxisList::iterator it = g_JoystickAxis.begin(); it != g_JoystickAxis.end(); ++it)
    {
        g_Game->GetJoystickManager()->OnAxisChanged(it->joystick, it->stick, it->axis, it->value);
    }

    for (JoystickButtonList::iterator it = g_JoystickButton.begin(); it != g_JoystickButton.end(); ++it)
    {
        if (it->isDown)
            g_Game->GetJoystickManager()->OnButtonDown(it->joystick, it->button);
        else
            g_Game->GetJoystickManager()->OnButtonUp(it->joystick, it->button);
    }

    g_Touches.clear();
    g_JoystickAxis.clear();
    g_JoystickButton.clear();

    pthread_mutex_unlock(&g_InputMutex);

    g_Game->Update(delta);
}

JNIEXPORT void JNICALL Java_com_pixelballoon_pixelboost_PixelboostLib_render(JNIEnv * env, jobject obj)
{
    g_Game->Render();
}

JNIEXPORT void JNICALL Java_com_pixelballoon_pixelboost_PixelboostLib_onSurfaceCreated(JNIEnv * env, jobject obj)
{
    if (g_Game)
    {
        pb::GraphicsDevice::Instance()->OnContextLost();
    }
}

JNIEXPORT void JNICALL Java_com_pixelballoon_pixelboost_PixelboostLib_onPause(JNIEnv * env, jobject obj, jboolean quit)
{
    if (g_Game)
    {
        if (quit)
        {
            g_Game->OnAppQuit();
        } else {
            g_Game->OnAppLoseFocus();
        }
    }
}

JNIEXPORT void JNICALL Java_com_pixelballoon_pixelboost_PixelboostLib_onResume(JNIEnv * env, jobject obj)
{
    if (g_Game)
    {
        pb::SoundManager::Instance()->ReloadSfx();
        if (!pb::SoundManager::Instance()->IsBgmMuted())
        {
            pb::SoundManager::Instance()->MuteBgm(true);
            pb::SoundManager::Instance()->MuteBgm(false);
        }

        g_Game->OnAppGainFocus();
    }
}

JNIEXPORT void JNICALL Java_com_pixelballoon_pixelboost_PixelboostLib_onPointerDown(JNIEnv * env, jobject obj,  jint touchIndex, jint x, jint y)
{
    pthread_mutex_lock(&g_InputMutex);

    TouchInfo t;
    t.type = 0;
    t.index = touchIndex;
    t.x = x;
    t.y = y;

    g_Touches.push_back(t);

    pthread_mutex_unlock(&g_InputMutex);
}

JNIEXPORT void JNICALL Java_com_pixelballoon_pixelboost_PixelboostLib_onPointerMove(JNIEnv * env, jobject obj,  jint touchIndex, jint x, jint y)
{
    pthread_mutex_lock(&g_InputMutex);

    TouchInfo t;
    t.type = 1;
    t.index = touchIndex;
    t.x = x;
    t.y = y;

    g_Touches.push_back(t);
    
    pthread_mutex_unlock(&g_InputMutex);
}

JNIEXPORT void JNICALL Java_com_pixelballoon_pixelboost_PixelboostLib_onPointerUp(JNIEnv * env, jobject obj,  jint touchIndex, jint x, jint y)
{
    pthread_mutex_lock(&g_InputMutex);

    TouchInfo t;
    t.type = 2;
    t.index = touchIndex;
    t.x = x;
    t.y = y;

    g_Touches.push_back(t);

    pthread_mutex_unlock(&g_InputMutex);
}

JNIEXPORT void JNICALL Java_com_pixelballoon_pixelboost_PixelboostLib_onJoystickButtonDown(JNIEnv * env, jobject obj, jint joystick, jint button)
{
    pthread_mutex_lock(&g_InputMutex);

    JoystickButtonInfo j;
    j.isDown = true;
    j.joystick = joystick;
    j.button = button;

    g_JoystickButton.push_back(j);

    pthread_mutex_unlock(&g_InputMutex);
}

JNIEXPORT void JNICALL Java_com_pixelballoon_pixelboost_PixelboostLib_onJoystickButtonUp(JNIEnv * env, jobject obj, jint joystick, jint button)
{
    pthread_mutex_lock(&g_InputMutex);

    JoystickButtonInfo j;
    j.isDown = false;
    j.joystick = joystick;
    j.button = button;

    g_JoystickButton.push_back(j);

    pthread_mutex_unlock(&g_InputMutex);
}

JNIEXPORT void JNICALL Java_com_pixelballoon_pixelboost_PixelboostLib_onJoystickAxis(JNIEnv * env, jobject obj, jint joystick, jint stick, jint axis, jfloat value)
{
    pthread_mutex_lock(&g_InputMutex);

    JoystickAxisInfo j;
    j.joystick = joystick;
    j.stick = stick;
    j.axis = axis;
    j.value = value;

    g_JoystickAxis.push_back(j);

    pthread_mutex_unlock(&g_InputMutex);
}
