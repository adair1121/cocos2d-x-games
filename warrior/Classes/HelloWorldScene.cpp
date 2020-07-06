#include "HelloWorldScene.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "GameScene.hpp"
#include "LevelLayer.hpp"
#include "RecordLayer.hpp"
#include "Config.h"

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
    
    map = static_cast<Sprite*>(rootNode->getChildByName("map"));
    startBtn = static_cast<Sprite*>(rootNode->getChildByName("startBtn"));
    levelBtn = static_cast<Sprite*>(rootNode->getChildByName("levelBtn"));
    recordBtn = static_cast<Sprite*>(rootNode->getChildByName("recordBtn"));
    popLayout = static_cast<Layout*>(rootNode->getChildByName("popLayout"));
    
    _listener=EventListenerTouchOneByOne::create();
    _listener->setSwallowTouches(true);
    _listener->onTouchBegan = CC_CALLBACK_2(HelloWorld::menuBegin,this);
    _listener->onTouchEnded = CC_CALLBACK_2(HelloWorld::menuEndCallback, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(_listener, this);
    
    this->adapter();
    
    return true;
}
void HelloWorld::adapter()
{
    int bgW = 750;
    int bgH = 1334;
    Size visibleSize = Director::getInstance()->getVisibleSize();
    float scaleX = visibleSize.width/bgW;
    float scaleY = visibleSize.height/bgH;
    map->setScale(scaleX);
    map->setAnchorPoint(Vec2(0,1));
    map->setPosition(Vec2(0,visibleSize.height));
    popLayout->setAnchorPoint(Vec2(0,1));
    popLayout->setPosition(Vec2(0,visibleSize.height));
    popLayout->setContentSize(Size(visibleSize.width,visibleSize.height));
    startBtn->setPosition(Vec2(startBtn->getPositionX()*scaleX,startBtn->getPositionY()*scaleY));
    levelBtn->setPosition(Vec2(levelBtn->getPositionX()*scaleX,levelBtn->getPositionY()*scaleY));
    recordBtn->setPosition(Vec2(recordBtn->getPositionX()*scaleX,recordBtn->getPositionY()*scaleY));
}
bool HelloWorld::menuBegin(cocos2d::Touch* tTouch,cocos2d::Event* eEvent)
{
    Point p = this->convertToNodeSpace(tTouch->getLocation());
    if(startBtn->getBoundingBox().containsPoint(p))
    {
        startBtn->setScale(0.9f);
        return true;
    }else if(levelBtn->getBoundingBox().containsPoint(p))
    {
        levelBtn->setScale(0.9f);
        return true;
    }else if(recordBtn->getBoundingBox().containsPoint(p))
    {
        recordBtn->setScale(0.9f);
        return true;
    }
    return false;
}
void HelloWorld::menuEndCallback(cocos2d::Touch* tTouch,cocos2d::Event* eEvent)
{
    Point p = this->convertToNodeSpace(tTouch->getLocation());
    if(startBtn->getBoundingBox().containsPoint(p) && popLayout->getChildren().size() <= 0)
    {
        CF()->level = 1;
        startBtn->setScale(1.0f);
        auto scene = GameScene::createScene();
        Director::getInstance()->replaceScene(TransitionCrossFade::create(1, scene));
    }else if(levelBtn->getBoundingBox().containsPoint(p) && popLayout->getChildren().size() <= 0)
    {
        levelBtn->setScale(1.0f);
        auto scene = LevelLayer::createScene();
        Director::getInstance()->replaceScene(TransitionCrossFade::create(1, scene));
       
    }else if(recordBtn->getBoundingBox().containsPoint(p) && popLayout->getChildren().size() <= 0)
    {
        recordBtn->setScale(1.0f);
        RecordLayer*record = new RecordLayer();
        popLayout->addChild(record);
    }
}
