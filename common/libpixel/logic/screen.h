#ifndef COMMON__LOGIC__SCREEN__H
#define COMMON__LOGIC__SCREEN__H

namespace libpixel
{

class Game;
class TouchHandler;

class Screen
{
public:
    Screen(Game* game);
	virtual ~Screen();
	
	virtual void Update(float time);
	virtual void Render();
    
    void SetTouchActive();
    
    void SetTouchHandler(TouchHandler* touchHandler);
    TouchHandler* GetTouchHandler();
    
    Game* GetGame();
    
protected:
    Game* _Game;
    
private:
    TouchHandler* _TouchHandler;
};

}

#endif