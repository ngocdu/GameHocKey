//
//  HelloWorldScene.h
//  GameHockey
//
//  Created by MinhNT on 13/06/12.
//  Copyright __MyCompanyName__ 2013年. All rights reserved.
//
#ifndef __HELLO_WORLD_H__
#define __HELLO_WORLD_H__

// When you import this file, you import all the cocos2d classes
#include "cocos2d.h"
#include "Box2D.h"

class PhysicsSprite : public cocos2d::CCSprite
{
public:
    PhysicsSprite();
    void setPhysicsBody(b2Body * body);
    virtual bool isDirty(void);
    virtual cocos2d::CCAffineTransform nodeToParentTransform(void);
private:
    b2Body* m_pBody;    // strong ref
};

class HelloWorld : public cocos2d::CCLayer {
public:
    ~HelloWorld();
    HelloWorld();
    
    // returns a Scene that contains the HelloWorld as the only child
    static cocos2d::CCScene* scene();
    
    void initPhysics();
    // adds a new sprite at a given coordinate
    void addNewSpriteAtPosition(cocos2d::CCPoint p);

    virtual void draw();
    virtual void ccTouchesEnded(cocos2d::CCSet* touches, cocos2d::CCEvent* event);
    virtual void ccTouchesBegan(cocos2d::CCSet* touches, cocos2d::CCEvent* event);
    virtual void ccTouchesMoved(cocos2d::CCSet* touch,cocos2d::CCEvent* event);
    virtual void accelerometer(cocos2d::CCAcceleration *accelerometer,  cocos2d::CCAcceleration *acceleration);
    void update(float dt);
    
    void kick(float dt);
    
private:
    b2World* world;
    cocos2d::CCTexture2D* m_pSpriteTexture; // weak ref
    
    b2Body *_body;
    cocos2d::CCSprite *_ball;

    b2Body *groundBody;
    b2Fixture *_bottomFixture;
    b2Fixture *_ballFixture;
    
    b2Body *_paddleBody;
    b2Fixture *_paddleFixture;
    b2MouseJoint *_mouseJoint;
};

#endif // __HELLO_WORLD_H__