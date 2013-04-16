#pragma once

#include <map>
#include <string>
#include <vector>

#include "glm/glm.hpp"

#include "pixelboost/animation/timeline/element.h"

namespace pb
{
       
    class Timeline : public TimelineElement
    {
    public:
        Timeline();
        ~Timeline();
        
    public:
        virtual float GetLength();
        
    public:
        void Insert(TimelineElement* element, float time);
        void Append(TimelineElement* element);
        
        void AddLabel(const std::string& name, float time);
        
    protected:
        virtual void OnUpdate(float time, float delta);
        
    private:
        bool _LengthDirty;
        float _Length;
        
        std::map<std::string, float> _Labels;
        std::vector<TimelineElement*> _Elements;
    };
}
