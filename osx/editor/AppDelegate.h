#import <Cocoa/Cocoa.h>
#import <WebKit/WebKit.h>

namespace pixeleditor
{
    class Core;
}

@interface AppDelegate : NSObject <NSApplicationDelegate>
{
    pixeleditor::Core* _Core;
}

@property (assign) IBOutlet NSWindow *window;
@property (assign) IBOutlet WebView *webView;

- (BOOL)applicationShouldTerminateAfterLastWindowClosed:(NSApplication *)sender;

@end
