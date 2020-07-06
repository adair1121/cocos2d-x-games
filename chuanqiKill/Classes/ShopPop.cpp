//
//  ShopPop.cpp
//  ChuanqiKill-mobile
//
//  Created by 志强 on 2020/5/28.
//

#include "ShopPop.hpp"
#include "Date.h"

ShopPop::ShopPop()
{
    auto rootNode = CSLoader::createNode("ShopPop.csb");

    addChild(rootNode);
    
    bg = static_cast<Sprite*>(rootNode->getChildByName("bg"));
    contentGroup = static_cast<Layout*>(rootNode->getChildByName("contentGroup"));
    closeBtn = static_cast<Sprite*>(contentGroup->getChildByName("closeBtn"));
    buy1 = static_cast<Sprite*>(contentGroup->getChildByName("buy1"));
    buy2 = static_cast<Sprite*>(contentGroup->getChildByName("buy2"));
    buy3 = static_cast<Sprite*>(contentGroup->getChildByName("buy3"));
    
    _listener=EventListenerTouchOneByOne::create();
    _listener->setSwallowTouches(true);
    _listener->onTouchBegan = CC_CALLBACK_2(ShopPop::menuBegin,this);
    _listener->onTouchEnded = CC_CALLBACK_2(ShopPop::menuEndCallback, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(_listener, this);
}
bool ShopPop::menuBegin(cocos2d::Touch* tTouch,cocos2d::Event* eEvent)
{
    Point localP = contentGroup->convertToNodeSpace(tTouch->getLocation());
    if(buy1->getBoundingBox().containsPoint(localP))
    {
        buy1->setScale(0.9f);
        return true;
    }else if(buy2->getBoundingBox().containsPoint(localP))
    {
        buy2->setScale(0.9f);
        return true;
    }else if(buy3->getBoundingBox().containsPoint(localP))
    {
        buy3->setScale(0.9f);
        return true;
    }
    else if(closeBtn->getBoundingBox().containsPoint(localP))
    {
        closeBtn->setScale(0.9f);
        return true;
    }
    return false;
}
void ShopPop::menuEndCallback(cocos2d::Touch* tTouch,cocos2d::Event* eEvent)
{
    Point localP = contentGroup->convertToNodeSpace(tTouch->getLocation());
    if(buy1->getBoundingBox().containsPoint(localP))
    {
        buy1->setScale(1.0f);
        this->touchBuy(1);
    }else if(buy2->getBoundingBox().containsPoint(localP))
    {
        buy2->setScale(1.0f);
        this->touchBuy(2);
        
    }else if(buy3->getBoundingBox().containsPoint(localP))
    {
        buy3->setScale(1.0f);
        this->touchBuy(3);
    }else if(closeBtn->getBoundingBox().containsPoint(localP))
    {
        closeBtn->setScale(1.0f);
        this->removeFromParent();
        if(touchBuyCall)
        {
            touchBuyCall();
        }
    }
}
void ShopPop::touchBuy(int index)
{
    int costGold = index == 1?75:index == 2?185:275;
    if(DT()->gold < costGold)
    {
        this->floatFont("Not enough gold");
        return;
    }else{
        DT()->gold -= costGold;
        (index == 1)?(DT()->skill1num +=1):(index == 2)?(DT()->skill2num+=1):(DT()->skill3num+=1);
        this->floatFont("Successful purchase");
    }
}
void ShopPop::adapter()
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
ShopPop::~ShopPop()
{
    _eventDispatcher->removeEventListener(_listener);
}
void ShopPop::floatFont(std::string cnt)
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
