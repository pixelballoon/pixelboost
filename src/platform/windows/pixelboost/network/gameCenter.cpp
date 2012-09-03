#ifdef PIXELBOOST_PLATFORM_WINDOWS

#include "pixelboost/framework/game.h"
#include "pixelboost/network/gameCenter.h"
#include "pixelboost/network/networkMessage.h"

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
}
    
GameCenter::~GameCenter()
{
}
    
void GameCenter::SetDelegate(GameCenterDelegate* delegate)
{
}

void GameCenter::Connect()
{
}
    
void GameCenter::Disconnect()
{
}

bool GameCenter::IsConnected()
{
    return _IsConnected;
}
    
bool GameCenter::IsHost()
{
    return false;
}
    
void GameCenter::FindMatch(int group)
{
}
    
void GameCenter::SendMessage(NetworkMessage& message)
{
}

bool GameCenter::IsAvailable()
{
    return false;
}

void GameCenter::ProcessPending()
{
} 
    
void GameCenter::SubmitScore(const std::string &leaderboard, int score)
{
}
    
void GameCenter::SubmitScore(void* scoreReporter)
{
}

void GameCenter::SubmitAchievement(const std::string& achievementName, float percent)
{
}
      
void GameCenter::SubmitAchievement(void* achievement)
{
}

}

#endif