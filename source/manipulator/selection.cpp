#include "manipulator/selection.h"

using namespace pixeleditor;

Selection::Selection()
{
    
}
    
void Selection::Clear()
{
    _Selection.clear();
}

void Selection::AddEntity(Uid id)
{
    SelectionMap::iterator it = _Selection.find(id);
    
    if (it == _Selection.end())
    {
        _Selection[id] = Components();
    }
}

void Selection::RemoveEntity(Uid id)
{
    SelectionMap::iterator it = _Selection.find(id);
    
    if (it == _Selection.end())
        return;
    
    _Selection.erase(it);
}
    
bool Selection::IsEntitySelected(Uid id) const
{
    SelectionMap::const_iterator it = _Selection.find(id);
    
    if (it != _Selection.end())
        return true;
    
    return false;
}
    
const Selection::SelectionMap& Selection::GetSelection() const
{
    return _Selection;
}
