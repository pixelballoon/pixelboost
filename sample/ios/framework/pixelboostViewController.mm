#import <Twitter/Twitter.h>
#import <QuartzCore/QuartzCore.h>

#import "pixelboostAppDelegate.h"
#import "pixelboostViewController.h"
#import "EAGLView.h"

#include "pixelboost/graphics/device/device.h"
#include "pixelboost/input/touchManager.h"

#include "game.h"

@interface pixelboostViewController ()
@property (nonatomic, retain) EAGLContext *context;
@property (nonatomic, assign) CADisplayLink *displayLink;
@end

@implementation pixelboostViewController

@synthesize animating, context, displayLink;
@synthesize game;

- (void)awakeFromNib
{
    EAGLContext *aContext = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES1];
    
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
    
    if([self.view respondsToSelector:@selector(setContentScaleFactor:)])
    {
        self.view.contentScaleFactor = [[UIScreen mainScreen] scale];
    }
    
    pb::GraphicsDevice::Instance()->SetDisplayResolution(glm::vec2(eaglView.framebufferWidth, eaglView.framebufferHeight));
    
    float displayDensity = 16.f;
    
    if (UI_USER_INTERFACE_IDIOM() == UIUserInterfaceIdiomPad)
    {
        displayDensity = 32.f;
    }
        
    if (([[UIScreen mainScreen] respondsToSelector:@selector(scale)] == YES && [[UIScreen mainScreen] scale] == 2.00))
        displayDensity *= 2.f;
    
    pb::GraphicsDevice::Instance()->SetDisplayDensity(displayDensity);
    
    game = new Game(self);
    game->Initialise();
}

- (void)dealloc
{ 
    delete game;
    
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
    if (game)
    {
        if (game->IsLandscape())
            return UIInterfaceOrientationIsLandscape(toInterfaceOrientation);   
        else
            return UIInterfaceOrientationIsPortrait(toInterfaceOrientation);
    }
    
    return true;
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
    game->Update(1/30.f);
	
    [(EAGLView *)self.view setFramebuffer];
	
	game->Render();
    
    [(EAGLView *)self.view presentFramebuffer];
}

- (void)touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event
{
    pb::TouchManager* touchManager = game->GetTouchManager();
	
	for (UITouch* uiTouch in touches)
	{
		CGPoint pt = [uiTouch locationInView:self.view];
		touchManager->AddTouch(uiTouch, glm::vec2(pt.x, pt.y));
    }
}

- (void)touchesMoved:(NSSet *)touches withEvent:(UIEvent *)event
{
	pb::TouchManager* touchManager = game->GetTouchManager();
	
	for (UITouch* uiTouch in touches)
	{
		CGPoint pt = [uiTouch locationInView:self.view];
		touchManager->UpdateTouch(uiTouch, glm::vec2(pt.x, pt.y));
    }	
}

- (void)touchesEnded:(NSSet *)touches withEvent:(UIEvent *)event
{
	pb::TouchManager* touchManager = game->GetTouchManager();
	
	for (UITouch* uiTouch in touches)
	{
		touchManager->RemoveTouch(uiTouch);
	}
}

- (void)touchesCancelled:(NSSet *)touches withEvent:(UIEvent *)event
{
	pb::TouchManager* touchManager = game->GetTouchManager();
	
	for (UITouch* uiTouch in touches)
	{
		touchManager->RemoveTouch(uiTouch);
    }
}

@end
