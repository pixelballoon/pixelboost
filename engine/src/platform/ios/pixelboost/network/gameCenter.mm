#ifdef PIXELBOOST_PLATFORM_IOS

#include "sys/sysctl.h"

#include "pixelboost/framework/engine.h"
#include "pixelboost/network/gameCenter.h"
#include "pixelboost/network/networkMessage.h"

#import "GameKit/GameKit.h"

@interface MatchDelegate : NSObject<GKMatchDelegate> {
@private
    bool matchStarted;
    GKMatch* currentMatch;
    pb::GameCenterDelegate* delegate;
}

@property (assign) bool matchStarted;
@property (retain) GKMatch* currentMatch;
@property (assign) pb::GameCenterDelegate* delegate;

- (void)disconnect;
- (void)startMatch;
- (void)stopMatch;

@end

@implementation MatchDelegate

@synthesize matchStarted;
@synthesize currentMatch;
@synthesize delegate;

- (void)match:(GKMatch *)match didReceiveData:(NSData *)data fromPlayer:(NSString *)playerID
{
    if (delegate)
    {
        pb::NetworkMessage msg;
        msg.SetData(data.length-4, ((char*)data.bytes)+4);
        delegate->ReceiveMessage(msg);
    }
}

- (void)match:(GKMatch *)match player:(NSString *)playerID didChangeState:(GKPlayerConnectionState)state
{
    switch (state)
    {
        case GKPlayerStateConnected:
            if (delegate)
            {
                delegate->PlayerConnected([playerID UTF8String]);
            }
            break;
        case GKPlayerStateDisconnected:
            if (delegate)
            {
                delegate->PlayerDisconnected([playerID UTF8String]);
                
                [self disconnect];
            }
            break;
    }
    
    if (!self.matchStarted && match.expectedPlayerCount == 0 && [match.playerIDs count] == 1)
    {
        [self startMatch];
    }
}

- (void)match:(GKMatch *)match connectionWithPlayerFailed:(NSString *)playerID withError:(NSError *)error;
{
    
}

- (void)match:(GKMatch *)match didFailWithError:(NSError *)error
{
    
}

- (void)disconnect
{
    [currentMatch disconnect];
    [self stopMatch];
}

- (void)startMatch
{
    if (self.matchStarted == NO)
    {
        self.matchStarted = YES;
        if (delegate)
            delegate->MatchStarted();
    }
}

- (void)stopMatch
{
    if (self.matchStarted == YES)
    {
        self.matchStarted = NO;
        if (delegate)
            delegate->MatchStopped();
    }
}

@end

@interface GameCenterDelegate : NSObject<GKMatchmakerViewControllerDelegate> {
@private
    pb::GameCenter* gameCenter;
    GKMatchmakerViewController* matchMaker;
    MatchDelegate* matchDelegate;
}

@property (assign) pb::GameCenter* gameCenter;
@property (assign) GKMatchmakerViewController* matchMaker;
@property (retain) MatchDelegate* matchDelegate;

@end

@implementation GameCenterDelegate

@synthesize gameCenter;
@synthesize matchMaker;
@synthesize matchDelegate;

- (void)matchmakerViewControllerWasCancelled:(GKMatchmakerViewController *)viewController
{
    [viewController dismissModalViewControllerAnimated:YES];
    
    // implement any specific code in your application here.
}

- (void)matchmakerViewController:(GKMatchmakerViewController *)viewController didFailWithError:(NSError *)error
{
    [viewController dismissModalViewControllerAnimated:YES];
    
    // Display the error to the user.
}

- (void)matchmakerViewController:(GKMatchmakerViewController *)viewController didFindMatch:(GKMatch *)match
{
    [viewController dismissModalViewControllerAnimated:YES];
    
    matchDelegate.currentMatch = match;
    match.delegate = matchDelegate;
    
    if (!matchDelegate.matchStarted && match.expectedPlayerCount == 0)
    {
        [matchDelegate startMatch];
    }
}

@end

