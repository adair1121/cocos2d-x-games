//
//  ScoreEffect.h
//  jiangshidadishu
//
//  Created by youxi123 on 2020/6/3.
//

#ifndef ScoreEffect_h
#define ScoreEffect_h

#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"

USING_NS_CC;

using namespace cocos2d::ui;

class ScoreEffect : public cocos2d::Layer
{
public:
    ScoreEffect();
    virtual bool init();
    TextBMFont* scoreText;
    void moveScore(int num);
    void releasePage();
};

#endif /* ScoreEffect_h */
