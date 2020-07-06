//
//  RoleTitleLayer.hpp
//  chuanqiGame-mobile
//
//  Created by 志强 on 2020/6/10.
//

#ifndef RoleTitleLayer_hpp
#define RoleTitleLayer_hpp

#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"

USING_NS_CC;

using namespace cocos2d::ui;

class RoleTitleLayer : public cocos2d::Layer{
    
private:
    Layout*topGroup;
    Sprite*headIcon;
    Text*nameLab;
    LoadingBar*hpbar;
    Text*hpProLab;
    Text*goldLab;
public:
    RoleTitleLayer();
    ~RoleTitleLayer();
public:
    void refreshInfo();
};

#endif /* RoleTitleLayer_hpp */
