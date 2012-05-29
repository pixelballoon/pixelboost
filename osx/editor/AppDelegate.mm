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
    _View->Initialise(Vec2(1200,1000));
    _View->Render();
}

- (void)applicationDidFinishLaunching:(NSNotification *)aNotification
{
    
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

- (IBAction)networkExport:(id)sender
{
    _Core->GetCommandManager()->Exec("export", "-n");
}

- (IBAction)save:(id)sender
{
    _Core->GetCommandManager()->Exec("save");
}

- (IBAction)delete:(id)sender
{
    _Core->GetCommandManager()->Exec("delete");
}

- (BOOL)application:(NSApplication *)theApplication openFile:(NSString *)filename
{
    if (_Core)
        _Core->GetCommandManager()->Exec("open", [filename UTF8String]);
    [self.webView reload:self];
    return true;
}

@end
