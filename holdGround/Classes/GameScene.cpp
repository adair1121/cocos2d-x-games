//
//  GameScene.cpp
//  holdGround-mobile
//
//  Created by 志强 on 2020/6/4.
//

#include "GameScene.hpp"
#include "SoundCtl.h"
#include "HelloWorldScene.h"
#include "ModelEntity.hpp"
#include "Config.h"
#include "ResultScene.hpp"
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
    rootNode = CSLoader::createNode("GameScene.csb");
    
    addChild(rootNode);
    
    bg = static_cast<Sprite*>(rootNode->getChildByName("bg"));
    roleNode = static_cast<Node*>(rootNode->getChildByName("roleNode"));
    grass = static_cast<Sprite*>(rootNode->getChildByName("grass"));
    homeImg = static_cast<Sprite*>(rootNode->getChildByName("homeImg"));
    returnBtn = static_cast<Sprite*>(rootNode->getChildByName("returnBtn"));
    skillIcon = static_cast<Sprite*>(rootNode->getChildByName("skillIcon"));
    scoreIcon = static_cast<Sprite*>(rootNode->getChildByName("scoreIcon"));
    scoreLab = static_cast<TextBMFont*>(rootNode->getChildByName("scoreLab"));
    timeLab = static_cast<TextBMFont*>(rootNode->getChildByName("timeLab"));
    soundBtn = static_cast<Button*>(rootNode->getChildByName("soundBtn"));
    proBar = static_cast<LoadingBar*>(rootNode->getChildByName("proBar"));
    proBg = static_cast<Sprite*>(rootNode->getChildByName("proBg"));
    cdLab = static_cast<TextBMFont*>(rootNode->getChildByName("cdLab"));
     
    _listener=EventListenerTouchOneByOne::create();
    _listener->setSwallowTouches(true);
    _listener->onTouchBegan = CC_CALLBACK_2(GameScene::menuBegin,this);
    _listener->onTouchMoved = CC_CALLBACK_2(GameScene::menuMove, this);
    _listener->onTouchEnded = CC_CALLBACK_2(GameScene::menuEndCallback, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(_listener, this);
    cdLab->setVisible(false);
    visibleSize = Director::getInstance()->getVisibleSize();
    this->adapter();
    if(SoundCtl::getInstance()->musicBool)
    {
        soundBtn->setBrightStyle(BRIGHT_HIGHLIGHT);
    }else{
        soundBtn->setBrightStyle(BRIGHT_NORMAL);
    }
    auto moveTo1 = MoveTo::create(2.0f, Vec2(scoreIcon->getPositionX(),scoreIcon->getPositionY() + 10));
    auto moveTo2 = MoveTo::create(2.0f, Vec2(scoreIcon->getPositionX(),scoreIcon->getPositionY() - 10));
    score = CF()->score;
    hp = CF()->hp;
    scoreIcon->runAction(RepeatForever::create(Sequence::create(moveTo1,moveTo2, NULL)));
    scoreLab->setString(StringUtils::format("%d",score));
    this->schedule(schedule_selector(GameScene::countDownTime), 1.0f);
    this->schedule(schedule_selector(GameScene::createEnemy), 0.1f);
    roleAction = CSLoader::createTimeline("roleNode.csb");
    rootNode->runAction(roleAction);
    
    return true;
}
void GameScene::createEnemy(float dt)
{
    curTime += 0.1;
    if(curTime >= createTime)
    {
        curTime = 0.0f;
        srand(int(time(0))+ rand());//置随机数种子
        int num = CCRANDOM_0_1()*3+1;
        ModelEntity*entity = new ModelEntity(num);
        entity->setName("enemy");
        rootNode->addChild(entity);
        int dic = (CCRANDOM_0_1()*100) > 50?-1:1;
        float x = dic == -1?visibleSize.width + 100 : -100;
        float y = homeImg->getPositionY();
        if(num == 3)
        {
            y = homeImg->getPositionY() + roleNode->getPositionY() + 150;
        }
        entity->setPosition(Vec2(x,y));
        float time = CCRANDOM_0_1()*5+2;
        float mx = (homeImg->getPositionX() - (homeImg->getContentSize().width*homeImg->getScaleX())/2);
        if(dic == -1)
        {
            entity->setRevert();
            mx = homeImg->getPositionX() + (homeImg->getContentSize().width*homeImg->getScaleX())/2;
        }
        float offx = CCRANDOM_0_1()*50;
        mx -= dic*offx;
        if(num == 1)
        {
            mx -= dic*(CCRANDOM_0_1()*100+100);
        }
        auto moveTo = MoveTo::create(time, Vec2(mx,y));
        entity->execMoveAct();
        auto callback = CallFunc::create([=](){
            entity->execStandAct();
            entity->atkState = true;
        });
        entity->runAction(Sequence::create(moveTo,callback, NULL));
    }
    Vector<Node*> childrens = rootNode->getChildren();
    Vector<Sprite*> arrows;
    Vector<ModelEntity*> entitys;
    for(auto c : childrens)
    {
        if(c->getName() == "enemy")
        {
            entitys.pushBack(static_cast<ModelEntity*>(c));
        }else if(c->getName() == "arrow")
        {
            arrows.pushBack(static_cast<Sprite*>(c));
        }
    }
    childrens.clear();
    for(auto c : arrows)
    {
        for(auto e : entitys)
        {
            float distance = sqrt(pow(c->getPositionX()-e->getPositionX(),2)+pow(c->getPositionY()-e->getPositionY(),2));
            if(distance <= 100)
            {
                e->stopAllActions();
                e->removeFromParent();
                entitys.eraseObject(e);
                score += 1;
                scoreLab->setString(StringUtils::format("%d",score));
            }
        }
    }
    for(auto c : entitys)
    {
        if(c->atkState)
        {
            c->execAtkAct();
            if(c->type == 1 || c->type == 3)
            {
                Sprite*arrow = Sprite::create("assets/arrow-sheet0.png");
                arrow->setPosition(Vec2(c->getPosition()));
                arrow->setScale(0.5f);
           
                float allW = (homeImg->getContentSize().width*homeImg->getScaleX());
                float allH = (homeImg->getContentSize().height*homeImg->getScaleY());
                float targetx = CCRANDOM_0_1()*allW + (homeImg->getPositionX()-allW/2);
                float targety = CCRANDOM_0_1()*allH + (homeImg->getPositionY());
                
                float enemyAngle = atan2(targety - c->getPositionY(), targetx - c->getPositionX());
                float rotateAngle = CC_RADIANS_TO_DEGREES(enemyAngle);
                arrow->setRotation(-rotateAngle);
                rootNode->addChild(arrow,1);
                
                auto moveTo = MoveTo::create(0.5f, Vec2(targetx,targety));
                auto callback = CallFunc::create([=](){
                    arrow->removeFromParent();
                });
                arrow->runAction(Sequence::create(moveTo,callback, NULL));
            }
            hp -= 3;
            if(hp <= 0)
            {
                hp = 0;
                gameEnd();
                break;
            }
            proBar->setPercent(hp);
        }
    }
    entitys.clear();
    arrows.clear();
}
void GameScene::countDownTime(float dt)
{
    this->refreshTimeShow();
    if(cdLab->isVisible())
    {
        cdTime -= 1;
        cdLab->setString(StringUtils::format("%d",cdTime));
        if(cdTime <= 0)
        {
            cdTime = 20;
            cdLab->setVisible(false);
        }
    }
    
}
void GameScene::adapter()
{
    int bgW = bg->getContentSize().width;
    int bgH = bg->getContentSize().height;
    Size visibleSize = Director::getInstance()->getVisibleSize();
    float scaleX = visibleSize.width/bgW;
    float scaleY = visibleSize.height/bgH;
    bg->setScale(scaleX, scaleY);
    grass->setScale(scaleX, scaleY);
    roleNode->setPosition(Vec2(roleNode->getPositionX()*scaleX,roleNode->getPositionY()*scaleY));
    homeImg->setPosition(Vec2(homeImg->getPositionX()*scaleX,homeImg->getPositionY()*scaleY));
    returnBtn->setPosition(Vec2(returnBtn->getPositionX()*scaleX,returnBtn->getPositionY()*scaleY));
    soundBtn->setPosition(Vec2(soundBtn->getPositionX()*scaleX,soundBtn->getPositionY()*scaleY));
    skillIcon->setPosition(Vec2(skillIcon->getPositionX()*scaleX,skillIcon->getPositionY()*scaleY));
    proBar->setPosition(Vec2(proBar->getPositionX()*scaleX,proBar->getPositionY()*scaleY));
    proBg->setPosition(Vec2(proBg->getPositionX()*scaleX,proBg->getPositionY()*scaleY));
    timeLab->setPosition(Vec2(timeLab->getPositionX()*scaleX,timeLab->getPositionY()*scaleY));
    scoreLab->setPosition(Vec2(scoreLab->getPositionX()*scaleX,scoreLab->getPositionY()*scaleY));
    scoreIcon->setPosition(Vec2(scoreIcon->getPositionX()*scaleX,scoreIcon->getPositionY()*scaleY));
    cdLab->setPosition(Vec2(cdLab->getPositionX()*scaleX,cdLab->getPositionY()*scaleY));
//    audioBtn->setPositionX(audioBtn->getPositionX()*scaleX);
}
void GameScene::refreshTimeShow()
{
    allTime -= 1;
    int minute = allTime/60;
    std::string minutestr = minute < 10 ?StringUtils::format("0%d",minute):StringUtils::format("%d",minute);
    int second = allTime - minute*60;
    std::string secondstr = second < 10 ?StringUtils::format("0%d",second):StringUtils::format("%d",second);
    timeLab->setString(minutestr + ":" + secondstr);
    if(allTime <= 0)
    {
        gameEnd();
    }
}
void GameScene::gameEnd()
{
    CF()->score = score;
    this->unschedule(schedule_selector(GameScene::countDownTime));
    this->unschedule(schedule_selector(GameScene::archery));
    this->unschedule(schedule_selector(GameScene::createEnemy));
    auto scene = ResultScene::createScene();
    Director::getInstance()->replaceScene(TransitionCrossFade::create(1, scene));
}
bool GameScene::menuBegin(cocos2d::Touch* tTouch,cocos2d::Event* eEvent)
{
    Point localP = this->convertToNodeSpace(tTouch->getLocation());
    if(soundBtn->getBoundingBox().containsPoint(localP))
    {
        soundBtn->setScale(0.4f);
        return true;
    }else if(returnBtn->getBoundingBox().containsPoint(localP))
    {
        returnBtn->setScale(0.4f);
        return true;
    }else if(skillIcon->getBoundingBox().containsPoint(localP) && !cdLab->isVisible())
    {
        skillIcon->setScale(0.4f);
        return true;
    }
    touchDown = true;
    posP = localP;
    angle = atan2(localP.y - roleNode->getPositionY(), localP.x - roleNode->getPositionX());
    this->schedule(schedule_selector(GameScene::archery), 0.2f);
    roleAction->gotoFrameAndPlay(0,10, true);
    return true;
}
void GameScene::menuMove(cocos2d::Touch* tTouch,cocos2d::Event* eEvent)
{
    if(touchDown)
    {
        Point localP = this->convertToNodeSpace(tTouch->getLocation());
        posP = localP;
        angle = atan2(localP.y - roleNode->getPositionY(), localP.x - roleNode->getPositionX());
    }
}
void GameScene::archery(float dt)
{
    Sprite*arrow = Sprite::create("assets/arrow-sheet0.png");
    float rotateAngle = CC_RADIANS_TO_DEGREES(angle);
    arrow->setName("arrow");
    arrow->setRotation(-rotateAngle);
    if(posP.x > roleNode->getPositionX())
    {
        roleNode->setScaleX(0.5f);
    }else {
        roleNode->setScaleX(-0.5f);
    }
    if(rotateAngle<= -50 && rotateAngle >= -100)
    {
        rotateAngle = -50;
    }
    if(rotateAngle <= -100 && rotateAngle >= -180)
    {
        rotateAngle = 50;
    }
    if(rotateAngle > 90)
    {
        rotateAngle = rotateAngle - 180;
    }

//    if(rotateAngle > 0){rotateAngle -= 90;}
//
//    if(rotateAngle >=50 || rotateAngle < -90)
//    {
//        rotateAngle = 50;
//    }
    roleNode->setRotation(-rotateAngle);
    rootNode->addChild(arrow,1);
    arrow->setPosition(Vec2(roleNode->getPositionX(),roleNode->getPositionY() + 100));
    Point centerP = Point(roleNode->getPositionX(),roleNode->getPositionY());
    //射击方向向量
    Point shootVector = posP - centerP;
    //向量标准化(即向量长度为1)
    Point normalizedVector = ccpNormalize(shootVector);
    //移动长度向量
    Point overShootVector = normalizedVector * 900;
    //超出屏幕的点
    Point offScreenPoint = centerP + overShootVector;
    //假设速度为500（pix/s）
    float moveSpeed = 700;
    //移动时间
    float distance=sqrt(pow(centerP.x-offScreenPoint.x,2)+pow(centerP.y-offScreenPoint.y,2));
    float moveDuration = distance / moveSpeed;
    
    auto move = MoveTo::create(moveDuration,offScreenPoint);
    auto callback = CallFunc::create([=](){
       if(arrow)
       {
           arrow->removeFromParent();
       }
    });
    arrow->runAction(Sequence::create(move,callback, NULL));
}
void GameScene::menuEndCallback(cocos2d::Touch* tTouch,cocos2d::Event* eEvent)
{
    touchDown = false;
    roleAction->gotoFrameAndPause(0);
    this->unschedule(schedule_selector(GameScene::archery));
    Point localP = this->convertToNodeSpace(tTouch->getLocation());
    if(soundBtn->getBoundingBox().containsPoint(localP))
    {
        soundBtn->setScale(0.5f);
        if(SoundCtl::getInstance()->musicBool)
        {
            soundBtn->setBrightStyle(BRIGHT_NORMAL);
            SoundCtl::getInstance()->stopBg();
        }else{
            soundBtn->setBrightStyle(BRIGHT_HIGHLIGHT);
            SoundCtl::getInstance()->playBg("sound/music.m4a", true);
        }
    }else if(returnBtn->getBoundingBox().containsPoint(localP))
    {
        returnBtn->setScale(0.5f);
        CF()->score = score;
        auto scene = HelloWorld::createScene();
        Director::getInstance()->replaceScene(TransitionCrossFade::create(1, scene));
    }else if(skillIcon->getBoundingBox().containsPoint(localP) && !cdLab->isVisible())
    {
        skillIcon->setScale(0.5f);
        cdLab->setVisible(true);
        cdLab->setString("10");
        Size visibleSize = Director::getInstance()->getVisibleSize();
        for(int i = 0;i < 2;i++)
        {
            
            for(int j = 0;j<100;j++)
            {
                srand(int(time(0))+ rand());//置随机数种子
                float x = CCRANDOM_0_1()*(homeImg->getPositionX() - (homeImg->getContentSize().width*homeImg->getScaleX())/2);
                if(i == 1)
                {
                    x += homeImg->getPositionX() + (homeImg->getContentSize().width*homeImg->getScaleX())/2;
                }
                Sprite*arrow = Sprite::create("assets/arrow-sheet0.png");
                arrow->setRotation(90);
                arrow->setName("arrow");
                arrow->setAnchorPoint(Vec2(0.5,0));
                rootNode->addChild(arrow,1);
                float y = visibleSize.height + 100;
                arrow->setPosition(Vec2(x,y));
                auto moveTo = MoveTo::create(0.5f, Vec2(x,180));
                auto easein = EaseSineIn::create(moveTo->clone());
                float time = CCRANDOM_0_1()*2;
                auto delay = DelayTime::create(time);
                auto callback = CallFunc::create([=](){
                    arrow->removeFromParent();
                });
                arrow->runAction(Sequence::create(delay,easein,delay,callback, NULL));
            }
        }
    }
    
}
