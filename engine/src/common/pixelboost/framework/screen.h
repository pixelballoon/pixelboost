#pragma once

#include <vector>

namespace pb
{

    class Viewport;

    class Screen
    {
    public:
        Screen();
        virtual ~Screen();
        
        virtual void Update(float timeDelta, float gameDelta);
        
        bool IsActive();
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
