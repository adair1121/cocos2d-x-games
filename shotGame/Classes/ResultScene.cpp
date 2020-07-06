//
//  ResultScene.cpp
//  shotGame-mobile
//
//  Created by 志强 on 2020/5/20.
//

#include "ResultScene.h"
#include "GameScene.h"
#include "SoundCtl.h"
#include "Config.h"

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
    
    resetBtn = static_cast<Button*>(rootNode->getChildByName("resetBtn"));
    
    scoreLab= static_cast<TextBMFont*>(rootNode->getChildByName("scoreLab"));
    scorebg = static_cast<Sprite*>(rootNode->getChildByName("scorebg"));
    bg = static_cast<Sprite*>(rootNode->getChildByName("bg"));
    
    _listener=EventListenerTouchOneByOne::create();
    _listener->setSwallowTouches(true);
    _listener->onTouchBegan = CC_CALLBACK_2(ResultScene::menuBegin,this);
    _listener->onTouchEnded = CC_CALLBACK_2(ResultScene::menuEndCallback, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(_listener, this);
    
    scoreLab->setString(StringUtils::format("SCORE:%d",Config::getInstance()->score));
    
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
    
    resetBtn->setPosition(Vec2(resetBtn->getPositionX()*scaleX,resetBtn->getPositionY()*scaleY));
    scoreLab->setPosition(Vec2(scoreLab->getPositionX()*scaleX,scoreLab->getPositionY()*scaleY));
    scorebg->setPosition(Vec2(scorebg->getPositionX()*scaleX,scorebg->getPositionY()*scaleY));
}
void ResultScene::onExit()
{
    Layer::onExit();
    _eventDispatcher->removeEventListener(_listener);
}
bool ResultScene::menuBegin(cocos2d::Touch* tTouch,cocos2d::Event* eEvent)
{
    Point localP = this->convertToNodeSpace(tTouch->getLocation());
    if(resetBtn->getBoundingBox().containsPoint(localP)){
       return true;
    }
    return false;
}
void ResultScene::menuEndCallback(cocos2d::Touch* tTouch,cocos2d::Event* eEvent)
{
    Point localP = this->convertToNodeSpace(tTouch->getLocation());
    
    if(resetBtn->getBoundingBox().containsPoint(localP)){
        SoundCtl::getInstance()->playEffect("sound/click.m4a");
        auto scene = GameScene::createScene();
        Director::getInstance()->replaceScene(TransitionFade::create(1, scene));
        return;
    }
}
