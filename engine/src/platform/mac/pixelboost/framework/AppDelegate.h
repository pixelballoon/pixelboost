#import <Cocoa/Cocoa.h>
#import <WebKit/WebKit.h>

namespace pb
{
	class Engine;
}

@interface AppDelegate : NSObject <NSApplicationDelegate>
{
    pb::Engine* _Engine;
}

@property (assign) IBOutlet NSWindow *window;
@property (assign) IBOutlet WebView *webView;

- (BOOL)applicationShouldTerminateAfterLastWindowClosed:(NSApplication *)sender;

@end
