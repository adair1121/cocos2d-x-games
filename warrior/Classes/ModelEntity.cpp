//
//  ModelEntity.cpp
//  holdGround-mobile
//
//  Created by 志强 on 2020/6/4.
//

#include "ModelEntity.hpp"
ModelEntity::ModelEntity(int _type)
{
    type = _type;
    std::string ass = StringUtils::format("mon%d.csb",type);
    node = CSLoader::createNode(ass);
    this->addChild(node);
    monAction = CSLoader::createTimeline(ass);
    node->runAction(monAction);
    speed = type == 1?2.0f:type == 2?3.0f:type == 3?1.0f:2.0f;
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
    if(monAction)
    {
        monAction->gotoFrameAndPlay(0,25,true);
    }
    
}
void ModelEntity::execAtkAct()
{

    if(monAction)
    {
        monAction->gotoFrameAndPlay(30, 55, 0);
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
    
    if(monAction)
    {
        monAction->gotoFrameAndPlay(0,25,true);
    }
    
}
