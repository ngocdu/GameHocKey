//
//  CCTouchJoint.h
//  GameHockey
//
//
//

#ifndef GameHockey_CCTouchJoint_h
#define GameHockey_CCTouchJoint_h

#include "Box2D.h"
#include "cocos2d.h"
#include "SimpleAudioEngine.h"
using namespace std;
class CCTouchJoint : public cocos2d::CCObject
{
private:
	b2MouseJoint *mouseJoint;
    cocos2d::CCTouch *touch;
    int direction;
		
public:
	CCTouchJoint();
	CCTouchJoint(b2MouseJoint *mouseJoint, cocos2d::CCTouch *touch);// ham tao co doi so
    
    b2MouseJoint * getMouseJoint();
    void setMouseJoint(b2MouseJoint *mouseJoint);
    
    cocos2d::CCTouch *getTouch();
    void setTouch(cocos2d::CCTouch *touch);
	
    int getDirection();
    void setDirection(int d);
    
    void destroyJoint();
    
};



#endif
