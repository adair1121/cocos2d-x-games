#include "HelloWorldScene.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "utils/SoundCtl.h"
#include "MovieClip.h"
USING_NS_CC;

using namespace cocostudio::timeline;

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
    
    bg = static_cast<Sprite*>(rootNode->getChildByName("bg"));
    topGroup = static_cast<Layout*>(rootNode->getChildByName("topGroup"));
    energyBar = static_cast<LoadingBar*>(topGroup->getChildByName("energyBar"));
    killLab = static_cast<TextBMFont*>(topGroup->getChildByName("killLab"));
    timeLab = static_cast<TextBMFont*>(topGroup->getChildByName("timeLab"));
    
    endGroup = static_cast<Layout*>(rootNode->getChildByName("endGroup"));
    againBtn = static_cast<Button*>(endGroup->getChildByName("againBtn"));
    resultTimeLab = static_cast<TextBMFont*>(endGroup->getChildByName("resultTimeLab"));
    resultKillLab = static_cast<TextBMFont*>(endGroup->getChildByName("resultKillLab"));
    
    startGroup = static_cast<Layout*>(rootNode->getChildByName("startGroup"));
    logo = static_cast<Sprite*>(startGroup->getChildByName("logo"));
    startBtn = static_cast<Button*>(startGroup->getChildByName("startBtn"));
    
    gameGroup = static_cast<Layout*>(rootNode->getChildByName("gameGroup"));
    topG = static_cast<Layout*>(gameGroup->getChildByName("topG"));
    arrow = static_cast<Sprite*>(gameGroup->getChildByName("arrow"));
    bottomG = static_cast<Layout*>(gameGroup->getChildByName("bottomG"));
    leftG = static_cast<Layout*>(gameGroup->getChildByName("leftG"));
    rightG = static_cast<Layout*>(gameGroup->getChildByName("rightG"));
    centerG = static_cast<Layout*>(gameGroup->getChildByName("centerG"));
    
    _listener=EventListenerTouchOneByOne::create();
    _listener->setSwallowTouches(true);
    _listener->onTouchBegan = CC_CALLBACK_2(HelloWorld::menuBegin,this);
    _listener->onTouchEnded = CC_CALLBACK_2(HelloWorld::menuEndCallback, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(_listener, this);
    endGroup->setVisible(false);
    SoundCtl::getInstance()->playBg("sound/music.m4a", true);
    
    heroSp = Sprite::create("heroA/img_heroD.png");
    gameGroup->addChild(heroSp);
    heroSp->setPosition(Vec2(centerG->getPositionX(),centerG->getPositionY() +30));
    heroSp->setName("role");

    this->adapter();
    return true;
}
void HelloWorld::adapter()
{
    int bgW = bg->getContentSize().width;
    int bgH = bg->getContentSize().height;
    Size visibleSize = Director::getInstance()->getVisibleSize();
    float scaleX = visibleSize.width/bgW;
    float scaleY = visibleSize.height/bgH;
    bg->setScale(scaleX, scaleY);
    gameGroup->setScale(scaleX,scaleY);
    topGroup->setPositionY(topGroup->getPositionY()*scaleY);
    arrow->setPosition(centerG->getPosition());
    endGroup->setPosition(Vec2(endGroup->getPositionX()*scaleX,endGroup->getPositionY()*scaleY));
}

