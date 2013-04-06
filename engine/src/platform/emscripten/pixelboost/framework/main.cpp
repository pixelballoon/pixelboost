#include <algorithm>

#include <emscripten.h>

#include <SDL.h>
#include <SDL_opengl.h>

#include "pixelboost/debug/log.h"
#include "pixelboost/framework/engine.h"
#include "pixelboost/graphics/device/device.h"
#include "pixelboost/input/mouseManager.h"
#include "pixelboost/input/touchManager.h"

SDL_Surface *screen = NULL;
pb::Engine* g_App;

#define SCR_DEFAULTW 1024
#define SCR_DEFAULTH 576

int desktopw = 0;
int desktoph = 0;
int scr_w = -1;
int scr_h = -1;
int colorbits = 0;
int depthbits = 0;
int stencilbits = 0;
int fsaa = -1;
int vsync = -1;
int fullscreen = 0;
int hasstencil = 0;

void setupscreen(int &usedcolorbits, int &useddepthbits, int &usedfsaa)
{
    int flags = SDL_RESIZABLE;
    #if defined(WIN32) || defined(__APPLE__)
    flags = 0;
    #endif
    if(fullscreen) flags = SDL_FULLSCREEN;
    SDL_Rect **modes = SDL_ListModes(NULL, SDL_OPENGL|flags);
    if(modes && modes!=(SDL_Rect **)-1)
    {
        int widest = -1, best = -1;
        for(int i = 0; modes[i]; i++)
        {
            if(widest < 0 || modes[i]->w > modes[widest]->w || (modes[i]->w == modes[widest]->w && modes[i]->h > modes[widest]->h)) 
                widest = i; 
        }
        if(scr_w < 0 || scr_h < 0)
        {
            int w = scr_w, h = scr_h, ratiow = desktopw, ratioh = desktoph;
            if(w < 0 && h < 0) { w = SCR_DEFAULTW; h = SCR_DEFAULTH; }
            if(ratiow <= 0 || ratioh <= 0) { ratiow = modes[widest]->w; ratioh = modes[widest]->h; }
            for(int i = 0; modes[i]; i++) if(modes[i]->w*ratioh == modes[i]->h*ratiow)
            {
                if(w <= modes[i]->w && h <= modes[i]->h && (best < 0 || modes[i]->w < modes[best]->w))
                    best = i;
            }
        } 
        if(best < 0)
        {
            int w = scr_w, h = scr_h;
            if(w < 0 && h < 0) { w = SCR_DEFAULTW; h = SCR_DEFAULTH; }
            else if(w < 0) w = (h*SCR_DEFAULTW)/SCR_DEFAULTH;
            else if(h < 0) h = (w*SCR_DEFAULTH)/SCR_DEFAULTW;
            for(int i = 0; modes[i]; i++)
            {
                if(w <= modes[i]->w && h <= modes[i]->h && (best < 0 || modes[i]->w < modes[best]->w || (modes[i]->w == modes[best]->w && modes[i]->h < modes[best]->h)))
                    best = i;
            }
        }
        if(flags&SDL_FULLSCREEN)
        {
            if(best >= 0) { scr_w = modes[best]->w; scr_h = modes[best]->h; }
            else if(desktopw > 0 && desktoph > 0) { scr_w = desktopw; scr_h = desktoph; }
            else if(widest >= 0) { scr_w = modes[widest]->w; scr_h = modes[widest]->h; } 
        }
        else if(best < 0)
        { 
            scr_w = std::min(scr_w >= 0 ? scr_w : (scr_h >= 0 ? (scr_h*SCR_DEFAULTW)/SCR_DEFAULTH : SCR_DEFAULTW), (int)modes[widest]->w); 
            scr_h = std::min(scr_h >= 0 ? scr_h : (scr_w >= 0 ? (scr_w*SCR_DEFAULTH)/SCR_DEFAULTW : SCR_DEFAULTH), (int)modes[widest]->h);
        }
    }

    if(scr_w < 0 && scr_h < 0) { scr_w = SCR_DEFAULTW; scr_h = SCR_DEFAULTH; }
    else if(scr_w < 0) scr_w = (scr_h*SCR_DEFAULTW)/SCR_DEFAULTH;
    else if(scr_h < 0) scr_h = (scr_w*SCR_DEFAULTH)/SCR_DEFAULTW;

    bool hasbpp = true;
    if(colorbits)
        hasbpp = SDL_VideoModeOK(scr_w, scr_h, colorbits, SDL_OPENGL|flags)==colorbits;

    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
#if SDL_VERSION_ATLEAST(1, 2, 11) && !EMSCRIPTEN
    if(vsync>=0) SDL_GL_SetAttribute(SDL_GL_SWAP_CONTROL, vsync);
#endif
    static int configs[] =
    {
        0x7, /* try everything */
        0x6, 0x5, 0x3, /* try disabling one at a time */
        0x4, 0x2, 0x1, /* try disabling two at a time */
        0 /* try disabling everything */
    };
    int config = 0;
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 0);
    if(!depthbits) SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
    if(!fsaa)
    {
        SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 0);
        SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 0);
    }
    for (int i=0; i<sizeof(configs)/sizeof(configs[0]); i++)
    {
        config = configs[i];
        if(!depthbits && config&1) continue;
        if(!stencilbits && config&2) continue;
        if(fsaa<=0 && config&4) continue;
        if(depthbits) SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, config&1 ? depthbits : 16);
        if(stencilbits)
        {
            hasstencil = config&2 ? stencilbits : 0;
            SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, hasstencil);
        }
        else hasstencil = 0;
        if(fsaa>0)
        {
            SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, config&4 ? 1 : 0);
            SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, config&4 ? fsaa : 0);
        }
        screen = SDL_SetVideoMode(scr_w, scr_h, hasbpp ? colorbits : 0, SDL_OPENGL|flags);
        if(screen) break;
    }
   if(!screen)
   {
      printf("Unable to create OpenGL screen: %s", SDL_GetError());
      return;
   }
   else
   {
      if(!hasbpp) printf("%d bit color buffer not supported - disabling", colorbits);
      if(depthbits && (config&1)==0) printf("%d bit z-buffer not supported - disabling", depthbits);
      if(stencilbits && (config&2)==0) printf("Stencil buffer not supported - disabling");
      if(fsaa>0 && (config&4)==0) printf("%dx anti-aliasing not supported - disabling", fsaa);
   }

   scr_w = screen->w;
   scr_h = screen->h;

   usedcolorbits = hasbpp ? colorbits : 0;
   useddepthbits = config&1 ? depthbits : 0;
   usedfsaa = config&4 ? fsaa : 0;
}

