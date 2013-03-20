#include "core/selection.h"
#include "core/uidHelpers.h"

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

void Selection::AddEntity(SelectionUid uid)
{
    Entities::iterator it = _Selection.find(uid);
    
    if (it == _Selection.end())
    {
        _Selection[uid] = Properties();
    }
}

void Selection::RemoveEntity(SelectionUid uid)
{
    Entities::iterator it = _Selection.find(uid);
    
    if (it == _Selection.end())
        return;
    
    _Selection.erase(it);
}

void Selection::ToggleEntity(SelectionUid uid)
{
    Entities::iterator it = _Selection.find(uid);
    
    if (it == _Selection.end())
    {
        _Selection[uid] = Properties();
    } else {
        _Selection.erase(it);
    }
}
    
bool Selection::IsSelected(SelectionUid uid) const
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
