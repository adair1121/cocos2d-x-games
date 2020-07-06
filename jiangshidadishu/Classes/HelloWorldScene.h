#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "Monster.h"
#include "ScoreEffect.h"

USING_NS_CC;

using namespace cocos2d::ui;
using namespace cocostudio::timeline;

class HelloWorld : public cocos2d::Layer
{
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();
    Rect rect;
    
    Layout* start_layout;
    Layout* game_layout;
    Layout* dong_layout;
    Layout* hp_layout;
    Layout* over_layout;
    ImageView* start_bg;
    ImageView* game_bg;
    Sprite* game_title;
    Sprite* hp_1;
    Sprite* hp_2;
    Sprite* hp_3;
    Button* start_btn;
    Button* over_btn;
    Vector<Sprite*> chooseVec;
    Vector<Sprite*> bgVec;
    Vector<Monster*> monsterVec;
    TextBMFont* score;
    Text* bastText;
    Text* scoreText;
    int scoreNum = 0;
    int scoreMax = 0;
    int hp = 3;
    int monsterNum = 1;
    int timeNum = 0;
    void update(float ft);
    void removeUpdate(float ft);
    void addScoreEffect(int _score, float _x, float _y);
    void createAni(float _x, float _y, std::string bg_id);
    void onSVTouch(Ref *object, cocos2d::ui::Widget::TouchEventType type);
    CREATE_FUNC(HelloWorld);
};

#endif // __HELLOWORLD_SCENE_H__
