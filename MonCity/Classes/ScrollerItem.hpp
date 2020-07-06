//
//  ScrollerItem.hpp
//  MonCity-mobile
//
//  Created by 志强 on 2020/6/16.
//

#ifndef ScrollerItem_hpp
#define ScrollerItem_hpp

#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"

USING_NS_CC;

using namespace cocos2d::ui;


class ScrollerItem : public cocos2d::Layer{
public:
    Sprite*iconImg;
    Text*itemNameLab;
    Text*levelLab;
    Text*descLab;
    CheckBox*rise10;
    CheckBox*rise100;
    Layout*upgradeBtn;
    int multiple = 1;
    int upgradeCost = 0;
    int promptNum = 0;
    int tabIndex;
    int selectIndex;
public:
    ScrollerItem(int tabindex,int selectIndex);
    ~ScrollerItem();
    
    void refreshItem();
};

#endif /* ScrollerItem_hpp */
