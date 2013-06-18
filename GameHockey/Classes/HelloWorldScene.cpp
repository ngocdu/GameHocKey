//
//  HelloWorldScene.cpp
//  GameHockey
//
//
#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"
#include "MyContactListener.h"
#include "CCTouchJoint.h"
using namespace cocos2d;
using namespace CocosDenshion;

#define PTM_RATIO 32

enum {
    kTagParentNode = 1,
};

PhysicsSprite::PhysicsSprite()
: m_pBody(NULL)
{

}

void PhysicsSprite::setPhysicsBody(b2Body * body)
{
    m_pBody = body;
}

// this method will only get called if the sprite is batched.
// return YES if the physics values (angles, position ) changed
// If you return NO, then nodeToParentTransform won't be called.
bool PhysicsSprite::isDirty(void)
{
    return true;
}

// returns the transform matrix according the Chipmunk Body values
CCAffineTransform PhysicsSprite::nodeToParentTransform(void)
{
    b2Vec2 pos  = m_pBody->GetPosition();

    float x = pos.x * PTM_RATIO;
    float y = pos.y * PTM_RATIO;

    if ( isIgnoreAnchorPointForPosition() ) {
        x += m_obAnchorPointInPoints.x;
        y += m_obAnchorPointInPoints.y;
    }

    // Make matrix
    float radians = m_pBody->GetAngle();
    float c = cosf(radians);
    float s = sinf(radians);

    if( ! m_obAnchorPointInPoints.equals(CCPointZero) ){
        x += c*-m_obAnchorPointInPoints.x + -s*-m_obAnchorPointInPoints.y;
        y += s*-m_obAnchorPointInPoints.x + c*-m_obAnchorPointInPoints.y;
    }

    // Rot, Translate Matrix
    m_sTransform = CCAffineTransformMake( c,  s,
        -s,    c,
        x,    y );

    return m_sTransform;
}

