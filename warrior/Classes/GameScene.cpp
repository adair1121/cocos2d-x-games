//
//  GameScene.cpp
//  warrior-mobile
//
//  Created by 志强 on 2020/6/9.
//

#include "GameScene.hpp"
#include "ModelEntity.hpp"
#include "Config.h"
#include "HelloWorldScene.h"
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
    leftArrow = static_cast<Sprite*>(rootNode->getChildByName("leftArrow"));
    rightArrow = static_cast<Sprite*>(rootNode->getChildByName("rightArrow"));
    
    gameCon = static_cast<Layout*>(rootNode->getChildByName("gameCon"));
    roleNode = static_cast<Node*>(gameCon->getChildByName("roleNode"));
    guidePanel = static_cast<Layout*>(rootNode->getChildByName("guidePanel"));
    bloodPic = static_cast<Sprite*>(rootNode->getChildByName("bloodPic"));
    bloodPic->setOpacity(0.0f);
    topGroup = static_cast<Layout*>(rootNode->getChildByName("topGroup"));
    hp1 = static_cast<Sprite*>(topGroup->getChildByName("hp1"));
    hp2 = static_cast<Sprite*>(topGroup->getChildByName("hp2"));
    hp3 = static_cast<Sprite*>(topGroup->getChildByName("hp3"));
    timeLab = static_cast<Text*>(topGroup->getChildByName("timeLab"));
    resultPanel = static_cast<Layout*>(rootNode->getChildByName("resultPanel"));
    resultPanel->setVisible(false);
    
    _listener=EventListenerTouchOneByOne::create();
    _listener->setSwallowTouches(true);
    _listener->onTouchBegan = CC_CALLBACK_2(GameScene::menuBegin,this);
    _listener->onTouchEnded = CC_CALLBACK_2(GameScene::menuEndCallback, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(_listener, this);
    
    this->adapter();
    visibleSize = Director::getInstance()->getVisibleSize();
    auto moveto = MoveTo::create(0.5f, Vec2(guidePanel->getPositionX(),visibleSize.height/2));
    auto easein = EaseSineIn::create(moveto->clone());
    auto delay = DelayTime::create(1.0f);
    
    auto moveto2 = MoveTo::create(0.5f, Vec2(guidePanel->getPositionX(),visibleSize.height + guidePanel->getContentSize().height + 50));
    auto easeout = EaseSineOut::create(moveto2->clone());
    
    auto callback = CallFunc::create([&](){
        gameStart = true;
        this->schedule(schedule_selector(GameScene::createEneity), 0.5f);
    });
    
    roleAction = CSLoader::createTimeline("roleNode.csb");
    roleNode->runAction(roleAction);
    roleAction->gotoFrameAndPlay(0,15,true);
    guidePanel->runAction(Sequence::create(delay,easein,delay,easeout,callback, NULL));
    birthP = Point(roleNode->getPositionX(),roleNode->getPositionY());
    return true;
}
void GameScene::resetData()
{
    gameStart = true;
    atkState = false;
    timecount = 0;
    count = 0.0f;
    hp = 3;
    killnum = 0;
    timeLab->setString("0s");
    this->schedule(schedule_selector(GameScene::createEneity), 0.5f);
    Vector<Node*> childrens = gameCon->getChildren();
    for(auto c : childrens)
    {
        if(c->getName() == "mon")
        {
            c->removeFromParent();
        }
    }
    roleNode->setPosition(Vec2(birthP.x,birthP.y));
    roleAction->gotoFrameAndPlay(0,15,true);
    hp1->setVisible(true);
    hp2->setVisible(true);
    hp3->setVisible(true);
}
void GameScene::createEneity(float dt)
{
    count += 0.5f;
    if(count >= 1.0f)
    {
        timecount += 1;
        count = 0.0f;
        timeLab->setString(StringUtils::format("%ds",timecount));
    }
    srand(int(time(0))+ rand());//置随机数种子
    int type=CCRANDOM_0_1()*CF()->level+1;
    ModelEntity*entity = new ModelEntity(type);
    entity->execMoveAct();
    entity->setName("mon");
    int num = CCRANDOM_0_1()*100;
    int dic = num > 50 ?1:-1;
    gameCon->addChild(entity);
    entity->setPosition(Vec2(dic == 1? -100 : visibleSize.width + 100,roleNode->getPositionY()));
    entity->setScaleX( entity->getScaleX()*dic);
    float mx = dic == 1?roleNode->getPositionX() - 60:roleNode->getPositionX() + 60;
    float changeValue = CCRANDOM_0_1()*50;
    mx -= changeValue*dic;
    auto moveto = MoveTo::create(entity->speed, Vec2(mx,roleNode->getPositionY()));
    auto callback = CallFunc::create([=](){
        entity->execStandAct();
        entity->atkState = true;
    });
    entity->runAction(Sequence::create(moveto,callback, NULL));
    Vector<Node*> childrens = gameCon->getChildren();
    for(auto c : childrens)
    {
        if(c->getName() == "mon" && static_cast<ModelEntity*>(c)->atkState){
            static_cast<ModelEntity*>(c)->atkState = false;
            static_cast<ModelEntity*>(c)->execAtkAct();
            auto delay = DelayTime::create(0.2f);
            hp -= 1;
            refreshHp();
            auto callback = CallFunc::create([=](){
               if(c)
               {
                   static_cast<ModelEntity*>(c)->atkState = true;
                   static_cast<ModelEntity*>(c)->execStandAct();
               }
            });
            c->runAction(Sequence::create(delay,callback, NULL));
        }
    }
}
void GameScene::refreshHp()
{
    hp1->setVisible(true);
    hp2->setVisible(true);
    hp3->setVisible(true);
    auto fadein = FadeIn::create(0.5f);
    auto fadeout = FadeOut::create(0.5f);
    bloodPic->runAction(Sequence::create(fadein,fadeout, NULL));
    if(hp <= 0){
        hp = 0;
        hp1->setVisible(false);
        hp3->setVisible(false);
        hp2->setVisible(false);
        //游戏结束
        this->unschedule(schedule_selector(GameScene::createEneity));
        gameStart = false;
        auto jumpTo = JumpTo::create(1.0f, Vec2(roleNode->getPositionX() - 100,-200), 400, 1);
        auto callback = CallFunc::create([&](){
            resultPanel->setVisible(true);
            
            std::string cnt = StringUtils::format("%d",timecount)+" s";
            static_cast<Text*>(resultPanel->getChildByName("cntLab"))->setString(cnt);
            if(CF()->level == 1)
            {
                if(CF()->bestScore1 < timecount)
                {
                    CF()->bestScore1 = timecount;
                }
            }else if(CF()->level == 2)
            {
               if(CF()->bestScore2 < timecount)
                {
                    CF()->bestScore2 = timecount;
                }
            }else{
                if(CF()->bestScore3 < timecount)
                {
                    CF()->bestScore3 = timecount;
                }
            }
        });
        roleNode->runAction(Sequence::create(jumpTo,callback, NULL));
    }
    if(hp == 1)
    {
        hp3->setVisible(false);
        hp2->setVisible(false);
    }else if(hp == 2)
    {
        hp3->setVisible(false);
    }
}
void GameScene::adapter()
{
    int bgW = 750;
    int bgH = 1334;
    Size visibleSize = Director::getInstance()->getVisibleSize();
    float scaleX = visibleSize.width/bgW;
    float scaleY = visibleSize.height/bgH;
    bg->setScale(scaleX, scaleY);
    topGroup->setPosition(Vec2(topGroup->getPositionX()*scaleX,topGroup->getPositionY()*scaleY));
    gameCon->setContentSize(Size(visibleSize.width,visibleSize.height));
    guidePanel->setPosition(Vec2(guidePanel->getPositionX()*scaleX,guidePanel->getPositionY()*scaleY));
    roleNode->setPosition(Vec2(roleNode->getPositionX()*scaleX,roleNode->getPositionY()*scaleY));
    hp1->setPosition(Vec2(hp1->getPositionX()*scaleX,hp1->getPositionY()*scaleY));
    hp2->setPosition(Vec2(hp2->getPositionX()*scaleX,hp2->getPositionY()*scaleY));
    hp3->setPosition(Vec2(hp3->getPositionX()*scaleX,hp3->getPositionY()*scaleY));
    
    leftArrow->setPosition(Vec2(leftArrow->getPositionX()*scaleX,leftArrow->getPositionY()*scaleY));
    rightArrow->setPosition(Vec2(rightArrow->getPositionX()*scaleX,rightArrow->getPositionY()*scaleY));
}
bool GameScene::menuBegin(cocos2d::Touch* tTouch,cocos2d::Event* eEvent)
{
   
    Point localp = this->convertToNodeSpace(tTouch->getLocation());
    if(rightArrow->getBoundingBox().containsPoint(localp) && gameStart)
    {
        rightArrow->setScale(-0.9f);
        return true;
    }
    if(leftArrow->getBoundingBox().containsPoint(localp) && gameStart)
    {
        leftArrow->setScale(0.9f);
        return true;
    }
    if(resultPanel->getBoundingBox().containsPoint(localp) && resultPanel->isVisible())
    {
        Button*againBtn = static_cast<Button*>(resultPanel->getChildByName("againBtn"));
        Button*backBtn = static_cast<Button*>(resultPanel->getChildByName("backBtn"));
        Point groupP = resultPanel->convertToNodeSpace(tTouch->getLocation());
        if(againBtn->getBoundingBox().containsPoint(groupP))
        {
            againBtn->setScale(0.9f);
        }else if(backBtn->getBoundingBox().containsPoint(groupP))
        {
            backBtn->setScale(0.9f);
        }
        return true;
    }
    return false;
}
void GameScene::judgeAtk()
{
    Vector<Node*> childrens = gameCon->getChildren();
    bool atkboo = false;
    ModelEntity*item;
    for(auto c : childrens)
    {
        if(c->getName() == "mon" && atkState)
        {
            int dic = roleNode->getScaleX() > 0?1:-1;
            float distance = sqrt(pow(c->getPositionX()-roleNode->getPositionX(),2)+pow(c->getPositionY()-roleNode->getPositionY(),2));
            if((dic > 0 && distance <= 180 && c->getPositionX() > roleNode->getPositionX()) || (dic < 0 && distance <= 180 && c->getPositionX() < roleNode->getPositionX()))
            {
                atkboo = true;
                item = static_cast<ModelEntity*>(c);
                auto node = CSLoader::createNode("blood.csb");
                gameCon->addChild(node);
                node->setPosition(Vec2(item->getPosition()));
                cocostudio::timeline::ActionTimeline * bloodAction = CSLoader::createTimeline("blood.csb");
                node->runAction(bloodAction);
                bloodAction->gotoFrameAndPlay(0,10,0);
                auto delay = DelayTime::create(0.1f);
//                killnum += 1;
//                if(killnum >= 15)
//                {
//
//                    killnum = 0;
//                    CF()->level += 1;
//                    if(CF()->level >= 6)
//                    {
//                        CF()->level = 6;
//                    }
//                }
                auto callback = CallFunc::create([=](){
                    item->stopAllActions();
                    node->removeFromParent();
                    item->removeFromParent();
                });
                node->runAction(Sequence::create(delay,callback, NULL));
            }
        }
    }
    if(atkboo)
    {
        
        atkState = false;
        auto delay = DelayTime::create(0.2f);
        auto callback2 = CallFunc::create([&](){
           roleAction->gotoFrameAndPlay(0,15,true);
        });
        gameCon->runAction(Sequence::create(delay,callback2, NULL));
    }else{
        shakeNode(this);
        Sprite*water = Sprite::create("assets/miss.png");
        gameCon->addChild(water);
        water->setName("water");
        water->setPosition(Vec2(roleNode->getPositionX() + 35*roleNode->getScaleX(),roleNode->getPositionY() + 100));
        
        auto delay = DelayTime::create(1.0f);
        auto callback = CallFunc::create([&](){
            roleAction->gotoFrameAndPlay(0,15,true);
            Sprite*water = static_cast<Sprite*>(gameCon->getChildByName("water"));
            water->removeFromParent();
            atkState = false;
        });
        gameCon->runAction(Sequence::create(delay,callback, NULL));
    }
}
void GameScene::shakeNode(cocos2d::Node *node)
{
    node->runAction(RepeatForever::create(Sequence::create(
       MoveTo::create(0.02f, Vec2(2,3)),
       MoveTo::create(0.02f, Vec2(-2,3)),
       MoveTo::create(0.02f, Vec2(-5,3)),
       MoveTo::create(0.02f, Vec2(3,-2)),
       MoveTo::create(0.02f, Vec2(-2,1)),
       MoveTo::create(0.02f, Vec2(2,-2)),
       MoveTo::create(0.02f, Vec2(-3,-1)),
       MoveTo::create(0.02f, Vec2(1,2)),
       MoveTo::create(0.02f, Vec2(0,0)),
        NULL)));
    auto delay = DelayTime::create(0.5f);
    auto callback = CallFunc::create([=](){
        node->stopAllActions();
    });
    auto seque = Sequence::create(delay,callback, NULL);
    node->runAction(seque);
}
void GameScene::menuEndCallback(cocos2d::Touch* tTouch,cocos2d::Event* eEvent)
{
   
    Point localp = this->convertToNodeSpace(tTouch->getLocation());
    if(rightArrow->getBoundingBox().containsPoint(localp) && !atkState && gameStart)
    {
        rightArrow->setScale(-1.0f);
        roleNode->setScaleX(0.6f);
        roleAction->gotoFrameAndPlay(20,30,0);
        atkState = true;
        judgeAtk();
    }
    if(leftArrow->getBoundingBox().containsPoint(localp) && !atkState && gameStart)
    {
        leftArrow->setScale(1.0f);
        roleAction->gotoFrameAndPlay(20,30,0);
        roleNode->setScaleX(-0.6f);
        atkState = true;
        judgeAtk();
    }
    if(resultPanel->getBoundingBox().containsPoint(localp) && resultPanel->isVisible())
    {
        Button*againBtn = static_cast<Button*>(resultPanel->getChildByName("againBtn"));
        Button*backBtn = static_cast<Button*>(resultPanel->getChildByName("backBtn"));
        Point groupP = resultPanel->convertToNodeSpace(tTouch->getLocation());
        if(againBtn->getBoundingBox().containsPoint(groupP))
        {
            againBtn->setScale(1.0f);
            this->resetData();
            resultPanel->setVisible(false);
        }else if(backBtn->getBoundingBox().containsPoint(groupP))
        {
            auto scene = HelloWorld::createScene();
            Director::getInstance()->replaceScene(TransitionCrossFade::create(1, scene));
        }
    }
}
