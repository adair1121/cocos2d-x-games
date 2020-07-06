//
//  Monster.h
//  jiangshidadishu
//
//  Created by youxi123 on 2020/6/3.
//

#ifndef Monster_h
#define Monster_h

#include "cocos2d.h"
#include "MovieClip.h"
#include "MessageCenter.h"

USING_NS_CC;

using namespace cocos2d::ui;

class Monster : public cocos2d::Layer
{
public:
    Monster();
    virtual bool init();
    int monster_id;
    std::string bg_id;
    MovieClip* ani;
    Sprite* icon;
    bool touch = false;
    bool drop = false;
    void releasePage();
    void setAni(int _id, std::string _bg_id);
    void monsterAttack();
};

#endif /* Monster_h */
