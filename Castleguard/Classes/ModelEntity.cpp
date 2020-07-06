//
//  ModelEntity.cpp
//  Castleguard-mobile
//
//  Created by 志强 on 2020/6/3.
//

#include "ModelEntity.hpp"


ModelEntity::ModelEntity(int _type)
{
    type = _type;
    roleMc = new MovieClip();
    std::string plistPath = StringUtils::format("mon%d.plist",_type);
    int endIndex = _type == 1?3:_type ==2?3:4;
    std::string keyname = StringUtils::format("mon%d_idle",type);
    roleMc->playFile(plistPath.c_str(), keyname.c_str(), 1, endIndex, -1, false);
    this->addChild(roleMc);
    roleMc->setScale(0.5f);
    roleMc->setScaleX(-0.5f);
//    this->setContentSize(Size(100,240));
}
void ModelEntity::resetModel(int _type)
{
    type = _type;
    std::string plistPath = StringUtils::format("mon%d.plist",_type);
    int endIndex = _type == 1?3:_type ==2?3:4;
    std::string keyname = StringUtils::format("mon%d_idle",type);
    roleMc->playFile(plistPath.c_str(), keyname.c_str(), 1, endIndex, -1, false);
}
ModelEntity::~ModelEntity()
{
    roleMc->releaseCache();
}
void ModelEntity::execDeadAction()
{
    std::string keyname = StringUtils::format("mon%d_die",type);
    roleMc->mcPlayEndFunc = CC_CALLBACK_0(ModelEntity::removeEntity,this);
    roleMc->changeAction(keyname.c_str(), 1, 2, 1, true);
}
void ModelEntity::execStandAction()
{
    std::string keyname = StringUtils::format("mon%d_idle",type);
    int endIndex = type == 1?3:type ==2?3:4;
    roleMc->changeAction(keyname.c_str(), 1, endIndex, -1, false);
}
void ModelEntity::removeEntity()
{
    this->removeFromParent();
}
void ModelEntity::execAtkAction()
{
    std::string keyname = StringUtils::format("mon%d_atk",type);
    int endIndex = type == 1?6:type == 2?6:6;
    roleMc->changeAction(keyname.c_str(), 1, endIndex, 1, false);
    roleMc->mcPlayEndFunc = CC_CALLBACK_0(ModelEntity::execStandAction, this);
}
void ModelEntity::execMoveAction()
{
    std::string keyname = StringUtils::format("mon%d_run",type);
    int endIndex = type == 1?6:type == 2?6:6;
    roleMc->changeAction(keyname.c_str(), 1, endIndex, -1, false);
}
