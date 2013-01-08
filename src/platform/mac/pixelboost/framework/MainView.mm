#include <OpenGL/gl.h>

#include "pixelboost/framework/engine.h"
#include "pixelboost/input/keyboardManager.h"
#include "pixelboost/input/mouseManager.h"
#include "pixelboost/input/touchManager.h"
#include "pixelboost/graphics/device/device.h"

#import "MainView.h"

float desiredFps = 60.f;
float speedupFactor = 3.f;

/* keycodes for keys that are independent of keyboard layout*/
enum {
    kVK_Return                    = 0x24,
    kVK_Tab                       = 0x30,
    kVK_Space                     = 0x31,
    kVK_Delete                    = 0x33,
    kVK_Escape                    = 0x35,
    kVK_Command                   = 0x37,
    kVK_Shift                     = 0x38,
    kVK_CapsLock                  = 0x39,
    kVK_Option                    = 0x3A,
    kVK_Control                   = 0x3B,
    kVK_RightShift                = 0x3C,
    kVK_RightOption               = 0x3D,
    kVK_RightControl              = 0x3E,
    kVK_Function                  = 0x3F,
    kVK_F17                       = 0x40,
    kVK_VolumeUp                  = 0x48,
    kVK_VolumeDown                = 0x49,
    kVK_Mute                      = 0x4A,
    kVK_F18                       = 0x4F,
    kVK_F19                       = 0x50,
    kVK_F20                       = 0x5A,
    kVK_F5                        = 0x60,
    kVK_F6                        = 0x61,
    kVK_F7                        = 0x62,
    kVK_F3                        = 0x63,
    kVK_F8                        = 0x64,
    kVK_F9                        = 0x65,
    kVK_F11                       = 0x67,
    kVK_F13                       = 0x69,
    kVK_F16                       = 0x6A,
    kVK_F14                       = 0x6B,
    kVK_F10                       = 0x6D,
    kVK_F12                       = 0x6F,
    kVK_F15                       = 0x71,
    kVK_Help                      = 0x72,
    kVK_Home                      = 0x73,
    kVK_PageUp                    = 0x74,
    kVK_ForwardDelete             = 0x75,
    kVK_F4                        = 0x76,
    kVK_End                       = 0x77,
    kVK_F2                        = 0x78,
    kVK_PageDown                  = 0x79,
    kVK_F1                        = 0x7A,
    kVK_LeftArrow                 = 0x7B,
    kVK_RightArrow                = 0x7C,
    kVK_DownArrow                 = 0x7D,
    kVK_UpArrow                   = 0x7E
};

@implementation MainView

- (id)initWithFrame:(NSRect)frame
{
    self = [super initWithFrame:frame];
    if (self) {
        // Initialization code here.
    }
    
    return self;
}

- (id)initWithCoder:(NSCoder *)decoder
{
    self = [super initWithCoder:decoder];
    if (self) {
        NSTrackingArea* trackingArea = [[NSTrackingArea alloc] initWithRect:[self bounds] options:(NSTrackingMouseMoved | NSTrackingActiveAlways | NSTrackingInVisibleRect) owner:self userInfo:nil];
        [self addTrackingArea:trackingArea];
        [trackingArea release];
        
        NSTimer* timer = [[NSTimer alloc] initWithFireDate:[NSDate date] interval:(1.0f/desiredFps) target:self selector:@selector(onRedrawTimer) userInfo:nil repeats:YES];
        [[NSRunLoop currentRunLoop] addTimer:timer forMode:NSDefaultRunLoopMode];
    }
    
    return self;
}

- (void)onRedrawTimer
{
    pb::Engine::Instance()->Update(1.f/(desiredFps/speedupFactor));
    [self setNeedsDisplay:true];
}

- (void)drawRect:(NSRect)dirtyRect
{
    if (!pb::Engine::Instance())
        return;

    pb::Engine::Instance()->Render();
    
    glFlush();
}

- (BOOL)acceptsFirstResponder
{
    return true;
}

- (void)update
{
    [super update];
}

