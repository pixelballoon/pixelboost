#include "pixelboost/logic/component/transform.h"
#include "pixelboost/logic/message/transform.h"
#include "pixelboost/logic/system/graphics/render/render.h"
#include "pixelboost/logic/entity.h"

namespace pb
{

template <class T> RenderableComponent<T>::RenderableComponent(Entity* parent)
    : Component(parent)
{
    _Renderable = new T(parent->GetUid());
    
    GetScene()->template GetSystemByType<pb::RenderSystem>()->AddItem(_Renderable);

    GetEntity()->template RegisterMessageHandler<TransformChangedMessage>(MessageHandler(this, &RenderableComponent<T>::OnTransformChanged));
    
    UpdateTransform();
}

template <class T> RenderableComponent<T>::~RenderableComponent()
{
    GetEntity()->template UnregisterMessageHandler<TransformChangedMessage>(MessageHandler(this, &RenderableComponent<T>::OnTransformChanged));
    
    GetScene()->template GetSystemByType<pb::RenderSystem>()->RemoveItem(_Renderable);
    
    delete _Renderable;
}
        
template <class T> T* RenderableComponent<T>::GetRenderable() const
{
    return _Renderable;
}

template <class T> RenderPass RenderableComponent<T>::GetRenderPass() const
{
    return _Renderable->GetRenderPass();
}

template <class T> void RenderableComponent<T>::SetRenderPass(RenderPass renderPass)
{
    _Renderable->SetRenderPass(renderPass);
}

template <class T> int RenderableComponent<T>::GetLayer() const
{
    return _Renderable->GetLayer();
}

template <class T> void RenderableComponent<T>::SetLayer(int layer)
{
    _Renderable->SetLayer(layer);
}

template <class T> const glm::mat4x4& RenderableComponent<T>::GetLocalTransform() const
{
    return _LocalTransform;
}

template <class T> void RenderableComponent<T>::SetLocalTransform(const glm::mat4x4& localTransform)
{
    _LocalTransform = localTransform;
    
    UpdateTransform();
}

template <class T> void RenderableComponent<T>::OnTransformChanged(const Message& message)
{
    UpdateTransform();
}

template <class T> void RenderableComponent<T>::UpdateTransform()
{
    TransformComponent* transform = GetEntity()->template GetComponent<TransformComponent>();    
    
    if (transform)
    {
        _Renderable->SetTransform(transform->GetMatrix() * _LocalTransform);
    }
}

template <class T> bool RenderableComponent<T>::IsA(Uid type) const
{
    return type == GetStaticType() || Component::IsA(type);
}

template <class T> Uid RenderableComponent<T>::GetType() const
{
    return GetStaticType();
}

template <class T> Uid RenderableComponent<T>::GetStaticType()
{
    return TypeHash("RenderableComponent");
}

}
