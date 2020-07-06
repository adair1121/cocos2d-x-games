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
    std::string ass = StringUtils::format("Enemy%dNode.csb",type);
    node = CSLoader::createNode(ass);
    this->addChild(node);
    node->setScale(0.5f);
    if(_type == 3)
    {
        node->setScaleX(-0.5f);
    }
    monAction = CSLoader::createTimeline(ass);
    node->runAction(monAction);
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
        monAction->gotoFrameAndPause(0);
    }
    
}
void ModelEntity::execAtkAct()
{
    int startnum = type == 1?40:type == 2?0:1;
    int endnum = type == 1?50:type == 2?35:1;
    if(monAction)
    {
        monAction->gotoFrameAndPlay(startnum, endnum, 1);
    }
    
    atkState = false;
    auto delay = DelayTime::create(1.0f);
    auto callback = CallFunc::create([&](){
        atkState = true;
    });
    if(node)
    {
        node->runAction(Sequence::create(delay,callback, NULL));
    }
    
}
void ModelEntity::execMoveAct()
{
    int startnum = type == 1?0:type == 2?40:0;
    int endnum = type == 1?40:type == 2?50:0;
    if(monAction)
    {
        monAction->gotoFrameAndPlay(startnum,endnum,true);
    }
    
}
