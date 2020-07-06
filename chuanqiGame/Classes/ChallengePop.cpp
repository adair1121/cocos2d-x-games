//
//  ChallengePop.cpp
//  chuanqiGame-mobile
//
//  Created by 志强 on 2020/6/11.
//

#include "ChallengePop.hpp"
#include "Config.h"
#include "ChallengeItem.hpp"
#include "CommonUtil.h"
#include "BattleLayer.hpp"

ChallengePop::ChallengePop()
{
    auto rootNode = CSLoader::createNode("ChallengePop.csb");

    addChild(rootNode);
    rect = static_cast<Layout*>(rootNode->getChildByName("rect"));
    contentGroup = static_cast<Layout*>(rootNode->getChildByName("contentGroup"));
    closeBtn = static_cast<Sprite*>(contentGroup->getChildByName("closeBtn"));
    scroller = static_cast<ScrollView*>(contentGroup->getChildByName("scroller"));
    
    _listener=EventListenerTouchOneByOne::create();
    _listener->setSwallowTouches(true);
    _listener->onTouchBegan = CC_CALLBACK_2(ChallengePop::menuBegin,this);
    _listener->onTouchEnded = CC_CALLBACK_2(ChallengePop::menuEndCallback, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(_listener, contentGroup);
    
    int col = 1;
    int interval = 20;
    int itemH = 160;
    float numchidren =6;
    float num = float((numchidren)/1.0f);
    int scrollH =  ceil(num)*(itemH + interval);
    int scrollHeight = scroller->getContentSize().height;
    if(scrollH <= scrollHeight)
    {
        scrollH = scrollHeight;
    }
    continerLayer = Layer::create();
    continerLayer->setContentSize(Size(scroller->getContentSize().width,scrollH));
    continerLayer->setAnchorPoint(Vec2(0,1));
    
    continerLayer->setPosition(Point(0,0));
    
    for(int i = 0;i<6;i++)
    {
        
        ChallengeItem*item = new ChallengeItem((i+1));
        continerLayer->addChild(item);
        
        int y =  scrollH - ((i/col)>>0)*(item->getContentSize().height + interval) - itemH;
        item->setPosition(Vec2(0,y));
    }
    scroller->setSwallowTouches(false);
    scroller->addChild(continerLayer);
    scroller->setInnerContainerSize(Size(scroller->getContentSize().width,scrollH));
    scroller->setScrollBarEnabled(false);
    this->adapter();
}
void ChallengePop::adapter()
{
    int bgW = 750;
    int bgH = 1334;
    Size visibleSize = Director::getInstance()->getVisibleSize();
    float scaleX = visibleSize.width/bgW;
    float scaleY = visibleSize.height/bgH;
    rect->setScale(scaleX,scaleY);
    contentGroup->setScale(scaleX);
    contentGroup->setPosition(Vec2(visibleSize.width/2,visibleSize.height/2));
}
bool ChallengePop::menuBegin(cocos2d::Touch* tTouch,cocos2d::Event* eEvent)
{
    Point p = contentGroup->convertToNodeSpace(tTouch->getLocation());
    if(closeBtn->getBoundingBox().containsPoint(p))
    {
        closeBtn->setScale(1.4f);
        return true;
    }else if(scroller->getBoundingBox().containsPoint(p))
    {
        Vector<Node*> childrens = continerLayer->getChildren();
        for(auto c : childrens)
        {
            ChallengeItem*item = static_cast<ChallengeItem*>(c);
            Point itemP = item->convertToNodeSpace(tTouch->getLocation());
            if(item->challengeBtn->getBoundingBox().containsPoint(itemP))
            {
                return true;
            }
        }
    }
    return false;
}

void ChallengePop::menuEndCallback(cocos2d::Touch* tTouch,cocos2d::Event* eEvent)
{
    Point p = contentGroup->convertToNodeSpace(tTouch->getLocation());
    if(closeBtn->getBoundingBox().containsPoint(p))
    {
        closeBtn->setScale(1.5f);
        this->removeFromParent();
    }else if(scroller->getBoundingBox().containsPoint(p))
    {
        Vector<Node*> childrens = continerLayer->getChildren();
        int index = 0;
        for(auto c : childrens)
        {
            index += 1;
            ChallengeItem*item = static_cast<ChallengeItem*>(c);
            Point itemP = item->convertToNodeSpace(tTouch->getLocation());
            if(item->challengeBtn->getBoundingBox().containsPoint(itemP))
            {
                if(CF()->t_chapter >= index)
                {
                    //可以挑战
                    CF()->c_chapter = index;
                    //进入战斗场景
                    this->removeFromParent();
                    auto scene = BattleLayer::createScene();
                    Director::getInstance()->replaceScene(TransitionFadeTR::create(1, scene));
                }else{
                    CommonUtil::floatFont("Please clear the first level", this);
                }
                return;
            }
        }
    }
    
}
ChallengePop::~ChallengePop()
{
    _eventDispatcher->removeEventListener(_listener);
}
