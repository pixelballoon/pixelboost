#pragma once

#ifndef PIXELBOOST_DISABLE_BOX2D

#include "Box2D/Box2D.h"

#include "pixelboost/logic/system.h"

namespace pb
{
    
    class Box2dRenderer;

    class PhysicsSystem2D : public pb::SceneSystem, b2ContactListener
    {
    public:
        PhysicsSystem2D(glm::vec2 gravity = glm::vec2(0,0));
        ~PhysicsSystem2D();
        
        virtual pb::Uid GetType() const;
        static pb::Uid GetStaticType();
        
        virtual void Update(Scene* scene, float time);
        virtual void Render(Scene* scene, Viewport* viewport, RenderPass renderPass);
        
    public:
        b2World* GetPhysicsWorld();
        
        void SetDebugRender(bool debugRender, int flags = b2Draw::e_shapeBit|b2Draw::e_jointBit/*|b2Draw::e_aabbBit|b2Draw::e_pairBit|b2Draw::e_centerOfMassBit*/, int layer = 0);
        
    private:
        void BeginContact(b2Contact* contact);
        void EndContact(b2Contact* contact);
        
        bool _DebugRender;
        pb::Box2dRenderer* _DebugRenderer;
        
        int _PositionIterations;
        int _VelocityIterations;
        
        b2World* _World;
    };
    
}

#endif