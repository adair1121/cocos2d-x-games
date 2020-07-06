//
//  GameScene.cpp
//  CastleguardV2-mobile
//
//  Created by 志强 on 2020/6/15.
//

#include "GameScene.hpp"
#include "HelloWorldScene.h"
#include "Config.h"

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
    
    auto rootNode = CSLoader::createNode("BattleLayer.csb");

    addChild(rootNode);
    
    bg = static_cast<Sprite*>(rootNode->getChildByName("bg"));
    gameCon = static_cast<Layout*>(rootNode->getChildByName("gameCon"));
    enemyRect = static_cast<Sprite*>(gameCon->getChildByName("enemyRect"));
    ownRect = static_cast<Sprite*>(gameCon->getChildByName("ownRect"));
    enemy_tower = static_cast<Sprite*>(rootNode->getChildByName("enemy_tower"));
    own_tower = static_cast<Sprite*>(rootNode->getChildByName("own_tower"));
    returnBtn = static_cast<Sprite*>(rootNode->getChildByName("returnBtn"));
    topGroup = static_cast<Layout*>(rootNode->getChildByName("topGroup"));
    ownScore = static_cast<Text*>(topGroup->getChildByName("ownScore"));
    enemyScore = static_cast<Text*>(topGroup->getChildByName("enemyScore"));
    helpImg = static_cast<Sprite*>(rootNode->getChildByName("helpImg"));
    hpbg = static_cast<Sprite*>(rootNode->getChildByName("hpbg"));
    hpBar = static_cast<LoadingBar*>(rootNode->getChildByName("hpBar"));
    hero1 = static_cast<Sprite*>(rootNode->getChildByName("hero1"));
    hero2 = static_cast<Sprite*>(rootNode->getChildByName("hero2"));
    hero3 = static_cast<Sprite*>(rootNode->getChildByName("hero3"));
    hpShow = static_cast<Sprite*>(rootNode->getChildByName("hpShow"));
    
    resultPanel = static_cast<Layout*>(rootNode->getChildByName("resultPanel"));
    continueBtn = static_cast<Sprite*>(resultPanel->getChildByName("continueBtn"));
    scoreLab = static_cast<Text*>(resultPanel->getChildByName("scoreLab"));
    scoreFont = static_cast<Sprite*>(resultPanel->getChildByName("scoreFont"));
    loseicon = static_cast<Sprite*>(resultPanel->getChildByName("loseicon"));
    
    resultPanel->setVisible(false);
    hpShow->setVisible(false);
    
    ownRect->setOpacity(0);
    enemyRect->setVisible(false);
    helpImg->setVisible(false);
    visibleSize = Director::getInstance()->getVisibleSize();
    ownScore->setString("0");
    enemyScore->setString("0");
    hpBar->setPercent(100.0f);
    _listener=EventListenerTouchOneByOne::create();
    _listener->setSwallowTouches(true);
    _listener->onTouchBegan = CC_CALLBACK_2(GameScene::menuBegin,this);
    _listener->onTouchMoved = CC_CALLBACK_2(GameScene::menuMove,this);
    _listener->onTouchEnded = CC_CALLBACK_2(GameScene::menuEndCallback, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(_listener, this);
    
    if(CF()->firstboo)
    {
        Size visibleSize = Director::getInstance()->getVisibleSize();
        float scale = visibleSize.height/1334;
        helpImg->setVisible(true);
        helpImg->setScale(0.0f);
        helpImg->setOpacity(0);
        auto fadein = FadeIn::create(1.0f);
        auto scaleto = ScaleTo::create(1.0f, scale);
        auto swapn = Spawn::createWithTwoActions(fadein, scaleto);
        auto delay = DelayTime::create(2.0f);
        auto callback = CallFunc::create([&](){
            CF()->firstboo = false;
            auto fadeout = FadeOut::create(0.5f);
            helpImg->runAction(fadeout);
            this->schedule(schedule_selector(GameScene::createEntity), createTime);
            this->scheduleUpdate();
        });
        helpImg->runAction(Sequence::create(delay,swapn,delay,callback, NULL));
    }else{
        //游戏开始
        this->schedule(schedule_selector(GameScene::createEntity), createTime);
        this->scheduleUpdate();
    }
    
    this->adapter();
    return true;
}
void GameScene::update(float dt)
{
    timeCount += 0.016f;
    if(timeCount >= 20.0f*(singleCount+1) && singleCount < 4)
    {
        singleCount += 1;
        createTime = 5- singleCount;
        this->unschedule(schedule_selector(GameScene::createEntity));
        this->schedule(schedule_selector(GameScene::createEntity), createTime);
    }
    Vector<Node*> childrens = gameCon->getChildren();
    Vector<Node*> enemys;
    Vector<Node*> owns;
    for(auto c : childrens)
    {
        if(c->getName() == "mon")
        {
            if(c->getTag() == 1)
            {
                enemys.pushBack(c);
            }else if(c->getTag() == 0){
                owns.pushBack(c);
            }
            ModelEntity*entity = static_cast<ModelEntity*>(c);
            int towerh = enemy_tower->getContentSize().height*enemy_tower->getScaleY();
            if(c->getPositionY() >= visibleSize.height - towerh || c->getPositionY() < towerh)
            {
                entity->setHalfAlpha();
            }else{
                entity->resetAlpha();
            }
        }
    }
    for(int i = 0;i<enemys.size();i++)
    {
        ModelEntity*enemy = static_cast<ModelEntity*>(enemys.at(i));
        for(int j = 0;j<owns.size();j++)
        {
            ModelEntity*own = static_cast<ModelEntity*>(owns.at(j));
            float disx = abs(enemy->getPositionX() - own->getPositionX());
            float disy = abs(own->getPositionY() - enemy->getPositionY());
            if(disx <= 50 && enemy->getPositionY() > own->getPositionY() && disy <= 100 && !own->isdead && !enemy->isdead)
            {
                //到达了攻击距离。根据相克关系击杀
                auto delay = DelayTime::create(0.5f);
                if(own->type == 1)
                {
                    //当前我是骑兵单位
                    if(enemy->type == 1)
                    {
                        own->execAtkAct();
                        score += 10;
                        ownScore->setString(StringUtils::format("%d",score));
                        enemy->isdead = true;
                        int newtype = own->type == 1?3:own->type == 2?1:2;
                        createSkill(newtype, enemy->getPosition());
                        //敌方为刀兵。敌方死亡
                        auto callback = CallFunc::create([=](){
                            enemy->stopAllActions();
                            enemy->removeFromParent();
                        });
                        enemy->runAction(Sequence::create(delay,callback, NULL));
                        
                    }else if(enemy->type == 2)
                    {
                        enemy->execAtkAct();
                        //敌方为弓箭兵 我放死亡
                        emenyscore += 10;
                        enemyScore ->setString(StringUtils::format("%d",emenyscore));
                        
                        createSkill(enemy->type, own->getPosition());
                        own->isdead = true;
                        //敌方为刀兵。敌方死亡
                        auto callback = CallFunc::create([=](){
                            own->stopAllActions();
                            own->removeFromParent();
                        });
                        own->runAction(Sequence::create(delay,callback, NULL));
                    }
                }else if(own->type == 2)
                {
                    //当前我方是刀兵
                    if(enemy->type == 2)
                    {
                        own->execAtkAct();
                        score += 10;
                        ownScore->setString(StringUtils::format("%d",score));
                        //敌方为弓箭兵 敌方死亡
                        int newtype = own->type == 1?3:own->type == 2?1:2;
                        createSkill(newtype, enemy->getPosition());
                        enemy->isdead = true;
                        //敌方为刀兵。敌方死亡
                        auto callback = CallFunc::create([=](){
                            enemy->stopAllActions();
                            enemy->removeFromParent();
                        });
                        enemy->runAction(Sequence::create(delay,callback, NULL));
                    }else if(enemy->type == 3)
                    {
                        enemy->execAtkAct();
                        emenyscore += 10;
                        enemyScore ->setString(StringUtils::format("%d",emenyscore));
                        //敌方为骑兵我方死亡
//                        int newtype = own->type == 1?3:own->type == 2?1:2;
                        createSkill(enemy->type, own->getPosition());
                        own->isdead = true;
                        auto callback = CallFunc::create([=](){
                            own->stopAllActions();
                            own->removeFromParent();
                        });
                        own->runAction(Sequence::create(delay,callback, NULL));
                    }
                }else if(own->type == 3)
                {
                    //当前我方为弓箭bing
                    if(enemy->type == 1)
                    {
                        enemy->execAtkAct();
                        emenyscore += 10;
                        enemyScore ->setString(StringUtils::format("%d",emenyscore));
                        //敌方为刀兵 我方死亡
                        own->isdead = true;
                        
                        createSkill(enemy->type, own->getPosition());
                        auto callback = CallFunc::create([=](){
                            own->stopAllActions();
                            own->removeFromParent();
                        });
                        own->runAction(Sequence::create(delay,callback, NULL));
                    }else if(enemy->type == 3)
                    {
                        own->execAtkAct();
                        score += 10;
                        ownScore->setString(StringUtils::format("%d",score));
                        enemy->isdead = true;
                        int newtype = own->type == 1?3:own->type == 2?1:2;
                        createSkill(newtype, enemy->getPosition());
                        //敌方为刀兵。敌方死亡
                        auto callback = CallFunc::create([=](){
                            enemy->stopAllActions();
                            enemy->removeFromParent();
                        });
                        enemy->runAction(Sequence::create(delay,callback, NULL));
                    }
                }
                enemys.clear();
                owns.clear();
                break;
            }
        }
    }
    owns.clear();
    enemys.clear();
}
void GameScene::createSkill(int type,Vec2 pos)
{
    std::string skillass = StringUtils::format("Skill%d.csb",type);
    auto node = CSLoader::createNode(skillass);
    this->addChild(node);
    cocostudio::timeline::ActionTimeline*skillAction = CSLoader::createTimeline(skillass);
    node->runAction(skillAction);
    node->setPosition(pos);
    skillAction->gotoFrameAndPlay(0,45,0);
    auto delay = DelayTime::create(0.5f);
    auto callback = CallFunc::create([=](){
        node->removeFromParent();
    });
    node->runAction(Sequence::create(delay,callback, NULL));
}
void GameScene::createEntity(float dt)
{
    srand(int(time(0))+ rand());//置随机数种子
    
    int type=CCRANDOM_0_1()*3 + 1;
    ModelEntity*entity = new ModelEntity(type,StringUtils::format("mon%d",type) ,-1);
    entity->execMoveAct();
    entity->setTag(1);
    entity->setName("mon");
    float x = CCRANDOM_0_1()*(visibleSize.width - 300) + 100;
    gameCon->addChild(entity);
    entity->setPosition(Vec2(x,visibleSize.height ));
    
    auto moveto = MoveTo::create(entity->speed, Vec2(x,-50));
    auto callback = CallFunc::create([&](){
        if(!hpShow->isVisible())
        {
            hpShow->setVisible(true);
            hpShow->setOpacity(0.0f);
            auto fadein = FadeIn::create(0.5f);
            auto fadeout = FadeOut::create(0.5f);
            auto callback = CallFunc::create([&](){
                hpShow->setVisible(false);
                hpShow->setOpacity(255.0f);
            });
            
            hpShow->runAction(Sequence::create(fadein,fadeout,fadein,fadeout,callback, NULL));
        }
       
        hp -= 5;
        if(hp <= 0){hp = 0;
            //游戏结束
            this->unscheduleUpdate();
            this->unschedule(schedule_selector(GameScene::createEntity));
            resultPanel->setVisible(true);
            scoreLab->setString(ownScore->getString());
        }
        Vector<Node*> children = gameCon->getChildren();
        for(auto c : children)
        {
            if(hp <= 0)
            {
                c->stopAllActions();
            }
            if(c->getName() == "mon" && c->getPositionY() <= -50)
            {
                c->removeFromParent();
            }
        }
        hpBar->setPercent(hp);
    });
    entity->runAction(Sequence::create(moveto,callback, NULL));
}
void GameScene::adapter()
{
    int bgW = 750;
    int bgH = 1334;
    Size visibleSize = Director::getInstance()->getVisibleSize();
    float scaleX = visibleSize.width/bgW;
    float scaleY = visibleSize.height/bgH;
    bg->setScale(scaleX);
    bg->setAnchorPoint(Vec2(0,1));
    bg->setPosition(Vec2(0,visibleSize.height));
    hpShow->setScale(scaleX, scaleY);
    gameCon->setContentSize(Size(visibleSize.width,visibleSize.height));
    enemyRect->setScale(scaleX, scaleY);
    enemyRect->setPosition(Vec2(enemyRect->getPositionX()*scaleX,enemyRect->getPositionY()*scaleY));
    ownRect->setScale(scaleX, scaleY);
    ownRect->setPosition(Vec2(ownRect->getPositionX()*scaleX,ownRect->getPositionY()*scaleY));
    own_tower->setScale(scaleX);
    own_tower->setPosition(Vec2(own_tower->getPositionX()*scaleX,own_tower->getPositionY()*scaleY));
    enemy_tower->setScale(scaleX);
    enemy_tower->setPosition(Vec2(enemy_tower->getPositionX()*scaleX,enemy_tower->getPositionY()*scaleY));
    returnBtn->setPosition(Vec2(returnBtn->getPositionX()*scaleX,returnBtn->getPositionY()*scaleY));
    topGroup->setPosition(Vec2(topGroup->getPositionX()*scaleX,topGroup->getPositionY()*scaleY));
    hpbg->setScale(scaleX);
    hpBar->setScale(scaleX);
    hpbg->setPosition(Vec2(hpbg->getPositionX()*scaleX,hpbg->getPositionY()*scaleY));
    hpBar->setPosition(Vec2(hpBar->getPositionX()*scaleX,hpBar->getPositionY()*scaleY));
    hero1->setPosition(Vec2(hero1->getPositionX()*scaleX,hero1->getPositionY()*scaleY));
    hero2->setPosition(Vec2(hero2->getPositionX()*scaleX,hero2->getPositionY()*scaleY));
    hero3->setPosition(Vec2(hero3->getPositionX()*scaleX,hero3->getPositionY()*scaleY));
    helpImg->setPosition(Vec2(helpImg->getPositionX()*scaleX,helpImg->getPositionY()*scaleY));
    resultPanel->setContentSize(Size(visibleSize.width,visibleSize.height));
    resultPanel->setPosition(Vec2(0,0));
    continueBtn ->setPosition(Vec2(continueBtn->getPositionX()*scaleX,continueBtn->getPositionY()*scaleY));
    scoreLab ->setPosition(Vec2(scoreLab->getPositionX()*scaleX,scoreLab->getPositionY()*scaleY));
    scoreFont ->setPosition(Vec2(scoreFont->getPositionX()*scaleX,scoreFont->getPositionY()*scaleY));
    loseicon ->setPosition(Vec2(loseicon->getPositionX()*scaleX,loseicon->getPositionY()*scaleY));
}
bool GameScene::menuBegin(cocos2d::Touch* tTouch,cocos2d::Event* eEvent)
{
    Point p = this->convertToNodeSpace(tTouch->getLocation());
    if(returnBtn->getBoundingBox().containsPoint(p) && !CF()->firstboo && !resultPanel->isVisible())
    {
        returnBtn->setScale(0.9f);
        return true;
    }else if((hero1->getBoundingBox().containsPoint(p) && !CF()->firstboo) && !resultPanel->isVisible())
    {
        hero1->setScale(0.9f);
        clickCardFunc(1);
        return true;
    }else if((hero2->getBoundingBox().containsPoint(p) && !CF()->firstboo) && !resultPanel->isVisible())
    {
        hero2->setScale(0.9f);
        clickCardFunc(2);
        return true;
    }else if((hero3->getBoundingBox().containsPoint(p) && !CF()->firstboo) && !resultPanel->isVisible())
    {
        hero3->setScale(0.9f);
        clickCardFunc(3);
        return true;
    }else if(continueBtn->getBoundingBox().containsPoint(p) && resultPanel->isVisible())
    {
        return true;
    }
    return false;
}
void GameScene::clickCardFunc(int type)
{
    clickCard = true;
    auto fadein = FadeIn::create(0.5f);
    auto fadeout = FadeOut::create(0.5f);
    ownRect->runAction(RepeatForever::create(Sequence::create(fadein,fadeout, NULL)));
    dragEntity = new ModelEntity(type,StringUtils::format("general%d",type),1);
    gameCon->addChild(dragEntity);
    dragEntity->setHalfAlpha();
    dragEntity->setScale(1.5f);
}
void GameScene::menuMove(cocos2d::Touch* tTouch,cocos2d::Event* eEvent)
{
    if(dragEntity)
    {
        Point p = gameCon->convertToNodeSpace(tTouch->getLocation());
        dragEntity->setPosition(Vec2(p.x,p.y));
    }
}
void GameScene::menuEndCallback(cocos2d::Touch* tTouch,cocos2d::Event* eEvent)
{
    Point p = this->convertToNodeSpace(tTouch->getLocation());
    if(returnBtn->getBoundingBox().containsPoint(p) && !CF()->firstboo && !resultPanel->isVisible())
    {
        returnBtn->setScale(1.0f);
        auto scene = HelloWorld::createScene();
        Director::getInstance()->replaceScene(TransitionCrossFade::create(1, scene));
        return;
    }else if(continueBtn->getBoundingBox().containsPoint(p) && resultPanel->isVisible())
    {
        auto scene = HelloWorld::createScene();
        Director::getInstance()->replaceScene(TransitionCrossFade::create(1, scene));
        return;
    }else if((hero1->getBoundingBox().containsPoint(p) || hero2->getBoundingBox().containsPoint(p) || hero3->getBoundingBox().containsPoint(p)) && !CF()
             ->firstboo && !resultPanel->isVisible())
    {
        if(clickCard)
        {
            hero1->setScale(1.0f);
            hero2->setScale(1.0f);
            hero3->setScale(1.0f);
            clickCard = false;
            ownRect->setOpacity(0.0f);
            ownRect->stopAllActions();
            if(dragEntity)
            {
                dragEntity->removeFromParent();
                dragEntity = NULL;
            }
        }
        return;
    }
    if(clickCard)
    {
        ModelEntity*entity = new ModelEntity(dragEntity->type,dragEntity->_monass,1);
        gameCon->addChild(entity);
        entity->setName("mon");
        entity->setTag(0);
        entity->setPosition(Vec2(p.x,p.y));
        entity->execMoveAct();
        if(entity->type == 1){entity->speed = 15.0f;}
        else if(entity->type == 2){entity->speed = 25.0f;}
        else{entity->speed = 20.0f;}
        auto moveto = MoveTo::create(entity->speed, Vec2(p.x,visibleSize.height + 100));
        auto callback = CallFunc::create([=](){
            entity->removeFromParent();
        });
        entity->runAction(Sequence::create(moveto,callback, NULL));
        
        hero1->setScale(1.0f);
        hero2->setScale(1.0f);
        hero3->setScale(1.0f);
        clickCard = false;
        ownRect->setOpacity(0.0f);
        ownRect->stopAllActions();
        if(dragEntity)
        {
            dragEntity->removeFromParent();
            dragEntity = NULL;
        }
    }
}

