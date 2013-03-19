#pragma once

#include "pixelboost/graphics/renderer/sprite/sprite.h"
#include "pixelboost/resource/resourceManager.h"

#include "view/entity/property/property.h"

namespace pb
{
    class SpriteComponent;
    class TextureResource;
    
    struct Sprite;
}

class SpriteViewProperty : public ViewProperty
{
    PB_DECLARE_ENTITY
    
protected:
    SpriteViewProperty(pb::Scene* scene, pb::Entity* parent, pb::DbEntity* creationEntity);
    virtual ~SpriteViewProperty();
    
public:
    virtual void Initialise(const std::string &path, const SchemaItem *schemaItem);
    
    virtual void Refresh();
    
protected:
    virtual pb::BoundingBox CalculateBounds();
    
private:
    void OnResourceLoaded(pb::ResourceHandleBase* resource, bool error);
    void OnResourceUnloading(pb::ResourceHandleBase* resource);
    
    std::string _SpriteName;
    pb::Sprite _Sprite;
    
    std::shared_ptr<pb::ResourceHandle<pb::TextureResource> > _Texture;
};
