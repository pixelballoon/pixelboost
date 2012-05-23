#pragma once

#include <map>
#include <vector>

#include "sigslot/signal.h"

#include "project/definitions.h"

namespace pixeleditor
{

class Selection
{
public:
    Selection();
    
    Selection& operator=(const Selection& b);
    
    void Clear();
    
    void AddEntity(SelectionUid uid);
    void RemoveEntity(SelectionUid uid);
    void ToggleEntity(SelectionUid uid);
    
    bool IsSelected(SelectionUid id) const;
    
public:
    typedef std::vector<Uid> Components;
    typedef std::map<Uid, Components> Properties;
    typedef std::map<Uid, Properties> Entities;
    
    const Entities& GetSelection() const;
    
    sigslot::Signal1<const Selection*> selectionChanged;
    
private:
    Entities _Selection;
};

}
