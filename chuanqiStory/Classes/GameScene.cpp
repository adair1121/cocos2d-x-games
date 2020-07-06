//
//  GameScene.cpp
//  chuanqiStory-mobile
//
//  Created by 志强 on 2020/5/22.
//

#include "GameScene.h"
#include "SoundCtl.h"
#include "Config.h"
#include "CommonUtil.h"
#include "HelloWorldScene.h"
#include "ResultPop.h"

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
    mpGroup = static_cast<Layout*>(rootNode->getChildByName("mpGroup"));
    hpGroup = static_cast<Layout*>(rootNode->getChildByName("hpGroup"));
    gameGroup = static_cast<Layout*>(rootNode->getChildByName("gameGroup"));
    tower = static_cast<Sprite*>(gameGroup->getChildByName("tower"));
    pauseBtn = static_cast<Sprite*>(rootNode->getChildByName("pauseBtn"));
    collectBtn = static_cast<Sprite*>(rootNode->getChildByName("collectBtn"));
    skill1 = static_cast<Button*>(rootNode->getChildByName("skill1"));
    skill2 = static_cast<Button*>(rootNode->getChildByName("skill2"));
    skill3 = static_cast<Button*>(rootNode->getChildByName("skill3"));
    skill4 = static_cast<Button*>(rootNode->getChildByName("skill4"));
//    guide1 = static_cast<Sprite*>(rootNode->getChildByName("guide1"));
//    guide2 = static_cast<Sprite*>(rootNode->getChildByName("guide2"));
//    guide3 = static_cast<Sprite*>(rootNode->getChildByName("guide3"));
//    guide4 = static_cast<Sprite*>(rootNode->getChildByName("guide4"));
    pauseGroup = static_cast<Layout*>(rootNode->getChildByName("pauseGroup"));
    bottomGroup = static_cast<Layout*>(rootNode->getChildByName("bottomGroup"));
    iceNumLab = static_cast<Text*>(rootNode->getChildByName("iceNumLab"));
    lightNumLab = static_cast<Text*>(rootNode->getChildByName("lightNumLab"));
    
    pauseGroup->setVisible(false);
