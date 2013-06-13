//
//  HelloWorldScene.h
//  GameHockey
//
//
#ifndef __HELLO_WORLD_H__
#define __HELLO_WORLD_H__

// When you import this file, you import all the cocos2d classes
#include "cocos2d.h"
#include "Box2D.h"
#include "MyContactListener.h"
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
    virtual void accelerometer(cocos2d::CCAccelerometer *accelerometer,  cocos2d::CCAcceleration *acceleration);
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
    
    b2Body *_paddleBody2;
    b2Fixture *_paddleFixture2;
    
    b2MouseJoint *_mouseJoint;
    b2MouseJoint *_mouseJoint2;
    
    
    cocos2d::CCSprite *gol1;
    b2Body *body_gol1;
    b2Fixture *gol1_Fixture;
    cocos2d::CCSprite *gol2;
    b2Body *body_gol2;
    b2Fixture *gol2_Fixture;
    int sumPoint1;
    cocos2d:: CCSprite * textSumPoint1;
    
    int sumPoint2;
    cocos2d:: CCSprite * textSumPoint2;
    
    MyContactListener *_contactListener;
};

#endif // __HELLO_WORLD_H__
