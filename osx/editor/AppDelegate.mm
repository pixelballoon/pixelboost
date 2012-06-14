#import "AppDelegate.h"

#include "command/manager.h"
#include "project/record.h"
#include "core.h"
#include "view.h"

@implementation AppDelegate

@synthesize window = _window;
@synthesize webView = _webView;

- (void)applicationWillFinishLaunching:(NSNotification *)notification
{
    _Core = new pixeleditor::Core();
    
    _View = new pixeleditor::View();
    _View->Initialise(glm::vec2(1200,1000));
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

- (IBAction)copy:(id)sender
{
    _Core->GetCommandManager()->Exec("copy");
}

- (IBAction)paste:(id)sender
{
    pixeleditor::Record* record = _View->GetRecord();
    
    char params[32];
    sprintf(params, "-r %d", record ? record->GetUid() : 0);
    _Core->GetCommandManager()->Exec("paste", params);
}

- (IBAction)sendToBack:(id)sender
{
    _Core->GetCommandManager()->Exec("modifyDepth", "-b");
}

- (IBAction)bringToFront:(id)sender
{
    _Core->GetCommandManager()->Exec("modifyDepth", "-f");
}

- (BOOL)application:(NSApplication *)theApplication openFile:(NSString *)filename
{
    if (_Core)
        _Core->GetCommandManager()->Exec("open", [filename UTF8String]);
    [self.webView reload:self];
    return true;
}

@end
