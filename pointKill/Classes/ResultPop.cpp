//
//  ResultPop.cpp
//  pointKill-mobile
//
//  Created by 志强 on 2020/5/18.
//

#include "ResultPop.h"
#include "SoundCtl.h"
#include "HelloWorldScene.h"
#include "Config.h"
ResultPop::ResultPop(int state)
{
    auto rootNode = CSLoader::createNode("ResultPop.csb");
    addChild(rootNode);
    bg = static_cast<Layout*>(rootNode->getChildByName("bg"));
    
    failGroup = static_cast<Layout*>(rootNode->getChildByName("failGroup"));
    mainBtn = static_cast<Sprite*>(failGroup->getChildByName("mainBtn"));
    resetBtn = static_cast<Sprite*>(failGroup->getChildByName("resetBtn"));
    
    winGroup = static_cast<Layout*>(rootNode->getChildByName("winGroup"));
    mainBtn2 = static_cast<Sprite*>(winGroup->getChildByName("mainBtn2"));
    nextBtn = static_cast<Sprite*>(winGroup->getChildByName("nextBtn"));
    
    
    winGroup->setVisible(state == 1);
    failGroup->setVisible(!(state == 1));
   
    
    _listener=EventListenerTouchOneByOne::create();
    _listener->setSwallowTouches(true);
    _listener->onTouchBegan = CC_CALLBACK_2(ResultPop::menuBegin,this);
    _listener->onTouchEnded = CC_CALLBACK_2(ResultPop::menuEndCallback, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(_listener, this);
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
    failGroup->setScale(scaleY);
    failGroup->setPosition(Vec2(failGroup->getPositionX()*scaleX,failGroup->getPositionY()*scaleY));
    winGroup->setScale(scaleY);
    winGroup->setPosition(Vec2(winGroup->getPositionX()*scaleX,winGroup->getPositionY()*scaleY));
}
bool ResultPop::menuBegin(cocos2d::Touch* tTouch,cocos2d::Event* eEvent)
{
    Point localP = failGroup->convertToNodeSpace(tTouch->getLocation());
    Point localP2 = winGroup->convertToNodeSpace(tTouch->getLocation());
    if((mainBtn->getBoundingBox().containsPoint(localP) && failGroup->isVisible()) || (mainBtn2->getBoundingBox().containsPoint(localP2) && winGroup->isVisible())){
       mainBtn->setScale(0.9f);
       return true;
    }else if(resetBtn->getBoundingBox().containsPoint(localP) && failGroup->isVisible())
    {
        resetBtn->setScale(0.9f);
        return true;
    }else if(nextBtn->getBoundingBox().containsPoint(localP2) && winGroup->isVisible())
    {
        nextBtn->setScale(0.9f);
        return true;
    }
    return false;
}
void ResultPop::menuEndCallback(cocos2d::Touch* tTouch,cocos2d::Event* eEvent)
{
    Point localP = failGroup->convertToNodeSpace(tTouch->getLocation());
    Point localP2 = winGroup->convertToNodeSpace(tTouch->getLocation());
    if((mainBtn->getBoundingBox().containsPoint(localP) && failGroup->isVisible()) || (mainBtn2->getBoundingBox().containsPoint(localP2) && winGroup->isVisible())){
        mainBtn->setScale(1.0f);
        this->removeFromParent();
        SoundCtl::getInstance()->playEffect("sound/click.m4a");
        auto scene = HelloWorld::createScene();
        Director::getInstance()->replaceScene(TransitionTurnOffTiles::create(1, scene));
        return;
    }else if(resetBtn->getBoundingBox().containsPoint(localP) && failGroup->isVisible())
    {
        resetBtn->setScale(1.0f);
        this->removeFromParent();
        SoundCtl::getInstance()->playEffect("sound/click.m4a");
        resetFun();
    }else if(nextBtn->getBoundingBox().containsPoint(localP2) && winGroup->isVisible())
    {
        nextBtn->setScale(1.0f);
        if(Config::getInstance()->level >= 5)
        {
            this->floatFont("This is the max level");
            return;
        }
        this->removeFromParent();
        SoundCtl::getInstance()->playEffect("sound/click.m4a");
        
        resetFun();
    }
}
void ResultPop::floatFont(std::string cnt)
{
    Text*content = Text::create();
    this->addChild(content);
    content->setString(cnt);
    content->setFontSize(30);
    content->setColor(Color3B(255,255,255));
    Size visible = Director::getInstance()->getVisibleSize();
    content->setPosition(Vec2(visible.width/2,visible.height/2));
    auto move = MoveTo::create(1.0f, Vec2(content->getPositionX(),content->getPositionY() + 100));
    auto call = CallFunc::create([=](){
        content->removeFromParent();
    });
    auto seque = Sequence::create(move,call, NULL);
    content->runAction(seque);
}
ResultPop::~ResultPop()
{
    _eventDispatcher->removeEventListener(_listener);
    this->release();
}