//    guide1->setVisible(false);
//    guide2->setVisible(false);
//    guide3->setVisible(false);
//    guide4->setVisible(false);
    skill2->setVisible((CF()->lightNum >= 1));
    skill3->setVisible((CF()->iceNum >= 1));
    skill4->setVisible((CF()->way2Level >= 1));
    
    skill2->setBrightStyle(BRIGHT_HIGHLIGHT);
    skill3->setBrightStyle(BRIGHT_HIGHLIGHT);
    skill4->setBrightStyle(BRIGHT_HIGHLIGHT);
    iceNumLab->setString(StringUtils::format("%d",CF()->iceNum));
    lightNumLab->setString(StringUtils::format("%d",CF()->lightNum));
    iceNumLab->setVisible(CF()->iceNum>=1);
    lightNumLab->setVisible(CF()->lightNum>=1);
    pauseGroup->setLocalZOrder(9999);
    
    static_cast<Text*>(bottomGroup->getChildByName("lab"))->setString(StringUtils::format("level%d",CF()->challengeLev));
    static_cast<Text*>(pauseGroup->getChildByName("lab"))->setString(StringUtils::format("level%d",CF()->challengeLev));
    static_cast<Text*>(mpGroup->getChildByName("lab"))->setString(StringUtils::format("%d",CF()->mpLevel));
    static_cast<Text*>(hpGroup->getChildByName("lab"))->setString(StringUtils::format("%d",CF()->hpNum));
    
    if(SoundCtl::getInstance()->musicBool)
    {
        static_cast<Button*>(pauseGroup->getChildByName("audioBtn"))->setBrightStyle(BRIGHT_HIGHLIGHT);
    }else{
        static_cast<Button*>(pauseGroup->getChildByName("audioBtn"))->setBrightStyle(BRIGHT_NORMAL);
    }
    
    _listener=EventListenerTouchOneByOne::create();
    _listener->setSwallowTouches(true);
    _listener->onTouchBegan = CC_CALLBACK_2(GameScene::menuBegin,this);
    _listener->onTouchEnded = CC_CALLBACK_2(GameScene::menuEndCallback, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(_listener, this);
    
    this->adapter();
    this->resetData();
    visiblity = Director::getInstance()->getVisibleSize();
    roleMc = new MovieClip();
    gameGroup->addChild(roleMc);
    roleMc->playFile("role.plist", "stand", 1, 4, -1, false);
    
    roleMc->setPosition(Vec2(tower->getPositionX(),tower->getPositionY() + 50));
    
//    Sprite*parapet = Sprite::create("assets/parapet-sheet0.png");
//    gameGroup->addChild(parapet);
//    parapet->setPosition(Vec2(roleMc->getPositionX(),roleMc->getPositionY() - 35));
    
    this->startGame();
    
    
    return true;
}
void GameScene::startGame()
{
    this->schedule(schedule_selector(GameScene::updateTime), 1.0f);
    this->schedule(schedule_selector(GameScene::createMon), 3.0f);
    this->schedule(schedule_selector(GameScene::ticktime), 0.1f);
    Vector<MovieClip*> mons;
    Vector<Node*> children = gameGroup->getChildren();
    for(auto c : children)
    {
        if(c->getName() == "mons")
        {
            mons.pushBack(static_cast<MovieClip*>(c));
        }
    }
    if(mons.size() > 0)
    {
        for(auto c: mons)
        {
            float disx = abs(roleMc->getPositionX() - c->getPositionX());
            float time = disx / 100.0f;
            auto moveto = MoveTo::create(time, Vec2(roleMc->getPositionX(),c->getPositionY()));
            auto callback = CallFunc::create([&](){
                operHp(-20);
                Vector<MovieClip*> mons;
                Vector<Node*> children = gameGroup->getChildren();
                for(auto c : children)
                {
                    if(c->getName() == "mons")
                    {
                        mons.pushBack(static_cast<MovieClip*>(c));
                    }
                }
                for(auto m : mons)
                {
                    if(m->getPositionX() == tower->getPositionX())
                    {
                        m->removeFromParent();
                        mons.eraseObject(m);
                    }
                }
            });
            c->runAction(Sequence::create(moveto,callback, NULL));
        }
    }
}
void GameScene::ticktime(float dt)
{
    Vector<Sprite*> bullets;
    Vector<MovieClip*> mons;
    Vector<Node*> children = gameGroup->getChildren();
    for(auto c : children)
    {
        if(c->getName() == "bullet")
        {
            bullets.pushBack(static_cast<Sprite*>(c));
        }else if(c->getName() == "mons")
        {
            mons.pushBack(static_cast<MovieClip*>(c));
        }
    }
    for(auto c : bullets)
    {
        for(auto m : mons)
        {
            float distance=sqrt(pow(c->getPositionX()-m->getPositionX(),2)+pow(c->getPositionY()-m->getPositionY(),2));
            if(distance <= 50)
            {
                int tag = m->getTag();
                if(skillIndex == 4)
                {
                  tag -= 2;
                }else{
                  tag -= 1;
                }
                
                c->stopAllActions();
                c->removeFromParent();
                bullets.eraseObject(c);
                if(tag <= 0)
                {
                    m->stopAllActions();
                    createEnergy(Vec2(m->getPositionX(),m->getPositionY()));
                    m->removeFromParent();
                    mons.eraseObject(m);
                    curChallengeCout  -= 1;
                    float percent = float(curChallengeCout)/float(challengeCout)*100.0f;
                    static_cast<LoadingBar*>(bottomGroup->getChildByName("bar"))->setPercent(percent);
                    if(curChallengeCout <= 0)
                    {
                        
                        this->stopGame();
                        //游戏当前关卡胜利
                        battleEnd = true;
                        ResultPop*result = new ResultPop(1);
                        result->callFunc = CC_CALLBACK_1(GameScene::resultCallBack, this);
                        this->addChild(result);
                    }
                }else{
                    m->setTag(tag);
                }
                
                break;
            }
        }
    }
    bullets.clear();
    mons.clear();
    children.clear();
}
void GameScene::resultCallBack(int oper)
{
    if(oper == 0)
    {
        this->resetData();
        this->startGame();
    }else if(oper == 1)
    {
        CF()->challengeLev += 1;
        if(CF()->challengeLev >= 16){CF()->challengeLev = 15;}
        if(CF()->challengeLev > CF()->level)
        {
            CF()->level = CF()->challengeLev;
        }
        this->resetData();
        this->startGame();
    }else if(oper == 2)
    {
        //返回主页面
        auto scene = HelloWorld::createScene();
        Director::getInstance()->replaceScene(TransitionFadeTR::create(1, scene));
    }
}
void GameScene::createMon(float dt)
{
    //创建怪物
    int monId = 1;
    srand(int(time(0))+ rand());//置随机数种子
    if(CF()->challengeLev <= 5)
    {
        monId = CCRANDOM_0_1()*CF()->challengeLev + 1;
    }else{
        monId = CCRANDOM_0_1()*5 + 1;
    }
    std::string monass = StringUtils::format("mon%d.plist",monId);
    std::string keyname = StringUtils::format("run%d",monId);
    MovieClip*monmc = new MovieClip();
    gameGroup->addChild(monmc);
//    monmc->setScale(0.4f);
    int tagHp = (CCRANDOM_0_1()*2+2);
    monmc->setTag(tagHp);
    monmc->playFile(monass.c_str(), keyname.c_str(), 1, 8, -1, false);
    int direc = (CCRANDOM_0_1()*100) > 50?1:-1;
    monmc->setScaleX(monmc->getScaleX()*direc);
    float mx = direc == 1?-100:visiblity.width + 100;
    monmc->setPosition(Vec2(mx,roleMc->getPositionY()));
    float disx = abs(tower->getPositionX() - monmc->getPositionX());
    float time = disx / 100.0f;
    auto moveto = MoveTo::create(time, Vec2(tower->getPositionX(),monmc->getPositionY()));
    monmc->setName("mons");
    auto callback = CallFunc::create([&](){
        operHp(-20);
        Sprite*img = Sprite::create("assets/enemydamage-sheet0.png");
        gameGroup->addChild(img);
        img->setPosition(Vec2(tower->getPositionX(),tower->getPositionY()));
        img->setOpacity(0);
        auto fadein = FadeIn::create(0.3f);
        auto delay = DelayTime::create(0.3f);
        auto delaycall = CallFunc::create([=](){
            img->removeFromParent();
        });
        img->runAction(Sequence::create(fadein,delay,delaycall, NULL));
        Vector<MovieClip*> mons;
        Vector<Node*> children = gameGroup->getChildren();
        for(auto c : children)
        {
            if(c->getName() == "mons")
            {
               if(c->getPositionX() == tower->getPositionX())
                {
                    c->removeFromParent();
                    break;
                }
            }
        }
        mons.clear();
        children.clear();
    });
    monmc->runAction(Sequence::create(moveto,callback, NULL));
}
void GameScene::updateTime(float dt)
{
    if(curMp < (totalMp + CF()->mpLevel*20))
    {
        //恢复mp
        operMp(20);
    }
}
void GameScene::stopGame()
{
    this->unschedule(schedule_selector(GameScene::updateTime));
    this->unschedule(schedule_selector(GameScene::createMon));
    this->unschedule(schedule_selector(GameScene::ticktime));
    Vector<MovieClip*> mons;
    Vector<Node*> children = gameGroup->getChildren();
    for(auto c : children)
    {
        if(c->getName() == "mons")
        {
            mons.pushBack(static_cast<MovieClip*>(c));
        }
    }
    if(mons.size() > 0)
    {
        for(auto c: mons)
        {
            c->stopAllActions();
        }
    }
    mons.clear();
    children.clear();
}
void GameScene::createEnergy(Vec2 pos)
{
    Sprite*energy = Sprite::create("assets/manaball-sheet0.png");
    gameGroup->addChild(energy);
    Point startPoint = Point(pos.x,pos.y);
    energy->setPosition(pos);
    int dic = (pos.x > visiblity.width/2)?-1:1;
    Point endPoint = Point(startPoint.x + 100*dic,startPoint.y);
    
   
    auto move = CommonUtil::createParabola(1.0f, startPoint, endPoint, 200, 20);
    energys.pushBack(energy);
    energy->runAction(move);
    
}
void GameScene::adapter()
{
    int bgW = bg->getContentSize().width;
    int bgH = bg->getContentSize().height;
    Size visibleSize = Director::getInstance()->getVisibleSize();
    float scaleX = visibleSize.width/bgW;
    float scaleY = visibleSize.height/bgH;
    bg->setScale(scaleX, scaleY);
    mpGroup->setPosition(Vec2(mpGroup->getPositionX()*scaleX,mpGroup->getPositionY()*scaleY));
    hpGroup->setPosition(Vec2(hpGroup->getPositionX()*scaleX,hpGroup->getPositionY()*scaleY));

    pauseBtn->setPosition(Vec2(pauseBtn->getPositionX()*scaleX,pauseBtn->getPositionY()*scaleY));
    collectBtn->setPosition(Vec2(collectBtn->getPositionX()*scaleX,collectBtn->getPositionY()*scaleY));
    skill1->setPosition(Vec2(skill1->getPositionX()*scaleX,skill1->getPositionY()*scaleY));
    skill2->setPosition(Vec2(skill2->getPositionX()*scaleX,skill2->getPositionY()*scaleY));
    skill3->setPosition(Vec2(skill3->getPositionX()*scaleX,skill3->getPositionY()*scaleY));
    skill4->setPosition(Vec2(skill4->getPositionX()*scaleX,skill4->getPositionY()*scaleY));

    pauseGroup->setPosition(Vec2(pauseGroup->getPositionX()*scaleX,pauseGroup->getPositionY()*scaleY));
    bottomGroup->setPosition(Vec2(bottomGroup->getPositionX()*scaleX,bottomGroup->getPositionY()*scaleY));
    gameGroup->setScale(scaleX);
    gameGroup->setPosition(Vec2(gameGroup->getPositionX()*scaleX,gameGroup->getPositionY()*scaleY));
    iceNumLab->setPosition(Vec2(iceNumLab->getPositionX()*scaleX,iceNumLab->getPositionY()*scaleY));
    lightNumLab->setPosition(Vec2(lightNumLab->getPositionX()*scaleX,lightNumLab->getPositionY()*scaleY));
}
void GameScene::resetData()
{
    curHp = totalHp = 100;
    curMp = totalMp = CF()->mpLevel*100;
    curChallengeCout = challengeCout = 12 + CF()->level*5;
    battleEnd = false;
    skillIndex = 1;
    static_cast<Text*>(bottomGroup->getChildByName("lab"))->setString(StringUtils::format("level%d",CF()->challengeLev));
    static_cast<Text*>(pauseGroup->getChildByName("lab"))->setString(StringUtils::format("level%d",CF()->challengeLev));
    static_cast<LoadingBar*>(mpGroup->getChildByName("bar"))->setPercent(100);
    static_cast<LoadingBar*>(hpGroup->getChildByName("bar"))->setPercent(100);
    static_cast<LoadingBar*>(bottomGroup->getChildByName("bar"))->setPercent(100);
    if(energys.size() > 0)
    {
        for(auto c : energys)
        {
            if(c)
            {
                c->removeFromParent();
            }
            
        }
        energys.clear();
    }
    Vector<Node*> children = gameGroup->getChildren();
    for(auto c : children)
    {
        if(c->getName() == "mons" || c->getName() == "bullet")
        {
            c->removeFromParent();
        }
    }
    children.clear();
}

