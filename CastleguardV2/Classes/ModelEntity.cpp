//
//  ModelEntity.cpp
//  holdGround-mobile
//
//  Created by 志强 on 2020/6/4.
//

#include "ModelEntity.hpp"
ModelEntity::ModelEntity(int _type,std::string monass,int camp)
{
    type = _type;
    _monass = monass;
    node = CSLoader::createNode(monass + ".csb");
    this->addChild(node);
    monAction = CSLoader::createTimeline(monass + ".csb");
    node->runAction(monAction);
    
    speed = type == 1?25.0f:type == 2?20.0f:15.0f;
    
    std::string iconass = StringUtils::format("assets/icon%d.png",type);
     int offy = type == 1?100:type == 2?90:160;
    if(camp == 1)
    {
        int newtype = type == 1?3:type == 2?1:2;
        iconass = StringUtils::format("assets/icon%d.png",newtype);
        offy  = newtype == 1?100:newtype == 2?90:160;
    }
   
    Sprite*icon = Sprite::create(iconass);
    this->addChild(icon);
    icon->setPositionY(node->getContentSize().height + offy);
}
void ModelEntity::setHalfAlpha()
{
    node->setOpacity(125.0f);
}
void ModelEntity::resetAlpha()
{
    node->setOpacity(255.0f);
}
ModelEntity::~ModelEntity()
{
    
}
void ModelEntity::execStandAct()
{
    if(monAction)
    {
        int startNum = getStartNum("stand");
        int endNum = getEndNum("stand");
        monAction->gotoFrameAndPlay(startNum,endNum,true);
    }
    
}
void ModelEntity::execAtkAct()
{

    if(monAction)
    {
        int startNum = getStartNum("atk");
        int endNum = getEndNum("atk");
        monAction->gotoFrameAndPlay(startNum, endNum, 0);
    }
    
    atkState = false;
    auto delay = DelayTime::create(1.0f);
    auto callback = CallFunc::create([&](){
        atkState = true;
        execMoveAct();
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
        int startNum = getStartNum("run");
        int endNum = getEndNum("run");
        monAction->gotoFrameAndPlay(startNum,endNum,true);
    }
}
int ModelEntity::getStartNum(std::string act)
{
    if(act == "stand")
    {
        return _monass == "general1"?110:_monass == "general2"?160:_monass == "general3"?140:
        _monass == "mon1"?160:_monass == "mon2"?160:130;
        
    }else if(act == "atk")
    {
        return 0;
    }else if(act == "run")
    {
        return _monass == "general1"?60:_monass == "general2"?80:_monass == "general3"?60:
        _monass == "mon1"?80:_monass == "mon2"?80:80;
    }
}
int ModelEntity::getEndNum(std::string act)
{
    if(act == "stand")
    {
        return _monass == "general1"?165:_monass == "general2"?195:_monass == "general3"?175:
        _monass == "mon1"?235:_monass == "mon2"?235:165;
        
    }else if(act == "atk")
    {
        return _monass == "general1"?55:_monass == "general2"?75:_monass == "general3"?55:
               _monass == "mon1"?75:_monass == "mon2"?75:75;
    }else if(act == "run")
    {
        return _monass == "general1"?105:_monass == "general2"?155:_monass == "general3"?135:
        _monass == "mon1"?155:_monass == "mon2"?155:125;
    }
}

