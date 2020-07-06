//
//  GameScene.cpp
//  zhanShen-mobile
//
//  Created by 志强 on 2020/5/22.
//

#include "GameScene.h"
#include "Config.h"
#include "SoundCtl.h"
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
    auto rootNode = CSLoader::createNode("GameScene.csb");

    addChild(rootNode);
    
    bg = static_cast<Sprite*>(rootNode->getChildByName("bg"));
    gameGroup = static_cast<Layout*>(rootNode->getChildByName("gameGroup"));
    topbg = static_cast<Sprite*>(rootNode->getChildByName("topbg"));
    audioBtn = static_cast<Button*>(rootNode->getChildByName("audioBtn"));
    scoreLab = static_cast<TextBMFont*>(rootNode->getChildByName("scoreLab"));
    bottombg = static_cast<Layout*>(rootNode->getChildByName("bottombg"));
    
    bottomPanel = static_cast<Layout*>(rootNode->getChildByName("bottomPanel"));
    timeLab = static_cast<TextBMFont*>(bottomPanel->getChildByName("timeLab"));
    waveLab = static_cast<TextBMFont*>(bottomPanel->getChildByName("waveLab"));
    hpLab = static_cast<TextBMFont*>(bottomPanel->getChildByName("hpLab"));
    goldLab = static_cast<TextBMFont*>(bottomPanel->getChildByName("goldLab"));
    boneIcon = static_cast<Sprite*>(bottomPanel->getChildByName("boneIcon"));
    contentGroup = static_cast<Layout*>(rootNode->getChildByName("contentGroup"));
    overBg = static_cast<Layout*>(rootNode->getChildByName("overBg"));
    
    returnBtn = static_cast<Sprite*>(contentGroup->getChildByName("returnBtn"));
    continueBtn = static_cast<Layout*>(contentGroup->getChildByName("continueBtn"));
    
    overBg->setVisible(false);
    contentGroup->setVisible(false);
    
    _listener=EventListenerTouchOneByOne::create();
    _listener->setSwallowTouches(true);
    _listener->onTouchBegan = CC_CALLBACK_2(GameScene::menuBegin,this);
    _listener->onTouchEnded = CC_CALLBACK_2(GameScene::menuEndCallback, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(_listener, this);
    this->adapter();
    
    goldLab->setString(StringUtils::format("%d",CF()->gold));
    roleMc = new MovieClip();
    this->addChild(roleMc);
    roleMc->playFile("role.plist", "stand", 1, 4, -1, false);
    visiblity = Director::getInstance()->getVisibleSize();
    roleMc->setPosition(visiblity.width/2,bottomPanel->getPositionY() + 150);
    
    if(SoundCtl::getInstance()->musicBool)
    {
        audioBtn->setBrightStyle(BRIGHT_NORMAL);
    }else{
        audioBtn->setBrightStyle(BRIGHT_HIGHLIGHT);
    }
    
    this->schedule(schedule_selector(GameScene::startGame), 1.0f);
    this->schedule(schedule_selector(GameScene::ticktime),0.1f);
    return true;
}
void GameScene::startGame(float dt)
{
    timecount -= 1;
    if(timecount <= 0)
    {
        //游戏结束
        this->stopGame();
    }
    int minute = timecount/60;
    std::string minutestr = minute < 10 ?StringUtils::format("0%d",minute):StringUtils::format("%d",minute);
    int second = timecount - minute*60;
    std::string secondstr = second < 10 ?StringUtils::format("0%d",second):StringUtils::format("%d",second);
    timeLab->setString("TIME "+minutestr + ":" + secondstr);
    
    MovieClip*mon = new MovieClip();
    gameGroup->addChild(mon);
    mon->setName("mon");
    srand(int(time(0))+ rand());//置随机数种子
    int rx= 100 + CCRANDOM_0_1()*(visiblity.width - 200);
    int ry = visiblity.height + 100;
    
    int randomNum = CCRANDOM_0_1()*100;
    if(randomNum >= 50)
    {
        std::string namestr = "";
        std::string ass = "";
        if(randomNum >= 98)
        {
            namestr = "gold";
            ass = "assets/goldicon-sheet0.png";
        }else if(randomNum >= 95)
        {
            namestr = "hp";
            ass = "assets/healthicon-sheet0.png";
        }else if(randomNum >= 90)
        {
            namestr = "obone";
            int boneindex = CCRANDOM_0_1()*3;
            if(boneindex == 0){ass = "assets/weaponcrescentmoon-sheet0.png";}
            else if(boneindex == 1){ass = "assets/weaponicon-sheet0_01.png";}
            else{ass = "assets/weaponicon-sheet0_02.png";}
        }
        if(ass != "")
        {
            Sprite*gold = Sprite::create(ass);
            gameGroup->addChild(gold);
            auto delay = DelayTime::create(4.0f);
            auto callback = CallFunc::create([=](){
               if(gold)
               {
                   gold->removeFromParent();
               }
            });
            gold->runAction(Sequence::create(delay,callback, NULL));
             gold->setName(namestr);
            int gx = 100 + CCRANDOM_0_1()*(visiblity.width - 200);
            int gy = CCRANDOM_0_1()*(visiblity.height/2) + 300;
            gold->setPosition(Vec2(gx,gy));
        }
        
    }
    
    mon->setPosition(Vec2(rx,ry));
    mon->playFile("mon.plist", "run", 1, 6, -1, false);
    int time = 15.0f - level;
    if(time <= 5.0f){time = 5.0f;}
    auto moveTo = MoveTo::create(time, Vec2(rx,-100));
    auto callback = CallFunc::create([=](){
       if(mon)
       {
           mon->removeFromParent();
           hp -= 1;
           hpLab->setString(StringUtils::format("x%d",hp));
           if(hp <= 0)
           {
               this->stopGame();
           }
       }
    });
    mon->runAction(Sequence::create(moveTo,callback, NULL));
    
    
}
void GameScene::ticktime(float dt)
{
    Vector<Sprite*> bones;
    Vector<Node*> entitys;
    Vector<Node*> childrens = gameGroup->getChildren();
    for(auto c : childrens)
    {
        if(c->getName() == "bone")
        {
            bones.pushBack(static_cast<Sprite*>(c));
        }
        if(c->getName() == "mon" || c->getName() == "gold" || c->getName() == "hp" || c->getName() == "obone")
        {
            entitys.pushBack(static_cast<MovieClip*>(c));
        }
    }
    for(auto c : bones)
    {
        for(auto e:entitys)
        {
            float distance=sqrt(pow(c->getPositionX()-e->getPositionX(),2)+pow(c->getPositionY()-e->getPositionY(),2));
            if(distance <= 100)
            {
                if(e->getName() == "mon")
                {
                   score += 10;
                   if(score >= level * 500)
                   {
                       level += 1;
                       waveLab->setString(StringUtils::format("WAVE:%d",level));
                       this->showLevelAdd();
                   }
                   scoreLab->setString(StringUtils::format("SCORE:%d",score));
                   MovieClip*mc = new MovieClip();
                   mc->playFile("boom.plist", "frame", 1, 14, 1, true);
                   gameGroup->addChild(mc);
                   mc->setPosition(Vec2(e->getPositionX(),e->getPositionY()));
                   Config::getInstance()->score += 10;
                }else if(e->getName() == "gold")
                {
                    CF()->gold += 1;
                    goldLab->setString(StringUtils::format("%d",CF()->gold));
                }else if(e->getName() == "hp")
                {
                    hp+=1;
                    hpLab->setString(StringUtils::format("x%d",hp));
                }else if(e->getName() == "obone")
                {
                    //更换子弹
                    boneIcon->setTexture(static_cast<Sprite*>(e)->getTexture());
                }
               
                c->removeFromParent();
                e->removeFromParent();
                break;
            }
        }
    }
    bones.clear();
    entitys.clear();
}
void GameScene::showLevelAdd()
{
    auto myLabel = Label::createWithBMFont("assets/myFont.fnt", StringUtils::format("WAVE:%d",level));
    gameGroup->addChild(myLabel);
    myLabel->setPosition(Vec2(visiblity.width/2,visiblity.height/2));
    myLabel->setOpacity(0);
    auto fadeIn1 = FadeIn::create(0.5f);
    auto fadeout1 = FadeOut::create(0.5f);
    auto callback = CallFunc::create([=](){
       if(myLabel)
       {
           myLabel->removeFromParent();
       }
    });
    myLabel->runAction(Sequence::create(fadeIn1,fadeout1,fadeIn1,fadeout1,callback, NULL));
}
void GameScene::stopGame()
{
    this->unschedule(schedule_selector(GameScene::startGame));
    this->unschedule(schedule_selector(GameScene::ticktime));
    Vector<Node*> childrens = gameGroup->getChildren();
    for(auto c : childrens)
    {
        if(c->getName() == "mon" || c->getName() == "bone" || c->getName() == "hp" || c->getName() == "obone")
        {
            c->stopAllActions();
            c->removeFromParent();
        }
    }
    overBg->setVisible(true);
    contentGroup->setVisible(true);
    if(CF()->score < score)
    {
        CF()->score = score;
    }
    static_cast<TextBMFont*>(contentGroup->getChildByName("bestScore"))->setString(StringUtils::format("BEST SCORE:%d",CF()->score));
    static_cast<TextBMFont*>(contentGroup->getChildByName("scoreLab"))->setString(StringUtils::format("YOUR SCORE:%d",score));
}
void GameScene::adapter()
{
    int bgW = bg->getContentSize().width;
    int bgH = bg->getContentSize().height;
    Size visibleSize = Director::getInstance()->getVisibleSize();
    float scaleX = visibleSize.width/bgW;
    float scaleY = visibleSize.height/bgH;
    bg->setScale(scaleX, scaleY);
    audioBtn->setPosition(Vec2(audioBtn->getPositionX()*scaleX,audioBtn->getPositionY()*scaleY));
    scoreLab->setPosition(Vec2(scoreLab->getPositionX()*scaleX,scoreLab->getPositionY()*scaleY));
    topbg->setScale(scaleX, scaleY);
    topbg->setPosition(Vec2(topbg->getPositionX()*scaleX,topbg->getPositionY()*scaleY));
    bottombg->setScale(scaleX,scaleY);
    bottombg->setPosition(Vec2(bottombg->getPositionX()*scaleX,bottombg->getPositionY()*scaleY));
    gameGroup->setScale(scaleX,scaleY);
    gameGroup->setPosition(Vec2(gameGroup->getPositionX()*scaleX,gameGroup->getPositionY()*scaleY));
    bottomPanel->setScale(scaleY);
    bottomPanel->setPosition(Vec2(bottomPanel->getPositionX()*scaleX,bottomPanel->getPositionY()*scaleY));
    overBg->setScale(scaleX,scaleY);
    contentGroup->setPosition(Vec2(contentGroup->getPositionX()*scaleX,contentGroup->getPositionY()*scaleY));
}
void GameScene::onExit()
{
    Layer::onExit();
    _eventDispatcher->removeEventListener(_listener);
}
bool GameScene::menuBegin(cocos2d::Touch* tTouch,cocos2d::Event* eEvent)
{
    Point localP = this->convertToNodeSpace(tTouch->getLocation());
    Point contentP = contentGroup->convertToNodeSpace(tTouch->getLocation());
    if(gameGroup->getBoundingBox().containsPoint(localP) && !overBg->isVisible())
    {
        return true;
    }else if(audioBtn->getBoundingBox().containsPoint(localP) && !overBg->isVisible())
    {
        return true;
    }else if(contentGroup->getBoundingBox().containsPoint(localP) && overBg->isVisible())
    {
        return true;
    }
    return false;
}
void GameScene::menuEndCallback(cocos2d::Touch* tTouch,cocos2d::Event* eEvent)
{
    Point localP = this->convertToNodeSpace(tTouch->getLocation());
    if(audioBtn->getBoundingBox().containsPoint(localP) && audioBtn->isVisible() && !overBg->isVisible())
    {
        if(SoundCtl::getInstance()->musicBool)
        {
            SoundCtl::getInstance()->stopBg();
            audioBtn->setBrightStyle(BRIGHT_NORMAL);
        }else{
            SoundCtl::getInstance()->playBg("sound/music.m4a", true);
            audioBtn->setBrightStyle(BRIGHT_HIGHLIGHT);
        }
        return;
    }
    else if(gameGroup->getBoundingBox().containsPoint(localP) && !overBg->isVisible())
    {
        
        roleMc->stopAllActions();
        int offx = localP.x - roleMc->getPositionX();
        float time = abs(offx)/400.0f;
        auto moveto = MoveTo::create(time, Vec2(roleMc->getPositionX() + offx,roleMc->getPositionY()));
        roleMc->runAction(moveto);
        
        Sprite*bone = Sprite::createWithTexture(boneIcon->getTexture());
        gameGroup->addChild(bone);
        bone->setPosition(Vec2(roleMc->getPositionX(),roleMc->getPositionY()));
        bone->setName("bone");
        auto boneMove = MoveTo::create(1.0f, Vec2(roleMc->getPositionX(),visiblity.height));
        auto callback = CallFunc::create([=](){
           if(bone)
           {
               bone->removeFromParent();
           }
        });
        bone->runAction(Sequence::create(boneMove,callback, NULL));
    }else if(contentGroup->getBoundingBox().containsPoint(localP) && overBg->isVisible())
    {
        Point contentP = contentGroup->convertToNodeSpace(tTouch->getLocation());
        Rect rect = returnBtn->getBoundingBox();
        if(rect.containsPoint(contentP))
        {
            auto scene = HelloWorld::createScene();
            Director::getInstance()->replaceScene(TransitionFade::create(1, scene));
        }else if(continueBtn->getBoundingBox().containsPoint(contentP))
        {
            if(CF()->gold >= 5)
            {
                hp = 5;
                hpLab->setString("x5");
                timecount = 5*60;
                this->schedule(schedule_selector(GameScene::startGame), 1.0f);
                this->schedule(schedule_selector(GameScene::ticktime),0.1f);
                overBg->setVisible(false);
                contentGroup->setVisible(false);
                CF()->gold -= 5;
                goldLab->setString(StringUtils::format("%d",CF()->gold));
            }else{
                this->floatFont("not enough gold");
            }
            
        }
    }

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
