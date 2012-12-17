#import <Cocoa/Cocoa.h>

@interface MainView : NSOpenGLView
- (BOOL)acceptsFirstResponder;

- (void)update;		// moved or resized
- (void)reshape;	// scrolled, moved or resized

- (void)mouseMoved:(NSEvent *)event;
- (void)mouseDown:(NSEvent *)event;
- (void)mouseDragged:(NSEvent *)event;
- (void)mouseUp:(NSEvent *)event;
- (void)rightMouseDown:(NSEvent *)event;
- (void)rightMouseDragged:(NSEvent *)event;
- (void)rightMouseUp:(NSEvent *)event;
- (void)scrollWheel:(NSEvent *)event;
- (void)magnifyWithEvent:(NSEvent *)event;
- (void)keyDown:(NSEvent *)event;
- (void)keyUp:(NSEvent *)event;
@end
