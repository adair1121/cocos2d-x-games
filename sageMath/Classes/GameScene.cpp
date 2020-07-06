//
//  GameScene.cpp
//  sageMath-mobile
//
//  Created by 志强 on 2020/5/21.
//

#include "GameScene.h"
#include "Config.h"
#include "ResultPop.h"
#include "HelloWorldScene.h"
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
    hp1 = static_cast<Button*>(rootNode->getChildByName("hp1"));
    hp2 = static_cast<Button*>(rootNode->getChildByName("hp2"));
    hp3 = static_cast<Button*>(rootNode->getChildByName("hp3"));
    setBtn = static_cast<Button*>(rootNode->getChildByName("setBtn"));
    hpBg = static_cast<Sprite*>(rootNode->getChildByName("hpBg"));
     
    topPanel = static_cast<Layout*>(rootNode->getChildByName("topPanel"));
    levelLab = static_cast<TextBMFont*>(topPanel->getChildByName("levelLab"));
     
    gameGroup = static_cast<Layout*>(rootNode->getChildByName("gameGroup"));
    
    operGrop = static_cast<Layout*>(rootNode->getChildByName("operGrop"));
    bottomBg = static_cast<Sprite*>(operGrop->getChildByName("bottomBg"));
    lab1 = static_cast<TextBMFont*>(operGrop->getChildByName("lab1"));
    lab2 = static_cast<TextBMFont*>(operGrop->getChildByName("lab2"));
    add = static_cast<Sprite*>(operGrop->getChildByName("add"));
    sub = static_cast<Sprite*>(operGrop->getChildByName("sub"));
    mul = static_cast<Sprite*>(operGrop->getChildByName("mul"));
    div = static_cast<Sprite*>(operGrop->getChildByName("div"));
    addRect = static_cast<Layout*>(operGrop->getChildByName("addRect"));
    subRect = static_cast<Layout*>(operGrop->getChildByName("subRect"));
    mulRect = static_cast<Layout*>(operGrop->getChildByName("mulRect"));
    divRect = static_cast<Layout*>(operGrop->getChildByName("divRect"));
    
    backBtn = static_cast<Button*>(rootNode->getChildByName("backBtn"));
    
    
    _listener=EventListenerTouchOneByOne::create();
    _listener->setSwallowTouches(true);
    _listener->onTouchBegan = CC_CALLBACK_2(GameScene::menuBegin,this);
    _listener->onTouchEnded = CC_CALLBACK_2(GameScene::menuEndCallback, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(_listener, operGrop);
    this->adapter();
    
    levelLab->setString(StringUtils::format("%d",CF()->curChallengeLev));
    
    Texture2D*texture = Director::getInstance()->getTextureCache()->addImage(StringUtils::format("assets/map%d.png",CF()->enterMap));
    bg->setTexture(texture);
    
    roleMc = new MovieClip();
    gameGroup->addChild(roleMc);
    roleMc->playFile("sagemath.plist", "sagemath_idle_", 1, 4, -1, false);
    roleMc->setScale(1.5f);
    roleMc->setPosition(Vec2(100,300));
    this->resetData();
    return true;
}
void GameScene::resetData()
{
    battleEnd = false;
    hpcount = 3;
    gameCount = 1;
    hp1->setBrightStyle(BRIGHT_NORMAL);
    hp2->setBrightStyle(BRIGHT_NORMAL);
    hp3->setBrightStyle(BRIGHT_NORMAL);
    lab1->setString("0");
    lab2->setString("0");
    levelLab->setString(StringUtils::format("%d",CF()->curChallengeLev));
    this->calculNum();
}
void GameScene::changeRoleAction(std::string actionstr)
{
    int endIndex = (actionstr == "attack")?7:4;
    int loop = -1;
    if(actionstr == "attack")
    {
        MovieClip*atkmc = new MovieClip();
        gameGroup->addChild(atkmc);
        atkmc->setName("atk");
        atkmc->frameRate = 0.02f;
        atkmc->playFile("atk.plist", "frame", 1, 18, 1, true);
        atkmc->setPosition(Vec2(enemy->getPositionX() - 100,enemy->getPositionY()));
        loop = 1;
        auto delay = DelayTime::create(0.5f);
        auto callback = CallFunc::create([&](){
            changeRoleAction("idle");
            MovieClip*atk = static_cast<MovieClip*>(gameGroup->getChildByName("atk"));
            if(atk)
            {
                atk->removeFromParent();
            }
        });
        roleMc->runAction(Sequence::create(delay,callback, NULL));
    }
    std::string keystr = "sagemath_"+actionstr+"_";
    roleMc->changeAction(keystr.c_str(), 1, endIndex, loop, false);
}
void GameScene::calculNum()
{
    int baseValue = CF()->curMapLev* 400;
    srand(int(time(0))+ rand());//置随机数种子
    int oper = CCRANDOM_0_1()*4;
    int startNum;
    int endNum;
    int remainNum;
    if(oper == 0)
    {
        startNum = CCRANDOM_0_1()*(baseValue/2) + 1;
        remainNum = CCRANDOM_0_1()*(baseValue/2) + 1;
        endNum = startNum + remainNum;
    }else if(oper == 1)
    {
        startNum = CCRANDOM_0_1()*(baseValue/2) + (baseValue/2);
        remainNum = CCRANDOM_0_1()*(baseValue/2) + 1;
        endNum = startNum - remainNum;
    }else if(oper == 2)
    {
        startNum = CCRANDOM_0_1()*(baseValue/2) + 1;
        remainNum = CCRANDOM_0_1()*20 + 1;
        endNum = startNum*remainNum;
    }else
    {
        int num1 = CCRANDOM_0_1()*(baseValue/2)+ 1;
        int num2 = CCRANDOM_0_1()*20 + 1;
        int num3 = num1*num2;
        remainNum = num2;
        endNum = num1;
        
        startNum = num3;
    }
    lab1->setString(StringUtils::format("%d",startNum));
    lab2->setString(StringUtils::format("%d",endNum));
    //创建怪物
    enemy = new MonEntity(remainNum);
    gameGroup->addChild(enemy);
    enemy->setPosition(Vec2(gameGroup->getContentSize().width + 100,roleMc->getPositionY()));
    float time = 12.0f - (4.0f/CF()->totalLev)*float(CF()->curChallengeLev);
    auto moveTo = MoveTo::create(time, Vec2(roleMc->getPositionX(),roleMc->getPositionY() + 30));
    auto callback = CallFunc::create([&](){
        enemy->removeFromParent();
        //游戏失败
        this->gameFail();
    });
    enemy->runAction(Sequence::create(moveTo,callback, NULL));
}
void GameScene::adapter()
{
    int bgW = bg->getContentSize().width;
    int bgH = bg->getContentSize().height;
    Size visibleSize = Director::getInstance()->getVisibleSize();
    float scaleX = visibleSize.width/bgW;
    float scaleY = visibleSize.height/bgH;
    bg->setScale(scaleX, scaleY);
    hp1->setPosition(Vec2(hp1->getPositionX()*scaleX,hp1->getPositionY()*scaleY));
    hp2->setPosition(Vec2(hp2->getPositionX()*scaleX,hp2->getPositionY()*scaleY));
    hp3->setPosition(Vec2(hp3->getPositionX()*scaleX,hp3->getPositionY()*scaleY));
    hpBg->setPosition(Vec2(hpBg->getPositionX()*scaleX,hpBg->getPositionY()*scaleY));
    gameGroup->setScale(scaleX,scaleY);
    gameGroup->setPosition(Vec2(gameGroup->getPositionX()*scaleX,gameGroup->getPositionY()*scaleY));
    operGrop->setScale(scaleX);
    operGrop->setPosition(Vec2(operGrop->getPositionX()*scaleX,operGrop->getPositionY()*scaleY));
    setBtn->setPosition(Vec2(setBtn->getPositionX()*scaleX,setBtn->getPositionY()*scaleY));
    topPanel->setPosition(Vec2(topPanel->getPositionX()*scaleX,topPanel->getPositionY()*scaleY));
}
void GameScene::onExit()
{
    Layer::onExit();
    _eventDispatcher->removeEventListener(_listener);
}
bool GameScene::menuBegin(cocos2d::Touch* tTouch,cocos2d::Event* eEvent)
{
    Point localP = operGrop->convertToNodeSpace(tTouch->getLocation());
    Point localP2 = this->convertToNodeSpace(tTouch->getLocation());
    Rect rec = addRect->getBoundingBox();
    if(addRect->getBoundingBox().containsPoint(localP) && !battleEnd)
    {
        auto moveTo = MoveTo::create(0.2f, Vec2(add->getPositionX(),add->getPositionY() + 50));
        auto moveTo2 = MoveTo::create(0.2f, Vec2(add->getPositionX(),add->getPositionY()));
        add->runAction(Sequence::create(moveTo,moveTo2, NULL));
        return true;
    }else if(subRect->getBoundingBox().containsPoint(localP)  && !battleEnd)
    {
        auto moveTo = MoveTo::create(0.2f, Vec2(sub->getPositionX(),sub->getPositionY() + 50));
        auto moveTo2 = MoveTo::create(0.2f, Vec2(sub->getPositionX(),sub->getPositionY()));
        sub->runAction(Sequence::create(moveTo,moveTo2, NULL));
        return true;
    }else if(mulRect->getBoundingBox().containsPoint(localP) && !battleEnd)
    {
        auto moveTo = MoveTo::create(0.2f, Vec2(mul->getPositionX(),mul->getPositionY() + 50));
        auto moveTo2 = MoveTo::create(0.2f, Vec2(mul->getPositionX(),mul->getPositionY()));
        mul->runAction(Sequence::create(moveTo,moveTo2, NULL));
        return true;
    }else if(divRect->getBoundingBox().containsPoint(localP) && !battleEnd)
    {
        auto moveTo = MoveTo::create(0.2f, Vec2(div->getPositionX(),div->getPositionY() + 50));
        auto moveTo2 = MoveTo::create(0.2f, Vec2(div->getPositionX(),div->getPositionY()));
        div->runAction(Sequence::create(moveTo,moveTo2, NULL));
        return true;
    }else if(backBtn->getBoundingBox().containsPoint(localP2))
    {
        return true;
    }
    return false;
}
void GameScene::menuEndCallback(cocos2d::Touch* tTouch,cocos2d::Event* eEvent)
{
    Point localP = operGrop->convertToNodeSpace(tTouch->getLocation());
    Point localP2 = this->convertToNodeSpace(tTouch->getLocation());
    int startNum = atoi(lab1->getString().c_str());
    int remainNum = enemy->getRemainNum();
    int endNum = atoi(lab2->getString().c_str());
    
    if(addRect->getBoundingBox().containsPoint(localP) && !battleEnd)
    {
        this->judgeCorrect(startNum + remainNum,endNum);
        changeRoleAction("attack");
    }else if(mulRect->getBoundingBox().containsPoint(localP) && !battleEnd)
    {
        this->judgeCorrect(startNum * remainNum,endNum);
        changeRoleAction("attack");
    }
    else if(subRect->getBoundingBox().containsPoint(localP) && !battleEnd)
    {
        this->judgeCorrect(startNum - remainNum,endNum);
        changeRoleAction("attack");
    }
    else if(divRect->getBoundingBox().containsPoint(localP) && !battleEnd)
    {
        this->judgeCorrect(startNum / remainNum,endNum);
        changeRoleAction("attack");
    }else if(backBtn->getBoundingBox().containsPoint(localP2))
    {
        auto scene = HelloWorld::createScene();
        Director::getInstance()->replaceScene(TransitionFadeTR::create(1, scene));
    }
}
void GameScene::judgeCorrect(int correntNum,int endNum)
{
    if(correntNum == endNum)
    {
        //成立
        enemy->stopAllActions();
        enemy->changeAction("dead");
        SoundCtl::getInstance()->playEffect("sound/hit.m4a");
        auto delay = DelayTime::create(1.5f);
        gameCount += 1;
        if(gameCount >= 4)
        {
            gameCount = 1;
            if(CF()->enterMap == 1)
            {
                CF()->stars1.at(CF()->curChallengeLev-1) = StringUtils::format("%d",hpcount);
            }else if(CF()->enterMap == 2)
            {
                CF()->stars2.at(CF()->curChallengeLev-1) = StringUtils::format("%d",hpcount);
            }else if(CF()->enterMap == 3)
            {
                CF()->stars3.at(CF()->curChallengeLev-1) = StringUtils::format("%d",hpcount);
            }
            int num = CF()->enterMap == 1?CF()->map1Level:CF()->enterMap == 2?CF()->map2Level:CF()->map3Level;
            if(CF()->curChallengeLev == num)
            {
                CF()->enterMap == 1?CF()->map1Level+=1:CF()->enterMap == 2?CF()->map2Level+=1:CF()->map3Level+=1;
            }
            CF()->curChallengeLev += 1;
            if(CF()->curChallengeLev >= CF()->totalLev)
            {
                //已经通关当前地图关卡 直接退回到主场景
                CF()->showMapView = true;
                this->exitGameScene();
                return;
            }
            //游戏结束 当前关卡胜利
            this->gameWin();
            enemy->removeFromParent();
            return;
        }
        auto callback = CallFunc::create([&](){
            enemy->removeFromParent();
            this->calculNum();
        });
        enemy->runAction(Sequence::create(delay,callback, NULL));
    }else{
        enemy->changeAction("dash");
        hpcount -= 1;
        if(hpcount == 1)
        {
            hp1->setBrightStyle(BRIGHT_HIGHLIGHT);
            hp2->setBrightStyle(BRIGHT_HIGHLIGHT);
        }else if(hpcount == 2)
        {
            hp1->setBrightStyle(BRIGHT_HIGHLIGHT);
        }else if(hpcount == 0){
            hp1->setBrightStyle(BRIGHT_HIGHLIGHT);
            hp2->setBrightStyle(BRIGHT_HIGHLIGHT);
            hp3->setBrightStyle(BRIGHT_HIGHLIGHT);
        }
        if(hpcount <= 0)
        {
            enemy->stopAllActions();
            enemy->removeFromParent();
            //游戏结束 当前关卡失败
            this->gameFail();
        }
    }
}
void GameScene::gameWin()
{
    ResultPop*result = new ResultPop(1);
    this->addChild(result);
    battleEnd = true;
    result->resetFunc = CC_CALLBACK_0(GameScene::resetData, this);
    result->backFunc = CC_CALLBACK_0(GameScene::exitGameScene, this);
}
void GameScene::exitGameScene()
{
    CF()->curChallengeLev = 1;
    auto scene = HelloWorld::createScene();
    Director::getInstance()->replaceScene(TransitionFadeTR::create(1, scene));
}
void GameScene::gameFail()
{
    battleEnd = true;
    ResultPop*result = new ResultPop(0);
    this->addChild(result);
    result->resetFunc = CC_CALLBACK_0(GameScene::resetData, this);
    result->backFunc = CC_CALLBACK_0(GameScene::exitGameScene, this);
}