HelloWorld::HelloWorld()
{
    setTouchEnabled( true );
    setAccelerometerEnabled( true );
    CCSize s = CCDirector::sharedDirector()->getWinSize();
    // init physics
    this->initPhysics();
    sumPoint1 = 0;
    sumPoint2 = 0;
    touchJointList = new CCArray();
    //---------------------------rao giua san-------------------------------
    CCSprite * r = CCSprite::create("LongRed2.png");
    r->setPosition(ccp(s.width/2,s.height/2));
    r->setScaleX(10);
    r->setTag(5);
    this->addChild(r);
    
    // Create gol1 body
    b2BodyDef rBodyDef;
    rBodyDef.type = b2_staticBody;
    rBodyDef.position.Set(s.width/2/PTM_RATIO, s.height/2/PTM_RATIO);
    rBodyDef.userData = r;
    rBodyDef.linearDamping = 10000.0f;
    
    rBodyDef.angularDamping = 10000.0f;
    b2Body *body_r = world->CreateBody(&rBodyDef);
    
    // Create gol1 shape
    b2PolygonShape rShape;
    rShape.SetAsBox(r->getContentSize().width*10/PTM_RATIO,
                       r->getContentSize().height/PTM_RATIO/2);
    
    // Create shape definition and add to body
    b2FixtureDef rShapeDef;
    rShapeDef.shape = &rShape;
    rShapeDef.density = 10.0f;
    rShapeDef.friction = 0.4f;
    rShapeDef.restitution = 0.0f;
    
    rShapeDef.filter.groupIndex = -10;
    body_r->CreateFixture(&rShapeDef);
    
    //-----------------------add background , gol --------------------------
    //CCSprite *bg = CCSprite::create("");
    gol1 = CCSprite::create("LongRed2.png");
    gol1->setPosition(ccp(s.width/2,10));
    gol1->setScaleX(2);
    gol1->setTag(5);
    this->addChild(gol1);
    
    // Create gol1 body
    b2BodyDef gol1BodyDef;
    gol1BodyDef.type = b2_staticBody;
    gol1BodyDef.position.Set(s.width/2/PTM_RATIO, 10/PTM_RATIO);
    gol1BodyDef.userData = gol1;
    gol1BodyDef.linearDamping = 10000.0f;
    
    gol1BodyDef.angularDamping = 10000.0f;
    body_gol1 = world->CreateBody(&gol1BodyDef);
    
    // Create gol1 shape
    b2PolygonShape gol1Shape;
    gol1Shape.SetAsBox(gol1->getContentSize().width/PTM_RATIO,
                         gol1->getContentSize().height/PTM_RATIO/2);
    
    // Create shape definition and add to body
    b2FixtureDef gol1ShapeDef;
    gol1ShapeDef.shape = &gol1Shape;
    gol1ShapeDef.density = 10.0f;
    gol1ShapeDef.friction = 0.4f;
    gol1ShapeDef.restitution = 0.0f;
    gol1ShapeDef.filter.groupIndex = 10;
    gol1_Fixture = body_gol1->CreateFixture(&gol1ShapeDef);
    
    
    gol2 = CCSprite::create("LongGreen2.png");
    gol2->setPosition(ccp(s.width/2,s.height-5));
    gol2->setTag(6);
    gol2->setScaleX(2);
    this->addChild(gol2);
    
    // Create gol1 body
    b2BodyDef gol2BodyDef;
    gol2BodyDef.type = b2_staticBody;
    gol2BodyDef.position.Set(s.width/2/PTM_RATIO, (s.height-5)/PTM_RATIO);
    gol2BodyDef.userData = gol2;
    gol2BodyDef.linearDamping = 10000.0f;
    
    gol2BodyDef.angularDamping = 10000.0f;
    body_gol2 = world->CreateBody(&gol2BodyDef);
    
    // Create gol2 shape
    b2PolygonShape gol2Shape;
    gol2Shape.SetAsBox(gol2->getContentSize().width/PTM_RATIO,
                       gol2->getContentSize().height/PTM_RATIO/2);
    
    // Create shape definition and add to body
    b2FixtureDef gol2ShapeDef;
    gol2ShapeDef.shape = &gol2Shape;
    gol2ShapeDef.density = 10.0f;
    gol2ShapeDef.friction = 0.4f;
    gol2ShapeDef.restitution = 0.0f;
    gol2ShapeDef.filter.groupIndex = 10;
    gol2_Fixture = body_gol2->CreateFixture(&gol2ShapeDef);
    
    
    CCLabelTTF *lbPoint1=CCLabelTTF::create("Point1: ", "Times New Roman",24);
    lbPoint1->setPosition(ccp(45,s.height/2-50));
    this->addChild(lbPoint1,5);
    
    CCTexture2D *txPoint1=new CCTexture2D();
    char strPoint1[20] ={0};
    sprintf(strPoint1, "%i",sumPoint1);
    txPoint1->initWithString(strPoint1,"Times New Roman",24);
    
    textSumPoint1=CCSprite::createWithTexture(txPoint1);
    textSumPoint1->setPosition(ccp(100,s.height/2-50));
    this->addChild(textSumPoint1);
    
    CCLabelTTF *lbPoint2=CCLabelTTF::create("Point2: ", "Times New Roman",24);
    lbPoint2->setPosition(ccp(45,s.height/2+50));
    this->addChild(lbPoint2,5);
    
    CCTexture2D *txPoint2=new CCTexture2D();
    char strPoint2[20] ={0};
    sprintf(strPoint2, "%i",sumPoint2);
    txPoint2->initWithString(strPoint2,"Times New Roman",24);
    
    textSumPoint2=CCSprite::createWithTexture(txPoint2);
    textSumPoint2->setPosition(ccp(100,s.height/2+50));
    this->addChild(textSumPoint2);

    //----------------------------------------------------------------------

    
    scheduleUpdate();
    //this->schedule(schedule_selector(HelloWorld::kick), 4);
    //---------------------------ball --------------------------------------
    
    // Create sprite and add it to the layer
    //_ball = CCSprite spriteWithFile:@"ball.png" rect:CGRectMake(0, 0, 52, 52)];
    _ball = CCSprite::create("ball.png");
    _ball->setTag(1);
    _ball->setPosition(ccp(300, 300));
    this->addChild(_ball);
    
        
    // Create ball body and shape
    b2BodyDef ballBodyDef;
    ballBodyDef.type = b2_dynamicBody;
    ballBodyDef.position.Set(100/PTM_RATIO, 100/PTM_RATIO);
    ballBodyDef.userData = _ball;
    _body = world->CreateBody(&ballBodyDef);
    
    b2CircleShape circle;
    circle.m_radius = 26.0/PTM_RATIO;
    
    b2FixtureDef ballShapeDef;
    ballShapeDef.shape = &circle;
    ballShapeDef.density = 1.0f;
    ballShapeDef.friction = 0.4f;
    ballShapeDef.restitution = 0.5f;
    ballShapeDef.filter.groupIndex = -10;
    _body->CreateFixture(&ballShapeDef);
    //----------------------------------------------------------------------
    //---------------------------------paddle--------------------------------
    // Create paddle and add it to the layer
    CCSprite *paddle = CCSprite::create("paddle.png");
    paddle->setPosition(ccp(s.width/2, 50));
    
    paddle->setTag(2);
    this->addChild(paddle);
    
    // Create paddle body
    b2BodyDef paddleBodyDef;
    paddleBodyDef.type = b2_dynamicBody;
    paddleBodyDef.position.Set(s.width/2/PTM_RATIO, 50/PTM_RATIO);
    paddleBodyDef.userData = paddle;
    paddleBodyDef.linearDamping = 10000.0f;
    
    paddleBodyDef.angularDamping = 10000.0f;

    _paddleBody = world->CreateBody(&paddleBodyDef);
    
    // Create paddle shape
    b2PolygonShape paddleShape;
    paddleShape.SetAsBox(paddle->getContentSize().width/PTM_RATIO/2,
                         paddle->getContentSize().height/PTM_RATIO/2);
    
    // Create shape definition and add to body
    b2FixtureDef paddleShapeDef;
    paddleShapeDef.shape = &paddleShape;
    paddleShapeDef.density = 10.0f;
    paddleShapeDef.friction = 0.4f;
    paddleShapeDef.restitution = 0.0f;
    _paddleFixture = _paddleBody->CreateFixture(&paddleShapeDef);
    //----------------------------------------------------------
    //---------------------------------paddle2--------------------------------
    // Create paddle and add it to the layer
    CCSprite *paddle2 = CCSprite::create("paddle.png");
    paddle2->setPosition(ccp(s.width/2, s.height-50));
    
    paddle2->setTag(3);
    this->addChild(paddle2);
    
    // Create paddle body
    b2BodyDef paddleBodyDef2;
    paddleBodyDef2.type = b2_dynamicBody;
    paddleBodyDef2.position.Set(s.width/2/PTM_RATIO, (s.height-50)/PTM_RATIO);
    paddleBodyDef2.userData = paddle2;
    paddleBodyDef2.linearDamping = 10000.0f;
    
    paddleBodyDef2.angularDamping = 10000.0f;

    _paddleBody2 = world->CreateBody(&paddleBodyDef2);
    
    // Create paddle shape
    b2PolygonShape paddleShape2;
    paddleShape2.SetAsBox(paddle2->getContentSize().width/PTM_RATIO/2,
                         paddle2->getContentSize().height/PTM_RATIO/2);
    
    // Create shape definition and add to body
    b2FixtureDef paddleShapeDef2;
    paddleShapeDef2.shape = &paddleShape2; //hinh dang
    paddleShapeDef2.density = 10.0f; //mat do cai nay de quy dinh trong luong cua vat
    paddleShapeDef2.friction = 0.4f; //ma sat
    paddleShapeDef2.restitution = 0.0f; //su nay - dan hoi
    _paddleFixture2 = _paddleBody2->CreateFixture(&paddleShapeDef2);
    //----------------------------------------------------------
}
HelloWorld::~HelloWorld()
{
    delete world;
    world = NULL;
    _body = NULL;
    groundBody = NULL;
}

