//
//  MainView.h
//  editor
//
//  Created by Jamie Hales on 19/05/2012.
//  Copyright (c) 2012 Pixel Balloon. All rights reserved.
//

#import <Cocoa/Cocoa.h>

@interface MainView : NSOpenGLView
- (void)update;		// moved or resized
- (void)reshape;	// scrolled, moved or resized

- (void)mouseDown:(NSEvent *)event;
- (void)mouseDragged:(NSEvent *)event;
- (void)mouseUp:(NSEvent *)event;
- (void)mouseMoved:(NSEvent *)event;
- (void)scrollWheel:(NSEvent *)event;
- (void)magnifyWithEvent:(NSEvent *)event;
@end
