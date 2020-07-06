//
//  GameScene.cpp
//  stickMonkey-mobile
//
//  Created by 志强 on 2020/6/5.
//

#include "GameScene.hpp"
#include "Config.h"
#include "ResultScene.hpp"
#include "SoundCtl.h"
Scene* GameScene::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = GameScene::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool GameScene::init()
{
    
    if ( !Layer::init() )
    {
        return false;
    }
    
    auto rootNode = CSLoader::createNode("GameScene.csb");

    addChild(rootNode);
    
    bg = static_cast<Sprite*>(rootNode->getChildByName("bg"));
    scoreBg = static_cast<Sprite*>(rootNode->getChildByName("scoreBg"));
    scoreLab = static_cast<TextBMFont*>(rootNode->getChildByName("scoreLab"));
    guideGroup = static_cast<Layout*>(rootNode->getChildByName("guideGroup"));
    
    guideGroup->setOpacity(0);
    auto fadein = FadeIn::create(1.0f);
    auto delay = DelayTime::create(2.0f);
    auto fadeout = FadeOut::create(0.5f);
    auto callback = CallFunc::create([&](){
       //开始游戏
        gameStart = true;
        randomPillar();
    });
    guideGroup->runAction(Sequence::create(fadein,delay,fadeout,callback, NULL));
    
    _listener=EventListenerTouchOneByOne::create();
    _listener->setSwallowTouches(true);
    _listener->onTouchBegan = CC_CALLBACK_2(GameScene::menuBegin,this);
    _listener->onTouchEnded = CC_CALLBACK_2(GameScene::menuEndCallback, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(_listener, this);
    
//    srand(int(time(0))+ rand());//置随机数种子
//    bgNum = CCRANDOM_0_1()*5+1;
//
//    Texture2D*texture = Director::getInstance()->getTextureCache()->addImage(StringUtils::format("assets/bg%d.png",bgNum));
//    bg->setTexture(texture);
    
    Sprite*pillar = Sprite::create(StringUtils::format("assets/stick%d.png",1));
    this->addChild(pillar);
    pillar->setScale(0.5f);
    pillar->setAnchorPoint(Vec2(0,0));
    pillar->setPosition(Vec2(100,150));
    pillars.pushBack(pillar);
    
    normalPillarP.x = pillar->getPositionX() + pillar->getContentSize().width*pillar->getScaleX()/2;
    normalPillarP.y = pillar->getPositionY() + pillar->getContentSize().height*pillar->getScaleY();
    
    
    
    roleNode = CSLoader::createNode("player.csb");
    this->addChild(roleNode);
    roleAction = CSLoader::createTimeline("player.csb");
    roleNode->runAction(roleAction);
    
    visibleSize = Director::getInstance()->getVisibleSize();
    roleNode->setScale(0.5f);
    
    roleAction->gotoFrameAndPlay(40,55, true);
    roleNode->setPosition(Vec2(normalPillarP.x,normalPillarP.y));
    
    operSp = Sprite::create("assets/stick-sheet0.png");
    operSp->setAnchorPoint(Vec2(0.5,0));
    operSp->setScaleY(0.0f);
    this->addChild(operSp);
    operSp->setPosition(Vec2(roleNode->getPositionX()+pillar->getContentSize().width*pillar->getScaleX()/2,roleNode->getPositionY()));
    operSp->setVisible(false);
    this->adapter();
    return true;
}
void GameScene::randomPillar()
{
    Sprite*pillar = Sprite::create(StringUtils::format("assets/stick%d.png",bgNum));
    this->addChild(pillar);
    pillar->setName("pillar");
    srand(int(time(0))+ rand());//置随机数种子
    float minnum = 0.2;
    float maxnum = 1.2f;
    float changeNum = 1.0f;
    pillar->setScaleX(changeNum);
    pillar->setAnchorPoint(Vec2(0,1));
    float mx = roleNode->getPositionX() + 100 + CCRANDOM_0_1()*240;
    pillar->setPosition(Vec2(visibleSize.width + pillar->getContentSize().width*changeNum,normalPillarP.y));
    auto moveTo = MoveTo::create(0.5f, Vec2(mx,pillar->getPositionY()));
    auto easeIn = EaseSineOut::create(moveTo->clone());
    pillars.pushBack(pillar);
    auto callback = CallFunc::create([&](){
        gameStart = true;
    });
    pillar->runAction(Sequence::create(easeIn,callback, NULL));
}
void GameScene::adapter()
{
    int bgW = bg->getContentSize().width;
    int bgH = bg->getContentSize().height;
    Size visibleSize = Director::getInstance()->getVisibleSize();
    float scaleX = visibleSize.width/bgW;
    float scaleY = visibleSize.height/bgH;
    bg->setScale(scaleX, scaleY);
    guideGroup->setPosition(Vec2(guideGroup->getPositionX()*scaleX,guideGroup->getPositionY()*scaleY));
    scoreBg->setPosition(Vec2(scoreBg->getPositionX()*scaleX,scoreBg->getPositionY()*scaleY));
    scoreLab->setPosition(Vec2(scoreLab->getPositionX()*scaleX,scoreLab->getPositionY()*scaleY));
//    audioBtn->setPositionX(audioBtn->getPositionX()*scaleX);
}
bool GameScene::menuBegin(cocos2d::Touch* tTouch,cocos2d::Event* eEvent)
{
    Point localP = this->convertToNodeSpace(tTouch->getLocation());
    if(gameStart)
    {
        operSp->setVisible(true);
        gameStart = false;
        this->schedule(schedule_selector(GameScene::ticktime), 0.016f);
    }
    return true;
}
void GameScene::ticktime(float dt)
{
    if(operSp->isVisible())
    {
        float changeScaleY = operSp->getScaleY() + 0.02;
        operSp->setScaleY(changeScaleY);
    }
}
void GameScene::menuEndCallback(cocos2d::Touch* tTouch,cocos2d::Event* eEvent)
{
    Point localP = this->convertToNodeSpace(tTouch->getLocation());
    this->unschedule(schedule_selector(GameScene::ticktime));
    if(operSp->isVisible())
    {
        auto rotateTo = RotateBy::create(0.2f, 90);
        auto callback = CallFunc::create([&](){
            float mx = operSp->getPositionX() + operSp->getContentSize().height*operSp->getScaleY();
            auto moveTo = MoveTo::create(0.5f, Vec2(mx,normalPillarP.y));
            roleAction->gotoFrameAndPlay(0,35,true);
            auto movecall = CallFunc::create([&](){
                roleAction->gotoFrameAndPlay(40,55,true);
                Sprite*pillar = pillars.at(1);
                float pillarw = (pillar->getPositionX() + pillar->getContentSize().width*pillar->getScaleX());
                if(roleNode->getPositionX() > pillar->getPositionX() && roleNode->getPositionX() < pillarw)
                {
                    //成功;
                    this->gameWin();
                }else{
                    roleAction->gotoFrameAndPlay(40,55,0);
                    auto rotateTo = RotateBy::create(0.2f, 70);
                    operSp->runAction(rotateTo);
                    auto dieMo = MoveTo::create(1.0f, Vec2(roleNode->getPositionX(),-120));
                    this->unschedule(schedule_selector(GameScene::ticktime));
                    SoundCtl::getInstance()->playEffect("sound/gameover.m4a");
                    auto diecall = CallFunc::create([&](){
                        //切换场景
                        CF()->score = score;
                        if(CF()->score > CF()->bestScore)
                        {
                            CF()->bestScore = score;
                        }
                        auto scene = ResultScene::createScene();
                        Director::getInstance()->replaceScene(scene);
                    });
                    roleNode->runAction(Sequence::create(dieMo,diecall, NULL));
                }
            });
            roleNode->runAction(Sequence::create(moveTo,movecall, NULL));
        });
        operSp->runAction(Sequence::create(rotateTo,callback, NULL));
    }
}
void GameScene::gameWin()
{
    score += 1;
    scoreLab->setString(StringUtils::format("%d",score));
    operSp->setVisible(false);
    float moveX = pillars.at(1)->getPositionX() - normalPillarP.x;
    randomPillar();
    SoundCtl::getInstance()->playEffect("sound/walk.m4a");
    auto roleMove = MoveTo::create(0.5f, Vec2(roleNode->getPositionX() - moveX,roleNode->getPositionY()));
    auto roleEaseout = EaseSineOut::create(roleMove->clone());
    roleNode->runAction(roleEaseout);
    int index = -1;
    for(auto c : pillars)
    {
        index += 1;
        c->setTag(index);
        float mx = c->getPositionX() - moveX;
        auto moveto = MoveTo::create(0.5f, Vec2(mx,c->getPositionY()));
        auto easeout = EaseSineOut::create(moveto->clone());
        auto callback = CallFunc::create([&](){
            for(auto p : pillars)
            {
                if(p->getTag() == 0)
                {
                    p->removeFromParent();
                    pillars.eraseObject(p);
                }
            }
            Sprite*pillar = pillars.at(0);
            gameStart = true;
            operSp->setScaleY(0.0f);
            operSp->setRotation(0.0f);
            operSp->setPosition(Vec2(roleNode->getPositionX()+pillar->getContentSize().width*pillar->getScaleX()/2,roleNode->getPositionY()));
        });
        c->runAction(Sequence::create(easeout,callback, NULL));
    }
    
    
}
