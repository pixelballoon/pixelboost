#import "AppDelegate.h"

@implementation AppDelegate

@synthesize window = _window;

- (void)dealloc
{
    [super dealloc];
}

- (void)applicationDidFinishLaunching:(NSNotification *)aNotification
{
    // Insert code here to initialize your application
}

- (IBAction)onOpenFile:(id)sender
{
    NSOpenPanel* panel = [NSOpenPanel openPanel];
    
    [panel beginSheetModalForWindow:self.window completionHandler:nil];
}

@end