void GameScene::onExit()
{
    Layer::onExit();
    _eventDispatcher->removeEventListener(_listener);
}
bool GameScene::menuBegin(cocos2d::Touch* tTouch,cocos2d::Event* eEvent)
{
    Point localP = this->convertToNodeSpace(tTouch->getLocation());
    if(battleEnd){return true;}
    if(hpGroup->getBoundingBox().containsPoint(localP) && !pauseGroup->isVisible() && !battleEnd)
    {
        return true;
    }else if(skill1->getBoundingBox().containsPoint(localP) && !pauseGroup->isVisible() && skill1->isVisible() )
    {
        skill1->setScale(0.9f);
        return true;
    }else if(skill2->getBoundingBox().containsPoint(localP) && !pauseGroup->isVisible() && skill2->isVisible())
    {
        skill2->setScale(0.9f);
        return true;
    }else if(skill3->getBoundingBox().containsPoint(localP) && !pauseGroup->isVisible() && skill3->isVisible())
    {
        skill3->setScale(0.9f);
        return true;
    }else if(skill4->getBoundingBox().containsPoint(localP) && !pauseGroup->isVisible() && skill4->isVisible())
    {
        skill4->setScale(0.9f);
        return true;
    }
    else if(gameGroup->getBoundingBox().containsPoint(localP) && !pauseGroup->isVisible())
    {
        
        return true;
    }else if(pauseBtn->getBoundingBox().containsPoint(localP) && !pauseGroup->isVisible())
    {
        return true;
    }else if(pauseGroup->getBoundingBox().containsPoint(localP) && pauseGroup->isVisible())
    {
        return true;
    }else if(collectBtn->getBoundingBox().containsPoint(localP) && !pauseGroup->isVisible())
    {
        collectBtn->setScale(0.9f);
        collectMc = new MovieClip();
        this->gameGroup->addChild(collectMc);
        collectMc->setScale(0.4f);
        collectMc->playFile("collect.plist", "collect", 1, 12, -1, false);
        collectMc->setPosition(Vec2(roleMc->getPositionX(),roleMc->getPositionY() + 100));
        for(auto c : energys)
        {
            c->stopAllActions();
            float distance=sqrt(pow(c->getPositionX()-roleMc->getPositionX(),2)+pow(c->getPositionY()-(roleMc->getPositionY() + 100),2));
            float time = distance/300.0f;
            auto move  = MoveTo::create(time,Vec2(roleMc->getPositionX(),roleMc->getPositionY() + 60));
            auto callback = CallFunc::create([&](){
                for(auto c : energys)
                {
                    if(c->getPositionX() == roleMc->getPositionX() && (c->getPositionY() == (roleMc->getPositionY() + 60)))
                    {
                        c->removeFromParent();
                        energys.eraseObject(c);
                        operMp(20);
                        break;
                    }
                }
            });
            c->runAction(Sequence::create(move,callback, NULL));
        }
        return true;
    }
        
    return false;
}
void GameScene::menuEndCallback(cocos2d::Touch* tTouch,cocos2d::Event* eEvent)
{
    Point localP = this->convertToNodeSpace(tTouch->getLocation());
    if(battleEnd){return;}
    if(skill1->getBoundingBox().containsPoint(localP) && !pauseGroup->isVisible() && skill1->isVisible())
    {
        if(skillIndex == 1)
        {
            this->floatFont("This skill is being used");
            return;
        }
        skill1->setScale(1.0f);
        skillIndex = 1;
        this->changeSkillTab();
    }else if(skill2->getBoundingBox().containsPoint(localP) && !pauseGroup->isVisible() && skill2->isVisible())
    {
        if(skillIndex == 2)
        {
            this->floatFont("This skill is being used");
            return;
        }
        skill2->setScale(1.0f);
        skillIndex = 2;
        this->changeSkillTab();
    }else if(skill3->getBoundingBox().containsPoint(localP) && !pauseGroup->isVisible() && skill3->isVisible())
    {
        if(skillIndex == 3)
        {
            this->floatFont("This skill is being used");
            return;
        }
        skill3->setScale(1.0f);
        skillIndex = 3;
        this->changeSkillTab();
    }else if(skill4->getBoundingBox().containsPoint(localP) && !pauseGroup->isVisible() && skill4->isVisible())
    {
        if(skillIndex == 4)
        {
            this->floatFont("This skill is being used");
            return;
        }
        skill4->setScale(1.0f);
        skillIndex = 4;
        this->changeSkillTab();
    }
    else if(gameGroup->getBoundingBox().containsPoint(localP) && !pauseGroup->isVisible())
    {
        Point groupP = gameGroup->convertToNodeSpace(tTouch->getLocation());
        if(CF()->iceNum <= 0 && skillIndex == 3){skillIndex = 1;}
        if(CF()->lightNum <= 0 && skillIndex == 2){skillIndex = 1;}
        if(skillIndex == 2 || skillIndex == 3)
        {
            MovieClip*skillMc = new MovieClip();
            std::string ass = skillIndex == 2?"light":"ice";
            gameGroup->addChild(skillMc,1);
            int endIndex = skillIndex == 2?11:19;
            skillMc->playFile((ass+".plist").c_str(), ass.c_str(), 1, endIndex, 1, true);
            skillIndex == 2?CF()->lightNum -=1:CF()->iceNum-=1;
            iceNumLab->setString(StringUtils::format("%d",CF()->iceNum));
            lightNumLab->setString(StringUtils::format("%d",CF()->lightNum));
            skill2->setVisible((CF()->lightNum >= 1));
            skill3->setVisible((CF()->iceNum >= 1));
            iceNumLab->setVisible(CF()->iceNum>=1);
            lightNumLab->setVisible(CF()->lightNum>=1);
            skillMc->setPosition(Vec2(groupP.x,groupP.y));
            Vector<Node*> childrens = gameGroup->getChildren();
            for(auto c : childrens)
            {
                if(c->getName() == "mons")
                {
                     float distance=sqrt(pow(groupP.x - c->getPositionX(),2)+pow(groupP.y - c->getPositionY(),2));
                     if(distance <= 100)
                     {
                         int tag = c->getTag();
                         tag -= skillIndex;
                         if(tag <= 0)
                         {
                             c->stopAllActions();
                             createEnergy(Vec2(c->getPositionX(),c->getPositionY()));
                             c->removeFromParent();
                             curChallengeCout  -= 1;
                             float percent = float(curChallengeCout)/float(challengeCout)*100.0f;
                             static_cast<LoadingBar*>(bottomGroup->getChildByName("bar"))->setPercent(percent);
                             if(curChallengeCout <= 0)
                             {
                                 this->stopGame();
                                 //游戏当前关卡胜利
                                 battleEnd = true;
                                 ResultPop*result = new ResultPop(1);
                                 result->callFunc = CC_CALLBACK_1(GameScene::resultCallBack, this);
                                 this->addChild(result);
                             }
                         }else{
                             c->setTag(tag);
                         }
                         break;
                     }
                }
            }
            return;
        }
        if(curMp < 20){return;}
        if(localP.x > (visiblity.width/2))
        {
            //向右
            roleMc->setScaleX(1.0f);
        }else{
            roleMc->setScaleX(-1.0f);
        }
        this->operMp(-20);
        roleMc->changeAction("atk", 1, 6, 1, false);
        roleMc->frameRate = 0.03f;
        auto delay = DelayTime::create(0.3f);
        auto rolecall = CallFunc::create([&](){
            roleMc->changeAction("stand", 1, 4, -1, false);
        });
        roleMc->runAction(Sequence::create(delay,rolecall, NULL));
        
        Point centerP = Point(roleMc->getPositionX(),roleMc->getPositionY());
        //射击方向向量
        Point shootVector = groupP - centerP;
        //向量标准化(即向量长度为1)
        Point normalizedVector = ccpNormalize(shootVector);
        //移动长度向量
        Point overShootVector = normalizedVector * 900;
        //超出屏幕的点
        Point offScreenPoint = centerP + overShootVector;
        //假设速度为500（pix/s）
        float moveSpeed = 500;
        //移动时间
        float distance=sqrt(pow(centerP.x-offScreenPoint.x,2)+pow(centerP.y-offScreenPoint.y,2));
        float moveDuration = distance / moveSpeed;
        
        auto move = MoveTo::create(moveDuration,offScreenPoint);
        std::string bulletass = "assets/bullet.png";
        if(skillIndex == 4)
        {
            bulletass = "assets/bullet2.png";
        }
        Sprite*bullet = Sprite::create(bulletass);
        float angle = CC_RADIANS_TO_DEGREES(atan2(localP.y - centerP.y, localP.x - centerP.x));
        bullet->setRotation(-angle);
        gameGroup->addChild(bullet);
        bullet->setPosition(Vec2(roleMc->getPositionX(),roleMc->getPositionY()));
        bullet->setName("bullet");
        auto callback = CallFunc::create([=](){
           if(bullet)
           {
               bullet->removeFromParent();
           }
        });
        bullet->runAction(Sequence::create(move,callback, NULL));
    }else if(pauseBtn->getBoundingBox().containsPoint(localP) && !pauseGroup->isVisible()){
//        roleMc->setVisible(false);
        this->stopGame();
        pauseGroup->setVisible(true);
    }else if(pauseGroup->getBoundingBox().containsPoint(localP) && pauseGroup->isVisible())
    {
        Point groupP = pauseGroup->convertToNodeSpace(tTouch->getLocation());
        Sprite*homeBtn = static_cast<Sprite*>(pauseGroup->getChildByName("homeBtn"));
        Sprite*resetBtn = static_cast<Sprite*>(pauseGroup->getChildByName("resetBtn"));
        Sprite*continueBtn = static_cast<Sprite*>(pauseGroup->getChildByName("continueBtn"));
        Button*audioBtn = static_cast<Button*>(pauseGroup->getChildByName("audioBtn"));
        if(audioBtn->getBoundingBox().containsPoint(groupP))
        {
            if(SoundCtl::getInstance()->musicBool)
            {
                SoundCtl::getInstance()->stopBg();
                audioBtn->setBrightStyle(BRIGHT_NORMAL);
            }else{
                SoundCtl::getInstance()->playBg("sound/music.m4a", true);
                audioBtn->setBrightStyle(BRIGHT_HIGHLIGHT);
            }
        }else if(homeBtn->getBoundingBox().containsPoint(groupP))
        {
            auto scene = HelloWorld::createScene();
            Director::getInstance()->replaceScene(TransitionFadeTR::create(1, scene));
        }else if(resetBtn->getBoundingBox().containsPoint(groupP))
        {
//            roleMc->setVisible(true);
            pauseGroup->setVisible(false);
            this->resetData();
            this->startGame();
        }else if(continueBtn->getBoundingBox().containsPoint(groupP))
        {
//            roleMc->setVisible(true);
            pauseGroup->setVisible(false);
            this->startGame();
        }
    }else if(collectBtn->getBoundingBox().containsPoint(localP) && !pauseGroup->isVisible())
    {
        collectBtn->setScale(1.0f);
        if(collectMc){
            collectMc->releaseCache();
            collectMc = NULL;
        }
        for(auto c : energys)
        {
            c->stopAllActions();
            int dic = (c->getPositionX() > visiblity.width/2)?1:-1;
            Point startPoint = Point(c->getPositionX(),c->getPositionY());
            Point endPoint = Point(c->getPositionX() + 50*dic,tower->getPositionY() - 80);
//            auto move = CommonUtil::createParabola(1.0f, startPoint, endPoint, 200, 20);
            auto move = MoveTo::create(5.0f, Vec2(endPoint.x,endPoint.y));
            c->runAction(move);
        }
    }else if(hpGroup->getBoundingBox().containsPoint(localP) && !pauseGroup->isVisible())
    {
        Point hpGroupP = hpGroup->convertToNodeSpace(tTouch->getLocation());
        Sprite*hpicon = static_cast<Sprite*>(hpGroup->getChildByName("hpicon"));
        if(hpicon->getBoundingBox().containsPoint(hpGroupP))
        {
            if(curHp < totalHp)
            {
                int hpNum = CF()->hpNum;
                if(hpNum > 0)
                {
                    CF()->hpNum -= 1;
                    static_cast<Text*>(hpGroup->getChildByName("lab"))->setString(StringUtils::format("%d",CF()->hpNum));
                    operHp(20);
                }
                
            }
        }
    }
}
void GameScene::changeSkillTab()
{
    skill1->setBrightStyle(skillIndex == 1?BRIGHT_NORMAL:BRIGHT_HIGHLIGHT);
    skill2->setBrightStyle(skillIndex == 2?BRIGHT_NORMAL:BRIGHT_HIGHLIGHT);
    skill3->setBrightStyle(skillIndex == 3?BRIGHT_NORMAL:BRIGHT_HIGHLIGHT);
    skill4->setBrightStyle(skillIndex == 4?BRIGHT_NORMAL:BRIGHT_HIGHLIGHT);
}
void GameScene::operMp(int value)
{
    curMp += value;
    if(curMp >= totalMp){curMp = totalMp;}
    if(curMp <= 0){curMp = 0;}
    float percent = float(curMp)/float(totalMp)*100.0f;
    static_cast<LoadingBar*>(mpGroup->getChildByName("bar"))->setPercent(percent);
}
void GameScene::operHp(int value)
{
    curHp += value;
    if(curHp >= totalHp){curHp = totalHp;}
    if(curHp <= 0){
        curHp = 0;
        this->stopGame();
        //游戏结束
        ResultPop*result = new ResultPop(0);
        battleEnd = true;
        result->callFunc = CC_CALLBACK_1(GameScene::resultCallBack, this);
        this->addChild(result);
    }
    float percent = float(curHp)/float(totalHp)*100.0f;
    static_cast<LoadingBar*>(hpGroup->getChildByName("bar"))->setPercent(percent);
}
void GameScene::floatFont(std::string cnt)
{
    Text*content = Text::create();
    this->addChild(content);
    content->setString(cnt);
    content->setFontSize(30);
    content->setColor(Color3B(255,255,255));
    Size visible = Director::getInstance()->getVisibleSize();
    content->setPosition(Vec2(visible.width/2,visible.height/2));
    auto move = MoveTo::create(1.0f, Vec2(content->getPositionX(),content->getPositionY() + 100));
    auto call = CallFunc::create([=](){
        content->removeFromParent();
    });
    auto seque = Sequence::create(move,call, NULL);
    content->runAction(seque);
}