void HelloWorld::initPhysics()
{
    CCSize s = CCDirector::sharedDirector()->getWinSize();

    b2Vec2 gravity;
    gravity.Set(0.0f, 0.0f);
    world = new b2World(gravity);

    // Do we want to let bodies sleep?
    world->SetAllowSleeping(true);

    world->SetContinuousPhysics(true);

    _contactListener = new MyContactListener();
    world->SetContactListener(_contactListener);

    uint32 flags = 0;
    flags += b2Draw::e_shapeBit;
    // Define the ground body.
    b2BodyDef groundBodyDef;
    groundBodyDef.position.Set(0, 0); // bottom-left corner

    // Call the body factory which allocates memory for the ground body
    // from a pool and creates the ground box shape (also from a pool).
    // The body is also added to the world.
    groundBody = world->CreateBody(&groundBodyDef);

    // Define the ground box shape.
    b2EdgeShape groundBox;

    // bottom

    groundBox.Set(b2Vec2(0,0), b2Vec2(s.width/PTM_RATIO,0));
    groundBody->CreateFixture(&groundBox,0);

    // top
    groundBox.Set(b2Vec2(0,s.height/PTM_RATIO), b2Vec2(s.width/PTM_RATIO,s.height/PTM_RATIO));
    groundBody->CreateFixture(&groundBox,0);

    // left
    groundBox.Set(b2Vec2(0,s.height/PTM_RATIO), b2Vec2(0,0));
    groundBody->CreateFixture(&groundBox,0);

    // right
    groundBox.Set(b2Vec2(s.width/PTM_RATIO,s.height/PTM_RATIO), b2Vec2(s.width/PTM_RATIO,0));
    groundBody->CreateFixture(&groundBox,0);
    
    
   
}

