//
//  AppDelegate.m
//  DebugScope
//
//  Created by Jamie Hales on 04/04/2012.
//  Copyright (c) 2012 Pixel Balloon. All rights reserved.
//

#import "AppDelegate.h"

@implementation AppDelegate

@synthesize window = _window;
@synthesize webView = _webView;

- (void)applicationDidFinishLaunching:(NSNotification *)aNotification
{
    [self.webView setMaintainsBackForwardList:false];
    [self.webView setShouldCloseWithWindow:true];
    WebPreferences* preferences = [self.webView preferences];
    [preferences setUsesPageCache:false];
    [preferences setCacheModel:WebCacheModelDocumentBrowser];
    [preferences setPrivateBrowsingEnabled:true];
    [self.webView setPreferences:preferences];
    
    [self.webView setMainFrameURL:@"http://localhost:9091"];
}

@end
