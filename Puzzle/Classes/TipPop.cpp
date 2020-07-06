//
//  TipPop.cpp
//  Puzzle-mobile
//
//  Created by 志强 on 2020/5/28.
//

#include "TipPop.hpp"
#include "SelectTypeScene.h"

TipPop::TipPop()
{
    auto rootNode = CSLoader::createNode("TipPop.csb");

    addChild(rootNode);
    
    bg = static_cast<Layout*>(rootNode->getChildByName("bg"));
    content = static_cast<Layout*>(rootNode->getChildByName("content"));
    sureBtn = static_cast<Sprite*>(content->getChildByName("sureBtn"));
    
    _listener=EventListenerTouchOneByOne::create();
    _listener->setSwallowTouches(true);
    _listener->onTouchBegan = CC_CALLBACK_2(TipPop::menuBegin,this);
    _listener->onTouchEnded = CC_CALLBACK_2(TipPop::menuEndCallback, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(_listener, this);
    
}
void TipPop::adapter()
{
    int bgW = bg->getContentSize().width;
    int bgH = bg->getContentSize().height;
    Size visibleSize = Director::getInstance()->getVisibleSize();
    float scaleX = visibleSize.width/bgW;
    float scaleY = visibleSize.height/bgH;
    bg->setScale(scaleX, scaleY);
    content->setScale(scaleX);
    content->setPosition(Vec2(content->getPositionX()*scaleX,content->getPositionY()*scaleY));
//    audioBtn->setPositionX(audioBtn->getPositionX()*scaleX);
}
bool TipPop::menuBegin(cocos2d::Touch* tTouch,cocos2d::Event* eEvent)
{
    Point localP = content->convertToNodeSpace(tTouch->getLocation());
    if(sureBtn->getBoundingBox().containsPoint(localP)){
       sureBtn->setScale(0.9f);
       return true;
    }
//    else if(audioBtn->getBoundingBox().containsPoint(localP))
//    {
//        return true;
//    }
    return false;
}
void TipPop::menuEndCallback(cocos2d::Touch* tTouch,cocos2d::Event* eEvent)
{
    Point localP = content->convertToNodeSpace(tTouch->getLocation());
    
    if(sureBtn->getBoundingBox().containsPoint(localP)){
        sureBtn->setScale(1.0f);
        //打开布阵弹窗
        auto scene = SelectTypeScene::createScene();
        Director::getInstance()->replaceScene(TransitionCrossFade::create(1, scene));
        return;
    }

}
TipPop::~TipPop()
{
    _eventDispatcher->removeEventListener(_listener);
}