- (void)reshape
{
    [super reshape];
    
    pb::GraphicsDevice::Instance()->SetDisplayDensity(32.f);
    pb::GraphicsDevice::Instance()->SetDisplayResolution(glm::vec2(self.frame.size.width, self.frame.size.height));
    
    [self setNeedsDisplay:true];
}

- (void)mouseMoved:(NSEvent *)event
{
    if (!pb::Engine::Instance())
        return;
    
    NSPoint eventLocation = [self convertPoint:event.locationInWindow fromView:nil];
    
    pb::Engine::Instance()->GetMouseManager()->OnMouseMove(glm::vec2(eventLocation.x, self.frame.size.height-eventLocation.y));
}

- (void)mouseDown:(NSEvent *)event
{
    if (!pb::Engine::Instance())
        return;
    
    NSPoint eventLocation = [self convertPoint:event.locationInWindow fromView:nil];
    
    pb::ModifierKeys modifiers = 0;
    
    if ([event modifierFlags] & NSControlKeyMask)
        modifiers |= pb::kModifierCtrl;
    
    if ([event modifierFlags] & NSCommandKeyMask)
        modifiers |= pb::kModifierCtrl;
    
    if ([event modifierFlags] & NSShiftKeyMask)
        modifiers |= pb::kModifierShift;
    
    if ([event modifierFlags] & NSAlternateKeyMask)
        modifiers |= pb::kModifierAlt;
    
    pb::Engine::Instance()->GetTouchManager()->OnTouchDown(0, glm::vec2(eventLocation.x, self.frame.size.height-eventLocation.y));
    pb::Engine::Instance()->GetMouseManager()->OnMouseDown(pb::kMouseButtonLeft, modifiers, glm::vec2(eventLocation.x, self.frame.size.height-eventLocation.y));
}

- (void)mouseDragged:(NSEvent *)event
{
    if (!pb::Engine::Instance())
        return;
    
    NSPoint eventLocation = [self convertPoint:event.locationInWindow fromView:nil];
    
    pb::Engine::Instance()->GetTouchManager()->OnTouchMove(0, glm::vec2(eventLocation.x, self.frame.size.height-eventLocation.y));
    pb::Engine::Instance()->GetMouseManager()->OnMouseMove(glm::vec2(eventLocation.x, self.frame.size.height-eventLocation.y));
}

- (void)mouseUp:(NSEvent *)event
{
    if (!pb::Engine::Instance())
        return;
    
    NSPoint eventLocation = [self convertPoint:event.locationInWindow fromView:nil];
    
    pb::ModifierKeys modifiers = 0;
    
    if ([event modifierFlags] & NSControlKeyMask)
        modifiers |= pb::kModifierCtrl;
    
    if ([event modifierFlags] & NSCommandKeyMask)
        modifiers |= pb::kModifierCtrl;
    
    if ([event modifierFlags] & NSShiftKeyMask)
        modifiers |= pb::kModifierShift;
    
    if ([event modifierFlags] & NSAlternateKeyMask)
        modifiers |= pb::kModifierAlt;
    
    pb::Engine::Instance()->GetTouchManager()->OnTouchUp(0, glm::vec2(eventLocation.x, self.frame.size.height-eventLocation.y));
    pb::Engine::Instance()->GetMouseManager()->OnMouseUp(pb::kMouseButtonLeft, modifiers, glm::vec2(eventLocation.x, self.frame.size.height-eventLocation.y));
}

- (void)rightMouseDown:(NSEvent *)event
{
    if (!pb::Engine::Instance())
        return;
    
    NSPoint eventLocation = [self convertPoint:event.locationInWindow fromView:nil];
    
    pb::ModifierKeys modifiers = 0;
    
    if ([event modifierFlags] & NSControlKeyMask)
        modifiers |= pb::kModifierCtrl;
    
    if ([event modifierFlags] & NSCommandKeyMask)
        modifiers |= pb::kModifierCtrl;
    
    if ([event modifierFlags] & NSShiftKeyMask)
        modifiers |= pb::kModifierShift;
    
    if ([event modifierFlags] & NSAlternateKeyMask)
        modifiers |= pb::kModifierAlt;
    
    pb::Engine::Instance()->GetMouseManager()->OnMouseDown(pb::kMouseButtonRight, modifiers, glm::vec2(eventLocation.x, self.frame.size.height-eventLocation.y));
}

