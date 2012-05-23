#include "core/uidHelpers.h"

namespace pixeleditor
{

SelectionUid GenerateSelectionUid(Uid entity, Uid property, Uid component)
{
    return (SelectionUid)(entity & 0xFFFFFF) | (SelectionUid)(property & 0x3FF) << 24 | (SelectionUid)(component & 0x3FFF) << 34;
}

UidComponents SplitSelectionUid(SelectionUid uid)
{
    UidComponents components;
    components.entity = uid & 0xFFFFFF; // 24 bits
    components.property = (uid >> 24) & 0x3FF; // 10 bits
    components.component = (uid >> 34) & 0x3FFF; // 14 bits
    return components;
}

}
