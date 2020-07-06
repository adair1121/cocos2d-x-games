#include "HelloWorldScene.h"
#include "iostream"
#include "CommonUtil.h"
USING_NS_CC;

using namespace std;

Scene* HelloWorld::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    
    if ( !Layer::init() )
    {
        return false;
    }
    
    auto rootNode = CSLoader::createNode("MainScene.csb");

    addChild(rootNode);

    bgGroup = static_cast<Layout*>(rootNode->getChildByName("bgGroup"));
    gameGroup = static_cast<Layout*>(rootNode->getChildByName("gameGroup"));
    bg = static_cast<Layout*>(rootNode->getChildByName("bg"));
    entityGroup = static_cast<Layout*>(rootNode->getChildByName("entityGroup"));
    gameGroup->setVisible(false);
    startBtn = static_cast<Sprite*>(rootNode->getChildByName("startBtn"));
    atkIcon = static_cast<Sprite*>(gameGroup->getChildByName("atkIcon"));
    circle = static_cast<Sprite*>(gameGroup->getChildByName("circle"));
    weaponIcon = static_cast<Sprite*>(gameGroup->getChildByName("weaponIcon"));
    headIcon = static_cast<Sprite*>(gameGroup->getChildByName("headIcon"));
    shiedIcon = static_cast<Sprite*>(gameGroup->getChildByName("shiedIcon"));
    startBg = static_cast<Sprite*>(rootNode->getChildByName("startBg"));
    roleimg = static_cast<Sprite*>(rootNode->getChildByName("roleimg"));
    
    scoreImg = static_cast<TextBMFont*>(rootNode->getChildByName("scoreImg"));
    scoreLab = static_cast<TextBMFont*>(rootNode->getChildByName("scoreLab"));
    againBtn = static_cast<Sprite*>(rootNode->getChildByName("againBtn"));
    resultBg = static_cast<Sprite*>(rootNode->getChildByName("resultBg"));
    scoreImg->setVisible(false);
    scoreLab->setVisible(false);
    againBtn->setVisible(false);
    resultBg->setVisible(false);
