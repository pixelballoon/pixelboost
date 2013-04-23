#import <QuartzCore/QuartzCore.h>

#include <string>
#include <vector>

#include "pixelboost/framework/engine.h"
#include "pixelboost/graphics/device/device.h"
#include "pixelboost/input/touchManager.h"

#import "pixelboostAppDelegate.h"
#import "pixelboostViewController.h"
#import "EAGLView.h"

@interface pixelboostViewController ()
@property (nonatomic, retain) EAGLContext *context;
@property (nonatomic, assign) CADisplayLink *displayLink;
@end

@implementation pixelboostViewController

@synthesize animating, context, displayLink;
@synthesize app;

- (void)awakeFromNib
{
    app = pb::Engine::Create(self, 0, 0);
    
    EAGLContext *aContext = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES2];
    
    if (!aContext)
        NSLog(@"Failed to create ES context");
    else if (![EAGLContext setCurrentContext:aContext])
        NSLog(@"Failed to set ES context current");
    
	self.context = aContext;
	[aContext release];
    
    EAGLView* eaglView = (EAGLView *)self.view;
	
    [eaglView setContext:context];
    [eaglView setFramebuffer];
    
    animating = FALSE;
    animationFrameInterval = 2;
    self.displayLink = nil;
    
    float scale = 1.f;
    
    if([self.view respondsToSelector:@selector(setContentScaleFactor:)])
    {
        self.view.contentScaleFactor = [[UIScreen mainScreen] scale];
        scale = self.view.contentScaleFactor;
    }
    
    float displayDensity = 16.f;
    
    if ([[UIDevice currentDevice] userInterfaceIdiom] == UIUserInterfaceIdiomPad)
    {
        displayDensity = 32.f;
    }
    
    if (([[UIScreen mainScreen] respondsToSelector:@selector(scale)] == YES))
        displayDensity *= [[UIScreen mainScreen] scale];
    
    pb::GraphicsDevice::Instance()->SetDisplayDensity(displayDensity);
    
    GLfloat screenWidth = eaglView.framebufferWidth * scale;
    GLfloat screenHeight = eaglView.framebufferHeight * scale;
    
    if ([self shouldAutorotateToInterfaceOrientation:UIInterfaceOrientationPortrait])
    {
        pb::GraphicsDevice::Instance()->SetDisplayResolution(glm::vec2(screenWidth, screenHeight));
    } else
    {
        pb::GraphicsDevice::Instance()->SetDisplayResolution(glm::vec2(screenHeight, screenWidth));
    }
    
    app->Initialise();
}

- (void)dealloc
{ 
    delete app;
    
    // Tear down context.
    if ([EAGLContext currentContext] == context)
        [EAGLContext setCurrentContext:nil];
    
    [context release];
    
    [super dealloc];
}

- (void)didReceiveMemoryWarning
{
    // Releases the view if it doesn't have a superview.
    [super didReceiveMemoryWarning];
    
    // Release any cached data, images, etc. that aren't in use.
}

- (void)viewWillAppear:(BOOL)animated
{
    [self startAnimation];
    
    [super viewWillAppear:animated];
}

- (void)viewWillDisappear:(BOOL)animated
{
    [self stopAnimation];
    
    [super viewWillDisappear:animated];
}

- (void)viewDidUnload
{
	[super viewDidUnload];

    // Tear down context.
    if ([EAGLContext currentContext] == context)
        [EAGLContext setCurrentContext:nil];
	self.context = nil;	
}

- (NSInteger)animationFrameInterval
{
    return animationFrameInterval;
}

- (void)setAnimationFrameInterval:(NSInteger)frameInterval
{
    /*
	 Frame interval defines how many display frames must pass between each time the display link fires.
	 The display link will only fire 30 times a second when the frame internal is two on a display that refreshes 60 times a second. The default frame interval setting of one will fire 60 times a second when the display refreshes at 60 times a second. A frame interval setting of less than one results in undefined behavior.
	 */
    if (frameInterval >= 1) {
        animationFrameInterval = frameInterval;
        
        if (animating) {
            [self stopAnimation];
            [self startAnimation];
        }
    }
}

- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)toInterfaceOrientation
{
    if (app && app->IsLandscape())
    {
        return UIInterfaceOrientationIsLandscape(toInterfaceOrientation);
    } else
    {
        return UIInterfaceOrientationIsPortrait(toInterfaceOrientation);
    }
}

- (NSUInteger)supportedInterfaceOrientations
{
    return app->IsLandscape() ? UIInterfaceOrientationMaskLandscape : UIInterfaceOrientationMaskPortrait;
}

- (BOOL)shouldAutorotate
{
    return YES;
}

- (void)startAnimation
{
    if (!animating) {
        CADisplayLink *aDisplayLink;
        if ([[UIScreen mainScreen] respondsToSelector:@selector(displayLinkWithTarget:selector:)])
            aDisplayLink = [[UIScreen mainScreen] displayLinkWithTarget:self selector:@selector(drawFrame)];
        else
            aDisplayLink = [NSClassFromString(@"CADisplayLink") displayLinkWithTarget:self selector:@selector(drawFrame)];
        [aDisplayLink setFrameInterval:animationFrameInterval];
        [aDisplayLink addToRunLoop:[NSRunLoop currentRunLoop] forMode:NSDefaultRunLoopMode];
        self.displayLink = aDisplayLink;
        
        animating = TRUE;
    }
}

- (void)stopAnimation
{
    if (animating) {
        [self.displayLink invalidate];
        self.displayLink = nil;
        animating = FALSE;
    }
}

- (void)drawFrame
{
    pb::Engine::Instance()->Update(1/30.f, 1/30.f);
	
    [(EAGLView *)self.view setFramebuffer];
	
	pb::Engine::Instance()->Render();
    
    [(EAGLView *)self.view presentFramebuffer];
}

- (void)touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event
{
    pb::TouchManager* touchManager = pb::Engine::Instance()->GetTouchManager();
	
	for (UITouch* uiTouch in touches)
	{
		CGPoint pt = [uiTouch locationInView:self.view];
        
        if (([[UIScreen mainScreen] respondsToSelector:@selector(scale)] == YES))
        {
            pt.x *= [[UIScreen mainScreen] scale];
            pt.y *= [[UIScreen mainScreen] scale];
        }

		touchManager->OnTouchDown((int)uiTouch, glm::vec2(pt.x, pt.y));
    }
}

- (void)touchesMoved:(NSSet *)touches withEvent:(UIEvent *)event
{
	pb::TouchManager* touchManager = pb::Engine::Instance()->GetTouchManager();
	
	for (UITouch* uiTouch in touches)
	{
		CGPoint pt = [uiTouch locationInView:self.view];
        
        if (([[UIScreen mainScreen] respondsToSelector:@selector(scale)] == YES))
        {
            pt.x *= [[UIScreen mainScreen] scale];
            pt.y *= [[UIScreen mainScreen] scale];
        }
        
		touchManager->OnTouchMove((int)uiTouch, glm::vec2(pt.x, pt.y));
    }	
}

- (void)touchesEnded:(NSSet *)touches withEvent:(UIEvent *)event
{
	pb::TouchManager* touchManager = pb::Engine::Instance()->GetTouchManager();
	
	for (UITouch* uiTouch in touches)
	{
        CGPoint pt = [uiTouch locationInView:self.view];
        
        if (([[UIScreen mainScreen] respondsToSelector:@selector(scale)] == YES))
        {
            pt.x *= [[UIScreen mainScreen] scale];
            pt.y *= [[UIScreen mainScreen] scale];
        }
        
		touchManager->OnTouchUp((int)uiTouch, glm::vec2(pt.x, pt.y));
	}
}

- (void)touchesCancelled:(NSSet *)touches withEvent:(UIEvent *)event
{
	pb::TouchManager* touchManager = pb::Engine::Instance()->GetTouchManager();
	
	for (UITouch* uiTouch in touches)
	{
        CGPoint pt = [uiTouch locationInView:self.view];
        
        if (([[UIScreen mainScreen] respondsToSelector:@selector(scale)] == YES))
        {
            pt.x *= [[UIScreen mainScreen] scale];
            pt.y *= [[UIScreen mainScreen] scale];
        }
        
		touchManager->OnTouchUp((int)uiTouch, glm::vec2(pt.x, pt.y));
    }
}

@end
