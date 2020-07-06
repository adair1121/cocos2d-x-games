//
//  SoldierEntity.hpp
//  sibada-mobile
//
//  Created by 志强 on 2020/5/26.
//

#ifndef SoldierEntity_hpp
#define SoldierEntity_hpp

#include "cocos2d.h"
#include "MovieClip.h"

USING_NS_CC;

class SoldierEntity : public cocos2d::Layer{
public:
    bool atkState = false;
    
    bool defState = false;
private:
    MovieClip*roleMC;
    MovieClip*effMc;
   
    
    std::string actionStr;
    
    int state = 0;
public:
    SoldierEntity();
    ~SoldierEntity();
    
    void execAtk();
    
    void execRun();
    
    void execStand();
    
    void execDie();
    
    void execHit();
    
    void changeModel();
};
#endif /* SoldierEntity_hpp */
