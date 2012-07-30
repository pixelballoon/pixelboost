#include "core/uidHelpers.h"

namespace pixeleditor
{

SelectionUid GenerateSelectionUid(Uid entity, Uid property, Uid component)
{
    return (SelectionUid)(entity & 0xFFFFFFFF) | (SelectionUid)(property & 0x3FF) << 32 | (SelectionUid)(component & 0x3FFF) << 42;
}

UidComponents SplitSelectionUid(SelectionUid uid)
{
    UidComponents components;
    components.entity = uid & 0xFFFFFFFF; // 32 bits
    components.property = (uid >> 32) & 0x3FF; // 10 bits
    components.component = (uid >> 42) & 0x3FFF; // 14 bits
    return components;
}

}
