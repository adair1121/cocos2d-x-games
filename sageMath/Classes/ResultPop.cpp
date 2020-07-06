//
//  ResultPop.cpp
//  sageMath-mobile
//
//  Created by 志强 on 2020/5/21.
//

#include "ResultPop.h"

ResultPop::ResultPop(int state)
{
    auto rootNode = CSLoader::createNode("ResultPop.csb");

    addChild(rootNode);
    
    bg = static_cast<Sprite*>(rootNode->getChildByName("bg"));
    failGroup = static_cast<Layout*>(rootNode->getChildByName("failGroup"));
    backBtn = static_cast<Button*>(failGroup->getChildByName("backBtn"));
    retryBtn = static_cast<Button*>(failGroup->getChildByName("retryBtn"));
   
    winGroup = static_cast<Layout*>(rootNode->getChildByName("winGroup"));
//    retryBtn2 = static_cast<Button*>(winGroup->getChildByName("retryBtn2"));
    nextBtn = static_cast<Button*>(winGroup->getChildByName("nextBtn"));
    
    _listener=EventListenerTouchOneByOne::create();
    _listener->setSwallowTouches(true);
    _listener->onTouchBegan = CC_CALLBACK_2(ResultPop::menuBegin,this);
    _listener->onTouchEnded = CC_CALLBACK_2(ResultPop::menuEndCallback, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(_listener, this);
    
    
    winGroup->setVisible(state == 1?true:false);
    failGroup->setVisible(state == 1?false:true);
    
    
    this->adapter();
}
void ResultPop::adapter()
{
    int bgW = bg->getContentSize().width;
    int bgH = bg->getContentSize().height;
    Size visibleSize = Director::getInstance()->getVisibleSize();
    float scaleX = visibleSize.width/bgW;
    float scaleY = visibleSize.height/bgH;
    bg->setScale(scaleX, scaleY);
    winGroup->setScale(scaleX);
    failGroup->setScale(scaleX);
    winGroup->setPosition(Vec2(winGroup->getPositionX()*scaleX,winGroup->getPositionY()*scaleY));
    failGroup->setPosition(Vec2(failGroup->getPositionX()*scaleX,failGroup->getPositionY()*scaleY));
}

bool ResultPop::menuBegin(cocos2d::Touch* tTouch,cocos2d::Event* eEvent)
{
    Point localP = winGroup->convertToNodeSpace(tTouch->getLocation());
    Point failP = failGroup->convertToNodeSpace(tTouch->getLocation());
    if(nextBtn->getBoundingBox().containsPoint(localP) && winGroup->isVisible())
    {
        nextBtn->setBrightStyle(BRIGHT_HIGHLIGHT);
        return true;
    }else if(retryBtn->getBoundingBox().containsPoint(localP) && failGroup->isVisible())
    {
        retryBtn->setBrightStyle(BRIGHT_HIGHLIGHT);
        return true;
    }else if(backBtn->getBoundingBox().containsPoint(localP) && failGroup->isVisible())
    {
        backBtn->setBrightStyle(BRIGHT_HIGHLIGHT);
        return true;
    }
    return false;
}
void ResultPop::menuEndCallback(cocos2d::Touch* tTouch,cocos2d::Event* eEvent)
{
    Point localP = winGroup->convertToNodeSpace(tTouch->getLocation());
    Point failP = failGroup->convertToNodeSpace(tTouch->getLocation());
     if(nextBtn->getBoundingBox().containsPoint(localP) && winGroup->isVisible())
    {
        nextBtn->setBrightStyle(BRIGHT_NORMAL);
        this->removeFromParent();
        resetFunc();
        
    }else if(retryBtn->getBoundingBox().containsPoint(localP) && failGroup->isVisible())
    {
        retryBtn->setBrightStyle(BRIGHT_NORMAL);
        this->removeFromParent();
        resetFunc();
    }else if(backBtn->getBoundingBox().containsPoint(localP) && failGroup->isVisible())
    {
        backBtn->setBrightStyle(BRIGHT_NORMAL);
        this->removeFromParent();
        backFunc();
    }
}
ResultPop::~ResultPop()
{
    _eventDispatcher->removeEventListener(_listener);
    this->release();
}
