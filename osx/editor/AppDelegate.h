#import <Cocoa/Cocoa.h>
#import <WebKit/WebKit.h>

namespace pixeleditor
{
    class Core;
    class View;
}

@interface AppDelegate : NSObject <NSApplicationDelegate>
{
    pixeleditor::Core* _Core;
    pixeleditor::View* _View;
}

@property (assign) IBOutlet NSWindow *window;
@property (assign) IBOutlet WebView *webView;

- (BOOL)applicationShouldTerminateAfterLastWindowClosed:(NSApplication *)sender;
- (IBAction)open:(id)sender;
- (IBAction)export:(id)sender;
- (IBAction)save:(id)sender;
- (IBAction)delete:(id)sender;

- (IBAction)copy:(id)sender;
- (IBAction)paste:(id)sender;

- (IBAction)sendToBack:(id)sender;
- (IBAction)bringToFront:(id)sender;

- (BOOL)application:(NSApplication *)theApplication openFile:(NSString *)filename;

@end
