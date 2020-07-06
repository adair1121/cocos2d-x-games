//
//  SoldierEntity.cpp
//  sibada-mobile
//
//  Created by 志强 on 2020/5/26.
//

#include "SoldierEntity.hpp"

SoldierEntity::SoldierEntity()
{
    roleMC = new MovieClip();
    this->addChild(roleMC);
    roleMC->playFile("alien.plist", "stand", 1, 4, -1, false);
    
}
SoldierEntity::~SoldierEntity()
{
    roleMC->releaseCache();
}
void SoldierEntity::execAtk()
{
    atkState = true;
//    auto rotateTo = RotateBy::create(0.2f, 75);
//    auto rotateTo2 = RotateBy::create(0.2f, -75);
    auto delay = DelayTime::create(0.5f);
    auto callback = CallFunc::create([&](){
        atkState = false;
        this->execStand();
    });
    roleMC->changeAction("atk", 1, 6, 1, false);
    roleMC->runAction(Sequence::create(delay,callback, NULL));
}
void SoldierEntity::execRun()
{
    if(actionStr == "run"){return;}
    roleMC->frameRate = 0.1f;
    actionStr = "run";
    int startIndex = 7;
    int endIndex = 8;
    if(state == 1)
    {
        startIndex = 1;
        endIndex = 2;
    }
    roleMC->changeAction("run", 1, 6, -1, false);
}
void SoldierEntity::execStand()
{
    if(actionStr == "stand"){return;}
    actionStr = "stand";
    int startIndex = 10;
    std::string key = "alien-sheet0_";
    if(state == 1)
    {
        key = "alien-sheet0_0";
        startIndex = 9;
    }
    roleMC->changeAction("stand", 1, 4, -1, false);
}
void SoldierEntity::execDie()
{
    if(actionStr == "die"){return;}
    actionStr = "die";
    roleMC->changeAction("alien-sheet0_", 11, 11, -1, false);
}
void SoldierEntity::execHit()
{
//    auto fadeout = FadeOut::create(0.5f);
//    auto fadein = FadeIn::create(0.5f);
//    sword->runAction(Sequence::create(fadeout,fadein,fadeout,fadein,fadeout,fadein, NULL));
}
void SoldierEntity::changeModel()
{
    state = 1;
    this->execStand();
    //增加特效
    effMc = new MovieClip();
    this->addChild(effMc,0);
    effMc->playFile("quick.plist", "quick", 1, 7, -1, false);
    effMc->setPositionY(-50);
    auto delay = DelayTime::create(10.0f);

    auto callback = CallFunc::create([&](){
        state = 0;
        effMc->releaseCache();
        effMc = NULL;
    });
    roleMC->runAction(Sequence::create(delay,callback, NULL));
}
