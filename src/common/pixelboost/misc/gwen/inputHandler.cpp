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

bool GwenInputHandler::OnKeyboardEvent(KeyboardEvent event)
{
    switch (event.Type)
    {
        case KeyboardEvent::kKeyboardEventDown:
        {
            if (event.Modifier == kModifierKeyControl)
                return false;
            
            if (Gwen::KeyboardFocus == _Root)
                return false;
            
            if (event.Key == kKeyboardKeyCharacter)
                return _Canvas->InputCharacter(event.Character);
            else if (event.Key == kKeyboardKeySpace)
                return _Canvas->InputCharacter(' ');
            else
                return _Canvas->InputKey(event.Key, true);
            
            return false;
        }
            
        case KeyboardEvent::kKeyboardEventUp:
        {
            if (event.Modifier == kModifierKeyControl)
                return false;
            
            if (Gwen::KeyboardFocus == _Root)
                return false;
            
            if (event.Key != kKeyboardKeyCharacter && event.Key != kKeyboardKeySpace)
                return _Canvas->InputKey(event.Key, false);
                    
            return false;
        }
    }
    return false;
}

bool GwenInputHandler::OnMouseEvent(MouseEvent event)
{
    switch (event.Type)
    {
        case MouseEvent::kMouseEventDown:
        {
            _Canvas->InputMouseButton((int)event.Down.Button, true);
            break;
        }
        case MouseEvent::kMouseEventUp:
        {
            _Canvas->InputMouseButton((int)event.Up.Button, false);
            break;
        }
        case MouseEvent::kMouseEventMove:
        {
            glm::vec2 position(event.Move.Position[0], event.Move.Position[1]);
            
            glm::vec2 delta = position - _PrevMouse;
            _PrevMouse = position;
            
            _Canvas->InputMouseMoved(position.x, position.y, delta[0], delta[1]);
            break;
        }
        case MouseEvent::kMouseEventScroll:
        {
            _Canvas->InputMouseWheel(event.Scroll.Delta[1]*10);
            break;
        }
        default:
            break;
    }
    
    Gwen::Controls::Base* hoveredControl = Gwen::HoveredControl;
    if (!hoveredControl || hoveredControl == _Root)
        return false;
    
    return true;
}
    
#endif
