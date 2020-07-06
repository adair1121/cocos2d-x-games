//
//  LevelLayer.cpp
//  warrior-mobile
//
//  Created by 志强 on 2020/6/12.
//

#include "LevelLayer.hpp"
#include "GameScene.hpp"
#include "HelloWorldScene.h"
#include "Config.h"
Scene* LevelLayer::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = LevelLayer::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool LevelLayer::init()
{
    
    if ( !Layer::init() )
    {
        return false;
    }
    
    auto rootNode = CSLoader::createNode("LevelLayer.csb");

    addChild(rootNode);
    
    bg = static_cast<Sprite*>(rootNode->getChildByName("bg"));
    level1 = static_cast<Sprite*>(rootNode->getChildByName("level1"));
    level2 = static_cast<Sprite*>(rootNode->getChildByName("level2"));
    level3 = static_cast<Sprite*>(rootNode->getChildByName("level3"));
    returnBtn = static_cast<Sprite*>(rootNode->getChildByName("returnBtn"));
    
    _listener=EventListenerTouchOneByOne::create();
    _listener->setSwallowTouches(true);
    _listener->onTouchBegan = CC_CALLBACK_2(LevelLayer::menuBegin,this);
    _listener->onTouchEnded = CC_CALLBACK_2(LevelLayer::menuEndCallback, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(_listener, this);
    
    this->adapter();
    return true;
}
void LevelLayer::adapter()
{
    int bgW = 750;
    int bgH = 1334;
    Size visibleSize = Director::getInstance()->getVisibleSize();
    float scaleX = visibleSize.width/bgW;
    float scaleY = visibleSize.height/bgH;
    bg->setScale(scaleX);
    bg->setAnchorPoint(Vec2(0,1));
    bg->setPosition(Vec2(0,visibleSize.height));
    level1->setScale(scaleX);
    level1->setPosition(Vec2(level1->getPositionX()*scaleX,level1->getPositionY()*scaleY));
    level2->setScale(scaleX);
    level2->setPosition(Vec2(level2->getPositionX()*scaleX,level2->getPositionY()*scaleY));
    level3->setScale(scaleX);
    level3->setPosition(Vec2(level3->getPositionX()*scaleX,level3->getPositionY()*scaleY));
    returnBtn->setPosition(Vec2(returnBtn->getPositionX()*scaleX,returnBtn->getPositionY()*scaleY));
}
bool LevelLayer::menuBegin(cocos2d::Touch* tTouch,cocos2d::Event* eEvent)
{
    Point p = this->convertToNodeSpace(tTouch->getLocation());
    if(returnBtn->getBoundingBox().containsPoint(p))
    {
        returnBtn->setScale(0.9f);
        return true;
    }else if(level1->getBoundingBox().containsPoint(p))
    {
        level1->setScale(0.9f);
        return true;
    }else if(level2->getBoundingBox().containsPoint(p))
    {
        level2->setScale(0.9f);
        return true;
    }else if(level3->getBoundingBox().containsPoint(p))
    {
        level3->setScale(0.9f);
        return true;
    }
    return false;
}
void LevelLayer::menuEndCallback(cocos2d::Touch* tTouch,cocos2d::Event* eEvent)
{
    Point p = this->convertToNodeSpace(tTouch->getLocation());
    if(returnBtn->getBoundingBox().containsPoint(p))
    {
        returnBtn->setScale(1.0f);
        auto scene = HelloWorld::createScene();
        Director::getInstance()->replaceScene(TransitionCrossFade::create(1, scene));
    }else if(level1->getBoundingBox().containsPoint(p))
    {
        CF()->level = 1;
        level1->setScale(1.0f);
        auto scene = GameScene::createScene();
        Director::getInstance()->replaceScene(TransitionCrossFade::create(1, scene));
    }else if(level2->getBoundingBox().containsPoint(p))
    {
        CF()->level = 2;
        level2->setScale(1.0f);
        auto scene = GameScene::createScene();
        Director::getInstance()->replaceScene(TransitionCrossFade::create(1, scene));
    }else if(level3->getBoundingBox().containsPoint(p))
    {
        CF()->level = 3;
        level3->setScale(1.0f);
        auto scene = GameScene::createScene();
        Director::getInstance()->replaceScene(TransitionCrossFade::create(1, scene));
    }
}
