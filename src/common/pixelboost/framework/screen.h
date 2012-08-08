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
        
    public:
        typedef std::vector<Viewport*> ViewportList;
        
        void AddViewport(Viewport* viewport);
        void DestroyViewport(Viewport* viewport);
        const ViewportList& GetViewports();
        
    private:
        bool _Active;
        ViewportList _Viewports;
    };

}