void ProcessEvents()
{
    SDL_Event event;
    
    while(SDL_PollEvent(&event))
    {
        switch(event.type)
        {
            case SDL_QUIT:
                return;

            case SDL_VIDEORESIZE:
                break;
            
            case SDL_KEYDOWN:
            case SDL_KEYUP:
            {
               pb::KeyboardKey key = pb::kKeyboardKeyCharacter;
               pb::ModifierKeys modifier = 0;
               char character;

               if (event.key.keysym.unicode < 0x80 && event.key.keysym.unicode > 0)
               {
                  key = pb::kKeyboardKeyCharacter;
                  modifier = 0;
                  character = (char)event.key.keysym.unicode;
               }

               if (event.type == SDL_KEYDOWN)
               {
                  pb::Engine::Instance()->GetKeyboardManager()->OnKeyDown(key, modifier, character);
               } else {
                  pb::Engine::Instance()->GetKeyboardManager()->OnKeyUp(key, modifier, character);
               }
               
               break;
            }

            case SDL_MOUSEMOTION:
            {
                if (event.motion.state & SDL_BUTTON(1))
                {
                  pb::Engine::Instance()->GetTouchManager()->OnTouchMove(0, glm::vec2(event.motion.x, event.motion.y));
                }
                pb::Engine::Instance()->GetMouseManager()->OnMouseMove(glm::vec2(event.motion.x, event.motion.y));
                break;
             }

            case SDL_MOUSEBUTTONDOWN:
            {
               pb::Engine::Instance()->GetTouchManager()->OnTouchDown(0, glm::vec2(event.button.x, event.button.y));
               pb::Engine::Instance()->GetMouseManager()->OnMouseDown(event.button.button == SDL_BUTTON_LEFT ? pb::kMouseButtonLeft : pb::kMouseButtonRight, 0, glm::vec2(event.button.x, event.button.y));
               break;
            }

            case SDL_MOUSEBUTTONUP:
            {
               pb::Engine::Instance()->GetTouchManager()->OnTouchUp(0, glm::vec2(event.button.x, event.button.y));
               pb::Engine::Instance()->GetMouseManager()->OnMouseUp(event.button.button == SDL_BUTTON_LEFT ? pb::kMouseButtonLeft : pb::kMouseButtonRight, 0, glm::vec2(event.button.x, event.button.y));
               break;
            }
        }
    }
}

void main_loop()
{
   ProcessEvents();
   g_App->Update(1.f/30.f, 1.f/30.f);
   g_App->Render();
}

int main(int argc, char *argv[])
{
   if(SDL_Init(SDL_INIT_TIMER|SDL_INIT_VIDEO|SDL_INIT_AUDIO)<0)
   {
      printf("Unable to initialize SDL: %s", SDL_GetError());
      return 1;
   }

   const SDL_VideoInfo *video = SDL_GetVideoInfo();
   if(video) 
   {
      desktopw = video->current_w;
      desktoph = video->current_h;
   }

   int usedcolorbits = 0;
   int useddepthbits = 0;
   int usedfsaa = 0;
   setupscreen(usedcolorbits, useddepthbits, usedfsaa);

   pb::GraphicsDevice::Instance()->SetDisplayDensity(32.f);
   pb::GraphicsDevice::Instance()->SetDisplayResolution(glm::vec2(scr_w,scr_h));

   g_App = pb::Engine::Create(0, 0, 0);
   g_App->Initialise();

   emscripten_set_main_loop(&main_loop, 30, true);
   
   delete g_App;

   return 0;
}
