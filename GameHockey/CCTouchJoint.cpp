//
//  CCTouchJoint.cpp
//  GameHockey
//
//
//

#include "CCTouchJoint.h"

using namespace std;
using namespace cocos2d;

// Fish *obj = new Fish("a",1);
CCTouchJoint::CCTouchJoint()
{
    
}

CCTouchJoint::CCTouchJoint(b2MouseJoint *mouseJoint, cocos2d::CCTouch *touch)
{
    this->mouseJoint = mouseJoint;
    this->touch = touch;
}

b2MouseJoint *CCTouchJoint::getMouseJoint()
{
    return this->mouseJoint;
}
void CCTouchJoint::setMouseJoint(b2MouseJoint *mouseJoint)
{
    this->mouseJoint = mouseJoint;
}

CCTouch * CCTouchJoint::getTouch()
{
    return  this->touch;
}
void CCTouchJoint::setTouch(cocos2d::CCTouch *touch)
{
    this->touch = touch;
}

int CCTouchJoint::getDirection()
{
    return  this->direction;
}
void CCTouchJoint::setDirection(int d)
{
    this->direction = d;
}

void CCTouchJoint::destroyJoint()
{
    if (this->mouseJoint != NULL)
    {
        mouseJoint->GetBodyA()->GetWorld()->DestroyJoint(mouseJoint);
    }
}
