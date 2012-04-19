#pragma once

#include <map>
#include <vector>

#include "project/definitions.h"

namespace pixeleditor
{

class Selection
{
public:
    Selection();
    
    void Clear();
    
    void AddEntity(Uid id);
    void RemoveEntity(Uid id);
    
    bool IsEntitySelected(Uid id) const;
    
    typedef std::vector<Uid> Components;
    typedef std::map<Uid, Components> SelectionMap;
    
    const SelectionMap& GetSelection() const;
    
private:
    SelectionMap _Selection;
};

}
