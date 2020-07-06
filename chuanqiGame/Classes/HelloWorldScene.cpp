#include "HelloWorldScene.h"
#include "CreateLayer.hpp"

USING_NS_CC;

using namespace cocostudio::timeline;

Scene* HelloWorld::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
    
    auto rootNode = CSLoader::createNode("MainScene.csb");

    addChild(rootNode);
    
    bg = static_cast<Sprite*>(rootNode->getChildByName("bg"));
    startBtn = static_cast<Sprite*>(rootNode->getChildByName("startBtn"));
    
    _listener=EventListenerTouchOneByOne::create();
    _listener->setSwallowTouches(true);
    _listener->onTouchBegan = CC_CALLBACK_2(HelloWorld::menuBegin,this);
    _listener->onTouchEnded = CC_CALLBACK_2(HelloWorld::menuEndCallback, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(_listener, this);
    
    this->adapter();
    
    startBtn->setOpacity(125.0f);
    auto fadein = FadeIn::create(1.0f);
    auto scaleTo = ScaleTo::create(1.0f, 1.2f);
    auto swapn1 = Spawn::createWithTwoActions(fadein, scaleTo);
    auto fadeout = FadeTo::create(1.0, 125.0f);
    auto scaleFrom = ScaleTo::create(1.0f, 1.0f);
    auto swapn2 = Spawn::createWithTwoActions(fadeout, scaleFrom);
    startBtn->runAction(RepeatForever::create(Sequence::create(swapn1,swapn2, NULL)));
    return true;
}
void HelloWorld::adapter()
{
    int bgW = 750;
    int bgH = 1334;
    Size visibleSize = Director::getInstance()->getVisibleSize();
    float scaleX = visibleSize.width/bgW;
    float scaleY = visibleSize.height/bgH;
    bg->setScale(scaleX);
    bg->setPosition(Vec2(bg->getPositionX()*scaleX,bg->getPositionY()*scaleY));
    startBtn->setPosition(Vec2(startBtn->getPositionX()*scaleX,startBtn->getPositionY()*scaleY));
}
bool HelloWorld::menuBegin(cocos2d::Touch* tTouch,cocos2d::Event* eEvent)
{
    Point p = this->convertToNodeSpace(tTouch->getLocation());
    if(startBtn->getBoundingBox().containsPoint(p))
    {
        return true;
    }
    return true;
}
void HelloWorld::menuEndCallback(cocos2d::Touch* tTouch,cocos2d::Event* eEvent)
{
    Point p = this->convertToNodeSpace(tTouch->getLocation());
    if(startBtn->getBoundingBox().containsPoint(p))
    {
        auto scene = CreateLayer::createScene();
        Director::getInstance()->replaceScene(TransitionFadeTR::create(1, scene));
    }
}
