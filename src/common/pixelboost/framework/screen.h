#pragma once

namespace pb
{

class Game;

class Screen
{
public:
    Screen(Game* game);
	virtual ~Screen();
	
	virtual void Update(float time);
	virtual void Render();
    
    virtual void SetActive(bool active);
    
    Game* GetGame();
    
protected:
    Game* _Game;
};

}
