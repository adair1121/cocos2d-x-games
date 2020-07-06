//
//  ResultLayer.cpp
//  chuanqiGame-mobile
//
//  Created by 志强 on 2020/6/11.
//

#include "ResultLayer.hpp"
#include "Config.h"
#include "HomeLayer.hpp"

ResultLayer::ResultLayer(int state)
{
    auto rootNode = CSLoader::createNode("ResultLayer.csb");

    addChild(rootNode);
    
    winGroup = static_cast<Layout*>(rootNode->getChildByName("winGroup"));
    failGroup = static_cast<Layout*>(rootNode->getChildByName("failGroup"));
    rect = static_cast<Layout*>(rootNode->getChildByName("rect"));
    returnBtn = static_cast<Sprite*>(failGroup->getChildByName("returnBtn"));
    sureBtn = static_cast<Sprite*>(winGroup->getChildByName("sureBtn"));
    goldNumLab = static_cast<Text*>(winGroup->getChildByName("goldNumLab"));
    
    _listener=EventListenerTouchOneByOne::create();
    _listener->setSwallowTouches(true);
    _listener->onTouchBegan = CC_CALLBACK_2(ResultLayer::menuBegin,this);
    _listener->onTouchEnded = CC_CALLBACK_2(ResultLayer::menuEndCallback, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(_listener, this);
    
    winGroup->setVisible(state == 1);
    if(state == 1)
    {
        srand(int(time(0))+ rand());//置随机数种子
        int goldnum = CCRANDOM_0_1()*30+120;
        CF()->gold += goldnum;
        goldNumLab->setString(StringUtils::format("%d",goldnum));
    }
    failGroup->setVisible(state == 0);
    
    this->adapter();
}
void ResultLayer::adapter()
{
    int bgW = 750;
    int bgH = 1334;
    Size visibleSize = Director::getInstance()->getVisibleSize();
    float scaleX = visibleSize.width/bgW;
    float scaleY = visibleSize.height/bgH;
    rect->setScale(scaleX,scaleY);
    winGroup->setScale(scaleX);
    failGroup->setScale(scaleX);
    winGroup->setPosition(Vec2(winGroup->getPositionX()*scaleX,winGroup->getPositionY()*scaleY));
    failGroup->setPosition(Vec2(failGroup->getPositionX()*scaleX,failGroup->getPositionY()*scaleY));
}
bool ResultLayer::menuBegin(cocos2d::Touch* tTouch,cocos2d::Event* eEvent)
{
    Point p = this->convertToNodeSpace(tTouch->getLocation());
    if(failGroup->getBoundingBox().containsPoint(p) && failGroup->isVisible())
    {
        Point groupP = winGroup->convertToNodeSpace(tTouch->getLocation());
        if(returnBtn->getBoundingBox().containsPoint(groupP))
        {
            returnBtn->setScale(0.9f);
            return true;
        }
    }else if(winGroup->getBoundingBox().containsPoint(p) && winGroup->isVisible())
    {
        Point groupP = winGroup->convertToNodeSpace(tTouch->getLocation());
        if(sureBtn->getBoundingBox().containsPoint(groupP))
        {
            sureBtn->setScale(0.9f);
            return true;
        }
    }
    return false;
}

void ResultLayer::menuEndCallback(cocos2d::Touch* tTouch,cocos2d::Event* eEvent)
{
    Point p = this->convertToNodeSpace(tTouch->getLocation());
    if(failGroup->getBoundingBox().containsPoint(p) && failGroup->isVisible())
    {
        Point groupP = winGroup->convertToNodeSpace(tTouch->getLocation());
        if(returnBtn->getBoundingBox().containsPoint(groupP))
        {
            returnBtn->setScale(1.0f);
            this->removeFromParent();
            auto scene = HomeLayer::createScene();
            Director::getInstance()->replaceScene(TransitionFadeTR::create(1, scene));
        }
    }else if(winGroup->getBoundingBox().containsPoint(p) && winGroup->isVisible())
    {
        Point groupP = winGroup->convertToNodeSpace(tTouch->getLocation());
        if(sureBtn->getBoundingBox().containsPoint(groupP))
        {
            sureBtn->setScale(1.0f);
            this->removeFromParent();
            auto scene = HomeLayer::createScene();
            Director::getInstance()->replaceScene(TransitionFadeTR::create(1, scene));
        }
    }
    
}
ResultLayer::~ResultLayer()
{
    _eventDispatcher->removeEventListener(_listener);
}
