#include "HelloWorldScene.h"
#include "SoundCtl.h"
#include "GameScene.h"

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
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    auto rootNode = CSLoader::createNode("MainScene.csb");

    addChild(rootNode);
    _device = this->getDeviceScale();
    bg = static_cast<Sprite*>(rootNode->getChildByName("bg"));
    playBtn = static_cast<Sprite*>(rootNode->getChildByName("playBtn"));
    
    _listener=EventListenerTouchOneByOne::create();
    _listener->setSwallowTouches(true);
    _listener->onTouchBegan = CC_CALLBACK_2(HelloWorld::menuBegin,this);
    _listener->onTouchEnded = CC_CALLBACK_2(HelloWorld::menuEndCallback, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(_listener, this);
    SoundCtl::getInstance()->playBg("sound/Wizard.m4a", true);
    this->adapter();
    return true;
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
    bg->setScale(_device->scaleX, _device->scaleY);
}
DeviceScaleInfo* HelloWorld::getDeviceScale()
{
    int standW = 1334;
    int standH = 750;
    Size visibleSize = Director::getInstance()->getVisibleSize();
    DeviceScaleInfo*scaleInfo = new DeviceScaleInfo();
    scaleInfo->scaleX = visibleSize.width/standW;
    scaleInfo->scaleY = visibleSize.height/standH;
    
    scaleInfo->model = getModel();
    scaleInfo->sdkVersion = getSdkVersion();
    scaleInfo->osVersion = getOsVersion();
    scaleInfo->phoneNumber = getPhoneNumber();
    return scaleInfo;
}
void HelloWorld::onExit()
{
    Layer::onExit();
    _eventDispatcher->removeEventListener(_listener);
}
bool HelloWorld::menuBegin(cocos2d::Touch* tTouch,cocos2d::Event* eEvent)
{
    Point localP = this->convertToNodeSpace(tTouch->getLocation());
    if(playBtn->getBoundingBox().containsPoint(localP)){
       playBtn->setScale(0.9f);
       return true;
    }
    return false;
}
void HelloWorld::menuEndCallback(cocos2d::Touch* tTouch,cocos2d::Event* eEvent)
{
    Point localP = this->convertToNodeSpace(tTouch->getLocation());
    
    if(playBtn->getBoundingBox().containsPoint(localP)){
        playBtn->setScale(1.0f);
        //打开布阵弹窗
        SoundCtl::getInstance()->playEffect("sound/click.m4a");
        auto scene = GameScene::createScene();
        Director::getInstance()->replaceScene(TransitionPageTurn::create(1, scene, true));
        return;
    }
}
