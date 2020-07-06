//
//  GameScene.cpp
//  pointKill-mobile
//
//  Created by 志强 on 2020/5/18.
//

#include "GameScene.h"
#include "SoundCtl.h"
#include "HelloWorldScene.h"
#include "CommonUtil.h"
#include "ResultPop.h"
#include "Config.h"
#include "SelectLevel.hpp"
#include "SetPop.hpp"

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
bool GameScene::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
    auto rootNode = CSLoader::createNode("GameScene.csb");
    addChild(rootNode);
    
    topPanel = static_cast<Layout*>(rootNode->getChildByName("topPanel"));
    bottomGroup = static_cast<Layout*>(rootNode->getChildByName("bottomGroup"));
    otherGroup = static_cast<Layout*>(rootNode->getChildByName("otherGroup"));
    gameGroup = static_cast<Layout*>(rootNode->getChildByName("gameGroup"));
    map = static_cast<Sprite*>(rootNode->getChildByName("map"));
    setBtn = static_cast<Sprite*>(rootNode->getChildByName("setBtn"));
    bottombg = static_cast<Sprite*>(rootNode->getChildByName("bottombg"));
    goldNum = static_cast<Text*>(otherGroup->getChildByName("goldNum"));
    hpBar = static_cast<LoadingBar*>(topPanel->getChildByName("hpBar"));
    mpBar = static_cast<LoadingBar*>(topPanel->getChildByName("mpBar"));
    hpNum = static_cast<Text*>(bottomGroup->getChildByName("hpNum"));
    mpNum = static_cast<Text*>(bottomGroup->getChildByName("mpNum"));
    gemNum = static_cast<Text*>(bottomGroup->getChildByName("gemNum"));
    megaNum = static_cast<Text*>(bottomGroup->getChildByName("megaNum"));
    shieldNum = static_cast<Text*>(bottomGroup->getChildByName("shieldNum"));
    useHp = static_cast<Sprite*>(bottomGroup->getChildByName("useHp"));
    useMp = static_cast<Sprite*>(bottomGroup->getChildByName("useMp"));
    useGem = static_cast<Sprite*>(bottomGroup->getChildByName("useGem"));
    uesMega = static_cast<Sprite*>(bottomGroup->getChildByName("uesMega"));
    useShield = static_cast<Sprite*>(bottomGroup->getChildByName("useShield"));
    buyHp = static_cast<Sprite*>(bottomGroup->getChildByName("buyHp"));
    buyMp = static_cast<Sprite*>(bottomGroup->getChildByName("buyMp"));
    buyGem = static_cast<Sprite*>(bottomGroup->getChildByName("buyGem"));
    buyMega = static_cast<Sprite*>(bottomGroup->getChildByName("buyMega"));
    buySheld = static_cast<Sprite*>(bottomGroup->getChildByName("buySheld"));
    expBg = static_cast<Sprite*>(rootNode->getChildByName("expBg"));
    expBar = static_cast<LoadingBar*>(rootNode->getChildByName("expBar"));
    
    _listener=EventListenerTouchOneByOne::create();
    _listener->setSwallowTouches(true);
    _listener->onTouchBegan = CC_CALLBACK_2(GameScene::menuBegin,this);
    _listener->onTouchEnded = CC_CALLBACK_2(GameScene::menuEndCallback, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(_listener, this);
    this->adapter();
    this->refreshPageData();
    
    roleMc = new MovieClip();
    roleMc->setName("role");
    gameGroup->addChild(roleMc);
    roleMc->playFile("hero_6_4.plist", "hero-idle", 1, 4, -1, false);
    roleMc->setScale(1.5f);
    Size visible = Director::getInstance()->getVisibleSize();
    roleMc->setPosition(Vec2(visible.width/2,bottomGroup->getPositionY() + 300));
//    levelCount = 1;
//    this->createEnemy();
    Texture2D*texture = Director::getInstance()->getTextureCache()->addImage(StringUtils::format("backgrounds/map%d.png",Config::getInstance()->curLevel));
    map->setTexture(texture);
    expBar->setPercent(0.0f);
    if(Config::getInstance()->level != 1)
    {
        SelectLevel*select = new SelectLevel();
        topShow = true;
        select->callback = CC_CALLBACK_1(GameScene::topcallback, this);
        this->addChild(select);
    }else{
        topShow = true;
        Sprite*guidebg = Sprite::create("ui/guidebg.png");
        guidebg->setName("guide");
        this->addChild(guidebg);
        int bgW = map->getContentSize().width;
        int bgH = map->getContentSize().height;
        Size visibleSize = Director::getInstance()->getVisibleSize();
        guidebg->setAnchorPoint(Vec2(0.5,0.5));
        guidebg->setPosition(Vec2(visible.width/2,visible.height/2));
        float scaleX = visibleSize.width/bgW;
        float scaleY = visibleSize.height/bgH;
        guidebg->setScale(0.0f);
        guidebg->setOpacity(0);
  
        auto fadein = FadeIn::create(0.5f);
        auto scaleTo = ScaleTo::create(0.5f, scaleX, scaleY);
        auto spawn = Spawn::createWithTwoActions(fadein, scaleTo);
        auto delay = DelayTime::create(2.0f);
        auto callback = CallFunc::create([&](){
            topShow = false;
            this->schedule(schedule_selector(GameScene::updateEnemy), 5.0f);
            Sprite*guidebg = static_cast<Sprite*>(this->getChildByName("guide"));
            if(guidebg){
                guidebg->removeFromParent();
            }
        });
        guidebg->runAction(Sequence::create(delay,spawn,delay,callback, NULL));
    }
    return true;
}
void GameScene::topcallback(int state)
{
    if(state == 0)
    {
        auto scene = HelloWorld::createScene();
        Director::getInstance()->replaceScene(TransitionTurnOffTiles::create(1, scene));
    }else{
        topShow = false;
        this->schedule(schedule_selector(GameScene::updateEnemy), 5.0f);
    }
    
}
void GameScene::setCall()
{
    topShow = false;
    this->schedule(schedule_selector(GameScene::updateEnemy), 5.0f);
}
void GameScene::createEnemy()
{
    if(timecount % 20 == 0)
    {
        levelCount += 1;
    }
    Vector<Node*> children = gameGroup->getChildren();
    for(auto c : children)
    {
        if(c->getTag() == 111)
        {
            float distance=sqrt(pow(c->getPositionX()-roleMc->getPositionX(),2)+pow(c->getPositionY()-roleMc->getPositionY(),2));
            if(distance <= 50)
            {
                this->reduceHp();
            }
        }
    }
    Size visible = Director::getInstance()->getVisibleSize();
    for(int i = 0;i<levelCount;i++)
    {
        srand(int(time(0))+ rand());//置随机数种子
        int ran2=CCRANDOM_0_1()*3 + 1;
        std::string remainass = ran2 == 1?"6":ran2 == 2?"6":"4";
        MovieClip*monsterMc = new MovieClip();

        gameGroup->addChild(monsterMc);
        std::string keyname = StringUtils::format("monster%d_idle",ran2);
        std::string plistName = StringUtils::format("monster%d_",ran2)+remainass+".plist";
        int count = ran2*Config::getInstance()->curLevel;
        monsterMc->setName(StringUtils::format("%d",count));
        monsterMc->setTag(111);
        monsterMc->playFile(plistName.c_str(), keyname.c_str(), 1,atoi(remainass.c_str()), -1, false);
        _enemys.pushBack(monsterMc);
        if(ran2 == 1)
        {
            //天空飞的
            srand(int(time(0))+ rand());//置随机数种子
            int ranx = CCRANDOM_0_1()*visible.width;
            int rany = visible.height/2 + CCRANDOM_0_1()*(visible.height/2); + 100;
//            monsterMc->setScale(1.5f);
            monsterMc->setScale(-1.5f, 1.5f);
            monsterMc->setPosition(Vec2(ranx,rany));
            
            auto callback = CallFunc::create([=](){
                //移动执行完毕
                monsterMc->stopAllActions();
                
            });
            if(rany <= visible.height)
            {
                int index = CCRANDOM_0_1()*2;
                int scaleX = index == 0?-1:1;
                monsterMc->setScaleX(monsterMc->getScaleX()*scaleX);
                if(scaleX == -1)
                {
                    ranx = -100;
                }else{
                    ranx = visible.width + 100;
                }
                monsterMc->setPosition(Vec2(ranx,rany));
                int moveX = visible.width/2 + 150*scaleX;
                auto move1 = MoveTo::create(4.0f, Vec2(moveX,rany));
                
                float distance=sqrt(pow(moveX-roleMc->getPositionX(),2)+pow(rany-roleMc->getPositionY(),2));
                float time = distance/100.0f;
                auto move2 = MoveTo::create(time, Vec2(roleMc->getPositionX(),roleMc->getPositionY()));
                auto seque = Sequence::create(move1,move2,callback, NULL);
                monsterMc->runAction(seque);
            }else{
                float distance=sqrt(pow(ranx-roleMc->getPositionX(),2)+pow(rany-roleMc->getPositionY(),2));
                float time = distance/300.0f;
                auto move = MoveTo::create(time, Vec2(roleMc->getPositionX(),roleMc->getPositionY()));
                auto seque = Sequence::create(move,callback, NULL);
                monsterMc->runAction(seque);
            }
            
        }else if(ran2 == 2)
        {
            int ranx = CCRANDOM_0_1()*visible.width;
            int rany = roleMc->getPositionY() - 50;
            //地上爬的
            
            int index = CCRANDOM_0_1()*2;
            int scaleX = index == 0?-1:1;
            monsterMc->setScaleX(monsterMc->getScaleX()*(-scaleX));
            if(scaleX == -1)
            {
               ranx = -50;
            }else{
               ranx = visible.width + 50;
            }
            monsterMc->setPosition(Vec2(ranx,rany));
            float randomNum = CCRANDOM_0_1()*70;
            auto moveto = MoveTo::create(4.0f, Vec2(roleMc->getPositionX() + scaleX*randomNum,rany));
            monsterMc->runAction(moveto);
        }else if(ran2 == 3)
        {
            //天上掉下来的
            int ranx =roleMc->getPositionX() + CCRANDOM_0_1()*roleMc->getContentSize().width;
            int rany =visible.height + 100;
            monsterMc->setPosition(Vec2(ranx,rany));
            monsterMc->setScale(0.7f);
            auto move = MoveTo::create(1.0f, Vec2(ranx,roleMc->getPositionY() - 50));
            auto callback = CallFunc::create([=](){
                shakeNode(this);
            });
            auto seque = Sequence::create(move,callback, NULL);
            monsterMc->runAction(seque);
            
        }
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

void GameScene::updateEnemy(float dt)
{
    timecount += 1;
    this->createEnemy();
}
bool GameScene::menuBegin(cocos2d::Touch* tTouch,cocos2d::Event* eEvent)
{
    Point localP = this->convertToNodeSpace(tTouch->getLocation());
    Point groupP = bottomGroup->convertToNodeSpace(tTouch->getLocation());
    if(setBtn->getBoundingBox().containsPoint(localP) && !topShow){
       setBtn->setScale(0.9f);
       return true;
    }else if(buyHp->getBoundingBox().containsPoint(groupP) || buyMp->getBoundingBox().containsPoint(groupP) || buyGem->getBoundingBox().containsPoint(groupP) || buyMega->getBoundingBox().containsPoint(groupP) || buySheld->getBoundingBox().containsPoint(groupP))
    {
        if(topShow){return false;}
        return true;
    }else if(useHp->getBoundingBox().containsPoint(groupP) || useMp->getBoundingBox().containsPoint(groupP) || useGem->getBoundingBox().containsPoint(groupP) || uesMega->getBoundingBox().containsPoint(groupP) || useShield->getBoundingBox().containsPoint(groupP))
    {
         if(topShow){return false;}
        return true;
    }
    
    else if(gameGroup->getBoundingBox().containsPoint(localP) && !topShow)
    {
        return true;
    }
    
    return false;
}
void GameScene::menuEndCallback(cocos2d::Touch* tTouch,cocos2d::Event* eEvent)
{
    Point localP = this->convertToNodeSpace(tTouch->getLocation());
    Point groupP = bottomGroup->convertToNodeSpace(tTouch->getLocation());
    if(setBtn->getBoundingBox().containsPoint(localP)  && !topShow){
        setBtn->setScale(1.0f);
        topShow = true;
        SetPop*setpop = new SetPop(0);
        setpop->callback = CC_CALLBACK_0(GameScene::setCall, this);
        this->unschedule(schedule_selector(GameScene::updateEnemy));
        this->addChild(setpop);
        return;
    }else if(buyHp->getBoundingBox().containsPoint(groupP)  && !topShow)
    {
        if(gold < 50){this->floatFont("not enough money");return;}
        gold -= 50;
        SoundCtl::getInstance()->playEffect("sound/buy.m4a");
        hpnum += 1;
        this->refreshPageData();
    }else if(buyMp->getBoundingBox().containsPoint(groupP)  && !topShow)
    {
        if(gold < 50){this->floatFont("not enough money");return;}
        gold -= 50;
        mpnum += 1;
        SoundCtl::getInstance()->playEffect("sound/buy.m4a");
        this->refreshPageData();
    }else if(buyGem->getBoundingBox().containsPoint(groupP) && !topShow)
    {
        if(gold < 50){this->floatFont("not enough money");return;}
        gold -= 50;
        gemnum += 1;
        SoundCtl::getInstance()->playEffect("sound/buy.m4a");
        this->refreshPageData();
    }else if(buyMega->getBoundingBox().containsPoint(groupP) && !topShow)
    {
        if(gold < 50){this->floatFont("not enough money");return;}
        gold -= 50;
        meganum += 1;
        SoundCtl::getInstance()->playEffect("sound/buy.m4a");
        this->refreshPageData();
    }else if(buySheld->getBoundingBox().containsPoint(groupP) && !topShow)
    {
        
        if(gold < 50){this->floatFont("not enough money");return;}
        gold -= 50;
        shieldnum += 1;
        this->refreshPageData();
        SoundCtl::getInstance()->playEffect("sound/buy.m4a");
    }else if(useHp->getBoundingBox().containsPoint(groupP) && !topShow)
    {
        if(hpnum <= 0){this->floatFont("not enough good");return;}
        SoundCtl::getInstance()->playEffect("sound/potion.m4a");
        curHp += 15;
        hpnum -= 1;
        if(curHp >= totalHp){curHp = totalHp;}
        this->refreshPageData();
    }else if(useMp->getBoundingBox().containsPoint(groupP) && !topShow)
    {
        if(mpnum <= 0){this->floatFont("not enough good");return;}
        SoundCtl::getInstance()->playEffect("sound/potion.m4a");
        curMp += 15;
        mpnum -= 1;
        if(curMp >= totalMp){curMp = totalMp;}
        this->refreshPageData();
    }else if(useGem->getBoundingBox().containsPoint(groupP) && !topShow)
    {
        if(gemnum <= 0){this->floatFont("not enough good");return;}
        SoundCtl::getInstance()->playEffect("sound/potion.m4a");
        gemnum -= 1;
        this->refreshPageData();
        this->promptAtk();
    }else if(uesMega->getBoundingBox().containsPoint(groupP) && !topShow)
    {
        if(meganum <= 0){this->floatFont("not enough good");return;}
        meganum-=1;
        this->refreshPageData();
        this->atkAll();
    }else if(useShield->getBoundingBox().containsPoint(groupP) && !topShow)
    {
        if(shieldnum <= 0){this->floatFont("not enough good");return;}
        shieldnum -= 1;
        this->refreshPageData();
        this->promptDef();
    }else if(gameGroup->getBoundingBox().containsPoint(localP) && !topShow)
    {
        Size visible = Director::getInstance()->getVisibleSize();
        if(localP.x > (visible.width/2 + 50))
        {
            roleMc->setScaleX(1.5f);
        }else if (localP.x < (visible.width/2 - 50))
        {
            roleMc->setScaleX(-1.5f);
        }
        Point enemyP = gameGroup->convertToNodeSpace(tTouch->getLocation());
        Vector<Node*> childrens = gameGroup->getChildren();
        for(auto c :childrens)
        {
            if(c->getName() == "buff"){continue;}
            if(c->getName() == "role" || c->getName() == "none" || c->getName() == "daoguang"){continue;}
            if(c->getName() == "gold")
            {
                c->setName("none");
                auto fadeout = FadeOut::create(1.0f);
                auto scaleTo = ScaleTo::create(1.0f, 0.0f);
                auto callBack = CallFunc::create([=](){
                    c->removeFromParent();
                });
                gold += 10;
                goldNum->setString(StringUtils::format("%d",gold));
                auto swapn = Spawn::create(fadeout,scaleTo, NULL);
                auto sque = Sequence::create(swapn,callBack, NULL);
                c->runAction(sque);
                continue;
            }
            float distance=sqrt(pow(c->getPositionX()-localP.x,2)+pow(c->getPositionY()-localP.y,2));
            Rect rect = c->getBoundingBox();
            rect.size.width = rect.size.height = 100;
            if(distance <= 70)
            {
                roleMc->frameRate = 0.03f;
                roleMc->changeAction("hero-attack", 1, 6, 1, false);
                
                auto roleDelay = DelayTime::create(0.5f);
                 MovieClip*daoguang = new MovieClip();
                auto roleCallBack = CallFunc::create([&](){
                    roleMc->changeAction("hero-idle", 1, 4, -1, false);
                });
                auto callBack = CallFunc::create([=](){
                    daoguang->removeFromParent();
                });
                auto roleseque = Sequence::create(roleDelay,roleCallBack,callBack, NULL);
                roleMc->runAction(roleseque);
                
                MovieClip*mc = static_cast<MovieClip*>(c);
                
               
                daoguang->setName("daoguang");
                gameGroup->addChild(daoguang);
                daoguang->playFile("110104.plist", "110104", 1, 7, 1, false);
                daoguang->setPosition(Vec2(mc->getPositionX(),mc->getPositionY()));
                srand(int(time(0))+ rand());//置随机数种子
                int rotation=CCRANDOM_0_1()*360;
                daoguang->setRotation(rotation);
                
                int tag = mc->getTag();
                int num = atoi(mc->getName().c_str());
                num -= (1+promptatk);
                if(num <= 0)
                {
                    Sprite*goldIcon = Sprite::create("item/gold.png");
                    gameGroup->addChild(goldIcon);
                    goldIcon->setPosition(Vec2(mc->getPositionX(),mc->getPositionY()));
                    goldIcon->setName("gold");
                    Point startp = goldIcon->getPosition();
                    int offx = startp.x > (visible.width/2) ? -100 : 100;
                    Point endp = Point(startp.x + offx,roleMc->getPositionY() - 50);
                    auto move = CommonUtil::createParabola(0.5, startp, endp, 100, 20);
                    goldIcon->runAction(move);
                    mc->stopAllActions();
                    mc->removeFromParent();
                    curExp += 10;
                    float percent = float(curExp)/float(Config::getInstance()->curLevel*200.0f)*100.0f;
                    if(percent >= 100.0f)
                    {
                        Config::getInstance()->curLevel += 1;
                        if(Config::getInstance()->curLevel >= Config::getInstance()->level)
                        {
                            Config::getInstance()->level += 1;
                        }
                        percent = 100.0f;
                        topShow = true;
                        curExp = 0;
                        this->unschedule(schedule_selector(GameScene::updateEnemy));
                        ResultPop*result = new ResultPop(1);
                        this->addChild(result);
                        result->resetFun = CC_CALLBACK_0(GameScene::resetData, this);
                    }
                    expBar->setPercent(percent);
                    break;
                }
                mc->setName(StringUtils::format("%d",num));
                
               
                break;
            }
        }
    }
}
void GameScene::promptAtk()
{
    //开启狂暴 提升攻击
    if(promptatk == 1){return;}
    MovieClip*mc = new MovieClip;
    gameGroup->addChild(mc);
    mc->playFile("900013.plist", "900013", 1, 8, -1, false);
    mc->setOpacity(100);
    mc->setName("buff");
    mc->setScale(0.5f);
    mc->setPosition(Vec2(roleMc->getPositionX(),roleMc->getPositionY() + 50));
    auto tintTo = DelayTime::create(5.0f);
    promptatk = 1;
    auto callback = CallFunc::create([=](){
        mc->removeFromParent();
        promptatk = 0;
    });
    auto seque = Sequence::create(tintTo,callback, NULL);
    roleMc->runAction(seque);
}
void GameScene::promptDef()
{
    //开启盾 提升防御
    if(promptdef == 1){return;}
    MovieClip*mc = new MovieClip;
    gameGroup->addChild(mc);
    mc->playFile("900011.plist", "900011", 1, 8, -1, false);
    mc->setOpacity(100);
    mc->setScale(0.5f);
    mc->setName("buff");
    mc->setPosition(Vec2(roleMc->getPositionX(),roleMc->getPositionY() + 50));
    auto tintTo = DelayTime::create(5.0f);
    promptatk = 1;
    auto callback = CallFunc::create([=](){
        mc->removeFromParent();
        promptdef = 0;
    });
    auto seque = Sequence::create(tintTo,callback, NULL);
    roleMc->runAction(seque);
}
void GameScene::atkAll()
{
    //攻击全部
    Vector<Node*> childrens = gameGroup->getChildren();
    Size visible = Director::getInstance()->getVisibleSize();
    for(auto c :childrens)
    {
        MovieClip*mc = static_cast<MovieClip*>(c);
        if(c->getName() == "role" || c->getName() == "gold"){continue;}
        int num = atoi(mc->getName().c_str());
        num = 0;
        if(num <= 0)
        {
            Sprite*goldIcon = Sprite::create("item/gold.png");
            gameGroup->addChild(goldIcon);
            goldIcon->setPosition(Vec2(mc->getPositionX(),mc->getPositionY()));
            goldIcon->setName("gold");
            Point startp = goldIcon->getPosition();
            int offx = startp.x > (visible.width/2) ? -100 : 100;
            Point endp = Point(startp.x + offx,roleMc->getPositionY() - 50);
            auto move = CommonUtil::createParabola(0.5, startp, endp, 100, 20);
            goldIcon->runAction(move);
            mc->stopAllActions();
            mc->removeFromParent();
        }
    }
}
void GameScene::resetData()
{
    totalHp = 30;
    totalMp = 30;
    curHp = 30;
    curMp = 30; 
    hpnum = 0;
    mpnum = 0;
    gemnum = 0;
    meganum = 0;
    shieldnum = 0;
    timecount = 0;
    levelCount = 3;
    promptatk = 0;
    promptdef = 0;
    curExp = 0;
    expBar->setPercent(0.0f);
    battleEnd = false;
    topShow = false;
    Texture2D*texture = Director::getInstance()->getTextureCache()->addImage(StringUtils::format("backgrounds/map%d.png",Config::getInstance()->curLevel));
    map->setTexture(texture);
    Vector<Node*> childrens = gameGroup->getChildren();
    for(auto c : childrens)
    {
        MovieClip*mc = static_cast<MovieClip*>(c);
        if(mc->getName() == "role"){continue;}
        mc->removeFromParent();
    }
    this->refreshPageData();
    this->schedule(schedule_selector(GameScene::updateEnemy), 5.0f);
}
void GameScene::floatFont(std::string cnt)
{
    Text*content = Text::create();
    this->addChild(content);
    content->setString(cnt);
    content->setFontSize(30);
    content->setColor(Color3B(0,0,0));
    Size visible = Director::getInstance()->getVisibleSize();
    content->setPosition(Vec2(visible.width/2,visible.height/2));
    auto move = MoveTo::create(1.0f, Vec2(content->getPositionX(),content->getPositionY() + 100));
    auto call = CallFunc::create([=](){
        content->removeFromParent();
    });
    auto seque = Sequence::create(move,call, NULL);
    content->runAction(seque);
}
void GameScene::reduceHp()
{
    Text*content = Text::create();
    gameGroup->addChild(content);
    content->setString("1");
    content->setFontSize(30);
    content->setColor(Color3B(165,42,42));
    int ranx = roleMc->getPositionX() + CCRANDOM_0_1()*roleMc->getContentSize().width;
    int rany = roleMc->getPositionY() + CCRANDOM_0_1()*roleMc->getContentSize().height;
    
    content->setPosition(Vec2(ranx,rany));
    
    auto move = MoveTo::create(1.0f, Vec2(content->getPositionX(),content->getPositionY() + 100));
    auto call = CallFunc::create([=](){
        content->removeFromParent();
    });
    auto seque = Sequence::create(move,call, NULL);
    content->runAction(seque);
    curHp -= 1;
    this->refreshPageData();
    if(curHp <= 0 && !battleEnd)
    {
        battleEnd = true;
        //游戏失败
        this->unschedule(schedule_selector(GameScene::updateEnemy));
        Vector<Node*> childrens = gameGroup->getChildren();
        for(auto c : childrens)
        {
            MovieClip*mc = static_cast<MovieClip*>(c);
            if(mc->getName() == "role"){continue;}
            mc->removeFromParent();
        }
        
        ResultPop*result = new ResultPop(0);
        this->addChild(result);
        result->resetFun = CC_CALLBACK_0(GameScene::resetData, this);
    }
}
void GameScene::refreshPageData()
{
    float hpper = float(curHp)/float(totalHp)*100.0f;
    hpBar->setPercent(hpper);
    float mpper = float(curMp)/float(totalMp)*100.0f;
    mpBar->setPercent(mpper);
    goldNum->setString(StringUtils::format("%d",gold));
    hpNum->setString(StringUtils::format("%d",hpnum));
    mpNum->setString(StringUtils::format("%d",mpnum));
    gemNum->setString(StringUtils::format("%d",gemnum));
    megaNum->setString(StringUtils::format("%d",meganum));
    shieldNum->setString(StringUtils::format("%d",shieldnum));
}
void GameScene::adapter()
{
    int bgW = map->getContentSize().width;
    int bgH = map->getContentSize().height;
    Size visibleSize = Director::getInstance()->getVisibleSize();
    float scaleX = visibleSize.width/bgW;
    float scaleY = visibleSize.height/bgH;
//    map->setScale(scaleX, scaleY);
//    gameGroup->setScale(scaleX,scaleY);
    topPanel->setPositionY(topPanel->getPositionY()*scaleY);
    otherGroup->setPositionY(otherGroup->getPositionY()*scaleY);
    
    setBtn->setPosition(Vec2(setBtn->getPositionX()*scaleX,setBtn->getPositionY()*scaleY));
    bottomGroup->setScale(bottomGroup->getScaleX()*scaleY);
    bottomGroup->setPosition(Vec2(bottomGroup->getPositionX()*scaleX,bottomGroup->getPositionY()*scaleY));
    bottombg->setScale(bottombg->getScaleX()*scaleY);
    bottombg->setPosition(Vec2(bottombg->getPositionX()*scaleX,bottombg->getPositionY()*scaleY));
    expBg->setPosition(Vec2(expBg->getPositionX()*scaleX,expBg->getPositionY()*scaleY));
    expBar->setPosition(Vec2(expBar->getPositionX()*scaleX,expBar->getPositionY()*scaleY));
}
void GameScene::onExit()
{
    Layer::onExit();
    _eventDispatcher->removeEventListener(_listener);
}