namespace pb
{
    
void GameCenterDelegate::MatchStarted()
{
    
}
    
void GameCenterDelegate::MatchStopped()
{
    
}

void GameCenterDelegate::PlayerConnected(std::string playerId)
{
    
}

void GameCenterDelegate::PlayerDisconnected(std::string playerId)
{
    
}
    
void GameCenterDelegate::ReceiveMessage(NetworkMessage& message)
{
    
}
    
void GameCenterDelegate::OnAuthenticated()
{
    
}

void GameCenterDelegate::AchievementProgress(std::string achievement, float percent)
{
    
}
    
void GameCenterDelegate::AchievementData(std::string achievement, std::string title, std::string description)
{
    
}

GameCenter::GameCenter()
    : _Delegate(0)
    , _IsConnected(false)
{
    _MatchDelegate = [MatchDelegate new];
}
    
GameCenter::~GameCenter()
{
    [(MatchDelegate*)_MatchDelegate release];
}
    
void GameCenter::SetDelegate(GameCenterDelegate* delegate)
{
    _Delegate = delegate;
    ((MatchDelegate*)_MatchDelegate).delegate = delegate;
}

void GameCenter::Connect()
{
    if (IsAvailable())
    {
        GKLocalPlayer *localPlayer = [GKLocalPlayer localPlayer];
        [localPlayer authenticateWithCompletionHandler:^(NSError *error)
         {
             if (localPlayer.isAuthenticated)
             {
                 _IsConnected = true;
                 
                 if (_Delegate)
                     _Delegate->OnAuthenticated();
                 
                 [GKAchievementDescription loadAchievementDescriptionsWithCompletionHandler:
                  ^(NSArray *descriptions, NSError *error) {
                      if (error != nil)
                      {
                          // TODO: Handle errors
                      }
                      if (descriptions != nil)
                      {
                          for (GKAchievementDescription* description in descriptions)
                          {
                              _Delegate->AchievementData([description.identifier UTF8String], [description.title UTF8String], [description.achievedDescription UTF8String]);
                          }
                      }
                  }];
                 
                [GKAchievement loadAchievementsWithCompletionHandler:^(NSArray *achievements, NSError *error) {
                    if (error != nil)
                    {
                        NSString* string = [error localizedDescription];
                        NSLog(@"%@", string);
                        // TODO: Handle errors
                    }
                    if (achievements != nil)
                    {
                        for (GKAchievement* achievement in achievements)
                        {
                            if (_Delegate)
                            {
                                _Delegate->AchievementProgress([achievement.identifier UTF8String], achievement.percentComplete);
                            }
                        }
                    }
                }];
                 
                 ProcessPending();
             }
         }];
    }
}
    
void GameCenter::Disconnect()
{
    [(MatchDelegate*)_MatchDelegate disconnect];
}

bool GameCenter::IsConnected()
{
    return _IsConnected;
}
    
bool GameCenter::IsHost()
{
    GKLocalPlayer *localPlayer = [GKLocalPlayer localPlayer];
    
    for (NSString* playerId in [((MatchDelegate*)_MatchDelegate).currentMatch playerIDs])
    {
        if ([[localPlayer playerID] compare:playerId] == NSOrderedAscending)
            return true;
    }
    
    return false;
}
    
void GameCenter::FindMatch(int group)
{
    [(MatchDelegate*)_MatchDelegate stopMatch];
    
    GKMatchRequest* request = [[[GKMatchRequest alloc] init] autorelease];
    request.minPlayers = 2;
    request.maxPlayers = 2;
    request.playerGroup = group;
    
    GKMatchmakerViewController* mmvc = [[[GKMatchmakerViewController alloc] initWithMatchRequest:request] autorelease];
    
    ::GameCenterDelegate* gcDelegate = [::GameCenterDelegate new];
    gcDelegate.gameCenter = this;
    gcDelegate.matchMaker = mmvc;
    gcDelegate.matchDelegate = (MatchDelegate*)_MatchDelegate;
    
    mmvc.matchmakerDelegate = gcDelegate;
    
    UIViewController* view = (UIViewController*)pb::Engine::Instance()->GetPlatformContext();
    [view presentModalViewController:mmvc animated:YES];
}
    
void GameCenter::SendMessage(NetworkMessage& message)
{
    GKMatch* match = ((MatchDelegate*)_MatchDelegate).currentMatch;
    
    char* data = new char[message.GetMessageLength()];
    message.ConstructMessage(data, message.GetMessageLength());
    
    NSError* error;
    if (![match sendDataToAllPlayers:[NSData dataWithBytesNoCopy:data length:message.GetMessageLength()] withDataMode:GKMatchSendDataReliable error:&error])
    {
        // TODO: Examine error
        NSLog(@"%@", [error description]);
    }
}

bool GameCenter::IsAvailable()
{
    BOOL localPlayerClassAvailable = (NSClassFromString(@"GKLocalPlayer")) != nil;
    
    NSString *reqSysVer = @"4.1";
    NSString *currSysVer = [[UIDevice currentDevice] systemVersion];
    BOOL osVersionSupported = ([currSysVer compare:reqSysVer options:NSNumericSearch] != NSOrderedAscending);
    
    size_t size;
    sysctlbyname("hw.machine", NULL, &size, NULL, 0); 
    char* name = (char*)malloc(size);
    sysctlbyname("hw.machine", name, &size, NULL, 0);
    std::string machine = name;
    free(name);
    
    bool correctHardware = true;
    
    if (machine == "iPhone1,1")  
        correctHardware = false; // iPhone
    else if (machine == "iPhone1,2")  
        correctHardware = false; // iPhone 3G
    else if (machine == "iPod1,1")  
        correctHardware = false; // iPod
    
    return (correctHardware && localPlayerClassAvailable && osVersionSupported);
}

void GameCenter::ProcessPending()
{
    NSUserDefaults* defaults = [NSUserDefaults standardUserDefaults];
    NSArray* pending = (NSArray*)[defaults objectForKey:@"scoresPending"];
    [defaults removeObjectForKey:@"scoresPending"];
    
    for (int i = 0; i < [pending count]; i++)
    {
        GKScore* score = [NSKeyedUnarchiver unarchiveObjectWithData:[pending objectAtIndex:i]];
        if (score != nil)
            SubmitScore(score);
    }
} 
    
void GameCenter::SubmitScore(const std::string &leaderboard, int score)
{
    if (!IsAvailable() || !_IsConnected)
        return;
    
    GKScore* scoreReporter = [[[GKScore alloc] initWithCategory:[NSString stringWithUTF8String:leaderboard.c_str()]] autorelease];
    scoreReporter.value = score;
    
    SubmitScore(scoreReporter);
}
    
void GameCenter::SubmitScore(void* scoreReporter)
{
    [(GKScore*)scoreReporter reportScoreWithCompletionHandler:^(NSError *error) {
        if (error != nil)
        {
            NSUserDefaults* defaults = [NSUserDefaults standardUserDefaults];
            
            NSArray* pending = (NSArray*)[defaults objectForKey:@"scoresPending"];
            NSMutableArray* scoresPending;
            
            if (pending)
                scoresPending = [NSMutableArray arrayWithArray:pending];
            else
                scoresPending = [NSMutableArray array];
            
            [scoresPending addObject:[NSKeyedArchiver archivedDataWithRootObject:(GKScore*)scoreReporter]];
            
            [defaults setObject:scoresPending forKey:@"scoresPending"];
        }
    }];
}

void GameCenter::SubmitAchievement(const std::string& achievementName, float percent)
{
    if (!IsAvailable() || !_IsConnected)
        return;
    
    GKAchievement* achievement = [[[GKAchievement alloc] initWithIdentifier:[NSString stringWithUTF8String:achievementName.c_str()]] retain];
    if (achievement)
    {
        achievement.percentComplete = percent;
        
        SubmitAchievement(achievement);
    }
}
      
void GameCenter::SubmitAchievement(void* achievement)
{
    [(GKAchievement*)achievement setShowsCompletionBanner:true];
    
    [(GKAchievement*)achievement reportAchievementWithCompletionHandler:^(NSError *error)
     {
         // Don't bother handling errors, since we try to submit again later anyway
         
         [(GKAchievement*)achievement release];
     }];
}

}

#endif