bool HelloWorld::menuBegin(cocos2d::Touch* tTouch,cocos2d::Event* eEvent)
{
    Point startP = startGroup->convertToNodeSpace(tTouch->getLocation());
    Point gameP = this->convertToNodeSpace(tTouch->getLocation());
    Point endP = endGroup->convertToNodeSpace(tTouch->getLocation());
    if(startBtn->getBoundingBox().containsPoint(startP) && !startboo){
        startBtn->setBrightStyle(BRIGHT_HIGHLIGHT);
        return true;
    }else if(gameGroup->getBoundingBox().containsPoint(gameP) && !battleEnd)
    {
        beginP.x = gameP.x;
        beginP.y = gameP.y;
        return true;
    }else if(againBtn->getBoundingBox().containsPoint(endP) && battleEnd)
    {
        againBtn->setBrightStyle(BRIGHT_HIGHLIGHT);
        return true;
    }
    return false;
}
void HelloWorld::menuEndCallback(cocos2d::Touch* tTouch,cocos2d::Event* eEvent)
{
    Point startP = startGroup->convertToNodeSpace(tTouch->getLocation());
    Point gameP = this->convertToNodeSpace(tTouch->getLocation());
    Point endP = endGroup->convertToNodeSpace(tTouch->getLocation());
    if(startBtn->getBoundingBox().containsPoint(startP) && !startboo){
        SoundCtl::getInstance()->playEffect("sound/click.m4a");
        startBtn->setBrightStyle(BRIGHT_NORMAL);
        startboo = true;
        this->startGame();
    }else if(gameGroup->getBoundingBox().containsPoint(gameP) && !battleEnd)
    {
        float offx = gameP.x - beginP.x;
        float offy = gameP.y - beginP.y;
        
        std::string directstr = "";
        if(offx >= 20 && abs(offy) <= 130)
        {
            directstr = "R";
        }else if(offx <= -20 && abs(offy) <= 130)
        {
            directstr = "L";
        }else if(offy >= 20 && abs(offx) <= 130)
        {
            directstr = "U";
        }else if(offy <= -20 && abs(offx) <= 130)
        {
            directstr = "D";
        }
        if(directstr != "")
        {
            changeHeroAction(directstr, "attack");
            if(lightsp)
            {
                Size size = Director::getInstance()->getVisibleSize();
                Vector<Node*> childrens = gameGroup->getChildren();
                for(auto c : childrens)
                {
                    if(c->getName() == "mon")
                    {
                       
                        float distance=sqrt(pow(c->getPositionX()-lightsp->getPositionX(),2)+pow(c->getPositionY()-lightsp->getPositionY(),2));
                        if(distance <= 100)
                        {
                            c->stopAllActions();
                            int tag = c->getTag();
                            curEnerge += 5;
                            killCount += 1;
                            this->killLab->setString(StringUtils::format("%d",killCount));
                            if(curEnerge >= totalEnergy)
                            {
                                curEnerge = 0;
                                energyBar->setPercent(0);
                                this->releaseSkill();
                                break;
                            }
                            float percent = float(curEnerge)/float(totalEnergy)*100.0f;
                            energyBar->setPercent(percent);
                            auto callback = CallFunc::create([=](){
                                if(c)
                                {
                                    c->removeFromParent();
                                }
                            });
                            if(tag == 1 || tag == 3)
                            {
                                int dic = tag == 1?-1:1;
                                auto moveto = MoveTo::create(1.0f, Vec2(c->getPositionX()+dic*size.width + dic*100,c->getPositionY()));
                                c->runAction(Sequence::create(moveto,callback, NULL));
                            }else{
                                int dic = tag == 2?-1:1;
                                auto moveto = MoveTo::create(1.0f, Vec2(c->getPositionX(),c->getPositionY()+dic*size.height/2 + 100*dic));
                                c->runAction(Sequence::create(moveto,callback, NULL));
                            }
                        }
                    }
                }
            }
            
        }
    }
    else if(againBtn->getBoundingBox().containsPoint(endP) && battleEnd)
    {
        endGroup->setVisible(false);
        battleEnd = false;
        againBtn->setBrightStyle(BRIGHT_NORMAL);
        this->resetDate();
    }
}
void HelloWorld::releaseSkill()
{
    Vector<Node*> childrens = gameGroup->getChildren();
    MovieClip*skillmc = new MovieClip();
    gameGroup->addChild(skillmc);
    skillmc->setPosition(Vec2(centerG->getPositionX(),centerG->getPositionY()));
    skillmc->playFile("skill.plist", "img_shockwave", 1, 7, 1, true);
    Size size = Director::getInstance()->getVisibleSize();
    for(auto c : childrens)
    {
       if(c->getName() == "mon")
       {
           c->stopAllActions();
           killCount += 1;
           this->killLab->setString(StringUtils::format("%d",killCount));
           auto callback = CallFunc::create([=](){
               if(c)
               {
                   c->removeFromParent();
               }
           });
           int tag = c->getTag();
           if(tag == 1 || tag == 3)
           {
               int dic = tag == 1?-1:1;
               auto moveto = MoveTo::create(1.0f, Vec2(c->getPositionX()+dic*size.width + dic*100,c->getPositionY()));
               c->runAction(Sequence::create(moveto,callback, NULL));
           }else{
               int dic = tag == 2?-1:1;
               auto moveto = MoveTo::create(1.0f, Vec2(c->getPositionX(),c->getPositionY()+dic*size.height/2 + 100*dic));
               c->runAction(Sequence::create(moveto,callback, NULL));
           }
       }
    }
}
void HelloWorld::startGame()
{
    startGroup->setVisible(false);
    startGroup->setEnabled(false);
    this->schedule(schedule_selector(HelloWorld::updatetime),1.0f );
}
void HelloWorld::updatetime(float dt)
{
    countTime += 1;
    int hour = countTime/(24*60*60);
    int minute = (countTime - (hour*60*60))/60;
    int secord = countTime - hour*60*60 - minute*60;
    std::string hourstr = hour < 10?StringUtils::format("0%d",hour):StringUtils::format("%d",hour);
    std::string minutestr = minute < 10?StringUtils::format("0%d",minute):StringUtils::format("%d",minute);
    std::string secondstr = secord < 10?StringUtils::format("0%d",secord):StringUtils::format("%d",secord);
    timeLab->setString(hourstr+":"+minutestr+":"+secondstr);
    srand(int(time(0))+ rand());//置随机数种子
    int num=CCRANDOM_0_1()*4;
//    MovieClip*birthMc = new MovieClip();
//    gameGroup->addChild(birthMc);
    birthNum = num;
    if(num == 0){birthP.x = topG->getPositionX();birthP.y = topG->getPositionY();}
    else if(num == 1){birthP.x = leftG->getPositionX();birthP.y = leftG->getPositionY();}
    else if(num == 2){birthP.x = bottomG->getPositionX();birthP.y = bottomG->getPositionY();}
    else if(num == 3){birthP.x = rightG->getPositionX();birthP.y = rightG->getPositionY();}
//    birthMc->setPosition(Vec2(birthP.x,birthP.y));
//    birthMc->frameRate = 0.06f;
//    birthMc->playFile("brith.plist", "img_Slime", 1, 6, 1, true);
//    birthMc->mcPlayEndFunc = CC_CALLBACK_0(HelloWorld::createEnemy, this);
    createEnemy();
}
void HelloWorld::createEnemy()
{
    
    std::string directstr = birthNum==0?"Up":birthNum==1?"Right":birthNum==2?"Down":"Right";
    std::string slimNode = directstr+"Node.csb";
    Node*slime = CSLoader::createNode(slimNode);
    slime->setScale(0.5f);
    if(birthNum == 3)
    {
        slime->setScaleX(-0.5f);
    }
    cocostudio::timeline::ActionTimeline *slimAction = CSLoader::createTimeline(slimNode);
    slime->runAction(slimAction);
    slimAction->gotoFrameAndPlay(0,25,true);
    gameGroup->addChild(slime);
    slime->setName("mon");
    slime->setTag(birthNum);
    slime->setPosition(Vec2(birthP.x,birthP.y));
    auto callback = CallFunc::create([&](){
       //如果走到这里说明游戏结束了
        this->unschedule(schedule_selector(HelloWorld::updatetime));
        Vector<Node*> childrens = gameGroup->getChildren();
        for(auto c : childrens)
        {
            if(c->getName() == "mon")
            {
                c->stopAllActions();
                c->removeFromParent();
            }
            
        }
        battleEnd = true;
        endGroup->setVisible(true);
        resultTimeLab->setString(timeLab->getString());
        resultKillLab->setString(killLab->getString());
    });
    if(birthNum == 0 || birthNum == 2)
    {
        int dic = birthNum == 0?1:-1;
        //上下
//        auto move1 = MoveTo::create(0.1f, Vec2(birthP.x, birthP.y - 130*dic));
//        auto easeOut1 = EaseSineIn::create(move1->clone());
//        auto delay1 = DelayTime::create(0.8f);
//        auto move2 = MoveTo::create(0.1f,Vec2(birthP.x, birthP.y - 100*dic));
//        auto easeOut2 = EaseSineIn::create(move2->clone());
//        auto move3 = MoveTo::create(0.1f, Vec2(birthP.x,birthP.y - 100*dic - 130*dic));
//        auto easeOut3 = EaseSineIn::create(move3->clone());
//        auto move4 = MoveTo::create(0.1f, Vec2(birthP.x,birthP.y - 200*dic));
//        auto easeOut4 = EaseSineIn::create(move4->clone());
//        auto move5 = MoveTo::create(0.1f, Vec2(birthP.x,birthP.y - 200*dic - 130*dic));
//        auto easeOut5 = EaseSineIn::create(move5->clone());
//        auto move6 = MoveTo::create(0.1f, Vec2(birthP.x,centerG->getPositionY()));
//        auto easeOut6 = EaseSineIn::create(move6->clone());
        auto moveto = MoveTo::create(3.0f, Vec2(birthP.x,centerG->getPositionY()));
        auto seque = Sequence::create(moveto,callback, NULL);
        slime->runAction(seque);
    }else{
        //左右
        int dic = birthNum == 1?1:-1;
        //上下
//        auto move1 = MoveTo::create(0.1f, Vec2(birthP.x + 50*dic, birthP.y + 50));
//        auto easeOut1 = EaseSineIn::create(move1->clone());
//        auto delay1 = DelayTime::create(0.8f);
//        auto move2 = MoveTo::create(0.1f,Vec2(birthP.x + 100*dic, birthP.y));
//        auto easeOut2 = EaseSineIn::create(move2->clone());
//        auto move3 = MoveTo::create(0.1f, Vec2(birthP.x + 150*dic,birthP.y + 50 ));
//        auto easeOut3 = EaseSineIn::create(move3->clone());
//        auto move4 = MoveTo::create(0.1f, Vec2(birthP.x + 200*dic,birthP.y));
//        auto easeOut4 = EaseSineIn::create(move4->clone());
//        auto move5 = MoveTo::create(0.1f, Vec2(birthP.x + 250*dic,birthP.y + 50));
//        auto easeOut5 = EaseSineIn::create(move5->clone());
//        auto move6 = MoveTo::create(0.1f, Vec2(centerG->getPositionX(),birthP.y));
//        auto easeOut6 = EaseSineIn::create(move6->clone());
        auto moveto = MoveTo::create(3.0f, Vec2(centerG->getPositionX(),birthP.y));
        auto seque = Sequence::create(moveto,callback, NULL);
        slime->runAction(seque);
    }
}
void HelloWorld::resetDate()
{
    this->unschedule(schedule_selector(HelloWorld::updatetime));
    beginP.x = 0;
    beginP.y = 0;
    timeLab->setString("00:00:00");
    killLab->setString("0");
    countTime = 0;
    curEnerge = 0;
    killCount = 0;
    battleEnd = false;
    energyBar->setPercent(0);
    Vector<Node*> childrens = gameGroup->getChildren();
    srand(int(time(0))+ rand());//置随机数种子
    int num=CCRANDOM_0_1()*5+1;
//    Texture2D*texture = Director::getInstance()->getTextureCache()->addImage(StringUtils::format("assets/slm_ground%d.png",num));
//    bg->setTexture(texture);
    for(auto c : childrens)
    {
        if(c->getName() == "mon")
        {
            c->removeFromParent();
        }
    }
    this->startGame();
}
void HelloWorld::changeHeroAction(std::string direct ,std::string action)
{
    //切换人物动作
    std::string roleass = "";
    if(action == "stand")
    {
        roleass = "heroA/img_hero"+direct+".png";
    }else{
        roleass = "heroA/img_hero"+direct+"A.png";
    }
    
    Texture2D*texture = Director::getInstance()->getTextureCache()->addImage(roleass);
    
    heroSp->setContentSize(texture->getContentSize());
    
    heroSp->setTexture(texture);
    if(action == "attack")
    {
        auto delay = DelayTime::create(0.1f);
        if(!lightsp)
        {
            lightsp = Sprite::create("heroA/img_light.png");
            gameGroup->addChild(lightsp,0);
            lightsp->setOpacity(0);
            if(direct == "R")
            {
                lightsp->setRotation(-90);
                float x = centerG->getPositionX() + centerG->getContentSize().width/2;
                lightsp->setPosition(Vec2(x,centerG->getPositionY()));
            }else if(direct == "U")
            {
                lightsp->setRotation(-180);
                float y = centerG->getPositionY() + centerG->getContentSize().height/2;
                lightsp->setPosition(Vec2(centerG->getPositionX(),y));
            }else if(direct == "L")
            {
                lightsp->setRotation(90);
                float x = centerG->getPositionX() - centerG->getContentSize().width/2;
                lightsp->setPosition(Vec2(x,centerG->getPositionY()));
            }else if(direct == "D")
            {
                float y = centerG->getPositionY() - centerG->getContentSize().height/2;
                lightsp->setPosition(Vec2(centerG->getPositionX(),y));
            }
            auto fadeIn = FadeIn::create(0.1f);
            lightsp->runAction(fadeIn);
        }
        
        auto callback = CallFunc::create([=](){
            if(lightsp)
            {
                lightsp->removeFromParent();
                lightsp = NULL;
            }
            
            changeHeroAction(direct, "stand");
        });
        heroSp->runAction(Sequence::create(delay,callback, NULL));
    }
}

