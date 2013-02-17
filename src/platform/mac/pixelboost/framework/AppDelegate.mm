#import "AppDelegate.h"

#include <string>
#include <vector>

#include "pixelboost/framework/engine.h"

@implementation AppDelegate

@synthesize window = _window;
@synthesize webView = _webView;

- (void)applicationWillFinishLaunching:(NSNotification *)notification
{
    NSArray *args = [[NSProcessInfo processInfo] arguments];
    int argc = [args count];
    const char** argv;
    if (argc)
    {
        argv = new const char*[argc];
        for (int i=0; i<argc; i++)
        {
            NSString* arg = [args objectAtIndex:i];
            argv[i] = [arg UTF8String];
        }
    }
    
    _Engine = pb::Engine::Create(0, argc, argv);
    _Engine->Initialise();
}

- (void)applicationDidFinishLaunching:(NSNotification *)aNotification
{
    
}

- (BOOL)applicationShouldTerminateAfterLastWindowClosed:(NSApplication *)sender
{
    return YES;
}

@end
