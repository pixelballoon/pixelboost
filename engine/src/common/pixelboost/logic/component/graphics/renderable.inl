#include "pixelboost/graphics/resources/materialResource.h"
#include "pixelboost/logic/component/transform.h"
#include "pixelboost/logic/message/transform.h"
#include "pixelboost/logic/system/graphics/render/render.h"
#include "pixelboost/logic/entity.h"

namespace pb
{

template <class T> RenderableComponent<T>::RenderableComponent(Entity* parent)
    : Component(parent)
{
    _Renderable = new T();
    
    GetScene()->template GetSystemByType<RenderSystem>()->AddItem(_Renderable);

    GetEntity()->template RegisterMessageHandler<TransformChangedMessage>(MessageHandler(this, &RenderableComponent<T>::OnTransformChanged));
    
    UpdateTransform();
}

template <class T> RenderableComponent<T>::~RenderableComponent()
{
    if (_Material)
    {
        _Material->SignalResourceLoaded.Disconnect(this, &RenderableComponent<T>::OnResourceLoaded);
        _Material->SignalResourceUnloading.Disconnect(this, &RenderableComponent<T>::OnResourceUnloading);
    }

    GetEntity()->template UnregisterMessageHandler<TransformChangedMessage>(MessageHandler(this, &RenderableComponent<T>::OnTransformChanged));
    
    GetScene()->template GetSystemByType<RenderSystem>()->RemoveItem(_Renderable);
    
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

template <class T> Uid RenderableComponent<T>::GetGroup() const
{
    return _Renderable->GetGroup();
}

template <class T> void RenderableComponent<T>::SetGroup(Uid group)
{
    _Renderable->SetGroup(group);
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

template <class T> void RenderableComponent<T>::SetMaterial(const std::string& filename)
{
    if (_Material)
    {
        _Renderable->SetMaterial(0);
        _Material->SignalResourceLoaded.Disconnect(this, &RenderableComponent<T>::OnResourceLoaded);
        _Material->SignalResourceUnloading.Disconnect(this, &RenderableComponent<T>::OnResourceUnloading);
    }

    auto material = ResourceManager::Instance()->GetPool("default")->GetResource<MaterialResource>(filename);
    _Material = material;
    _Material->SignalResourceLoaded.Connect(this, &RenderableComponent<T>::OnResourceLoaded);
    _Material->SignalResourceUnloading.Connect(this, &RenderableComponent<T>::OnResourceUnloading);
    
    if (_Material->GetState() == kResourceStateReady)
    {
        OnResourceLoaded(_Material.get(), false);
    }
}

template <class T> std::shared_ptr<MaterialResource> RenderableComponent<T>::GetMaterial()
{
    return _Material;
}

template <class T> void RenderableComponent<T>::OnResourceLoaded(Resource* resource, bool error)
{
    if (error)
        return;

    if (resource == _Material.get())
    {
        _Renderable->SetMaterial(_Material->GetMaterial());
    }
}

template <class T> void RenderableComponent<T>::OnResourceUnloading(Resource* resource)
{
    if (resource == _Material.get())
    {
        _Renderable->SetMaterial(0);
    }
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
