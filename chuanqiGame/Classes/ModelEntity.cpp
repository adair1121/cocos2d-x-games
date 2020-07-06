//
//  ModelEntity.cpp
//  holdGround-mobile
//
//  Created by 志强 on 2020/6/4.
//

#include "ModelEntity.hpp"
#include "Config.h"
ModelEntity::ModelEntity(int _type)
{
    type = _type;
    std::string ass = StringUtils::format("mon%d.csb",type);
    node = CSLoader::createNode(ass);
    this->addChild(node);
    monAction = CSLoader::createTimeline(ass);
    node->runAction(monAction);
    speed = type == 1?3.0f:type == 2?5.0f:type == 3?1.0f:2.0f;
    
    Sprite*hpbg = Sprite::create("battle/battle_hp_bar_bg.png");
    this->addChild(hpbg);
    hpbg->setPositionY(200);
    
    hpbar = LoadingBar::create("battle/battle_hp_bar.png");
    this->addChild(hpbar);
    hpbar->setPositionY(198);
    hpbar->setPercent(100.0f);
    
    curHp = totalHp = CF()->c_chapter*100;
}
void ModelEntity::reduceHp(int dmg)
{
    curHp -= dmg;
    if(curHp <= 0)
    {
        curHp = 0;
    }
    float percent = float(curHp)/float(totalHp)*100.0f;
    hpbar->setPercent(percent);
}
void ModelEntity::setRevert()
{
    node->setScaleX(-0.5f);
    if(type == 3)
    {
        node->setScaleX(0.5f);
    }
}
ModelEntity::~ModelEntity()
{
    
}
void ModelEntity::execStandAct()
{
    int startnum = 60;
    int endnum = 70;
    if(type == 3 || type == 4)
    {
        startnum = 0;
        endnum = 25;
    }
    if(monAction)
    {
        monAction->gotoFrameAndPlay(startnum,endnum,true);
    }
    
}
void ModelEntity::execAtkAct()
{
    int startnum = 0;
    int endnum = 25;
    if(type == 3)
    {
        startnum = 30;
        endnum = 55;
    }
    else if(type == 4)
    {
        startnum = 60;
        endnum = 85;
    }
    if(monAction)
    {
        monAction->gotoFrameAndPlay(startnum, endnum, 0);
    }
    
    atkState = false;
    auto delay = DelayTime::create(1.0f);
    auto callback = CallFunc::create([&](){
        atkState = true;
        execStandAct();
    });
    if(node)
    {
        node->runAction(Sequence::create(delay,callback, NULL));
    }
    
}
void ModelEntity::execMoveAct()
{
    int startnum = 30;
    int endnum = 55;
    if(type == 3)
    {
        startnum = 60;
        endnum = 85;
    }else if(type == 4)
    {
        startnum = 30;
        endnum = 55;
    }
    if(monAction)
    {
        monAction->gotoFrameAndPlay(startnum,endnum,true);
    }
    
}
