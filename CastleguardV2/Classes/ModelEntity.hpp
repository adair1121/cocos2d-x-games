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
private:
    
    Node*node;
    cocostudio::timeline::ActionTimeline * monAction;
    
public:
    ModelEntity(int type,std::string monass,int camp);
    ~ModelEntity();
public:
    bool atkState = false;
    int type;
    bool isdead = false;
    std::string _monass;
    float speed;
    void execStandAct();
    void execAtkAct();
    void execMoveAct();
    void setHalfAlpha();
    void resetAlpha();
    int getStartNum(std::string ass);
    int getEndNum(std::string ass);
};

#endif /* ModelEntity_hpp */
