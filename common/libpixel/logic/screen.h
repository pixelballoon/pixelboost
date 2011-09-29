#ifndef COMMON__LOGIC__SCREEN__H
#define COMMON__LOGIC__SCREEN__H

namespace libpixel
{

class Game;

class Screen
{
public:
    Screen(Game* game);
	virtual ~Screen();
	
	virtual void Update(float time);
	virtual void Render();
    
    void SetActive(bool active);
    
    Game* GetGame();
    
protected:
    Game* _Game;
};

}

#endif