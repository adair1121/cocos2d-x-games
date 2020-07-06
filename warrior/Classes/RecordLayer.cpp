//
//  RecordLayer.cpp
//  warrior-mobile
//
//  Created by 志强 on 2020/6/12.
//

#include "RecordLayer.hpp"
#include "Config.h"

RecordLayer::RecordLayer()
{
    auto rootNode = CSLoader::createNode("RecordLayer.csb");

    addChild(rootNode);
   
    rect = static_cast<Layout*>(rootNode->getChildByName("rect"));
    contentGroup = static_cast<Layout*>(rootNode->getChildByName("contentGroup"));
    time1lab = static_cast<Text*>(contentGroup->getChildByName("time1lab"));
    time2lab = static_cast<Text*>(contentGroup->getChildByName("time2lab"));
    time3lab = static_cast<Text*>(contentGroup->getChildByName("time3lab"));
    closeBtn = static_cast<Sprite*>(contentGroup->getChildByName("closeBtn"));
    
    time1lab->setString(StringUtils::format("%ds",CF()->bestScore1));
    time2lab->setString(StringUtils::format("%ds",CF()->bestScore2));
    time3lab->setString(StringUtils::format("%ds",CF()->bestScore3));
   
    _listener=EventListenerTouchOneByOne::create();
    _listener->setSwallowTouches(true);
    _listener->onTouchBegan = CC_CALLBACK_2(RecordLayer::menuBegin,this);
    _listener->onTouchEnded = CC_CALLBACK_2(RecordLayer::menuEndCallback, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(_listener, this);
   
    this->adapter();
}
void RecordLayer::adapter()
{
    int bgW = 750;
    int bgH = 1334;
    Size visibleSize = Director::getInstance()->getVisibleSize();
    float scaleX = visibleSize.width/bgW;
    float scaleY = visibleSize.height/bgH;
    rect->setScale(scaleX,scaleY);
    rect->setPosition(Vec2(0,0));
    contentGroup->setScale(scaleX);
    contentGroup->setAnchorPoint(Vec2(0.5,0.5));
    contentGroup->setPosition(Vec2(visibleSize.width/2,visibleSize.height/2));
}
bool RecordLayer::menuBegin(cocos2d::Touch* tTouch,cocos2d::Event* eEvent)
{
    Point p = this->convertToNodeSpace(tTouch->getLocation());
    if(contentGroup->getBoundingBox().containsPoint(p))
    {
        Point localP = contentGroup->convertToNodeSpace(tTouch->getLocation());
        if(closeBtn->getBoundingBox().containsPoint(localP))
        {
            closeBtn->setScale(0.9f);
            return true;
        }
    }
    return false;
}
void RecordLayer::menuEndCallback(cocos2d::Touch* tTouch,cocos2d::Event* eEvent)
{
    Point p = this->convertToNodeSpace(tTouch->getLocation());
    if(contentGroup->getBoundingBox().containsPoint(p))
    {
        Point localP = contentGroup->convertToNodeSpace(tTouch->getLocation());
        if(closeBtn->getBoundingBox().containsPoint(localP))
        {
            closeBtn->setScale(1.0f);
            this->removeFromParent();
        }
    }
}
RecordLayer::~RecordLayer()
{
    _eventDispatcher->removeEventListener(_listener);
}
