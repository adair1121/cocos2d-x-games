//
//  ResultScene.cpp
//  stickMonkey-mobile
//
//  Created by 志强 on 2020/6/5.
//

#include "ResultScene.hpp"
#include "Config.h"
#include "GameScene.hpp"
#include "SoundCtl.h"
Scene* ResultScene::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = ResultScene::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool ResultScene::init()
{
    
    if ( !Layer::init() )
    {
        return false;
    }
    
    auto rootNode = CSLoader::createNode("ResultScene.csb");

    addChild(rootNode);
    bg = static_cast<Sprite*>(rootNode->getChildByName("bg"));
    img = static_cast<Sprite*>(rootNode->getChildByName("img"));
    light = static_cast<Sprite*>(rootNode->getChildByName("light"));
    scoreLab = static_cast<TextBMFont*>(rootNode->getChildByName("scoreLab"));
    scoreBg = static_cast<Sprite*>(rootNode->getChildByName("scoreBg"));
    bestScore = static_cast<TextBMFont*>(rootNode->getChildByName("bestScore"));
    resetBtn = static_cast<Button*>(rootNode->getChildByName("resetBtn"));
    
    scoreLab->setString(StringUtils::format("%d",CF()->score));
    bestScore->setString(StringUtils::format("%d",CF()->bestScore));
    
    auto rotate = RotateBy::create(3.0f, 360);
    light->runAction(RepeatForever::create(rotate));
    
    _listener=EventListenerTouchOneByOne::create();
    _listener->setSwallowTouches(true);
    _listener->onTouchBegan = CC_CALLBACK_2(ResultScene::menuBegin,this);
    _listener->onTouchEnded = CC_CALLBACK_2(ResultScene::menuEndCallback, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(_listener, this);
    
    this->adapter();
    return true;
}
void ResultScene::adapter()
{
    int bgW = bg->getContentSize().width;
    int bgH = bg->getContentSize().height;
    Size visibleSize = Director::getInstance()->getVisibleSize();
    float scaleX = visibleSize.width/bgW;
    float scaleY = visibleSize.height/bgH;
    bg->setScale(scaleX, scaleY);
    img->setPosition(Vec2(img->getPositionX()*scaleX,img->getPositionY()*scaleY));
    light->setPosition(Vec2(light->getPositionX()*scaleX,light->getPositionY()*scaleY));
    scoreLab->setPosition(Vec2(scoreLab->getPositionX()*scaleX,scoreLab->getPositionY()*scaleY));
    scoreBg->setPosition(Vec2(scoreBg->getPositionX()*scaleX,scoreBg->getPositionY()*scaleY));
    bestScore->setPosition(Vec2(bestScore->getPositionX()*scaleX,bestScore->getPositionY()*scaleY));
    resetBtn->setPosition(Vec2(resetBtn->getPositionX()*scaleX,resetBtn->getPositionY()*scaleY));
//    audioBtn->setPositionX(audioBtn->getPositionX()*scaleX);
}
bool ResultScene::menuBegin(cocos2d::Touch* tTouch,cocos2d::Event* eEvent)
{
    Point localP = this->convertToNodeSpace(tTouch->getLocation());
    if(resetBtn->getBoundingBox().containsPoint(localP))
    {
        resetBtn->setBrightStyle(BRIGHT_HIGHLIGHT);
        return true;
    }
    return false;
}
void ResultScene::menuEndCallback(cocos2d::Touch* tTouch,cocos2d::Event* eEvent)
{
    Point localP = this->convertToNodeSpace(tTouch->getLocation());
    
    if(resetBtn->getBoundingBox().containsPoint(localP))
    {
        SoundCtl::getInstance()->playEffect("sound/click.m4a");
        resetBtn->setBrightStyle(BRIGHT_NORMAL);
        auto scene = GameScene::createScene();
        Director::getInstance()->replaceScene(TransitionCrossFade::create(1, scene));
    }

}
