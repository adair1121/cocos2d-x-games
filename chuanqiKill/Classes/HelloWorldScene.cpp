#include "HelloWorldScene.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "utils/SoundCtl.h"
//#include "GameScene.h"
#include "SelectLevel.hpp"

USING_NS_CC;

//using namespace cocostudio::timeline;

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
    playBtn->setVisible(false);
    loadBg = static_cast<Sprite*>(rootNode->getChildByName("loadingBg"));
    loadBar = static_cast<LoadingBar*>(rootNode->getChildByName("loadingBar"));
    
    _listener=EventListenerTouchOneByOne::create();
    _listener->setSwallowTouches(true);
    _listener->onTouchBegan = CC_CALLBACK_2(HelloWorld::menuBegin,this);
    _listener->onTouchEnded = CC_CALLBACK_2(HelloWorld::menuEndCallback, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(_listener, this);
    SoundCtl::getInstance()->playBg("sound/music.m4a", true);
    
    
    
    loadBar->setPercent(0);
    this->adapter();
    
    this->schedule(schedule_selector(HelloWorld::loadingprocess), 0.05f);
    return true;
}
void HelloWorld::loadingprocess(float dt)
{
    pronum += 5;
    if(pronum >= 100)
    {
        pronum = 100;
        this->unschedule(schedule_selector(HelloWorld::loadingprocess));
        playBtn->setVisible(true);
        playBtn->setOpacity(0);
        auto fadein = FadeIn::create(0.5f);
        loadBg->setVisible(false);
        loadBar->setVisible(false);
        playBtn->runAction(fadein);
    }
    loadBar->setPercent(pronum);
}
bool HelloWorld::menuBegin(cocos2d::Touch* tTouch,cocos2d::Event* eEvent)
{
    Point localP = this->convertToNodeSpace(tTouch->getLocation());
    if(playBtn->getBoundingBox().containsPoint(localP) && playBtn->isVisible()){
       playBtn->setScale(0.9f);
       return true;
    }
    return false;
}
void HelloWorld::menuEndCallback(cocos2d::Touch* tTouch,cocos2d::Event* eEvent)
{
    Point localP = this->convertToNodeSpace(tTouch->getLocation());
    
    if(playBtn->getBoundingBox().containsPoint(localP) && playBtn->isVisible()){
        playBtn->setScale(1.0f);
  
        SoundCtl::getInstance()->playEffect("sound/click.m4a");
        auto scene = SelectLevel::createScene();
      
        Director::getInstance()->replaceScene(TransitionProgressRadialCW::create(1, scene));
        return;
    }
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
   
    loadBar->setPosition(Vec2(loadBar->getPositionX()*scaleX,loadBar->getPositionY()*scaleY));
    loadBg->setPosition(Vec2(loadBg->getPositionX()*scaleX,loadBg->getPositionY()*scaleY));
}
void HelloWorld::onExit()
{
    Layer::onExit();
    _eventDispatcher->removeEventListener(_listener);
}
