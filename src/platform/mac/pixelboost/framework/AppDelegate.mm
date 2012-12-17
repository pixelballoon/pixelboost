#import "AppDelegate.h"

#include "pixelboost/framework/engine.h"

@implementation AppDelegate

@synthesize window = _window;
@synthesize webView = _webView;

- (void)applicationWillFinishLaunching:(NSNotification *)notification
{
    _App = pb::Engine::Create(0);
    _App->Initialise();
}

- (void)applicationDidFinishLaunching:(NSNotification *)aNotification
{
    
}

- (BOOL)applicationShouldTerminateAfterLastWindowClosed:(NSApplication *)sender
{
    return YES;
}

@end
