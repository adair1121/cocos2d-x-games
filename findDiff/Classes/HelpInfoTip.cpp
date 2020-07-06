//
//  HelpInfoTip.cpp
//  FindDiff-mobile
//
//  Created by 志强 on 2020/5/15.
//

#include "HelpInfoTip.h"
#include "SoundCtl.h"

HelpInfoTip::HelpInfoTip()
{
    auto rootNode = CSLoader::createNode("HelpInfoTip.csb");

    addChild(rootNode);
    
    
    infoPop = static_cast<Sprite*>(rootNode->getChildByName("infoPop"));
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    infoPop->setPosition(Vec2(visibleSize.width/2,visibleSize.height/2));
    
    infoPop->setScale(0.0f);
    
    auto scaleTo = ScaleTo::create(1.0f, 1.0f);
    infoPop->runAction(scaleTo);
    
    
    _listener=EventListenerTouchOneByOne::create();
    _listener->setSwallowTouches(true);
    _listener->onTouchBegan = CC_CALLBACK_2(HelpInfoTip::menuBegin,this);
    _listener->onTouchEnded = CC_CALLBACK_2(HelpInfoTip::menuEndCallback, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(_listener, this);
}
bool HelpInfoTip::menuBegin(cocos2d::Touch* tTouch,cocos2d::Event* eEvent)
{
    Point localP = this->convertToNodeSpace(tTouch->getLocation());
    if(infoPop->getBoundingBox().containsPoint(localP))
    {
        return true;
    }
    return false;
}
   
void HelpInfoTip::menuEndCallback(cocos2d::Touch* tTouch,cocos2d::Event* eEvent)
{
    Point localP = this->convertToNodeSpace(tTouch->getLocation());
    if(infoPop->getBoundingBox().containsPoint(localP))
    {
        SC()->playEffect("sound/click.m4a");
        auto scaleTo = ScaleTo::create(1.0f, 0.0f);
        auto callBack = CallFunc::create([&](){
            callFunc();
            this->removeFromParent();
            this->release();
        });
        auto seque = Sequence::create(scaleTo,callBack, NULL);
        infoPop->runAction(seque);
    }
   
}
HelpInfoTip::~HelpInfoTip()
{
    _eventDispatcher->removeEventListener(_listener);
}
