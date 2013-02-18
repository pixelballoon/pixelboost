#pragma once

#include <memory>

#define PB_RESOURCE_DECLARE(type) Resource* PbResourceCreate ## type () { return new type(); }
#define PB_RESOURCE_REGISTER(type) pb::ResourceManager::Instance()->RegisterResourceCreator(type::GetStaticResourceType(), &PbResourceCreate ## type );
