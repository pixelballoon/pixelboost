//
//  MainView.m
//  editor
//
//  Created by Jamie Hales on 19/05/2012.
//  Copyright (c) 2012 Pixel Balloon. All rights reserved.
//

#include <OpenGL/gl.h>

#include "pixelboost/input/keyboardManager.h"
#include "pixelboost/input/mouseManager.h"
#include "pixelboost/graphics/render/primitive/primitiveRenderer.h"

#include "view.h"

#import "MainView.h"

using namespace pixeleditor;

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
        NSTrackingArea* trackingArea = [[NSTrackingArea alloc] initWithRect:[self bounds] options:(NSTrackingMouseMoved | NSTrackingActiveAlways) owner:self userInfo:nil];
        [self addTrackingArea:trackingArea];
        [trackingArea release];
    }
    
    return self;
}

- (void)drawRect:(NSRect)dirtyRect
{
    View* view = View::Instance();
    
    glViewport(0.f, 0.f, self.frame.size.width, self.frame.size.height);
    
    glClearColor(0.5, 0.5, 0.5, 0.5);
    glClear(GL_COLOR_BUFFER_BIT);
    
    if (view)
    {
        view->Update(1.f/30.f);
        view->Render();
    }

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
    
    View* view = View::Instance();
    
    if (view)
    {
        View::Instance()->SetCanvasSize(Vec2(self.frame.size.width, self.frame.size.height));
    }
    
    [self setNeedsDisplay:true];
}

- (void)mouseMoved:(NSEvent *)event
{
    View* view = View::Instance();
    
    if (view)
    {
        NSPoint eventLocation = [self convertPoint:event.locationInWindow fromView:nil];
        
        view->GetMouseManager()->OnMouseMove(glm::vec2(eventLocation.x, self.frame.size.height-eventLocation.y));
    }
    
    [self setNeedsDisplay:true];
}

- (void)mouseDown:(NSEvent *)event
{
    View* view = View::Instance();
    
    if (view)
    {
        NSPoint eventLocation = [self convertPoint:event.locationInWindow fromView:nil];
        
        view->GetMouseManager()->OnMouseDown(pb::kMouseButtonLeft, glm::vec2(eventLocation.x, self.frame.size.height-eventLocation.y));
    }
    
    [self setNeedsDisplay:true];
}

- (void)mouseDragged:(NSEvent *)event
{
    View* view = View::Instance();
    
    if (view)
    {
        NSPoint eventLocation = [self convertPoint:event.locationInWindow fromView:nil];
        
        view->GetMouseManager()->OnMouseMove(glm::vec2(eventLocation.x, self.frame.size.height-eventLocation.y));
    }
    
    [self setNeedsDisplay:true];
}

- (void)mouseUp:(NSEvent *)event
{
    View* view = View::Instance();
    
    if (view)
    {
        NSPoint eventLocation = [self convertPoint:event.locationInWindow fromView:nil];
        
        view->GetMouseManager()->OnMouseUp(pb::kMouseButtonLeft, glm::vec2(eventLocation.x, self.frame.size.height-eventLocation.y));
    }
    
    [self setNeedsDisplay:true];
}

- (void)rightMouseDown:(NSEvent *)event
{
    View* view = View::Instance();
    
    if (view)
    {
        NSPoint eventLocation = [self convertPoint:event.locationInWindow fromView:nil];
        
        view->GetMouseManager()->OnMouseDown(pb::kMouseButtonRight, glm::vec2(eventLocation.x, self.frame.size.height-eventLocation.y));
    }
    
    [self setNeedsDisplay:true];
}

- (void)rightMouseDragged:(NSEvent *)event
{
    View* view = View::Instance();
    
    if (view)
    {
        NSPoint eventLocation = [self convertPoint:event.locationInWindow fromView:nil];
        
        view->GetMouseManager()->OnMouseMove(glm::vec2(eventLocation.x, self.frame.size.height-eventLocation.y));
    }
    
    [self setNeedsDisplay:true];
}

- (void)rightMouseUp:(NSEvent *)event
{
    View* view = View::Instance();
    
    if (view)
    {
        NSPoint eventLocation = [self convertPoint:event.locationInWindow fromView:nil];
        
        view->GetMouseManager()->OnMouseUp(pb::kMouseButtonRight, glm::vec2(eventLocation.x, self.frame.size.height-eventLocation.y));
    }
    
    [self setNeedsDisplay:true];
}

- (void)scrollWheel:(NSEvent *)event
{
    View* view = View::Instance();
    
    if (view)
    {
        view->GetMouseManager()->OnMouseScroll(glm::vec2(-event.scrollingDeltaX, event.scrollingDeltaY));
    }
    
    [self setNeedsDisplay:true];
}

