#pragma once

#ifndef PIXELBOOST_DISABLE_GRAPHICS

#ifdef PIXELBOOST_PLATFORM_ANDROID

#include <map>
#include <set>
#include <vector>

#include "glm/glm.hpp"

namespace pb
{

class OrthographicCamera;
class TouchHandler;
class TouchManager;

class Touch
{
public:
	glm::vec2 GetDisplayPosition();
	glm::vec2 GetScreenPosition();
	glm::vec2 GetWorldPosition(OrthographicCamera* camera);
	
private:
	enum State {
		E_NEW,
		E_UPDATED,
		E_STATIC
	};
	
	glm::vec2 _Position;
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
	
	void AddTouch(void* touch, glm::vec2 position);
	void RemoveTouch(void* touch);
	void UpdateTouch(void* touch, glm::vec2 position);
	
	int GetNumTouches();
	Touch* GetTouch(int index);
	
private:
    void UpdateTouchHandlers();
    
    typedef std::map<TouchHandler*, bool> TouchHandlerList;
    
	void OnTouchBegin(Touch* touch);
	void OnTouchUpdate(Touch* touch);
	void OnTouchEnd(Touch* touch);
	
	bool _IsTouchActive;
	TouchHandlerList _Handlers;
	TouchHandlerList _HandlersToAdd;
	std::map<void*, Touch*> _Touches;
};
    
}

#endif

#endif
