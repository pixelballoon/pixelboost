#import "AppDelegate.h"

#include "command/manager.h"
#include "core.h"

@implementation AppDelegate

@synthesize window = _window;
@synthesize webView = _webView;

- (void)applicationDidFinishLaunching:(NSNotification *)aNotification
{
    _Core = new pixeleditor::Core();
    
    [self.webView setMaintainsBackForwardList:false];
    [self.webView setShouldCloseWithWindow:true];
    WebPreferences* preferences = [self.webView preferences];
    [preferences setUsesPageCache:false];
    [preferences setCacheModel:WebCacheModelDocumentBrowser];
    [preferences setPrivateBrowsingEnabled:true];
    [self.webView setPreferences:preferences];
    
    [self.webView setMainFrameURL:@"http://localhost:9090"];
}

- (BOOL)applicationShouldTerminateAfterLastWindowClosed:(NSApplication *)sender
{
    return YES;
}

@end
