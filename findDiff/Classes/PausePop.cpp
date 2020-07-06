//
//  PausePop.cpp
//  FindDiff-mobile
//
//  Created by 志强 on 2020/5/15.
//

#include "PausePop.h"
#include "SoundCtl.h"

PausePop::PausePop()
{
    auto rootNode = CSLoader::createNode("PausePop.csb");

    addChild(rootNode);
    
    playBtn = static_cast<Sprite*>(rootNode->getChildByName("playBtn"));
    Size visibleSize = Director::getInstance()->getVisibleSize();
    playBtn->setPosition(Vec2(visibleSize.width/2,visibleSize.height/2));
    
    _listener=EventListenerTouchOneByOne::create();
    _listener->setSwallowTouches(true);
    _listener->onTouchBegan = CC_CALLBACK_2(PausePop::menuBegin,this);
    _listener->onTouchEnded = CC_CALLBACK_2(PausePop::menuEndCallback, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(_listener, this);
}
bool PausePop::menuBegin(cocos2d::Touch* tTouch,cocos2d::Event* eEvent)
{
    Point localP = this->convertToNodeSpace(tTouch->getLocation());
    if(playBtn->getBoundingBox().containsPoint(localP))
    {
        return true;
    }
    return false;
}
   
void PausePop::menuEndCallback(cocos2d::Touch* tTouch,cocos2d::Event* eEvent)
{
    Point localP = this->convertToNodeSpace(tTouch->getLocation());
    if(playBtn->getBoundingBox().containsPoint(localP))
    {
        SC()->playEffect("sound/click.m4a");
        callFunc();
        this->removeFromParent();
        this->release();
    }
}
PausePop::~PausePop()
{
    _eventDispatcher->removeEventListener(_listener);
}
