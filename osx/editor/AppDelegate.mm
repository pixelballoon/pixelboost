#import "AppDelegate.h"

#include "command/manager.h"
#include "core.h"
#include "view.h"

@implementation AppDelegate

@synthesize window = _window;
@synthesize webView = _webView;

- (void)applicationWillFinishLaunching:(NSNotification *)notification
{
    _Core = new pixeleditor::Core();
    
    _View = new pixeleditor::View();
    _View->Initialise();
    _View->Render();
    
    [self.webView setMaintainsBackForwardList:false];
    [self.webView setShouldCloseWithWindow:true];
    WebPreferences* preferences = [self.webView preferences];
    [preferences setUsesPageCache:false];
    [preferences setCacheModel:WebCacheModelDocumentBrowser];
    [preferences setPrivateBrowsingEnabled:true];
    [self.webView setPreferences:preferences];
}

- (void)applicationDidFinishLaunching:(NSNotification *)aNotification
{
    [self.webView setMainFrameURL:@"http://localhost:9090"];
}

- (BOOL)applicationShouldTerminateAfterLastWindowClosed:(NSApplication *)sender
{
    return YES;
}

- (IBAction)open:(id)sender
{
    NSOpenPanel* openPanel = [[NSOpenPanel alloc] init];
    [openPanel setCanChooseFiles:false];
    [openPanel setCanChooseDirectories:true];
    
    if ([openPanel runModal] == NSFileHandlingPanelOKButton)
    {
        [self application:nil openFile:[[openPanel directoryURL] path]];
        [[NSDocumentController sharedDocumentController] noteNewRecentDocumentURL:[openPanel directoryURL]];
    }    
}

- (IBAction)export:(id)sender
{
    _Core->GetCommandManager()->Exec("export");
}

- (BOOL)application:(NSApplication *)theApplication openFile:(NSString *)filename
{
    if (_Core)
        _Core->GetCommandManager()->Exec("open", [filename UTF8String]);
    [self.webView reload:self];
    return true;
}

@end
