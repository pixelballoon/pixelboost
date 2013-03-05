#pragma once

#include "project/definitions.h"

struct UidComponents
{
    Uid entity;
    Uid property;
    Uid component;
};

SelectionUid GenerateSelectionUid(Uid entity, Uid property = 0, Uid component = 0);
UidComponents SplitSelectionUid(SelectionUid uid);
