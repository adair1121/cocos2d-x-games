#include "HelloWorldScene.h"
#include "GameScene.hpp"


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
    logo = static_cast<Sprite*>(rootNode->getChildByName("logo"));
    startBtn = static_cast<Sprite*>(rootNode->getChildByName("startBtn"));
    
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
    int bgW = bg->getContentSize().width;
    int bgH = bg->getContentSize().height;
    Size visibleSize = Director::getInstance()->getVisibleSize();
    float scaleX = visibleSize.width/bgW;
    float scaleY = visibleSize.height/bgH;
    bg->setScale(scaleX, scaleY);
    logo->setPosition(Vec2(logo->getPositionX()*scaleX,logo->getPositionY()*scaleY));
    startBtn->setPosition(Vec2(startBtn->getPositionX()*scaleX,startBtn->getPositionY()*scaleY));
}
void HelloWorld::onExit()
{
    Layer::onExit();
    _eventDispatcher->removeEventListener(_listener);
}
bool HelloWorld::menuBegin(cocos2d::Touch* tTouch,cocos2d::Event* eEvent)
{
    Point localP = this->convertToNodeSpace(tTouch->getLocation());
    if(startBtn->getBoundingBox().containsPoint(localP)){
       startBtn->setScale(0.9f);
       return true;
    }
    return false;
}
void HelloWorld::menuEndCallback(cocos2d::Touch* tTouch,cocos2d::Event* eEvent)
{
    Point localP = this->convertToNodeSpace(tTouch->getLocation());
    
    if(startBtn->getBoundingBox().containsPoint(localP)){
        startBtn->setScale(1.0f);
        //打开布阵弹窗
        auto scene = GameScene::createScene();

        Director::getInstance()->replaceScene(TransitionFadeDown::create(1, scene));
        return;
    }
}
