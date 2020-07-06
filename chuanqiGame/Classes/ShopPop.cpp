//
//  ShopPop.cpp
//  chuanqiGame-mobile
//
//  Created by 志强 on 2020/6/10.
//

#include "ShopPop.hpp"
#include "Config.h"
#include "ShopItem.hpp"
#include "CommonUtil.h"

ShopPop::ShopPop()
{
    auto rootNode = CSLoader::createNode("ShopPop.csb");

    addChild(rootNode);
    rect = static_cast<Layout*>(rootNode->getChildByName("rect"));
    contentGroup = static_cast<Layout*>(rootNode->getChildByName("contentGroup"));
    closeBtn = static_cast<Sprite*>(contentGroup->getChildByName("closeBtn"));
    scroller = static_cast<ScrollView*>(contentGroup->getChildByName("scroller"));
    
    _listener=EventListenerTouchOneByOne::create();
    _listener->setSwallowTouches(true);
    _listener->onTouchBegan = CC_CALLBACK_2(ShopPop::menuBegin,this);
    _listener->onTouchEnded = CC_CALLBACK_2(ShopPop::menuEndCallback, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(_listener, contentGroup);
    
    if(CF()->skillCfgs.size() <= 0)
    {
        SkillCfg*skill1cfg = new SkillCfg();
        skill1cfg->isUnlock = false;
        skill1cfg->lockNum = 0;
        skill1cfg->skillDesc = "Fire rose from the ground, causing burns to the enemy";
        skill1cfg->skillIndex = 1;
        skill1cfg->skillName = "Fire";
        skillCfgs.pushBack(skill1cfg);
        
        SkillCfg*skill2cfg = new SkillCfg();
        skill2cfg->isUnlock = true;
        skill2cfg->lockNum = 315;
        skill2cfg->skillDesc = "The dragon spewed out fire, causing a lot of damage";
        skill2cfg->skillIndex = 2;
        skill2cfg->skillName = "Dragon Fury";
        skillCfgs.pushBack(skill2cfg);
        
        SkillCfg*skill3cfg = new SkillCfg();
        skill3cfg->isUnlock = true;
        skill3cfg->lockNum = 535;
        skill3cfg->skillDesc = "Shockwave can cause devastating damage";
        skill3cfg->skillIndex = 3;
        skill3cfg->skillName = "Shockwave";
        skillCfgs.pushBack(skill3cfg);
        
        SkillCfg*skill4cfg = new SkillCfg();
        skill4cfg->isUnlock = true;
        skill4cfg->lockNum = 855;
        skill4cfg->skillDesc = "Deafening roar destroys the enemy";
        skill4cfg->skillIndex = 4;
        skill4cfg->skillName = "Roar";
        skillCfgs.pushBack(skill4cfg);
        
        SkillCfg*skill5cfg = new SkillCfg();
        skill5cfg->isUnlock = true;
        skill5cfg->lockNum = 1235;
        skill5cfg->skillDesc = "The jetted cylindrical flames cause devastating damage";
        skill5cfg->skillIndex = 4;
        skill5cfg->skillName = "Flame spray";
        skillCfgs.pushBack(skill5cfg);
        
        CF()->skillCfgs = skillCfgs;
    }else{
        skillCfgs = CF()->skillCfgs;
    }
    
    
    
    int col = 1;
    int interval = 20;
    int itemH = 148;
    float numchidren =skillCfgs.size();
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
    
    for(int i = 0;i<skillCfgs.size();i++)
    {
        
        ShopItem*item = new ShopItem();
        continerLayer->addChild(item);
        item->refreshInfo(skillCfgs.at(i));
        int y =  scrollH - ((i/col)>>0)*(item->getContentSize().height + interval) - itemH;
        item->setPosition(Vec2(0,y));
    }
    scroller->setSwallowTouches(false);
    scroller->addChild(continerLayer);
    scroller->setInnerContainerSize(Size(scroller->getContentSize().width,scrollH));
    scroller->setScrollBarEnabled(false);
    this->adapter();
}
void ShopPop::adapter()
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
bool ShopPop::menuBegin(cocos2d::Touch* tTouch,cocos2d::Event* eEvent)
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
            ShopItem*item = static_cast<ShopItem*>(c);
            Point itemP = item->convertToNodeSpace(tTouch->getLocation());
            if(item->buyBtn->getBoundingBox().containsPoint(itemP))
            {
                return true;
            }
        }
    }
    return false;
}

void ShopPop::menuEndCallback(cocos2d::Touch* tTouch,cocos2d::Event* eEvent)
{
    Point p = contentGroup->convertToNodeSpace(tTouch->getLocation());
    if(closeBtn->getBoundingBox().containsPoint(p))
    {
        closeBtn->setScale(1.5f);
        this->removeFromParent();
    }else if(scroller->getBoundingBox().containsPoint(p))
    {
        Vector<Node*> childrens = continerLayer->getChildren();
        int index = -1;
        for(auto c : childrens)
        {
            index += 1;
            ShopItem*item = static_cast<ShopItem*>(c);
            Point itemP = item->convertToNodeSpace(tTouch->getLocation());
            if(item->buyBtn->getBoundingBox().containsPoint(itemP))
            {
                int costNum = atoi(item->costLab->getString().c_str());
                if(CF()->gold < costNum)
                {
                    //提示金币不足
                    CommonUtil::floatFont("Not enough gold", this);
                }else{
                    CF()->gold -= costNum;
                    //提示当前技能解锁成功
                    CommonUtil::floatFont("Skill unlocked successfully", this);
                    item->skillCfg->isUnlock = false;
                    item->refreshInfo(item->skillCfg);
                    CF()->skillCfgs.at(index)->isUnlock = false;
                }
                return;
            }
        }
    }
    
}
ShopPop::~ShopPop()
{
    _eventDispatcher->removeEventListener(_listener);
}
