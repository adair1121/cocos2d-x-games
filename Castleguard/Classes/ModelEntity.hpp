//
//  ModelEntity.hpp
//  Castleguard-mobile
//
//  Created by 志强 on 2020/6/3.
//

#ifndef ModelEntity_hpp
#define ModelEntity_hpp

#include <stdio.h>
#include "cocos2d.h"
#include "MovieClip.h"

USING_NS_CC;
class ModelEntity : public cocos2d::Node{
public:
    int type;
    bool atkState = false;
private:
    MovieClip*roleMc;
    
public:
    ModelEntity(int type);
    ~ModelEntity();
public:
    void execDeadAction();
    void execAtkAction();
    void execMoveAction();
    void execStandAction();
    void removeEntity();
    void resetModel(int type);
};
#endif /* ModelEntity_hpp */
