#pragma once

#include "project/definitions.h"

namespace pixeleditor
{
    struct UidComponents
    {
        Uid entity;
        Uid property;
        Uid component;
    };
    
    Uid GenerateUid(Uid entity, Uid property = 0, Uid component = 0);
    UidComponents SplitUid(Uid uid);
}
