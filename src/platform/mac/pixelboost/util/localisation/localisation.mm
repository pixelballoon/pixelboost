#include "pixelboost/util/localisation/localisation.h"

std::string pb::GetCurrentLocale()
{
    return [[[NSLocale preferredLanguages] objectAtIndex:0] UTF8String];
}
