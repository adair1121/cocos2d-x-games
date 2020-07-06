//
//  ShopItem.hpp
//  chuanqiGame-mobile
//
//  Created by 志强 on 2020/6/10.
//

#ifndef ShopItem_hpp
#define ShopItem_hpp

#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "SkillCfg.hpp"

USING_NS_CC;

using namespace cocos2d::ui;

class ShopItem : public cocos2d::Layer{
    
public:
    Text*skillNameLab;
    Text*costLab;
    Text*skillDescLab;
    Sprite*buyBtn;
    Sprite*skillIcon;
    SkillCfg*skillCfg;
public:
    ShopItem();
    ~ShopItem();
public:
    void refreshInfo(SkillCfg*cfg);
};

#endif /* ShopItem_hpp */
