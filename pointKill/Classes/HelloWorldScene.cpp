#include "HelloWorldScene.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "SoundCtl.h"
#include "GameScene.h"
#include "SetPop.hpp"
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
    playBtn = static_cast<Sprite*>(rootNode->getChildByName("playBtn"));
    setBtn = static_cast<Sprite*>(rootNode->getChildByName("setBtn"));
    
    _listener=EventListenerTouchOneByOne::create();
    _listener->setSwallowTouches(true);
    _listener->onTouchBegan = CC_CALLBACK_2(HelloWorld::menuBegin,this);
    _listener->onTouchEnded = CC_CALLBACK_2(HelloWorld::menuEndCallback, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(_listener, this);
    SoundCtl::getInstance()->playBg("sound/music.m4a", true);
    
//    this->adapter();

    return true;
}
bool HelloWorld::menuBegin(cocos2d::Touch* tTouch,cocos2d::Event* eEvent)
{
    Point localP = this->convertToNodeSpace(tTouch->getLocation());
    if(playBtn->getBoundingBox().containsPoint(localP)&& !topPanel){
       playBtn->setScale(0.9f);
       return true;
    }else if(setBtn->getBoundingBox().containsPoint(localP) && !topPanel)
    {
        setBtn->setScale(0.9f);
        return true;
    }
    return false;
}
void HelloWorld::menuEndCallback(cocos2d::Touch* tTouch,cocos2d::Event* eEvent)
{
    Point localP = this->convertToNodeSpace(tTouch->getLocation());
    
    if(playBtn->getBoundingBox().containsPoint(localP) && !topPanel){
        playBtn->setScale(1.0f);
  
        SoundCtl::getInstance()->playEffect("sound/click.m4a");
        auto scene = GameScene::createScene();
        Director::getInstance()->replaceScene(TransitionTurnOffTiles::create(1, scene));
        return;
    }else if(setBtn->getBoundingBox().containsPoint(localP))
    {
        topPanel = true;
        SetPop*set = new SetPop(1);
        set->callback = CC_CALLBACK_0(HelloWorld::topCall, this);
        this->addChild(set);
        return;
    }
}
void HelloWorld::topCall()
{
    topPanel = false;
}
void HelloWorld::adapter()
{
    int bgW = bg->getContentSize().width;
    int bgH = bg->getContentSize().height;
    Size visibleSize = Director::getInstance()->getVisibleSize();
    float scaleX = visibleSize.width/bgW;
    float scaleY = visibleSize.height/bgH;
    bg->setScale(scaleX, scaleY);
    playBtn->setPosition(Vec2(visibleSize.width/2,150));
    setBtn->setPosition(Vec2(setBtn->getPositionX()*scaleX,setBtn->getPositionY()*scaleY));
//    bg->setScale(scaleX, scaleY);
}
void HelloWorld::onExit()
{
    Layer::onExit();
    _eventDispatcher->removeEventListener(_listener);
}
