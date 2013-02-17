#ifdef PIXELBOOST_LIBRARY_USE_GWEN

#include "glm/glm.hpp"

#include "gwen/Controls/Canvas.h"

#include "pixelboost/framework/engine.h"
#include "pixelboost/input/keyboardManager.h"
#include "pixelboost/input/mouseManager.h"
#include "pixelboost/misc/gwen/inputHandler.h"

using namespace pb;

GwenInputHandler::GwenInputHandler(Gwen::Controls::Canvas* canvas, Gwen::Controls::Base* root)
    : _Canvas(canvas)
    , _Root(root)
{
    Engine::Instance()->GetKeyboardManager()->AddHandler(this);
    Engine::Instance()->GetMouseManager()->AddHandler(this);
}

GwenInputHandler::~GwenInputHandler()
{
    Engine::Instance()->GetKeyboardManager()->RemoveHandler(this);
    Engine::Instance()->GetMouseManager()->RemoveHandler(this);
}

int GwenInputHandler::GetInputHandlerPriority()
{
    return 1000;
}

bool GwenInputHandler::OnKeyDown(KeyboardKey key, ModifierKeys modifier, char character)
{
    if (modifier == kModifierKeyControl)
        return false;
    
    if (Gwen::KeyboardFocus == _Root)
        return false;
    
    if (key == kKeyboardKeyCharacter)
        return _Canvas->InputCharacter(character);
    else if (key == kKeyboardKeySpace)
        return _Canvas->InputCharacter(' ');
    else
        return _Canvas->InputKey(key, true);
    
    return false;
}

bool GwenInputHandler::OnKeyUp(KeyboardKey key, ModifierKeys modifier, char character)
{
    if (modifier == kModifierKeyControl)
        return false;
    
    if (Gwen::KeyboardFocus == _Root)
        return false;
    
    if (key != kKeyboardKeyCharacter && key != kKeyboardKeySpace)
        return _Canvas->InputKey(key, false);
    
    return false;
}

bool GwenInputHandler::OnMouseDown(MouseButton button, ModifierKeys modifierKeys, glm::vec2 position)
{
    _Canvas->InputMouseButton((int)button, true);
    
    Gwen::Controls::Base* hoveredControl = Gwen::HoveredControl;
    if (!hoveredControl || hoveredControl == _Root)
        return false;
    
    return true;
}

bool GwenInputHandler::OnMouseMove(glm::vec2 position)
{
    glm::vec2 delta = position - _PrevMouse;
    _PrevMouse = position;
    
    _Canvas->InputMouseMoved(position.x, position.y, delta[0], delta[1]);
    
    Gwen::Controls::Base* hoveredControl = Gwen::HoveredControl;
    if (!hoveredControl || hoveredControl == _Root)
        return false;
    
    return true;
}

bool GwenInputHandler::OnMouseUp(MouseButton button, ModifierKeys modifierKeys, glm::vec2 position)
{
    _Canvas->InputMouseButton((int)button, false);
    
    Gwen::Controls::Base* hoveredControl = Gwen::HoveredControl;
    if (!hoveredControl || hoveredControl == _Root)
        return false;
    
    return true;
}

bool GwenInputHandler::OnMouseScroll(ModifierKeys modifierKeys, glm::vec2 scroll)
{
    _Canvas->InputMouseWheel(scroll.y*10);
    
    Gwen::Controls::Base* hoveredControl = Gwen::HoveredControl;
    if (!hoveredControl || hoveredControl == _Root)
        return false;
    
    return true;
}

bool GwenInputHandler::OnMouseZoom(glm::vec2 zoom)
{
    Gwen::Controls::Base* hoveredControl = Gwen::HoveredControl;
    if (!hoveredControl || hoveredControl == _Root)
        return false;
    
    return true;
}

bool GwenInputHandler::OnMouseRotate(float rotate)
{
    Gwen::Controls::Base* hoveredControl = Gwen::HoveredControl;
    if (!hoveredControl || hoveredControl == _Root)
        return false;
    
    return true;
}
    
#endif
