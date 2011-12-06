#pragma once

#include <string>

namespace pixelboost
{
    
class NetworkMessage;
    
class GameCenterDelegate
{
public:
    virtual void MatchStarted();
    virtual void MatchStopped();
    virtual void PlayerConnected(std::string playerId);
    virtual void PlayerDisconnected(std::string playerId);
    virtual void ReceiveMessage(NetworkMessage& message);
    virtual void OnAuthenticated();
    virtual void AchievementProgress(std::string achievement, float percent);
    virtual void AchievementData(std::string achievement, std::string title, std::string description);
};

class GameCenter
{
public:
    GameCenter();
    ~GameCenter();
    
    void SetDelegate(GameCenterDelegate* delegate);
    
    void Connect();
    void Disconnect();
    
    bool IsConnected();
    bool IsHost();
    
    void FindMatch(int group=0);
    
    void SendMessage(NetworkMessage& message);
    
    bool IsAvailable();
    
    void SubmitScore(const std::string& leaderboard, int score);
    void SubmitAchievement(const std::string& achievement, float percent);
    
private:
    void ProcessPending();
    
    void SubmitScore(void* score);
    void SubmitAchievement(void* achievement);
    
    GameCenterDelegate* _Delegate;
    
    bool _IsConnected;
    void* _MatchDelegate;
};
    
}
