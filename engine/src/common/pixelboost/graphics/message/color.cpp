#include "pixelboost/graphics/message/color.h"

using namespace pb;

PB_DEFINE_MESSAGE(pb::SetColorMessage)

SetColorMessage::SetColorMessage(glm::vec4 color)
    : Message(0,0)
{
    _Color = color;
}

glm::vec4 SetColorMessage::GetColor()
{
    return _Color;
}
