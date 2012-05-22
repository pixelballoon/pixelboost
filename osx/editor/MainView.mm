//
//  MainView.m
//  editor
//
//  Created by Jamie Hales on 19/05/2012.
//  Copyright (c) 2012 Pixel Balloon. All rights reserved.
//

#import "MainView.h"

#include <OpenGL/gl.h>

#include "pixelboost/input/touchManager.h"
#include "pixelboost/graphics/render/primitive/primitiveRenderer.h"

#include "view.h"

using namespace pixeleditor;

@implementation MainView

- (id)initWithFrame:(NSRect)frame
{
    self = [super initWithFrame:frame];
    if (self) {
        // Initialization code here.
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

- (void)mouseDown:(NSEvent *)event
{
    View* view = View::Instance();
    
    if (view)
    {
        NSPoint eventLocation = [self convertPoint:event.locationInWindow fromView:nil];
        
        view->GetTouchManager()->AddTouch(0, Vec2(eventLocation.x, self.frame.size.height-eventLocation.y));
    }
    
    [self setNeedsDisplay:true];
}

- (void)mouseDragged:(NSEvent *)event
{
    View* view = View::Instance();
    
    if (view)
    {
        NSPoint eventLocation = [self convertPoint:event.locationInWindow fromView:nil];
        
        view->GetTouchManager()->UpdateTouch(0, Vec2(eventLocation.x, self.frame.size.height-eventLocation.y));
    }
    
    [self setNeedsDisplay:true];
}

- (void)mouseUp:(NSEvent *)event
{
    View* view = View::Instance();
    
    if (view)
    {
        view->GetTouchManager()->RemoveTouch(0);
    }
    
    [self setNeedsDisplay:true];
}

- (void)mouseMoved:(NSEvent *)event
{
    
}

- (void)scrollWheel:(NSEvent *)event
{
    View* view = View::Instance();
    
    if (view)
    {
        view->Scroll(Vec2(-event.scrollingDeltaX, event.scrollingDeltaY)/10.f);
    }
    
    [self setNeedsDisplay:true];
}

- (void)magnifyWithEvent:(NSEvent *)event
{
    View* view = View::Instance();
    
    if (view)
    {
        view->Zoom(event.magnification);
    }
    
    [self setNeedsDisplay:true];
}

@end
