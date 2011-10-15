#ifndef LIBPIXEL__IPHONE__INPUT__TOUCHMANAGER__H
#define LIBPIXEL__IPHONE__INPUT__TOUCHMANAGER__H

#include "libpixel/math/maths.h"
#include <map>
#include <set>
#include <vector>

namespace libpixel
{

class Camera;
class TouchHandler;
class TouchManager;

class Touch
{
public:
	Vec2 GetScreenPosition();
	Vec2 GetWorldPosition(Camera* camera);
	
private:
	enum State {
		E_NEW,
		E_UPDATED,
		E_STATIC
	};
	
	Vec2 _Position;
	State _State;
	
	TouchManager* _Manager;
	
	friend class TouchManager;
};

class TouchHandler
{
public:
	virtual void OnTouchBegin(Touch* touch);
	virtual void OnTouchUpdate(Touch* touch);
	virtual void OnTouchEnd(Touch* touch);
	
protected:
	TouchManager* _Manager;
	
	friend class TouchManager;
};

class TouchManager
{
public:
    TouchManager();
    
	void AddTouchHandler(TouchHandler* handler);
    void RemoveTouchHandler(TouchHandler* handler);
	
	void AddTouch(void* touch, Vec2 position);
	void RemoveTouch(void* touch);
	void UpdateTouch(void* touch, Vec2 position);
	
	int GetNumTouches();
	Touch* GetTouch(int index);
	
private:
    typedef std::set<TouchHandler*> TouchHandlerList;
    
	void OnTouchBegin(Touch* touch);
	void OnTouchUpdate(Touch* touch);
	void OnTouchEnd(Touch* touch);
	
	TouchHandlerList _Handlers;
	std::map<void*, Touch*> _Touches;
};
    
}

#endif