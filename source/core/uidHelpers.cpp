#include "core/uidHelpers.h"

namespace pixeleditor
{

Uid GenerateUid(Uid entity, Uid property, Uid component)
{
    return (entity & 0xFFF) | (property & 0xFF) << 12 | (component & 0xFFF) << 20;
}

UidComponents SplitUid(Uid uid)
{
    UidComponents components;
    components.entity = uid & 0xFFF;
    components.property = (uid >> 12) & 0xFF;
    components.component = (uid >> 20) & 0xFFF;
    return components;
}

}
