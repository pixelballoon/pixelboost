//
//  AppDelegate.h
//  DebugScope
//
//  Created by Jamie Hales on 04/04/2012.
//  Copyright (c) 2012 Pixel Balloon. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import <WebKit/WebKit.h>

@interface AppDelegate : NSObject <NSApplicationDelegate>

@property (assign) IBOutlet NSWindow *window;
@property (weak) IBOutlet WebView *webView;

@end
