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
    helpBtn = static_cast<Sprite*>(rootNode->getChildByName("helpBtn"));
    startBtn = static_cast<Sprite*>(rootNode->getChildByName("startBtn"));
    helpImg = static_cast<Sprite*>(rootNode->getChildByName("helpImg"));
    
    helpImg->setVisible(false);
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
    bg->setScale(scaleX);
    bg->setAnchorPoint(Vec2(0,1));
    bg->setPosition(Vec2(0,visibleSize.height));
    helpImg->setScale(scaleX);
    helpImg->setPosition(Vec2(helpImg->getPositionX()*scaleX,helpImg->getPositionY()*scaleY));
    startBtn->setPosition(Vec2(startBtn->getPositionX()*scaleX,startBtn->getPositionY()*scaleY));
    helpBtn->setPosition(Vec2(helpBtn->getPositionX()*scaleX,helpBtn->getPositionY()*scaleY));
}
bool HelloWorld::menuBegin(cocos2d::Touch* tTouch,cocos2d::Event* eEvent)
{
    Point p = this->convertToNodeSpace(tTouch->getLocation());
    if(startBtn->getBoundingBox().containsPoint(p) && !helpImg->isVisible())
    {
        startBtn->setScale(0.9f);
        return true;
    }else if(helpBtn->getBoundingBox().containsPoint(p)  && !helpImg->isVisible())
    {
        helpImg->setScale(0.9f);
        return true;
    }else if(helpImg->getBoundingBox().containsPoint(p) && helpImg->isVisible() && clickboo)
    {
        return true;
    }
    return false;
}
void HelloWorld::menuEndCallback(cocos2d::Touch* tTouch,cocos2d::Event* eEvent)
{
    Point p = this->convertToNodeSpace(tTouch->getLocation());
    if(startBtn->getBoundingBox().containsPoint(p) && !helpImg->isVisible())
    {
        startBtn->setScale(1.0f);
        
        auto scene = GameScene::createScene();
        Director::getInstance()->replaceScene(TransitionCrossFade::create(1, scene));
        

    }else if(helpBtn->getBoundingBox().containsPoint(p)  && !helpImg->isVisible())
    {
        helpImg->setScale(1.0f);
        showHelp();
    }else if(helpImg->getBoundingBox().containsPoint(p) && helpImg->isVisible() && clickboo)
    {
        clickboo = false;
        float scale = helpImg->getScaleX();
        auto fadeout = FadeOut::create(0.5f);
        
        auto callback = CallFunc::create([&](){
            helpImg->setVisible(false);
        });
        helpImg->runAction(Sequence::create(fadeout,callback, NULL));
    }
}
void HelloWorld::showHelp()
{
    clickboo = false;
    Size visibleSize = Director::getInstance()->getVisibleSize();
    float scale = visibleSize.height/1334;
    
    helpImg->setVisible(true);
    helpImg->setScale(0.0f);
    helpImg->setOpacity(0);
    auto fadein = FadeIn::create(1.0f);
    auto scaleto = ScaleTo::create(1.0f, scale);
    auto swapn = Spawn::createWithTwoActions(fadein, scaleto);
    auto callback = CallFunc::create([&](){
        clickboo = true;
    });
    helpImg->runAction(Sequence::create(swapn,callback, NULL));
}