- (void)rightMouseDragged:(NSEvent *)event
{
    if (!pb::Engine::Instance())
        return;
    
    NSPoint eventLocation = [self convertPoint:event.locationInWindow fromView:nil];
    
    pb::Engine::Instance()->GetMouseManager()->OnMouseMove(glm::vec2(eventLocation.x, self.frame.size.height-eventLocation.y));
}

- (void)rightMouseUp:(NSEvent *)event
{
    if (!pb::Engine::Instance())
        return;
    
    NSPoint eventLocation = [self convertPoint:event.locationInWindow fromView:nil];
    
    pb::ModifierKeys modifiers = 0;
    
    if ([event modifierFlags] & NSControlKeyMask)
        modifiers |= pb::kModifierCtrl;
    
    if ([event modifierFlags] & NSCommandKeyMask)
        modifiers |= pb::kModifierCtrl;
    
    if ([event modifierFlags] & NSShiftKeyMask)
        modifiers |= pb::kModifierShift;
    
    if ([event modifierFlags] & NSAlternateKeyMask)
        modifiers |= pb::kModifierAlt;
    
    pb::Engine::Instance()->GetMouseManager()->OnMouseUp(pb::kMouseButtonRight, modifiers, glm::vec2(eventLocation.x, self.frame.size.height-eventLocation.y));
}

- (void)scrollWheel:(NSEvent *)event
{
    if (!pb::Engine::Instance())
        return;
    
    pb::ModifierKeys modifiers = 0;
    
    if ([event modifierFlags] & NSControlKeyMask)
        modifiers |= pb::kModifierCtrl;
    
    if ([event modifierFlags] & NSCommandKeyMask)
        modifiers |= pb::kModifierCtrl;
    
    if ([event modifierFlags] & NSShiftKeyMask)
        modifiers |= pb::kModifierShift;
    
    if ([event modifierFlags] & NSAlternateKeyMask)
        modifiers |= pb::kModifierAlt;
    
    pb::Engine::Instance()->GetMouseManager()->OnMouseScroll(modifiers, glm::vec2(-event.scrollingDeltaX, event.scrollingDeltaY));
}

- (void)magnifyWithEvent:(NSEvent *)event
{
    if (!pb::Engine::Instance())
        return;
    
    pb::Engine::Instance()->GetMouseManager()->OnMouseZoom(glm::vec2(event.magnification, event.magnification));
}

- (void)keyDown:(NSEvent *)event
{
    if (!pb::Engine::Instance())
        return;
    
    switch (event.keyCode)
    {
        case kVK_Return:
            pb::Engine::Instance()->GetKeyboardManager()->OnKeyDown(pb::kKeyboardKeyReturn);
            break;
            
        case kVK_Delete:
            pb::Engine::Instance()->GetKeyboardManager()->OnKeyDown(pb::kKeyboardKeyBackspace);
            break;
            
        case kVK_ForwardDelete:
            pb::Engine::Instance()->GetKeyboardManager()->OnKeyDown(pb::kKeyboardKeyDelete);
            break;
            
        case kVK_LeftArrow:
            pb::Engine::Instance()->GetKeyboardManager()->OnKeyDown(pb::kKeyboardKeyLeft);
            break;
            
        case kVK_RightArrow:
            pb::Engine::Instance()->GetKeyboardManager()->OnKeyDown(pb::kKeyboardKeyRight);
            break;
            
        case kVK_Shift:
            pb::Engine::Instance()->GetKeyboardManager()->OnKeyDown(pb::kKeyboardKeyShift);
            break;
            
        case kVK_Tab:
            pb::Engine::Instance()->GetKeyboardManager()->OnKeyDown(pb::kKeyboardKeyTab);
            break;
            
        case kVK_Home:
            pb::Engine::Instance()->GetKeyboardManager()->OnKeyDown(pb::kKeyboardKeyHome);
            break;
            
        case kVK_End:
            pb::Engine::Instance()->GetKeyboardManager()->OnKeyDown(pb::kKeyboardKeyEnd);
            break;
            
        case kVK_Control:
            pb::Engine::Instance()->GetKeyboardManager()->OnKeyDown(pb::kKeyboardKeyControl);
            break;
            
        case kVK_UpArrow:
            pb::Engine::Instance()->GetKeyboardManager()->OnKeyDown(pb::kKeyboardKeyUp);
            break;
            
        case kVK_DownArrow:
            pb::Engine::Instance()->GetKeyboardManager()->OnKeyDown(pb::kKeyboardKeyDown);
            break;
            
        case kVK_Escape:
            pb::Engine::Instance()->GetKeyboardManager()->OnKeyDown(pb::kKeyboardKeyEscape);
            break;
            
        case kVK_Option:
            pb::Engine::Instance()->GetKeyboardManager()->OnKeyDown(pb::kKeyboardKeyAlt);
            break;
            
        case kVK_Space:
            pb::Engine::Instance()->GetKeyboardManager()->OnKeyDown(pb::kKeyboardKeySpace);
            break;
            
        default:
            pb::Engine::Instance()->GetKeyboardManager()->OnKeyDown(pb::kKeyboardKeyCharacter, (int)[event.characters UTF8String][0]);
            break;
    }
}

