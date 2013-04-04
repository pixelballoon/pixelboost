#include "emscripten.h"

#include "pixelboost/framework/eglUtil.h"
#include "pixelboost/framework/engine.h"
#include "pixelboost/graphics/device/device.h"
#include "pixelboost/input/touchManager.h"

pb::Engine* g_App;

void main_loop()
{
   g_App->Update(1.f/30.f, 1.f/30.f);
   g_App->Render();
}

int main(int argc, char *argv[])
{
   ESContext esContext;
   
   esInitContext(&esContext);
   esCreateWindow(&esContext, PIXELBOOST_BUILD_TITLE, 800, 600, ES_WINDOW_RGB);

   pb::GraphicsDevice::Instance()->SetDisplayDensity(32.f);
   pb::GraphicsDevice::Instance()->SetDisplayResolution(glm::vec2(800,600));

   g_App = pb::Engine::Create(0, 0, 0);
   g_App->Initialise();

   emscripten_set_main_loop(&main_loop, 30, true);
   
   delete g_App;

   return 0;
}
