#import "AppDelegate.h"

#include <string>
#include <vector>

#include "pixelboost/framework/engine.h"

@implementation AppDelegate

@synthesize window = _window;
@synthesize webView = _webView;

- (void)applicationWillFinishLaunching:(NSNotification *)notification
{
    std::vector<std::string> arguments;
    NSArray *args = [[NSProcessInfo processInfo] arguments];
    for (NSString* arg in args)
    {
        arguments.push_back([arg UTF8String]);
    }
    _App = pb::Engine::Create(0, arguments);
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
