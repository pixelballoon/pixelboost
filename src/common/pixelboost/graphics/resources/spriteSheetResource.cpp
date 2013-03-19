#include "json/reader.h"

#include "pixelboost/debug/log.h"
#include "pixelboost/file/fileSystem.h"
#include "pixelboost/graphics/device/device.h"
#include "pixelboost/graphics/renderer/sprite/sprite.h"
#include "pixelboost/graphics/resources/spriteSheetResource.h"
#include "pixelboost/graphics/resources/textureResource.h"

using namespace pb;

PB_DEFINE_RESOURCE(pb::SpriteSheetResource)

SpriteSheetResource::SpriteSheetResource()
{
    _SpriteSheet = 0;
}

SpriteSheetResource::~SpriteSheetResource()
{
    
}

ResourceReadyState SpriteSheetResource::IsReadyToProcess(ResourceProcess process, std::string& errorDetails)
{
    switch (process)
    {
        case kResourceProcessLoad:
            return kResourceReadyStateReady;
        case kResourceProcessProcess:
        {
            if (_Texture->GetState() == kResourceStateError)
            {
                errorDetails = "Failed to load texture";
                return kResourceReadyStateError;
            }
            
            if (_Texture->GetState() != kResourceStateReady)
            {
                return kResourceReadyStateAwaitingDependencies;
            }
            
            return kResourceReadyStateReady;
        }
        case kResourceProcessPostProcess:
            return kResourceReadyStateReady;
        case kResourceProcessUnload:
            return kResourceReadyStateReady;
    }
}

ResourceError SpriteSheetResource::ProcessResource(ResourcePool* pool, ResourceProcess process, const std::string& filename, std::string& errorDetails)
{
    switch (process)
    {
        case kResourceProcessLoad:
        {
            if (_SpriteSheet)
            {
                delete _SpriteSheet;
            }
            _SpriteSheet = new pb::SpriteSheet();
            return LoadSheet(pool, filename, errorDetails);
        }
        case kResourceProcessProcess:
        {
            _SpriteSheet->SetTexture(_Texture->GetResource()->GetTexture());
            return kResourceErrorNone;
        }
        case kResourceProcessPostProcess:
        {
            return kResourceErrorNone;
        }
        case kResourceProcessUnload:
        {
            if (_SpriteSheet)
            {
                delete _SpriteSheet;
                _SpriteSheet = 0;
            }
            return kResourceErrorNone;
        }
    }
}

ResourceThread SpriteSheetResource::GetResourceThread(ResourceProcess process)
{
    return kResourceThreadMain;
}

SpriteSheet* SpriteSheetResource::GetSpriteSheet()
{
    return _SpriteSheet;
}

ResourceError SpriteSheetResource::LoadSheet(ResourcePool* pool, const std::string& filename, std::string& errorDetails)
{
    std::string modifier;
    float sheetDensity = 16.f;
    
    if (GraphicsDevice::Instance()->GetDisplayDensity() >= 64.f)
    {
        modifier = "-hdr";
        sheetDensity = 64.f;
    }
    else if (GraphicsDevice::Instance()->GetDisplayDensity() >= 32.f)
    {
        modifier = "-hd";
        sheetDensity = 32.f;
    }
    
    std::string jsonFilename = filename + modifier + ".json";
    
    pb::File* file = pb::FileSystem::Instance()->OpenFile(jsonFilename);
    
    if (!file)
    {
        return kResourceErrorNoSuchResource;
    }
    
    std::string rootData;
    
    file->ReadAll(rootData);
    delete file;
    
    json::Object root;
    
    json::Reader::Read(root, rootData);
    
    json::Object& meta = root["meta"];
    
    json::Object& size = meta["size"];
    json::Number& width = size["w"];
    json::Number& height = size["h"];
    
    json::Object& frames = root["frames"];
    
    for (json::Object::iterator it = frames.Begin(); it != frames.End(); ++it)
    {
        std::string name = it->name;
        json::Object data = it->element;
        
        json::Object frame = data["frame"];
        json::Number frameX = frame["x"];
        json::Number frameY = frame["y"];
        json::Number frameW = frame["w"];
        json::Number frameH = frame["h"];
        
        json::Object spriteSourceSize = data["spriteSourceSize"];
        json::Number spriteSourceSizeX = spriteSourceSize["x"];
        json::Number spriteSourceSizeY = spriteSourceSize["y"];
        json::Number spriteSourceSizeW = spriteSourceSize["w"];
        json::Number spriteSourceSizeH = spriteSourceSize["h"];
        
        json::Object sourceSize = data["sourceSize"];
        json::Number sourceSizeW = sourceSize["w"];
        json::Number sourceSizeH = sourceSize["h"];
        
        json::Boolean rotated = data["rotated"];
        
        
        glm::vec2 size = glm::vec2(frameW.Value(), frameH.Value()) / sheetDensity;
        glm::vec2 offset = glm::vec2(sourceSizeW.Value()/2.f-spriteSourceSizeX.Value()-spriteSourceSizeW.Value()/2.f, -(sourceSizeH.Value()/2.f-spriteSourceSizeY.Value()-spriteSourceSizeH.Value()/2.f)) / GraphicsDevice::Instance()->GetDisplayDensity();
        
        glm::vec2 uvPosition(frameX.Value()/width.Value(), frameY.Value()/height.Value());
        glm::vec2 uvSize = rotated.Value() ? glm::vec2(frameW.Value()/height.Value(), frameH.Value()/width.Value()) : glm::vec2(frameW.Value()/width.Value(), frameH.Value()/height.Value());
        
        std::string spriteName = name.substr(0, name.find_last_of('.'));

        _SpriteSheet->AddSprite(spriteName, size, offset, uvSize, uvPosition, rotated.Value());
    }
    
    _Texture = pool->GetResource<TextureResource>(filename.substr(0, filename.find_last_of('/')) + "/images" + filename.substr(filename.find_last_of('/')) + modifier + ".jpa");
    
    return kResourceErrorNone;
}
