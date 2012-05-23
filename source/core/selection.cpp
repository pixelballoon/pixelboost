#include "core/selection.h"
#include "core/uidHelpers.h"

using namespace pixeleditor;

Selection::Selection()
{
    
}

Selection& Selection::operator=(const Selection& b)
{
    _Selection = b._Selection;
    selectionChanged(this);
    return *this;
}
    
void Selection::Clear()
{
    _Selection.clear();
}

void Selection::Add(Uid uid)
{
    Entities::iterator it = _Selection.find(uid);
    
    if (it == _Selection.end())
    {
        _Selection[uid] = Properties();
    }
}

void Selection::Remove(Uid uid)
{
    Entities::iterator it = _Selection.find(uid);
    
    if (it == _Selection.end())
        return;
    
    _Selection.erase(it);
}
    
bool Selection::IsSelected(Uid uid) const
{
    Entities::const_iterator it = _Selection.find(uid);
    
    if (it != _Selection.end())
        return true;
    
    return false;
}
    
const Selection::Entities& Selection::GetSelection() const
{
    return _Selection;
}
