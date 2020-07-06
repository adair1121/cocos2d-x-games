//
//  ResultPop.cpp
//  chuanqiStory-mobile
//
//  Created by 志强 on 2020/5/25.
//

#include "ResultPop.h"
#include "Config.h"

ResultPop::ResultPop(int state)
{
    auto rootNode = CSLoader::createNode("ResultPop.csb");

    addChild(rootNode);
    winGroup = static_cast<Layout*>(rootNode->getChildByName("winGroup"));
    failGroup = static_cast<Layout*>(rootNode->getChildByName("failGroup"));
    pointLab = static_cast<Text*>(failGroup->getChildByName("pointLab"));
    resetBtn = static_cast<Sprite*>(winGroup->getChildByName("resetBtn"));
    nextBtn = static_cast<Sprite*>(winGroup->getChildByName("nextBtn"));
    
    againBtn = static_cast<Sprite*>(failGroup->getChildByName("againBtn"));
    homeBtn = static_cast<Sprite*>(failGroup->getChildByName("homeBtn"));
    bg = static_cast<Layout*>(rootNode->getChildByName("bg"));
    
    _listener=EventListenerTouchOneByOne::create();
    _listener->setSwallowTouches(true);
    _listener->onTouchBegan = CC_CALLBACK_2(ResultPop::menuBegin,this);
    _listener->onTouchEnded = CC_CALLBACK_2(ResultPop::menuEndCallback, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(_listener, this);
    winGroup->setVisible(false);
    failGroup->setVisible(true);
    if(state == 1)
    {
        srand(int(time(0))+ rand());//置随机数种子
        int point = CCRANDOM_0_1()*100+100;
        CF()->point += point;
        pointLab->setString(StringUtils::format("%d",point));
        if(CF()->challengeLev >= CF()->level)
        {
            CF()->level += 1;
        }
    }else{
        pointLab->setString("0");
    }
    this->adapter();
}
bool ResultPop::menuBegin(cocos2d::Touch* tTouch,cocos2d::Event* eEvent)
{
    Point localP = this->convertToNodeSpace(tTouch->getLocation());
    if(winGroup->getBoundingBox().containsPoint(localP) && winGroup->isVisible())
    {
        Point groupP = winGroup->convertToNodeSpace(tTouch->getLocation());
        if(resetBtn->getBoundingBox().containsPoint(groupP) && winGroup->isVisible())
        {
            resetBtn->setScale(0.9f);
            return true;
        }else if(nextBtn->getBoundingBox().containsPoint(groupP) && winGroup->isVisible())
        {
            nextBtn->setScale(0.9f);
            return true;
        }
    }else if(failGroup->getBoundingBox().containsPoint(localP) && failGroup->isVisible())
    {
        Point groupP = winGroup->convertToNodeSpace(tTouch->getLocation());
        if(againBtn->getBoundingBox().containsPoint(groupP) && failGroup->isVisible())
        {
            againBtn->setScale(0.9f);
            return true;
        }else if(homeBtn->getBoundingBox().containsPoint(groupP) && failGroup->isVisible())
        {
            homeBtn->setScale(0.9f);
            return true;
        }
    }
    
    return false;
}
void ResultPop::menuEndCallback(cocos2d::Touch* tTouch,cocos2d::Event* eEvent)
{
    Point localP = this->convertToNodeSpace(tTouch->getLocation());
    if(winGroup->getBoundingBox().containsPoint(localP) && winGroup->isVisible())
    {
        Point groupP = winGroup->convertToNodeSpace(tTouch->getLocation());
        if(resetBtn->getBoundingBox().containsPoint(groupP) && winGroup->isVisible())
        {
            resetBtn->setScale(1.0f);
            callFunc(0);
        }else if(nextBtn->getBoundingBox().containsPoint(groupP) && winGroup->isVisible())
        {
            nextBtn->setScale(1.0f);
            callFunc(1);
        }
        this->removeFromParent();
    }else if(failGroup->getBoundingBox().containsPoint(localP) && failGroup->isVisible())
    {
        Point groupP = winGroup->convertToNodeSpace(tTouch->getLocation());
        if(againBtn->getBoundingBox().containsPoint(groupP) && failGroup->isVisible())
        {
            againBtn->setScale(1.0f);
            callFunc(0);
            
        }else if(homeBtn->getBoundingBox().containsPoint(groupP) && failGroup->isVisible())
        {
            homeBtn->setScale(1.0f);
            callFunc(2);
        }
        this->removeFromParent();
    }
}
void ResultPop::adapter()
{
    int bgW = bg->getContentSize().width;
    int bgH = bg->getContentSize().height;
    Size visibleSize = Director::getInstance()->getVisibleSize();
    float scaleX = visibleSize.width/bgW;
    float scaleY = visibleSize.height/bgH;
    bg->setScale(scaleX, scaleY);
    winGroup->setPosition(Vec2(winGroup->getPositionX()*scaleX,winGroup->getPositionY()*scaleY));
    failGroup->setPosition(Vec2(failGroup->getPositionX()*scaleX,failGroup->getPositionY()*scaleY));
}
ResultPop::~ResultPop()
{
    _eventDispatcher->removeEventListener(_listener);
}