//    setTouchMode(Touch::DispatchMode::ALL_AT_ONCE);
    
    _listener=EventListenerTouchOneByOne::create();
    _listener->setSwallowTouches(true);
    _listener->onTouchBegan = CC_CALLBACK_2(HelloWorld::menuBegin,this);
    _listener->onTouchMoved = CC_CALLBACK_2(HelloWorld::menuMove,this);
    _listener->onTouchEnded = CC_CALLBACK_2(HelloWorld::menuEndCallback, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(_listener, this);
    
    
    visiblity = Director::getInstance()->getVisibleSize();
    soldier = new SoldierEntity();
    gameGroup->addChild(soldier);
    soldier->setPosition(Vec2(100,gameGroup->getContentSize().height/2 - 50));
    
    circleP = Point(circle->getPositionX(),circle->getPositionY());
    this->adapter();
    return true;
}
void HelloWorld::resetData()
{
    hp = 3;
    energe = 50;
    atkNum = 3;
    shieldNum = 3;
    headNum = 3;
    score = 0;
    wudi = false;
    execAction = false;
    direct = 0;
    battleEnd = false;
    scoreImg->setVisible(false);
    scoreLab->setVisible(false);
    againBtn->setVisible(false);
    resultBg->setVisible(false);
    this->refreshPageData();
    this->schedule(schedule_selector(HelloWorld::updateEnergy), 0.5f);
    this->schedule(schedule_selector(HelloWorld::actiontick), 0.0167f);
    this->schedule(schedule_selector(HelloWorld::createMon), 3.0f);
}
void HelloWorld::createMon(float dt)
{
    srand(int(time(0))+ rand());//置随机数种子
//    int num=CCRANDOM_0_1()*100;
//    if(num < 50)
//    {
//        //飞行怪
//        int monindex = CCRANDOM_0_1()*2;
//        string monass = "bee"
//        string keyname = monindex == 0?"bee_run_":"fly_run_";
//        int endIndex = monindex == 0?8:6;
//        MovieClip*mon = new MovieClip();
//        mon->frameRate = 0.095f;
//        bgGroup->addChild(mon);
//        int direct = (CCRANDOM_0_1()*100) > 50?-1:1;
//        mon->playFile((monass+".plist").c_str(), keyname.c_str(), 1, endIndex, -1, false);
//        mon->setScaleX(-direct);
//        mon->setName("mon");
//        float y = gameGroup->getContentSize().height/2 + 100;
//        float x = direct == 1?(bgGroup->getContentSize().width+100 ):-100;
//        mon->setPosition(Vec2(x,y));
//        float movex = direct == 1?-100:bgGroup->getContentSize().width + 100;
//        auto moveTo = MoveTo::create(7.0f, Vec2(movex,y));
//        auto callback = CallFunc::create([=](){
//           if(mon)
//           {
//               mon->removeFromParent();
//           }
//        });
//        mon->runAction(Sequence::create(moveTo,callback, NULL));
//    }
    int monindex = CCRANDOM_0_1()*3;
    string monass = monindex == 0?"ladybug":monindex == 1?"bee":"spider";
    string keyname = monindex == 0?"ladybug_run_":monindex == 1?"bee_run_":"spider_run_";
    int endIndex = 16;
    MovieClip*mon = new MovieClip();
    bgGroup->addChild(mon);
    int direct = (CCRANDOM_0_1()*100) > 50?-1:1;
    mon->playFile((monass+".plist").c_str(), keyname.c_str(), 1, endIndex, -1, false);
    mon->setScaleX(direct);
    mon->setName("mon");
    float y = gameGroup->getContentSize().height/2 - 150;
    float x = direct == 1?(bgGroup->getContentSize().width+100 ):-100;
    mon->setPosition(Vec2(x,y));
    float movex = direct == 1?-100:bgGroup->getContentSize().width + 100;
    auto moveTo = MoveTo::create(7.0f, Vec2(movex,y));
    auto callback = CallFunc::create([=](){
       if(mon)
       {
           mon->removeFromParent();
       }
    });
    mon->runAction(Sequence::create(moveTo,callback, NULL));
}
void HelloWorld::updateEnergy(float dt)
{
    if(energe < tenerge)
    {
        energe +=1;
        float percent = float(energe)/float(tenerge)*100.0f;
        static_cast<LoadingBar*>(gameGroup->getChildByName("energyBar"))->setPercent(percent);
    }
}
void HelloWorld::actiontick(float dt)
{
    if(direct == 3 && !execAction)
    {
        energe +=1;
        if(energe >= tenerge){energe = tenerge;}
        float percent = float(energe)/float(tenerge)*100.0f;
        static_cast<LoadingBar*>(gameGroup->getChildByName("energyBar"))->setPercent(percent);
        execAction = true;
        auto jumpTo = JumpTo::create(1.0f, Vec2(soldier->getPositionX(),soldier->getPositionY() ), 150, 1);
        soldier->execRun();
        auto jumpTo2 = JumpTo::create(1.0f, Vec2(bgGroup->getPositionX(),bgGroup->getPositionY() ), -80, 1);
        bgGroup->runAction(jumpTo2);
//        auto jumpTo3 = JumpTo::create(0.5f, Vec2(entityGroup->getPositionX(),entityGroup->getPositionY() ), -80, 1);
//        entityGroup->runAction(jumpTo3);
        auto callback = CallFunc::create([&](){
            execAction = false;
            soldier->execStand();
        });
        soldier->runAction(Sequence::create(jumpTo,callback, NULL));
    }
    if(direct == 1 || direct == 5)
    {
        int dic = direct == 1?1:-1;
        soldier->execRun();
        energe += 1;
        if(energe >= tenerge){energe = tenerge;}
        float percent = float(energe)/float(tenerge)*100.0f;
        static_cast<LoadingBar*>(gameGroup->getChildByName("energyBar"))->setPercent(percent);
        float x = soldier->getPositionX() - (5+otherSpeed)*dic;
        if((bgGroup->getPositionX() == 0) || (bgGroup->getPositionX() == visiblity.width - bgGroup->getContentSize().width))
        {
            soldier->setPositionX(x);
        }
        int xx = soldier->getPositionX();
        int sx = visiblity.width/2;
        if(abs(xx-sx) <= 20)
        {
            float mx = bgGroup->getPositionX() - (5+otherSpeed)*(-dic);
            bgGroup->setPositionX(mx);
        }
    }
    if(direct == 4 || direct == 2)
    {
        int dic = direct == 4?1:-1;
        soldier->execRun();
        energe += 1;
        if(energe >= tenerge){energe = tenerge;}
        float percent = float(energe)/float(tenerge)*100.0f;
        static_cast<LoadingBar*>(gameGroup->getChildByName("energyBar"))->setPercent(percent);
        float x = soldier->getPositionX() + (5+otherSpeed)*dic;
        if((bgGroup->getPositionX() == 0) || (bgGroup->getPositionX() == visiblity.width - bgGroup->getContentSize().width))
        {
            soldier->setPositionX(x);
        }
        int xx = soldier->getPositionX();
        int sx = visiblity.width/2;
        if(abs(xx-sx) <= 10)
        {
            float mx = bgGroup->getPositionX() + (5+otherSpeed)*(-dic);
            bgGroup->setPositionX(mx);
        }
//        bgGroup->setPositionX(-x);
        if(!execAction)
        {
            execAction = true;
            auto jumpTo = JumpTo::create(1.0f, Vec2(soldier->getPositionX(),soldier->getPositionY() ), 150, 1);
            soldier->execRun();
            auto jumpTo2 = JumpTo::create(1.0f, Vec2(bgGroup->getPositionX(),bgGroup->getPositionY() ), -80, 1);
            bgGroup->runAction(jumpTo2);
//            auto jumpTo3 = JumpTo::create(0.5f, Vec2(entityGroup->getPositionX(),entityGroup->getPositionY() ), -80, 1);
//            entityGroup->runAction(jumpTo3);
            auto callback = CallFunc::create([&](){
                execAction = false;
                soldier->execStand();
            });
            soldier->runAction(Sequence::create(jumpTo,callback, NULL));
        }
    }
    if(soldier->getPositionX() <= soldier->getContentSize().width)
    {
        soldier->setPositionX(soldier->getContentSize().width);
    }
    if(soldier->getPositionX() >= visiblity.width - soldier->getContentSize().width)
    {
        soldier->setPositionX(visiblity.width - soldier->getContentSize().width);
    }
    if(bgGroup->getPositionX() >= 0)
    {
        bgGroup->setPositionX(0);
    }
    if(bgGroup->getPositionX() <= visiblity.width - bgGroup->getContentSize().width)
    {
        bgGroup->setPositionX(visiblity.width - bgGroup->getContentSize().width);
    }
    if(soldier->atkState || soldier->defState)
    {
        Vector<Node*> childrens = bgGroup->getChildren();
        for(auto c : childrens)
        {
            if(c->getName()== "mon")
            {
                Point monWorldP = bgGroup->convertToWorldSpace(Vec2(c->getPositionX(),c->getPositionY()));
                Point roleWorldP = gameGroup->convertToWorldSpace(Vec2(soldier->getPositionX(),soldier->getPositionY()));
                float disx = monWorldP.x - roleWorldP.x;
                float disy = monWorldP.y - roleWorldP.y;
                float offx = soldier->defState?140:70;
                float offy = soldier->defState?150:100;
                if(abs(disx) <= offx && abs(disy ) <= offy && c->getTag() != 1)
                {
                    c->stopAllActions();
                    c->setTag(1);
                    float offx = disx < 0?-100:100;
                    auto jumpTo = JumpTo::create(0.5f, Vec2(c->getPositionX(),c->getPositionY() ), 50, 1);
                    auto moveTo = MoveTo::create(0.5f, Vec2(c->getPositionX() + offx,c->getPositionY()));
                    auto awapn = Spawn::createWithTwoActions(jumpTo, moveTo);
                    score += 1;
                    static_cast<TextBMFont*>(gameGroup->getChildByName("goldLab"))->setString(StringUtils::format("%d",score));
                    auto callback = CallFunc::create([=](){
                        c->removeFromParent();
                    });
                    c->runAction(Sequence::create(awapn,callback, NULL));
                    break;
                }
            }
        }
    }else{
        if(wudi){return;}
        Vector<Node*> childrens = bgGroup->getChildren();
        for(auto c : childrens)
        {
            if(c->getName()== "mon")
            {
                Point monWorldP = bgGroup->convertToWorldSpace(Vec2(c->getPositionX(),c->getPositionY()));
                Point roleWorldP = gameGroup->convertToWorldSpace(Vec2(soldier->getPositionX(),soldier->getPositionY()));
                float disx = monWorldP.x - roleWorldP.x;
                float disy = monWorldP.y - roleWorldP.y;
                if(abs(disx) <= 70 && abs(disy) <= 100)
                {
                    execAction = false;
                    shakeNode(soldier);
                    wudi = true;
                    hp-=1;
                    if(hp <= 0)
                    {
                        this->unschedule(schedule_selector(HelloWorld::updateEnergy));
                        this->unschedule(schedule_selector(HelloWorld::actiontick));
                        this->unschedule(schedule_selector(HelloWorld::createMon));
                        Vector<Node*> childrens = bgGroup->getChildren();
                        for(auto c : childrens)
                        {
                           if(c->getName()== "mon")
                           {
                               c->removeFromParent();
                           }
                        }
                        battleEnd = true;
                        scoreLab->setString(StringUtils::format("%d",score));
                        scoreImg->setVisible(true);
                        scoreLab->setVisible(true);
                        againBtn->setVisible(true);
                        resultBg->setVisible(true);
                    }
                    this->refreshPageData();
//                    soldier->execHit();
                    break;
                }
            }
        }
    }
}
void HelloWorld::shakeNode(cocos2d::Node *node)
{
    float x = node->getPositionX();
    float y = gameGroup->getContentSize().height/2 - 50;
    node->runAction(RepeatForever::create(Sequence::create(
       MoveTo::create(0.02f, Vec2(2+x,3+y)),
       MoveTo::create(0.02f, Vec2(-2+x,3+y)),
       MoveTo::create(0.02f, Vec2(-5+x,3+y)),
       MoveTo::create(0.02f, Vec2(3+x,-2+y)),
       MoveTo::create(0.02f, Vec2(-2+x,1+y)),
       MoveTo::create(0.02f, Vec2(2+x,-2+y)),
       MoveTo::create(0.02f, Vec2(-3+x,-1+y)),
       MoveTo::create(0.02f, Vec2(1+x,2+y)),
       MoveTo::create(0.02f, Vec2(0+x,0+y)),
        NULL)));
    auto delay = DelayTime::create(0.5f);
    execAction = false;
    auto callback = CallFunc::create([=](){
        execAction = false;
        node->stopAllActions();
        node->setPosition(Vec2(x,y));
        wudi = false;
    });
    auto seque = Sequence::create(delay,callback, NULL);
    node->runAction(seque);
}
void HelloWorld::refreshPageData()
{
    for(int i = 1;i<=3;i++)
    {
        std::string hpkey = StringUtils::format("hp%d",i);
        std::string ass = "";
        if(hp>=i)
        {
            static_cast<Sprite*>(gameGroup->getChildByName(hpkey))->setVisible(true);
        }else{
            static_cast<Sprite*>(gameGroup->getChildByName(hpkey))->setVisible(false);
        }
    }
    static_cast<TextBMFont*>(gameGroup->getChildByName("goldLab"))->setString(StringUtils::format("%d",score));
    float percent = float(energe)/float(tenerge)*100.0f;
    static_cast<LoadingBar*>(gameGroup->getChildByName("energyBar"))->setPercent(percent);
    static_cast<Text*>(gameGroup->getChildByName("weaponNum"))->setString(StringUtils::format("%d",atkNum));
    static_cast<Text*>(gameGroup->getChildByName("shiedNum"))->setString(StringUtils::format("%d",shieldNum));
    static_cast<Text*>(gameGroup->getChildByName("headNum"))->setString(StringUtils::format("%d",headNum));
}
void HelloWorld::adapter()
{
    int bgW = 1334;
    int bgH = 750;
    float scaleX = visiblity.width/bgW;
    float scaleY = visiblity.height/bgH;
//    bgGroup->setScale(scaleX,scaleY);
    bg->setScale(scaleX,scaleY);
    startBg->setScale(scaleX);
    startBg->setPosition(Vec2(startBg->getPositionX(),startBg->getPositionY()));
    gameGroup->setScale(scaleX,scaleY);
    entityGroup->setScale(scaleX,scaleY);
    startBtn->setPosition(Vec2(startBtn->getPositionX()*scaleX,startBtn->getPositionY()*scaleY));
    roleimg->setPosition(Vec2(roleimg->getPositionX()*scaleX,roleimg->getPositionY()*scaleY));
    scoreImg->setPosition(Vec2(scoreImg->getPositionX()*scaleX,scoreImg->getPositionY()*scaleY));
    scoreLab->setPosition(Vec2(scoreLab->getPositionX()*scaleX,scoreLab->getPositionY()*scaleY));
    againBtn->setPosition(Vec2(againBtn->getPositionX()*scaleX,againBtn->getPositionY()*scaleY));
    resultBg->setPosition(Vec2(resultBg->getPositionX()*scaleX,resultBg->getPositionY()*scaleY));
}
void HelloWorld::onExit()
{
    Layer::onExit();
    _eventDispatcher->removeEventListener(_listener);
}
bool HelloWorld::menuBegin(cocos2d::Touch* tTouch,cocos2d::Event* eEvent)
{
    Point localP = this->convertToNodeSpace(tTouch->getLocation());
    Point groupP = gameGroup->convertToNodeSpace(tTouch->getLocation());
    if(startBtn->getBoundingBox().containsPoint(localP) && startBtn->isVisible() && !battleEnd){
       startBtn->setScale(0.9f);
       return true;
    }else if(atkIcon->getBoundingBox().containsPoint(groupP) && !startBtn->isVisible()&& !battleEnd)
    {
        atkIcon->setScale(0.7f);
        return true;
    }else if(circle->getBoundingBox().containsPoint(groupP) && !startBtn->isVisible() &&!battleEnd)
    {
        touchCircle = true;
        return true;
    }else if(weaponIcon->getBoundingBox().containsPoint(groupP) && !startBtn->isVisible() &&!battleEnd)
    {
        return true;
    }else if(headIcon->getBoundingBox().containsPoint(groupP) && !startBtn->isVisible()&& !battleEnd)
    {
        return true;
    }else if(shiedIcon->getBoundingBox().containsPoint(groupP) && !startBtn->isVisible() &&!battleEnd)
    {
        return true;
    }else if(againBtn->getBoundingBox().containsPoint(localP) && battleEnd)
    {
        return true;
    }
    return false;
}
void HelloWorld::menuMove(cocos2d::Touch* tTouch,cocos2d::Event* eEvent)
{
    if(touchCircle && !startBtn->isVisible() &&!battleEnd)
    {
        Point localP = gameGroup->convertToNodeSpace(tTouch->getLocation());
        float angle = atan2(localP.y - circleP.y, localP.x - circleP.x);
        
        float touchRadius=sqrt(pow(localP.x-circleP.x,2)+pow(localP.y-circleP.y,2));
        float radius = MIN(_normalRadius, touchRadius);
        
        float x = circleP.x + cos(angle)*radius;
        float y = circleP.y + sin(angle)*radius;
        circle->setPosition(Vec2(x,y));
        
        float realAngle = CC_RADIANS_TO_DEGREES(angle);
        
        if((realAngle >= 0 && realAngle < 45 ) || (realAngle <= 0 && realAngle > -45))
        {
            //向右
            direct = 5;
            soldier->setScaleX(1);
        }
        if(realAngle >= 45 && realAngle < 80)
        {
            //右上
            direct = 4;
            soldier->setScaleX(1);
        }
        if(realAngle >= 80 && realAngle < 110)
        {
            //向上
            direct = 3;
        }
        if(realAngle >= 110 && realAngle < 135)
        {
            //左上;
            direct = 2;
            soldier->setScaleX(-1);
        }
        if((realAngle >= 135 && realAngle < 180) || (realAngle > -180 && realAngle <= -135) )
        {
            //向左
            direct = 1;
            soldier->setScaleX(-1);
        }
       
    }
}
void HelloWorld::menuEndCallback(cocos2d::Touch* tTouch,cocos2d::Event* eEvent)
{
    Point localP = this->convertToNodeSpace(tTouch->getLocation());
    Point groupP = gameGroup->convertToNodeSpace(tTouch->getLocation());
    if(startBtn->getBoundingBox().containsPoint(localP) && startBtn->isVisible() && !battleEnd){
        startBtn->setScale(1.0f);
        //打开布阵弹窗
        auto fadeOut = FadeOut::create(1.0f);
        auto fadeout2 = FadeOut::create(1.0f);
        auto fadeout3 = FadeOut::create(1.0f);
        auto callback = CallFunc::create([&](){
            startBtn->setVisible(false);
            startBg->setVisible(false);
            roleimg->setVisible(false);
            this->resetData();
        });
        startBtn->runAction(Sequence::create(fadeOut,callback, NULL));
        startBg->runAction(fadeout2);
        roleimg->runAction(fadeout3);
        gameGroup->setVisible(true);
        gameGroup->setOpacity(0);
        auto fadein = FadeIn::create(1.0f);
        gameGroup->runAction(fadein);
        return;
    }else if(atkIcon->getBoundingBox().containsPoint(groupP) && !startBtn->isVisible()&& !battleEnd)
    {
        atkIcon->setScale(0.8f);
        if(!soldier->atkState)
        {
            soldier->execAtk();
            energe -= 5;
            this->refreshPageData();
        }
    }else if(weaponIcon->getBoundingBox().containsPoint(groupP) && !startBtn->isVisible()&& !battleEnd)
    {
        if(atkNum > 0)
        {
            atkNum -= 1;
            static_cast<Text*>(gameGroup->getChildByName("weaponNum"))->setString(StringUtils::format("%d",atkNum));
            MovieClip*light = new MovieClip();
            gameGroup->addChild(light);
            light->setPosition(Vec2(soldier->getPositionX(),soldier->getPositionY() + 150));
            light->playFile("skill.plist", "img_shockwave", 1, 7, 1, true);
            
            Vector<Node*> childrens = bgGroup->getChildren();
            for(auto c : childrens)
            {
                if(c->getName()== "mon")
                {
                    Sprite*sword = Sprite::create("assets/effect_skill01_sword-sheet0.png");
                    gameGroup->addChild(sword);
                    float angle = atan2(c->getPositionY() - soldier->getPositionY(), c->getPositionX() - soldier->getPositionX());
                    float realAngle = CC_RADIANS_TO_DEGREES(angle);
                    sword->setPosition(Vec2(soldier->getPositionX(),soldier->getPositionY()));
                    sword->setRotation(-realAngle);
                    sword->setName("sword");
                    auto move = MoveTo::create(0.3f, Vec2(c->getPositionX(),c->getPositionY()));
                    auto callback = CallFunc::create([=](){
                        sword->removeFromParent();
                        c->removeFromParent();
                    });
                    score += 1;
                    static_cast<TextBMFont*>(gameGroup->getChildByName("goldLab"))->setString(StringUtils::format("%d",score));
                    sword->runAction(Sequence::create(move,callback, NULL));
                }
            }
            
        }
    }else if(headIcon->getBoundingBox().containsPoint(groupP) && !startBtn->isVisible()&& !battleEnd)
    {
        if(headNum > 0)
        {
            headNum -= 1;
            static_cast<Text*>(gameGroup->getChildByName("headNum"))->setString(StringUtils::format("%d",headNum));
            otherSpeed = 5;
            soldier->changeModel();
            auto delay = DelayTime::create(10.0f);
            auto callback = CallFunc::create([&](){
                otherSpeed = 0;
            });
            this->runAction(Sequence::create(delay,callback, NULL));
        }
        
    }else if(shiedIcon->getBoundingBox().containsPoint(groupP) && !startBtn->isVisible()&& !battleEnd)
    {
       if(shieldNum > 0)
       {
           shieldNum -= 1;
           static_cast<Text*>(gameGroup->getChildByName("shiedNum"))->setString(StringUtils::format("%d",shieldNum));
           Sprite*showEff = Sprite::create("assets/effect_skill01_man2-sheet0.png");
           showEff->setName("defeff");
           gameGroup->addChild(showEff);
           showEff->setPosition(Vec2(soldier->getPositionX(),soldier->getPositionY() - 50));
           auto fadeout = FadeOut::create(0.5f);
           auto scaleto = ScaleTo::create(0.5f, 2.0f);
           showEff->setAnchorPoint(Vec2(0.5,0));
           auto swapn = Spawn::createWithTwoActions(fadeout, scaleto);
           auto callback = CallFunc::create([=](){
//               showEff->removeFromParent();
               showEff->setOpacity(0);
               Sprite*defCircle = Sprite::create("assets/effect_skill02_shiledring-sheet0.png");
               defCircle->setAnchorPoint(Vec2(0.5,0));
               defCircle->setPositionY(-50);
               soldier->addChild(defCircle);
               defCircle->setName("def");
               soldier->defState = true;
               defCircle->setPositionY(defCircle->getPositionY() - 50);
               auto delay = DelayTime::create(10.0f);
               auto timeoutback = CallFunc::create([&](){
                   Sprite*defeff = static_cast<Sprite*>(gameGroup->getChildByName("defeff"));
                   defeff->removeFromParent();
                   soldier->defState = false;
                   Sprite*sp = static_cast<Sprite*>(soldier->getChildByName("def"));
                   sp->removeFromParent();
               });
               showEff->runAction(Sequence::create(delay,timeoutback, NULL));
           });
           showEff->runAction(Sequence::create(swapn,callback, NULL));
       }
    }else if(againBtn->getBoundingBox().containsPoint(localP) && battleEnd)
    {
        this->resetData();
    }
    if(touchCircle)
    {
        touchCircle = false;
        direct = -1;
        circle->setPosition(Vec2(circleP.x,circleP.y));
        soldier->execStand();
    }
}
