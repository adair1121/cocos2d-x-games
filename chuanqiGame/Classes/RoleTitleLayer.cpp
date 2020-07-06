//
//  RoleTitleLayer.cpp
//  chuanqiGame-mobile
//
//  Created by 志强 on 2020/6/10.
//

#include "RoleTitleLayer.hpp"
#include "Config.h"
RoleTitleLayer::RoleTitleLayer()
{
    auto rootNode = CSLoader::createNode("RoleTitleLayer.csb");

    addChild(rootNode);
    
    topGroup = static_cast<Layout*>(rootNode->getChildByName("topGroup"));
    headIcon = static_cast<Sprite*>(topGroup->getChildByName("headIcon"));
    nameLab = static_cast<Text*>(topGroup->getChildByName("nameLab"));
    hpbar = static_cast<LoadingBar*>(topGroup->getChildByName("hpbar"));
    hpProLab = static_cast<Text*>(topGroup->getChildByName("hpProLab"));
    goldLab = static_cast<Text*>(topGroup->getChildByName("goldLab"));
    
    Sprite*head = Sprite::create(CF()->sex == 1?"main/main_head_man.png":"main/main_head_woman.png");
    headIcon->setTexture(head->getTexture());
    this->refreshInfo();
}
void RoleTitleLayer::refreshInfo()
{
    goldLab->setString(StringUtils::format("%d",CF()->gold));
    nameLab->setString(CF()->namestr+"(LV."+StringUtils::format("%d)",CF()->level));
    hpProLab->setString(StringUtils::format("%d/",CF()->curHp)+StringUtils::format("%d",CF()->totalHp));
    float percent = float(CF()->curHp) / float(CF()->totalHp)*100.0F;
    hpbar->setPercent(percent);
}
RoleTitleLayer::~RoleTitleLayer()
{
    
}