void HelloWorld::draw()
{
    //
    // IMPORTANT:
    // This is only for debug purposes
    // It is recommend to disable it
    //
    CCLayer::draw();

    ccGLEnableVertexAttribs( kCCVertexAttribFlag_Position );

    kmGLPushMatrix();

    world->DrawDebugData();

    kmGLPopMatrix();
}

void HelloWorld::addNewSpriteAtPosition(CCPoint p)
{
    CCLOG("Add sprite %0.2f x %02.f",p.x,p.y);
    CCNode* parent = getChildByTag(kTagParentNode);
    
    //We have a 64x64 sprite sheet with 4 different 32x32 images.  The following code is
    //just randomly picking one of the images
    int idx = (CCRANDOM_0_1() > .5 ? 0:1);
    int idy = (CCRANDOM_0_1() > .5 ? 0:1);
    PhysicsSprite *sprite = new PhysicsSprite();
    sprite->initWithTexture(m_pSpriteTexture, CCRectMake(32 * idx,32 * idy,32,32));
    sprite->autorelease();
    
    parent->addChild(sprite);
    
    sprite->setPosition( CCPointMake( p.x, p.y) );
    
    // Define the dynamic body.
    //Set up a 1m squared box in the physics world
    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.position.Set(p.x/PTM_RATIO, p.y/PTM_RATIO);
    
    b2Body *body = world->CreateBody(&bodyDef);
    
    // Define another box shape for our dynamic body.
    b2PolygonShape dynamicBox;
    dynamicBox.SetAsBox(.5f, .5f);//These are mid points for our 1m box
    
    // Define the dynamic body fixture.
    b2FixtureDef fixtureDef;
    fixtureDef.shape = &dynamicBox;    
    fixtureDef.density = 1.0f;
    fixtureDef.friction = 0.3f;
    body->CreateFixture(&fixtureDef);
    
    sprite->setPhysicsBody(body);
}

void HelloWorld::kick(float dt)
{
    b2Vec2 force = b2Vec2(10, 10);
    _body->ApplyLinearImpulse(force,_body->GetPosition());
    
}