- (void)magnifyWithEvent:(NSEvent *)event
{
    View* view = View::Instance();
    
    if (view)
    {
        view->GetMouseManager()->OnMouseZoom(glm::vec2(event.magnification, event.magnification));
    }
    
    [self setNeedsDisplay:true];
}

- (void)keyDown:(NSEvent *)event
{
    View* view = View::Instance();
    
    if (view)
    {
        switch (event.keyCode)
        {
            case kVK_Return:
                view->GetKeyboardManager()->OnKeyDown(pb::kKeyboardKeyReturn);
                break;
                
            case kVK_Delete:
                view->GetKeyboardManager()->OnKeyDown(pb::kKeyboardKeyBackspace);
                break;
                
            case kVK_ForwardDelete:
                view->GetKeyboardManager()->OnKeyDown(pb::kKeyboardKeyDelete);
                break;
                
            case kVK_LeftArrow:
                view->GetKeyboardManager()->OnKeyDown(pb::kKeyboardKeyLeft);
                break;
                
            case kVK_RightArrow:
                view->GetKeyboardManager()->OnKeyDown(pb::kKeyboardKeyRight);
                break;
                
            case kVK_Shift:
                view->GetKeyboardManager()->OnKeyDown(pb::kKeyboardKeyShift);
                break;
                
            case kVK_Tab:
                view->GetKeyboardManager()->OnKeyDown(pb::kKeyboardKeyTab);
                break;
                
            case kVK_Home:
                view->GetKeyboardManager()->OnKeyDown(pb::kKeyboardKeyHome);
                break;
                
            case kVK_End:
                view->GetKeyboardManager()->OnKeyDown(pb::kKeyboardKeyEnd);
                break;
                
            case kVK_Control:
                view->GetKeyboardManager()->OnKeyDown(pb::kKeyboardKeyControl);
                break;
                
            case kVK_UpArrow:
                view->GetKeyboardManager()->OnKeyDown(pb::kKeyboardKeyUp);
                break;
                
            case kVK_DownArrow:
                view->GetKeyboardManager()->OnKeyDown(pb::kKeyboardKeyDown);
                break;
                
            case kVK_Escape:
                view->GetKeyboardManager()->OnKeyDown(pb::kKeyboardKeyEscape);
                break;
                
            case kVK_Option:
                view->GetKeyboardManager()->OnKeyDown(pb::kKeyboardKeyAlt);
                break;
                
            default:
                view->GetKeyboardManager()->OnKeyDown(pb::kKeyboardKeyCharacter, (int)[event.characters UTF8String][0]);
                break;
        }
    }
    
    [self setNeedsDisplay:true];
}

- (void)keyUp:(NSEvent *)event
{
    View* view = View::Instance();
    
    if (view)
    {
        switch (event.keyCode)
        {
            case kVK_Return:
                view->GetKeyboardManager()->OnKeyUp(pb::kKeyboardKeyReturn);
                break;
                
            case kVK_Delete:
                view->GetKeyboardManager()->OnKeyUp(pb::kKeyboardKeyBackspace);
                break;
                
            case kVK_ForwardDelete:
                view->GetKeyboardManager()->OnKeyUp(pb::kKeyboardKeyDelete);
                break;
                
            case kVK_LeftArrow:
                view->GetKeyboardManager()->OnKeyUp(pb::kKeyboardKeyLeft);
                break;
                
            case kVK_RightArrow:
                view->GetKeyboardManager()->OnKeyUp(pb::kKeyboardKeyRight);
                break;
                
            case kVK_Shift:
                view->GetKeyboardManager()->OnKeyUp(pb::kKeyboardKeyShift);
                break;
                
            case kVK_Tab:
                view->GetKeyboardManager()->OnKeyUp(pb::kKeyboardKeyTab);
                break;
                
            case kVK_Home:
                view->GetKeyboardManager()->OnKeyUp(pb::kKeyboardKeyHome);
                break;
                
            case kVK_End:
                view->GetKeyboardManager()->OnKeyUp(pb::kKeyboardKeyEnd);
                break;
                
            case kVK_Control:
                view->GetKeyboardManager()->OnKeyUp(pb::kKeyboardKeyControl);
                break;
                
            case kVK_UpArrow:
                view->GetKeyboardManager()->OnKeyUp(pb::kKeyboardKeyUp);
                break;
                
            case kVK_DownArrow:
                view->GetKeyboardManager()->OnKeyUp(pb::kKeyboardKeyDown);
                break;
                
            case kVK_Escape:
                view->GetKeyboardManager()->OnKeyUp(pb::kKeyboardKeyEscape);
                break;
                
            case kVK_Option:
                view->GetKeyboardManager()->OnKeyUp(pb::kKeyboardKeyAlt);
                break;
                
            default:
                view->GetKeyboardManager()->OnKeyUp(pb::kKeyboardKeyCharacter, (int)[event.characters UTF8String][0]);
                break;
        }
    }
    
    [self setNeedsDisplay:true];
}

@end
