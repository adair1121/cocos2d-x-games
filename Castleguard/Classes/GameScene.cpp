//
//  GameScene.cpp
//  Castleguard-mobile
//
//  Created by 志强 on 2020/6/3.
//

#include "GameScene.hpp"

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
    gameGroup = static_cast<Layout*>(rootNode->getChildByName("gameGroup"));
    topGroup = static_cast<Layout*>(rootNode->getChildByName("topGroup"));
    bar = static_cast<LoadingBar*>(topGroup->getChildByName("bar"));
    timeLab = static_cast<Text*>(topGroup->getChildByName("timeLab"));
    topwer = static_cast<Sprite*>(rootNode->getChildByName("topwer"));
    operGroup = static_cast<Layout*>(rootNode->getChildByName("operGroup"));
    skill1 = static_cast<Sprite*>(operGroup->getChildByName("skill1"));
    skill2 = static_cast<Sprite*>(operGroup->getChildByName("skill2"));
    skill3 = static_cast<Sprite*>(operGroup->getChildByName("skill3"));
    focus = static_cast<Sprite*>(operGroup->getChildByName("focus"));
    hpGroup = static_cast<Layout*>(rootNode->getChildByName("hpGroup"));
    resultGroup = static_cast<Layout*>(rootNode->getChildByName("resultGroup"));
    resetBtn = static_cast<Sprite*>(resultGroup->getChildByName("resetBtn"));
    ownScoreLab = static_cast<Text*>(resultGroup->getChildByName("ownScoreLab"));
    bestScoreLab = static_cast<Text*>(resultGroup->getChildByName("bestScoreLab"));
    
    _listener=EventListenerTouchOneByOne::create();
    _listener->setSwallowTouches(true);
    _listener->onTouchBegan = CC_CALLBACK_2(GameScene::menuBegin,this);
    _listener->onTouchEnded = CC_CALLBACK_2(GameScene::menuEndCallback, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(_listener, this);
    
    this->resultGroup->setVisible(false);
    hpGroup->setVisible(false);
    focus->setVisible(false);
    this->resetData();
    this->adapter();
    
    if(guideBoo)
    {
        guideMc = new ModelEntity(1);
        gameGroup->addChild(guideMc);
        guideMc->execStandAction();
        guideMc->setTag(1);
        guideMc->setPosition(Vec2(gameGroup->getContentSize().width/2,gameGroup->getContentSize().height/2));
        
        handMc = new MovieClip();
        operGroup->addChild(handMc);
        handMc->frameRate = 0.2f;
        handMc->playFile("hand.plist", "AllAcMc_00008", 1, 2, -1, false);
        handMc->setPosition(Vec2(skill1->getPositionX() + 30,skill1->getPositionY() - 30));
    }else{
        //没有引导 直接开始游戏
        this->schedule(schedule_selector(GameScene::createEntity), 1.0f);
    }
    return true;
}
void GameScene::createEntity(float dt)
{
    timecount += 1;
    int minute = timecount/60;
    std::string minutestr = minute < 10 ?StringUtils::format("0%d",minute):StringUtils::format("%d",minute);
    int second = timecount - minute*60;
    std::string secondstr = second < 10 ?StringUtils::format("0%d",second):StringUtils::format("%d",second);
    timeLab->setString(minutestr + ":" + secondstr);
    if(timecount % 30 == 0)
    {
        createCount -= 1;
        if(createCount <= 1)
        {
            createCount = 1;
        }
    }
    if(timecount%createCount == 0)
    {
        srand(int(time(0))+ rand());//置随机数种子
        int type = CCRANDOM_0_1()*3+1;
        ModelEntity*entity = new ModelEntity(type);
        gameGroup->addChild(entity);
        float y = gameGroup->getContentSize().height + 150;
        float x = CCRANDOM_0_1()*(gameGroup->getContentSize().width - 200) + 200;
        entity->setPosition(Vec2(x,y));
        
        float mx = x - 100;
        float my = topwer->getContentSize().height + topwer->getPositionY();
        if(type == 2 || type == 3)
        {
            my = CCRANDOM_0_1()*(gameGroup->getContentSize().height/3) + my;
        }
        float distance = sqrt(pow(mx-x,2)+pow(my-y,2));
        float time = distance/100.0f;
        auto moveTo = MoveTo::create(time, Vec2(mx,my));
        entity->execMoveAction();
        entitys.pushBack(entity);
        auto callback = CallFunc::create([=](){
            if(entity)
            {
                entity->execStandAction();
                entity->atkState = true;
            }
        });
        entity->runAction(Sequence::create(moveTo,callback, NULL));
    }
}
void GameScene::resetData()
{
    hp = thp;
    timecount = 0;
    bar->setPercent(100);
    timeLab->setString("00:00");
    this->schedule(schedule_selector(GameScene::ticktime), 0.05f);
}
void GameScene::gameEnd()
{
    //游戏结束
    this->unschedule(schedule_selector(GameScene::ticktime));
    this->unschedule(schedule_selector(GameScene::createEntity));
    for(auto c : entitys)
    {
        c->stopAllActions();
        c->removeFromParent();
    }
    entitys.clear();
    Vector<Node*> childrens = gameGroup->getChildren();
    for(auto c : childrens)
    {
        if(c->getName() == "skill" || c->getName() == "boomskill" || c->getName() == "qiangskill" ||c->getName() == "monskill")
        {
            c->stopAllActions();
            c->removeFromParent();
        }
    }
    if(timecount > bestcount)
    {
        bestcount = timecount;
    }
    resultGroup->setVisible(true);
    ownScoreLab->setString(timeLab->getString());
    
    int minute = bestcount/60;
    std::string minutestr = minute < 10 ?StringUtils::format("0%d",minute):StringUtils::format("%d",minute);
    int second = bestcount - minute*60;
    std::string secondstr = second < 10 ?StringUtils::format("0%d",second):StringUtils::format("%d",second);
    bestScoreLab->setString(minutestr + ":" + secondstr);
    
}
void GameScene::ticktime(float dt)
{
    Vector<Node*> childrens = gameGroup->getChildren();
    tickTimeCon += 0.05f;
    if(tickTimeCon >= 1.5f)
    {
        tickTimeCon = 0.0f;
        for(auto c : entitys)
        {
            if(c->atkState)
            {
                c->execAtkAction();
                if(c->type == 2)
                {
                    MovieClip*skillMc = new MovieClip();
                    skillMc->playFile("skill2.plist", "monskill_", 1, 4, -1, false);
                    skillMc->setName("monskill");
                    skillMc->setRotation(-30);
                    gameGroup->addChild(skillMc);
                    skillMc->setPosition(c->getPosition());
                    auto moveto = MoveTo::create(1.0f, Vec2(skillMc->getPositionX() - 150,topwer->getPositionY()));
                    auto callback = CallFunc::create([=](){
                       if(skillMc)
                       {
                           skillMc->releaseCache();
                       }
                    });
                    skillMc->runAction(Sequence::create(moveto,callback, NULL));
                }else if(c->type == 3)
                {
                    Sprite*skillP = Sprite::create("assets/akf_png.png");
                    this->addChild(skillP);
                    skillP->setAnchorPoint(Vec2(0.5,1));
                    skillP->setScaleY(0.0f);
                    skillP->setPosition(Vec2(c->getPositionX(),topwer->getPositionY() + topwer->getContentSize().height));
                    auto scaleTo = ScaleTo::create(0.2f, 1.0f);
                    auto callback = CallFunc::create([=](){
                       if(skillP)
                       {
                           skillP->removeFromParent();
                       }
                    });
                    skillP->runAction(Sequence::create(scaleTo,callback, NULL));
                }
                hp -= 2;
                if(!hitShow)
                {
                    hitShow = true;
                    hpGroup->setVisible(true);
                    hpGroup->setOpacity(0);
                    auto fadein = FadeIn::create(0.5f);
                    auto fadeout = FadeOut::create(0.5f);
                    auto callback = CallFunc::create([&](){
                        hitShow = false;
                    });
                    hpGroup->runAction(Sequence::create(fadein,fadeout,callback, NULL));
                }
                
                if(hp <= 0){
                    hp = 0;
                    gameEnd();
                }
                bar->setPercent(hp);
            }
        }
    }
    for(auto c : childrens)
    {
        if((c->getName() == "skill" || c->getName() == "boomskill" || c->getName() == "qiangskill") && c->getTag() != 1)
        {
            
            if(guideBoo)
            {
                float disP = sqrt(pow(guideMc->getPositionX()-c->getPositionX(),2)+pow(guideMc->getPositionY()-c->getPositionY(),2));
                if(disP <= 100)
                {
                    int tag = guideMc->getTag();
                    if(guideMc && guideMc->getTag() == skillIndex)
                    {
                        if(guideCount == 1)
                        {
                            handMc->setPosition(skill2->getPosition());
                        }else if(guideCount == 2)
                        {
                            handMc->setPosition(skill3->getPosition());
                        }
                        guideMc->removeFromParent();
                        guideMc->release();
                        guideMc = NULL;
                        guideCount += 1;
                        if(guideCount >= 4)
                        {
                            //结束引导
                            handMc->releaseCache();
                            guideBoo = false;
                            this->schedule(schedule_selector(GameScene::createEntity), 1.0f);
                        }else{
                            c->setTag(1);
//                            guideMc->resetModel(skillIndex + 1);
//                            guideMc->setTag(skillIndex + 1);
                            guideMc = new ModelEntity(skillIndex + 1);
                            gameGroup->addChild(guideMc);
                            guideMc->execStandAction();
                            guideMc->setName("guideMc");
                            guideMc->setTag(skillIndex+1);
                            guideMc->setPosition(Vec2(gameGroup->getContentSize().width/2,gameGroup->getContentSize().height/2));
                        }
                    }
                }
                
            }else{
                for(auto e : entitys)
                {
                    float disP = sqrt(pow(e->getPositionX()-c->getPositionX(),2)+pow(e->getPositionY()-c->getPositionY(),2));
                    if(disP <= 80 && skillIndex == e->type)
                    {
                        e->stopAllActions();
                        e->removeFromParent();
                        entitys.eraseObject(e);
                        break;
                    }
                }
            }
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
    gameGroup->setScale(scaleY);
    hpGroup->setScale(scaleX,scaleY);
    topwer->setScale(topwer->getScaleX()*scaleX, topwer->getScaleY()*scaleY);
    topwer->setPosition(Vec2(topwer->getPositionX()*scaleX,topwer->getPositionY()*scaleY));
    topGroup->setPosition(Vec2(topGroup->getPositionX()*scaleX,topGroup->getPositionY()*scaleY));
    operGroup->setPosition(Vec2(operGroup->getPositionX()*scaleX,operGroup->getPositionY()*scaleY));
    resultGroup->setPosition(Vec2(resultGroup->getPositionX()*scaleX,resultGroup->getPositionY()*scaleY));
}
void GameScene::onExit()
{
    Layer::onExit();
    _eventDispatcher->removeEventListener(_listener);
}
bool GameScene::menuBegin(cocos2d::Touch* tTouch,cocos2d::Event* eEvent)
{
    Point localP = this->convertToNodeSpace(tTouch->getLocation());
    if(operGroup->getBoundingBox().containsPoint(localP) && !resultGroup->isVisible())
    {
        Point operP = operGroup->convertToNodeSpace(tTouch->getLocation());
        if(skill1->getBoundingBox().containsPoint(operP) || skill2->getBoundingBox().containsPoint(operP) || skill3->getBoundingBox().containsPoint(operP))
        {
            return true;
        }
    }else if(gameGroup->getBoundingBox().containsPoint(localP)  && !resultGroup->isVisible())
    {
        return true;
    }else if(resultGroup->getBoundingBox().containsPoint(localP) && resultGroup->isVisible())
    {
        return true;
    }
    return false;
}
void GameScene::menuEndCallback(cocos2d::Touch* tTouch,cocos2d::Event* eEvent)
{
    Point localP = this->convertToNodeSpace(tTouch->getLocation());
    if(operGroup->getBoundingBox().containsPoint(localP)  && !resultGroup->isVisible())
    {
        Point operP = operGroup->convertToNodeSpace(tTouch->getLocation());
        if(skill1->getBoundingBox().containsPoint(operP))
        {
            focus->setVisible(true);
            skillIndex = 1;
            focus->setPosition(skill1->getPosition());
            if(guideBoo)
            {
                Point worldP = gameGroup->convertToWorldSpace(guideMc->getPosition());
                Point innerP = operGroup->convertToNodeSpace(worldP);
                handMc->setPosition(innerP);
            }
        }else if(skill2->getBoundingBox().containsPoint(operP))
        {
            focus->setVisible(true);
            skillIndex = 2;
            focus->setPosition(skill2->getPosition());
            if(guideBoo)
            {
                Point worldP = gameGroup->convertToWorldSpace(guideMc->getPosition());
                Point innerP = operGroup->convertToNodeSpace(worldP);
                handMc->setPosition(innerP);
            }
        }else if(skill3->getBoundingBox().containsPoint(operP))
        {
            focus->setVisible(true);
            skillIndex = 3;
            focus->setPosition(skill3->getPosition());
            if(guideBoo)
            {
                Point worldP = gameGroup->convertToWorldSpace(guideMc->getPosition());
                Point innerP = operGroup->convertToNodeSpace(worldP);
                handMc->setPosition(innerP);
            }
        }
    }else if(gameGroup->getBoundingBox().containsPoint(localP)  && !resultGroup->isVisible())
    {
        Point gameP = gameGroup->convertToNodeSpace(tTouch->getLocation());
        if(skillIndex != -1)
        {
//            MovieClip*circleMc = new MovieClip();
//            circleMc->setScale(2.0f);
//            gameGroup->addChild(circleMc,0);
//            circleMc->playFile("select.plist", "AllAcMc_00007", 1, 2, -1, false);
//            circleMc->setPosition(gameP);
            if(skillIndex == 1)
            {
                MovieClip*skillMc = new MovieClip();
                skillMc->playFile("skill1.plist", "Skill_00006", 1, 4, -1, false);
                skillMc->setRotation(90);
                gameGroup->addChild(skillMc);
                skillMc->setName("skill");
                skillMc->setPosition(Vec2(gameP.x,gameGroup->getContentSize().height + 100));
                auto moveTo = MoveTo::create(0.5f, Vec2(gameP));
                auto callback = CallFunc::create([=](){
                    skillMc->releaseCache();
                });
                skillMc->runAction(Sequence::create(moveTo,callback, NULL));
            }else if(skillIndex == 2)
            {
                Sprite*sp = Sprite::create("assets/bombSp_png.png");
                gameGroup->addChild(sp);
                sp->setName("boomskill");
                sp->setPosition(Vec2(gameP.x,gameGroup->getContentSize().height + 100));
                auto moveTo = MoveTo::create(0.5f, Vec2(gameP));
                auto callback = CallFunc::create([&](){
//                    sp->removeFromParent();
                    Sprite*item = static_cast<Sprite*>(gameGroup->getChildByName("boomskill"));
                    MovieClip*boomc = new MovieClip();
                    boomc->playFile("boom.plist", "eff_00005", 1, 8, 1, true);
                    gameGroup->addChild(boomc);
                    boomc->setPosition(item->getPosition());
                    item->removeFromParent();
                    
                });
                sp->runAction(Sequence::create(moveTo,callback, NULL));
            }else if(skillIndex == 3)
            {
                for(int i = 1;i<=6;i++)
                {
                    Sprite*sp = Sprite::create("assets/qiang_png.png");
                    gameGroup->addChild(sp);
                    sp->setName("qiangskill");
                    sp->setPosition(Vec2(gameP.x,gameGroup->getContentSize().height + 100));
                    sp->setAnchorPoint(Vec2(0,0));
                    auto moveTo = MoveTo::create(0.2f, Vec2(gameP));
                    auto delay = DelayTime::create(0.2f*float(i));
                    auto callback = CallFunc::create([=](){
                        sp->removeFromParent();
                    });
                    sp->runAction(Sequence::create(delay,moveTo,callback, NULL));
                }
                
            }
        }
        
    }else if(resultGroup->getBoundingBox().containsPoint(localP) && resultGroup->isVisible())
    {
        Point resultP = resultGroup->convertToNodeSpace(tTouch->getLocation());
        if(resetBtn->getBoundingBox().containsPoint(resultP))
        {
            resultGroup->setVisible(false);
            this->resetData();
            this->schedule(schedule_selector(GameScene::createEntity), 1.0f);
        }
    }
    
    
}
