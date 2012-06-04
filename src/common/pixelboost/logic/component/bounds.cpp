#include "pixelboost/logic/component/bounds.h"

using namespace pb;

BoundsComponent::BoundsComponent()
{
    
}

BoundsComponent::~BoundsComponent()
{
    
}

Uid BoundsComponent::GetType()
{
    return TypeHash("bounds");
}
