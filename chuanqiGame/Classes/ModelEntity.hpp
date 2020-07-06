//
//  ModelEntity.hpp
//  holdGround-mobile
//
//  Created by 志强 on 2020/6/4.
//

#ifndef ModelEntity_hpp
#define ModelEntity_hpp

#include <stdio.h>
#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"

USING_NS_CC;

using namespace cocos2d::ui;
class ModelEntity : public cocos2d::Node{
public:
    int totalHp;
    int curHp;
private:
    Node*node;
    cocostudio::timeline::ActionTimeline * monAction;
    LoadingBar*hpbar;
    
public:
    ModelEntity(int type);
    ~ModelEntity();
public:
    bool atkState = false;
    int type;
    float speed;
    void execStandAct();
    void execAtkAct();
    void execMoveAct();
    void setRevert();
    void reduceHp(int dmg);
};

#endif /* ModelEntity_hpp */