void HelloWorld::update(float dt)
{
    //It is recommended that a fixed time step is used with Box2D for stability
    //of the simulation, however, we are using a variable time step here.
    //You need to make an informed choice, the following URL is useful
    //http://gafferongames.com/game-physics/fix-your-timestep/
    
    CCSize s = CCDirector::sharedDirector()->getWinSize();
    int velocityIterations = 8;
    int positionIterations = 1;
    
    // Instruct the world to perform a single step of simulation. It is
    // generally best to keep the time step and iterations fixed.
    world->Step(dt, velocityIterations, positionIterations);
    
    //Iterate over the bodies in the physics world
    for (b2Body* b = world->GetBodyList(); b; b = b->GetNext())
    {
        if (b->GetUserData() != NULL) {
            //Synchronize the AtlasSprites position and rotation with the corresponding body
            CCSprite* myActor = (CCSprite*)b->GetUserData();
            
            if (myActor->getTag()==2 || myActor->getTag()==3) {
                    myActor->setPosition( CCPointMake( b->GetPosition().x * PTM_RATIO, b->GetPosition().y * PTM_RATIO) );
            }
            else
            {
                myActor->setRotation( -1 * CC_RADIANS_TO_DEGREES(b->GetAngle()) );
                myActor->setPosition( CCPointMake( b->GetPosition().x * PTM_RATIO, b->GetPosition().y * PTM_RATIO) );
            }
        }    
    }
    
    //---------------------------ball---------------------
    for(b2Body *b = world->GetBodyList(); b; b=b->GetNext()) {
        if (b->GetUserData() != NULL) {
            CCSprite *sprite = (CCSprite *)b->GetUserData();
            
            // if ball is going too fast, turn on damping
            if (sprite->getTag() == 1) {
                static int maxSpeed = 5;
                
                b2Vec2 velocity = b->GetLinearVelocity();
                float32 speed = velocity.Length();
                
                if (speed > maxSpeed) {
                    b->SetLinearDamping(0.5);
                } else if (speed < maxSpeed) {
                    b->SetLinearDamping(0.0);
                } 
            }
            if (sprite->getTag()==2 || sprite->getTag()==3) {
                    sprite->setPosition( CCPointMake( b->GetPosition().x * PTM_RATIO, b->GetPosition().y * PTM_RATIO) );
            }
            else
            {
                sprite->setRotation( -1 * CC_RADIANS_TO_DEGREES(b->GetAngle()) );
                sprite->setPosition( CCPointMake( b->GetPosition().x * PTM_RATIO, b->GetPosition().y * PTM_RATIO) );
            }
        }
    }
    std::vector<b2Body *>toDestroy;
    std:vector<MyContact>::iterator pos;
    
    for (pos = _contactListener->_contacts.begin();
         pos != _contactListener->_contacts.end(); ++pos){
        MyContact contact = *pos;
        
        b2Body *bodyA = contact.fixtureA->GetBody();
        b2Body *bodyB = contact.fixtureB->GetBody();
        if (bodyA->GetUserData() != NULL && bodyB->GetUserData() != NULL) {
            CCSprite *spriteA = (CCSprite *)bodyA->GetUserData();
            CCSprite *spriteB = (CCSprite *)bodyB->GetUserData();
            
            if ( (spriteA->getTag() == 1 && spriteB->getTag() == 5) || (spriteA->getTag() == 5 && spriteB->getTag() == 1)) {
                sumPoint1++;
                CCTintTo* tint1 = CCTintTo::create(0.2, 255, 50, 0);
                CCTintTo* tint2 = CCTintTo::create(0.2, 255, 0, 220);
                CCTintTo* tint3 = CCTintTo::create(0.2, 0, 50, 220);
                CCTintTo* tint4 = CCTintTo::create(0.2, 0, 0, 255);
                CCTintTo* tint5 = CCTintTo::create(0.2, 255, 255, 0);
                CCArray *ar = new CCArray();
                ar->addObject(tint1);
                ar->addObject(tint2);
                ar->addObject(tint3);
                ar->addObject(tint4);
                ar->addObject(tint5);
                CCSequence* sequence = CCSequence::create(ar);
                
                if (spriteA->getTag() == 5) {
                    spriteA->runAction(sequence);
                }
                else spriteB->runAction(sequence);
            } else if ((spriteA->getTag() == 1 && spriteB->getTag() == 6) || (spriteA->getTag() == 6 && spriteB->getTag() == 1)){
                sumPoint2++;
                CCTintTo* tint1 = CCTintTo::create(0.2, 255, 50, 0);
                CCTintTo* tint2 = CCTintTo::create(0.2, 255, 0, 220);
                CCTintTo* tint3 = CCTintTo::create(0.2, 0, 255, 0);
                CCTintTo* tint4 = CCTintTo::create(0.2, 0, 0, 255);
                CCTintTo* tint5 = CCTintTo::create(0.2, 255, 50, 0);
                CCArray *ar = new CCArray();
                ar->addObject(tint1);
                ar->addObject(tint2);
                ar->addObject(tint3);
                ar->addObject(tint4);
                ar->addObject(tint5);
                CCSequence* sequence = CCSequence::create(ar);
                
                if (spriteA->getTag() == 6) {
                    spriteA->runAction(sequence);
                }
                else spriteB->runAction(sequence);
            }
        }
    }

    CCTexture2D *txPoint1=new CCTexture2D();
    char strPoint1[20] ={0};
    sprintf(strPoint1, "%i",sumPoint1);
    txPoint1->initWithString(strPoint1,"Times New Roman",24);
    textSumPoint1->setTexture(txPoint1);
    
    CCTexture2D *txPoint2=new CCTexture2D();
    char strPoint2[20] ={0};
    sprintf(strPoint2, "%i",sumPoint2);
    txPoint2->initWithString(strPoint2,"Times New Roman",24);
    textSumPoint2->setTexture(txPoint2);
}
void HelloWorld::accelerometer(cocos2d::CCAccelerometer *acceleromet,  cocos2d::CCAcceleration *acceleration)
{
    b2Vec2 gravity(acceleration->y * 30, -acceleration->x * 30);
    world->SetGravity(gravity);
}
void HelloWorld::ccTouchesBegan(cocos2d::CCSet *touches, cocos2d::CCEvent *event)
{
    CCSize s = CCDirector::sharedDirector()->getWinSize();
    CCSetIterator it;
    CCTouch* touch;
    for( it = touches->begin(); it != touches->end(); it++)// get all touch point
    {
        touch = (CCTouch*)(*it);
        if(!touch)
            continue;
        CCPoint location = touch->getLocationInView();
        location = CCDirector::sharedDirector()->convertToGL(location);
        b2Vec2 locationWorld = b2Vec2(location.x/PTM_RATIO, location.y/PTM_RATIO);
        if (_paddleFixture->TestPoint(locationWorld)) {
        b2MouseJointDef md;
        md.bodyA = groundBody;
        md.bodyB = _paddleBody;
        md.target = locationWorld;
        md.collideConnected = true;
        md.maxForce = 10000.0f * _paddleBody->GetMass();
        md.dampingRatio = 0;
        md.frequencyHz =100000;
        b2MouseJoint *_mouseJoint = (b2MouseJoint *)world->CreateJoint(&md);
        _paddleBody->SetAwake(true);
            
        CCTouchJoint * tj = new CCTouchJoint(_mouseJoint,touch);
                   touchJointList->addObject(tj);
        }
        if (_paddleFixture2->TestPoint(locationWorld)) {
        b2MouseJointDef md;
        md.bodyA = groundBody;
        md.bodyB = _paddleBody2;
        md.target = locationWorld;
        md.collideConnected = true;
        md.maxForce = 10000.0f * _paddleBody2->GetMass();
        md.dampingRatio = 0;
        md.frequencyHz = 100000;
        b2MouseJoint *_mouseJoint2 = (b2MouseJoint *)world->CreateJoint(&md);
        _paddleBody2->SetAwake(true);
        CCTouchJoint * tj = new CCTouchJoint(_mouseJoint2,touch);
        touchJointList->addObject(tj);
    }   
    }
}
void HelloWorld::ccTouchesMoved(cocos2d::CCSet *touches, cocos2d::CCEvent *event)
{
    CCSize s = CCDirector::sharedDirector()->getWinSize();
    CCSetIterator it;
    CCTouch* touch;
    for( it = touches->begin(); it != touches->end(); it++)// get all touch point
    {
        touch = (CCTouch*)(*it);
        if(!touch)
        continue;
        CCPoint location = touch->getLocationInView();
        location = CCDirector::sharedDirector()->convertToGL(location);
        CCObject *i;
        CCARRAY_FOREACH(touchJointList, i)
        {
            CCTouchJoint * tj = (CCTouchJoint*)i;
            if ( tj->getMouseJoint() != NULL )
            {
                CCPoint location = tj->getTouch()->getLocationInView();
                location = CCDirector::sharedDirector()->convertToGL(location);
                
                b2Vec2 locationWorld = b2Vec2(location.x/PTM_RATIO, location.y/PTM_RATIO);
                
                tj->getMouseJoint()->SetTarget(locationWorld);
            }
            else if ( tj->getMouseJoint() != NULL )
            {
                CCPoint location = tj->getTouch()->getLocationInView();
                location = CCDirector::sharedDirector()->convertToGL(location);
                b2Vec2 locationWorld = b2Vec2(location.x/PTM_RATIO, location.y/PTM_RATIO);
                tj->getMouseJoint()->SetTarget(locationWorld);
            }
        }
    } 
}
void HelloWorld::ccTouchesEnded(CCSet* touches, CCEvent* event)
{
    CCSetIterator it;
    CCTouch* touch;
    CCArray * discardedItems =  new CCArray();
    for( it = touches->begin(); it != touches->end(); it++) 
    {
        touch = (CCTouch*)(*it);
        if(!touch)
            break;
        CCPoint location = touch->getLocationInView();
        location = CCDirector::sharedDirector()->convertToGL(location);
        CCObject *i;
        CCARRAY_FOREACH(touchJointList, i)
        {
            CCTouchJoint *tj = (CCTouchJoint*)i;
            if (tj->getTouch() == touch)
            {
                {
                    discardedItems->addObject(tj);
                    tj->destroyJoint();
                    tj->release();
                }
            }
        }
        touchJointList->removeObjectsInArray(discardedItems);
    }
}
CCScene* HelloWorld::scene()
{
    CCScene *scene = CCScene::create();
    CCLayer* layer = new HelloWorld();
    scene->addChild(layer);
    layer->release();
    return scene;
}
