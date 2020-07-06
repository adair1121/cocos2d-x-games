//
//  ResultScene.cpp
//  ChuanqiKill-mobile
//
//  Created by 志强 on 2020/5/16.
//

#include "ResultScene.h"
#include "SoundCtl.h"
#include "SelectLevel.hpp"
#include "Date.h"

ResultScene::ResultScene()
{
    auto rootNode = CSLoader::createNode("ResultScene.csb");

    addChild(rootNode);
    
    bg = static_cast<Sprite*>(rootNode->getChildByName("bg"));
    contentGroup = static_cast<Layout*>(rootNode->getChildByName("contentGroup"));
    returnBtn = static_cast<Sprite*>(contentGroup->getChildByName("returnBtn"));
    resultBg = static_cast<Sprite*>(contentGroup->getChildByName("resultBg"));
    goldLab = static_cast<TextBMFont*>(contentGroup->getChildByName("goldLab"));
    
    std::string goldstr = StringUtils::format("%d",Date::getInstance()->rewardGold);
    DT()->gold += DT()->rewardGold;
    
    std::string resultBgStr = DT()->battleState?"assets/winbg.png":"assets/failbg.png";
    Texture2D*texture = Director::getInstance()->getTextureCache()->addImage(resultBgStr);
    resultBg->setTexture(texture);
    
    goldLab->setString(goldstr);
    _listener=EventListenerTouchOneByOne::create();
    _listener->setSwallowTouches(true);
    _listener->onTouchBegan = CC_CALLBACK_2(ResultScene::menuBegin,this);
    _listener->onTouchEnded = CC_CALLBACK_2(ResultScene::menuEndCallback, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(_listener, this);
}
ResultScene::~ResultScene()
{
    this->adapter(); _eventDispatcher->removeEventListener(_listener);
}
bool ResultScene::menuBegin(cocos2d::Touch* tTouch,cocos2d::Event* eEvent)
{
    Point localP = contentGroup->convertToNodeSpace(tTouch->getLocation());
    if(returnBtn->getBoundingBox().containsPoint(localP))
    {
        returnBtn->setScale(0.9f);
        return true;
    }
    return false;
}
void ResultScene::menuEndCallback(cocos2d::Touch* tTouch,cocos2d::Event* eEvent)
{
    Point localP = contentGroup->convertToNodeSpace(tTouch->getLocation());
    
    if(returnBtn->getBoundingBox().containsPoint(localP))
    {
        returnBtn->setScale(1.0f);
        SoundCtl::getInstance()->playEffect("sound/click.m4a");
        this->removeFromParent();
        auto scene = SelectLevel::createScene();
        Director::getInstance()->replaceScene(TransitionProgressRadialCW::create(1, scene));
    }
}
void ResultScene::adapter()
{
    int bgW = bg->getContentSize().width;
    int bgH = bg->getContentSize().height;
    Size visibleSize = Director::getInstance()->getVisibleSize();
    float scaleX = visibleSize.width/bgW;
    float scaleY = visibleSize.height/bgH;
    bg->setScale(scaleX, scaleY);
    contentGroup->setScale(scaleX);
    contentGroup->setPosition(Vec2(contentGroup->getPositionX()*scaleX,contentGroup->getPositionY()*scaleY));
}
