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
    
private:
    Sprite*bg;
    Button*audioBtn;
    Button*playBtn;
    Sprite*map1;
    Sprite*map2;
    Sprite*map3;
    
    Layout*selectMapCon;
    Button*backBtn;
    
    Layout*selectLevCon;
    Button*backBtn2;
    Vector<Layout*> levels;
    EventListenerTouchOneByOne *_listener;
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();
    
    void onExit();
    // implement the "static create()" method manually
    CREATE_FUNC(HelloWorld);
    
private:
    void refreshPageData();
    
    bool menuBegin(cocos2d::Touch* tTouch,cocos2d::Event* eEvent);
       
    void menuEndCallback(cocos2d::Touch* tTouch,cocos2d::Event* eEvent);
    
    void showMapLevel(int maplevel);
    
    void refreshStar();
    
    void adapter();
};

#endif // __HELLOWORLD_SCENE_H__
