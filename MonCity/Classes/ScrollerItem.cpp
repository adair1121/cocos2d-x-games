//
//  ScrollerItem.cpp
//  MonCity-mobile
//
//  Created by 志强 on 2020/6/16.
//

#include "ScrollerItem.hpp"
#include "Config.h"

ScrollerItem::ScrollerItem(int _tabindex,int _selectIndex)
{
    auto rootNode = CSLoader::createNode("ScrollerItem.csb");

    addChild(rootNode);
    this->setContentSize(Size(700,137));
    tabIndex = _tabindex;
    selectIndex = _selectIndex;
    
    iconImg = static_cast<Sprite*>(rootNode->getChildByName("iconImg"));
    itemNameLab = static_cast<Text*>(rootNode->getChildByName("itemNameLab"));
    levelLab = static_cast<Text*>(rootNode->getChildByName("levelLab"));
    descLab = static_cast<Text*>(rootNode->getChildByName("descLab"));
    rise10 = static_cast<CheckBox*>(rootNode->getChildByName("rise10"));
    rise100 = static_cast<CheckBox*>(rootNode->getChildByName("rise100"));
    upgradeBtn = static_cast<Layout*>(rootNode->getChildByName("upgradeBtn"));
    std::string key = tabIndex == 1?"attr":tabIndex == 2?"role":"skill";
    std::string iconstr = "assets/"+key + StringUtils::format("%d.png",selectIndex);
    Texture2D*texture = Director::getInstance()->getTextureCache()->addImage(iconstr);
    iconImg->setTexture(texture);
    std::string itemattrstr1 = selectIndex == 1?"HERO":selectIndex == 2?"HURT":selectIndex == 3?"SPEED":"CRIT";
    std::string itemattrstr2 =selectIndex == 1?"GuoJia":selectIndex == 2?"ZhuGe":selectIndex == 3?"ZhangFei":"GuanYu";
    std::string itemattrstr3 =selectIndex == 1?"Puncture":selectIndex == 2?"Wipe out":selectIndex == 3?"Lighting":"Phalanx";
    std::string namestr = tabIndex == 1?itemattrstr1:tabIndex == 2?itemattrstr2:itemattrstr3;
    itemNameLab->setString(namestr);
    rise10->setSelected(false);
    rise100->setSelected(false);
    
    this->refreshItem();
    
}
void ScrollerItem::refreshItem()
{
    std::vector<int> levelCfgs = tabIndex == 1?CF()->tab1Cfg:tabIndex == 2?CF()->tab2Cfg:CF()->tab3Cfg;
    std::string descstr = tabIndex == 2?"DPS:":"Damage:";
    
    int value1 = selectIndex == 1?5:selectIndex == 2?10:selectIndex == 3?15:20;
    int value2 = 5;
    int value3 = selectIndex == 1?150:selectIndex == 2?175:selectIndex == 3?235:325;
    int standvalue = tabIndex == 1?value1:tabIndex == 2?value2:value3;
    int upnum = selectIndex == 1?50:selectIndex == 2?85:selectIndex == 3?135:175;
    if(tabIndex == 3)
    {
        upnum = selectIndex == 1?235:selectIndex == 2?375:selectIndex == 3?490:600;
    }
    upgradeCost = (levelCfgs.at(selectIndex - 1) + 1)*upnum*multiple;
    
    promptNum = standvalue*multiple;
    
    static_cast<Text*>(upgradeBtn->getChildByName("lab1"))->setString(StringUtils::format("%d",upgradeCost));
    static_cast<Text*>(upgradeBtn->getChildByName("lab2"))->setString(StringUtils::format("+%d",promptNum));
    
    int showVlaue = (levelCfgs.at(selectIndex-1))*standvalue;
    
    descLab->setString(descstr + StringUtils::format("%d",showVlaue));
    levelLab->setString(StringUtils::format("LV.%d",levelCfgs.at(selectIndex-1)));
}
ScrollerItem::~ScrollerItem()
{
    
}
