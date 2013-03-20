#include "pixelboost/debug/assert.h"
#include "pixelboost/logic/message.h"

namespace pb
{

template <class T> const T& Message::As() const
{
    PbAssert(GetType() == T::GetStaticType());
        
    return static_cast<const T&>(*this);
}

}