- (void)keyUp:(NSEvent *)event
{
    if (!pb::Engine::Instance())
        return;
    
    switch (event.keyCode)
    {
        case kVK_Return:
            pb::Engine::Instance()->GetKeyboardManager()->OnKeyUp(pb::kKeyboardKeyReturn);
            break;
            
        case kVK_Delete:
            pb::Engine::Instance()->GetKeyboardManager()->OnKeyUp(pb::kKeyboardKeyBackspace);
            break;
            
        case kVK_ForwardDelete:
            pb::Engine::Instance()->GetKeyboardManager()->OnKeyUp(pb::kKeyboardKeyDelete);
            break;
            
        case kVK_LeftArrow:
            pb::Engine::Instance()->GetKeyboardManager()->OnKeyUp(pb::kKeyboardKeyLeft);
            break;
            
        case kVK_RightArrow:
            pb::Engine::Instance()->GetKeyboardManager()->OnKeyUp(pb::kKeyboardKeyRight);
            break;
            
        case kVK_Shift:
            pb::Engine::Instance()->GetKeyboardManager()->OnKeyUp(pb::kKeyboardKeyShift);
            break;
            
        case kVK_Tab:
            pb::Engine::Instance()->GetKeyboardManager()->OnKeyUp(pb::kKeyboardKeyTab);
            break;
            
        case kVK_Home:
            pb::Engine::Instance()->GetKeyboardManager()->OnKeyUp(pb::kKeyboardKeyHome);
            break;
            
        case kVK_End:
            pb::Engine::Instance()->GetKeyboardManager()->OnKeyUp(pb::kKeyboardKeyEnd);
            break;
            
        case kVK_Control:
            pb::Engine::Instance()->GetKeyboardManager()->OnKeyUp(pb::kKeyboardKeyControl);
            break;
            
        case kVK_UpArrow:
            pb::Engine::Instance()->GetKeyboardManager()->OnKeyUp(pb::kKeyboardKeyUp);
            break;
            
        case kVK_DownArrow:
            pb::Engine::Instance()->GetKeyboardManager()->OnKeyUp(pb::kKeyboardKeyDown);
            break;
            
        case kVK_Escape:
            pb::Engine::Instance()->GetKeyboardManager()->OnKeyUp(pb::kKeyboardKeyEscape);
            break;
            
        case kVK_Option:
            pb::Engine::Instance()->GetKeyboardManager()->OnKeyUp(pb::kKeyboardKeyAlt);
            break;
            
        case kVK_Space:
            pb::Engine::Instance()->GetKeyboardManager()->OnKeyUp(pb::kKeyboardKeySpace);
            break;
            
        default:
            pb::Engine::Instance()->GetKeyboardManager()->OnKeyUp(pb::kKeyboardKeyCharacter, (int)[event.characters UTF8String][0]);
            break;
    }
}

@end
