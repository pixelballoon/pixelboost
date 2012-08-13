#pragma once

#ifndef PIXELBOOST_DISABLE_BOX2D

#include "pixelboost/logic/system.h"

class b2World;

namespace pb
{
    
    class Box2dRenderer;

    class PhysicsSystem2D : public pb::SceneSystem
    {
    public:
        PhysicsSystem2D(glm::vec2 gravity = glm::vec2(0,0));
        ~PhysicsSystem2D();
        
        virtual pb::Uid GetType();
        static pb::Uid GetStaticType();
        
        virtual void Update(Scene* scene, float time);
        virtual void Render(Scene* scene, Viewport* viewport, RenderPass renderPass);
        
    public:
        b2World* GetPhysicsWorld();
        
        void SetDebugRender(bool debugRender, int layer = 0);
        
    private:
        bool _DebugRender;
        pb::Box2dRenderer* _DebugRenderer;
        
        int _PositionIterations;
        int _VelocityIterations;
        
        b2World* _World;
    };
    
}

#endif