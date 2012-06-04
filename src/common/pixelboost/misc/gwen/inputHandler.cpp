#ifndef PIXELBOOST_DISABLE_GWEN

#include "glm/glm.hpp"

#include "gwen/Controls/Canvas.h"

#include "pixelboost/framework/game.h"
#include "pixelboost/input/keyboardManager.h"
#include "pixelboost/input/mouseManager.h"
#include "pixelboost/misc/gwen/inputHandler.h"

using namespace pb;

class pb::GwenKeyboardHandler : public KeyboardHandler
{
public:
    GwenKeyboardHandler(Gwen::Controls::Canvas* canvas, Gwen::Controls::Base* root)
        : _Canvas(canvas)
        , _Root(root)
    {
        
    }
    
    bool OnKeyDown(KeyboardKey key, char character)
    {
        if (key == kKeyboardKeyCharacter)
            _Canvas->InputCharacter(character);
        else
            _Canvas->InputKey(key, true);
        
        Gwen::Controls::Base* keyboardFocus = Gwen::KeyboardFocus;
        if (!keyboardFocus || keyboardFocus == _Root)
            return false;
        
        return true;
    }
    
    bool OnKeyUp(KeyboardKey key, char character)
    {
        if (key != kKeyboardKeyCharacter)
            _Canvas->InputKey(key, false);
        
        Gwen::Controls::Base* keyboardFocus = Gwen::KeyboardFocus;
        if (!keyboardFocus || keyboardFocus == _Root)
            return false;
        
        return true;
    }
    
    int GetPriority()
    {
        return 1000;
    }
    
private:
    Gwen::Controls::Base* _Root;
    Gwen::Controls::Canvas*	_Canvas;
};

class pb::GwenMouseHandler : public MouseHandler
{
public:
    GwenMouseHandler(Gwen::Controls::Canvas* canvas, Gwen::Controls::Base* root)
        : _Canvas(canvas)
        , _Root(root)
    {
        
    }
    
    int GetPriority()
    {
        return 1000;
    }
    
    bool OnMouseDown(MouseButton button, ModifierKeys modifierKeys, glm::vec2 position)
    {
        _Canvas->InputMouseButton((int)button, true);
        
        Gwen::Controls::Base* hoveredControl = Gwen::HoveredControl;
        if (!hoveredControl || hoveredControl == _Root)
            return false;
        
        return true;
    }
    
    bool OnMouseMove(glm::vec2 position)
    {
        glm::vec2 delta = position - _PrevMouse;
        _PrevMouse = position;
        
        _Canvas->InputMouseMoved(position.x, position.y, delta[0], delta[1]);
        
        Gwen::Controls::Base* hoveredControl = Gwen::HoveredControl;
        if (!hoveredControl || hoveredControl == _Root)
            return false;
        
        return true;
    }
    
    bool OnMouseUp(MouseButton button, ModifierKeys modifierKeys, glm::vec2 position)
    {
        _Canvas->InputMouseButton((int)button, false);
        
        Gwen::Controls::Base* hoveredControl = Gwen::HoveredControl;
        if (!hoveredControl || hoveredControl == _Root)
            return false;
        
        return true;
    }
    
    bool OnMouseScroll(ModifierKeys modifierKeys, glm::vec2 scroll)
    {
        _Canvas->InputMouseWheel(scroll.y*120);
        
        Gwen::Controls::Base* hoveredControl = Gwen::HoveredControl;
        if (!hoveredControl || hoveredControl == _Root)
            return false;
        
        return true;
    }
    
    bool OnMouseZoom(glm::vec2 zoom)
    {
        Gwen::Controls::Base* hoveredControl = Gwen::HoveredControl;
        if (!hoveredControl || hoveredControl == _Root)
            return false;
        
        return true;
    }
    
    bool OnMouseRotate(float rotate)
    {
        Gwen::Controls::Base* hoveredControl = Gwen::HoveredControl;
        if (!hoveredControl || hoveredControl == _Root)
            return false;
        
        return true;
    }
    
private:
    glm::vec2 _PrevMouse;
    
    Gwen::Controls::Base* _Root;
    Gwen::Controls::Canvas*	_Canvas;
};

GwenInputHandler::GwenInputHandler(Gwen::Controls::Canvas* canvas, Gwen::Controls::Base* root)
{
    _KeyboardHandler = new GwenKeyboardHandler(canvas, root);
    _MouseHandler = new GwenMouseHandler(canvas, root);
    
    pb::Game::Instance()->GetKeyboardManager()->AddHandler(_KeyboardHandler);
    pb::Game::Instance()->GetMouseManager()->AddHandler(_MouseHandler);
}

GwenInputHandler::~GwenInputHandler()
{
    pb::Game::Instance()->GetKeyboardManager()->RemoveHandler(_KeyboardHandler);
    pb::Game::Instance()->GetMouseManager()->RemoveHandler(_MouseHandler);
}

#endif
