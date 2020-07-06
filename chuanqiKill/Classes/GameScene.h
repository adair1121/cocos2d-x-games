//
//  GameScene.h
//  ChuanqiKill-mobile
//
//  Created by 志强 on 2020/5/16.
//

#ifndef GameScene_h
#define GameScene_h

#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"

USING_NS_CC;

using namespace cocos2d::ui;

class GameScene : public cocos2d::Layer{
private:
    Layout*leftGroup;
    TextBMFont*scoreLab;
    TextBMFont*levelLab;
    TextBMFont*goldLab;
    Sprite*menuBtn;
    Sprite*bg;
    Layout*container;
    Layout*boxContainer;
    Layout*effCon;
    LoadingBar*timeBar;
    LoadingBar*scoreBar;
    Sprite*gamebg;
    
    Layout*skillGroup;
    TextBMFont*num1Lab;
    TextBMFont*num2Lab;
    TextBMFont*num3Lab;
    Layout*skillGroup1;
    Layout*skillGroup2;
    Layout*skillGroup3;
    
    EventListenerTouchOneByOne *_listener;
    
    
    int level = 1;
    int count = 0;
    int totalCount = 60;
    int scoreNum = 0;
    Vector<Sprite*> linePoints;
    bool battleEnd = false;
public:
    static cocos2d::Scene* createScene();

  
    virtual bool init();

    void onExit();

    CREATE_FUNC(GameScene);
private:
    bool menuBegin(cocos2d::Touch* tTouch,cocos2d::Event* eEvent);
       
    void menuEndCallback(cocos2d::Touch* tTouch,cocos2d::Event* eEvent);
    
    void menuMove(cocos2d::Touch* tTouch,cocos2d::Event* eEvent);
    
    void resetData();
    
    void adapter();
    
    void floatFont(std::string cnt);
    
    void updateTime(float dt);
    
    int getMinIndex(int x,int y);
    
    Sprite* createItem(int row,int col);
    
    void touchSkill(int skillIndex);
    
    void execEliminate();
    
    void lineConnectExec(Vec2 posVec);
};

#endif /* GameScene_h */
