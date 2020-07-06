#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"

USING_NS_CC;

using namespace cocos2d::ui;
class HelloWorld : public cocos2d::Layer
{
private:
    Sprite*bg1;
    Sprite*bg2;
    Sprite*bg3;
    Sprite*roleImg;
    Sprite*bottomImg;
    Layout*startGroup;
    Layout*rope;
    TextBMFont*scoreLab;
    Sprite*startBtn;
    Sprite*arrow;
    Layout*gameCon;
    Layout*resultGroup;
    TextBMFont*scoreLab2;
    Sprite*titleImg;
    Sprite*againBtn;
    EventListenerTouchOneByOne *_listener;
    Vector<Sprite*> trees;
    bool startSpread = false;
    bool gameEnd;
    bool treeMove;
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();

    // implement the "static create()" method manually
    CREATE_FUNC(HelloWorld);
private:
    bool menuBegin(cocos2d::Touch* tTouch,cocos2d::Event* eEvent);
       
    void menuEndCallback(cocos2d::Touch* tTouch,cocos2d::Event* eEvent);
    
    void adapter();
    
    void resetData();
    
    void tickTime(float dt);
    
    Sprite * createTreeItem(std::string treeass);
};

#endif // __HELLOWORLD_SCENE_H__
