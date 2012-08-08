#pragma once

#include <vector>

namespace pb
{

    class Game;
    class Viewport;

    class Screen
    {
    public:
        Screen();
        virtual ~Screen();
        
        virtual void Update(float time);
        
        virtual void SetActive(bool active);
        
        void AddViewport(Viewport* viewport);
        void DestroyViewport(Viewport* viewport);
        
    private:
        typedef std::vector<Viewport*> ViewportList;
        
        bool _Active;
        ViewportList _Viewports;
    };

}
