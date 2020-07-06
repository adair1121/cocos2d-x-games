//
//  GameLayer.cpp
//  MonCity-mobile
//
//  Created by 志强 on 2020/6/16.
//

#include "GameLayer.hpp"
#include "ScrollerItem.hpp"
#include "Config.h"
#include "HelloWorldScene.h"
#include "CommonUtil.h"
Scene* GameLayer::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = GameLayer::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool GameLayer::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
    
    auto rootNode = CSLoader::createNode("BattleLayer.csb");

    addChild(rootNode);
    
    map = static_cast<Sprite*>(rootNode->getChildByName("map"));
    topGroup = static_cast<Layout*>(rootNode->getChildByName("topGroup"));
    loadBar = static_cast<LoadingBar*>(topGroup->getChildByName("loadBar"));
    levelLab = static_cast<TextBMFont*>(topGroup->getChildByName("levelLab"));
    
    bottomGroup = static_cast<Layout*>(rootNode->getChildByName("bottomGroup"));
    roleBtn = static_cast<Button*>(bottomGroup->getChildByName("roleBtn"));
    partnerBtn = static_cast<Button*>(bottomGroup->getChildByName("partnerBtn"));
    skillBtn = static_cast<Button*>(bottomGroup->getChildByName("skillBtn"));
    dmgLab = static_cast<Text*>(bottomGroup->getChildByName("dmgLab"));
    dpsLab = static_cast<Text*>(bottomGroup->getChildByName("dpsLab"));
    goldLab = static_cast<Text*>(bottomGroup->getChildByName("goldLab"));
    scroller = static_cast<ScrollView*>(bottomGroup->getChildByName("scroller"));
    returnBtn = static_cast<Sprite*>(rootNode->getChildByName("returnBtn"));
    
    enemyNode = static_cast<Node*>(rootNode->getChildByName("enemyNode"));
    cocostudio::timeline::ActionTimeline *monact = CSLoader::createTimeline("Enemy1.csb");
    enemyNode->runAction(monact);
    monact->gotoFrameAndPlay(0,35,true);
    
    mainGeneral = static_cast<Node*>(rootNode->getChildByName("mainGeneral"));
    mainAct = CSLoader::createTimeline("General1.csb");
    mainGeneral->runAction(mainAct);
    mainAct->gotoFrameAndPlay(40,75,true);
    
    general2 = static_cast<Node*>(rootNode->getChildByName("general2"));
    general2Act = CSLoader::createTimeline("General2.csb");
    general2->runAction(general2Act);
    general2Act->gotoFrameAndPlay(40,75,true);
    general2->setVisible(false);
    
    general3 = static_cast<Node*>(rootNode->getChildByName("general3"));
    general3Act = CSLoader::createTimeline("General3.csb");
    general3->runAction(general3Act);
    general3->setVisible(false);
    general3Act->gotoFrameAndPlay(40,75,true);
    
    general4 = static_cast<Node*>(rootNode->getChildByName("general4"));
    general4Act = CSLoader::createTimeline("General4.csb");
    general4->runAction(general4Act);
    general4->setVisible(false);
    general4Act->gotoFrameAndPlay(40,75,true);
    
    general5 = static_cast<Node*>(rootNode->getChildByName("general5"));
    general5Act = CSLoader::createTimeline("General5.csb");
    general5->runAction(general5Act);
    general5->setVisible(false);
    general5Act->gotoFrameAndPlay(40,75,true);
    
    _listener=EventListenerTouchOneByOne::create();
    _listener->setSwallowTouches(true);
    _listener->onTouchBegan = CC_CALLBACK_2(GameLayer::menuBegin,this);
    _listener->onTouchEnded = CC_CALLBACK_2(GameLayer::menuEndCallback, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(_listener, this);
    
    curHp = totalHp = totalLevel*50 + curLevel*10;
    loadBar->setPercent(100.0f);
    levelLab->setString(StringUtils::format("%d_",totalLevel)+StringUtils::format("%d",curLevel));
    
    int col = 1;
    int interval = 10;
    int itemH = 137;
    float numchidren =4;
    float num = float((numchidren)/1.0f);
    int scrollH =  ceil(num)*(itemH + interval);
    int scrollHeight = scroller->getContentSize().height;
    if(scrollH <= scrollHeight)
    {
        scrollH = scrollHeight;
    }
    continerLayer = Layer::create();
    continerLayer->setContentSize(Size(scroller->getContentSize().width,scrollH));
    continerLayer->setAnchorPoint(Vec2(0,1));
    
    continerLayer->setPosition(Point(0,0));
    
    for(int i = 0;i<4;i++)
    {
        
        ScrollerItem*item = new ScrollerItem(tabIndex,(i+1));
        continerLayer->addChild(item);
        int y =  scrollH - ((i/col)>>0)*(item->getContentSize().height + interval) - itemH;
        item->setPosition(Vec2(7,y));
    }
    scroller->setSwallowTouches(false);
    scroller->addChild(continerLayer);
    scroller->setInnerContainerSize(Size(scroller->getContentSize().width,scrollH));
    scroller->setScrollBarEnabled(false);
    
    this->adapter();
    
    this->changeTab();
    
    this->calculAttr();
    
    if(CF()->isFirst)
    {
        Size visibleSize = Director::getInstance()->getVisibleSize();
        float scale = visibleSize.height/1334;
        auto help = Sprite::create("assets/helpbg1.png");
        help->setAnchorPoint(Vec2(0.5,0.5));
        help->setPosition(Vec2(visibleSize.width/2,visibleSize.height/2));
        this->addChild(help);
        help->setOpacity(0.0f);
        help->setScale(0.0f);
        auto delay = DelayTime::create(1.0f);
        auto delay2 = DelayTime::create(2.0f);
        auto fadein = FadeIn::create(0.5f);
        auto scaleto = ScaleTo::create(0.5f, scale);
        auto spwan = Spawn::createWithTwoActions(fadein, scaleto);
        auto callback = CallFunc::create([=](){
            Texture2D*texture = Director::getInstance()->getTextureCache()->addImage("assets/helpbg2.png");
            help->setTexture(texture);
        });
        auto callbak2 = CallFunc::create([=](){
            Texture2D*texture = Director::getInstance()->getTextureCache()->addImage("assets/helpbg3.png");
            help->setTexture(texture);
        });
        auto callback3 = CallFunc::create([=](){
            help->removeFromParent();
            CF()->isFirst = false;
        });
        help->runAction(Sequence::create(delay,spwan,delay2,callback,delay2,callbak2 ,delay2,callback3,NULL));
    }
    this->schedule(schedule_selector(GameLayer::secondAtk), 1.0f);
    return true;
}
void GameLayer::secondAtk(float dt)
{
    skillTime += 1;
    if(skillTime >= 10)
    {
        skillTime = 1;
        //释放技能
        std::vector<int> indexs;
        for(int i = 0;i<4;i++)
        {
            int num = CF()->tab3Cfg.at(i);
            if(num > 0)
            {
                indexs.push_back(i);
            }
        }
        if(indexs.size() > 0)
        {
            srand(int(time(0))+ rand());//置随机数种子
            int ran=CCRANDOM_0_1()*indexs.size();
            int ranindex = indexs.at(ran);
            createSkill(ranindex+1);
            int level = CF()->tab3Cfg.at(ranindex);
            int standDMGvalue = ranindex == 0?150:ranindex == 1?175:ranindex == 2?235:325;
            this->showDamage(level*standDMGvalue);
        }
    }
    int dpsatk = atoi(dpsLab->getString().c_str());
    auto delay = DelayTime::create(0.8f);
    auto callback = CallFunc::create([&](){
        general2Act->gotoFrameAndPlay(40,75,true);
        general3Act->gotoFrameAndPlay(40,75,true);
        general4Act->gotoFrameAndPlay(40,75,true);
        general5Act->gotoFrameAndPlay(40,75,true);
    });
    for(int i = 0;i<4;i++)
    {
        int num = CF()->tab2Cfg.at(i);
        if(num > 0)
        {
            if(i == 0)
            {
                general2Act->gotoFrameAndPlay(0,40,0);
                general2->runAction(Sequence::create(delay,callback, NULL));
            }else if(i == 1)
            {
                general3Act->gotoFrameAndPlay(0,40,0);
                general3->runAction(Sequence::create(delay,callback, NULL));
            }else if(i ==2)
            {
                general4Act->gotoFrameAndPlay(0,40,0);
                general4->runAction(Sequence::create(delay,callback, NULL));
            }else if(i == 3)
            {
                general5Act->gotoFrameAndPlay(0,40,0);
                general5->runAction(Sequence::create(delay,callback, NULL));
            }
        }
    }
    if(dpsatk > 0)
    {
        showDamage(dpsatk);
    }
}
void GameLayer::nextLevel()
{
    curLevel += 1;
    if(curLevel >= 11)
    {
        curLevel = 1;
        totalLevel += 1;
    }
    levelLab->setString(StringUtils::format("%d_",totalLevel)+StringUtils::format("%d",curLevel));
    Vec2 pos = enemyNode->getPosition();
    enemyNode->removeFromParent();
    enemyNode = NULL;
    curHp = totalHp = totalLevel*50 + curLevel*10;
    loadBar->setPercent(100.0f);
    int num = curLevel%4;
    if(num == 0){num = 1;}
    enemyNode = CSLoader::createNode(StringUtils::format("Enemy%d.csb",num));
    this->addChild(enemyNode);
    enemyNode->setPosition(pos);
    cocostudio::timeline::ActionTimeline *monact = CSLoader::createTimeline(StringUtils::format("Enemy%d.csb",num));
    enemyNode->runAction(monact);
    monact->gotoFrameAndPlay(0,35,true);
}
void GameLayer::changeTab()
{
    roleBtn->setBrightStyle(tabIndex == 1?BRIGHT_HIGHLIGHT:BRIGHT_NORMAL);
    partnerBtn->setBrightStyle(tabIndex == 2?BRIGHT_HIGHLIGHT:BRIGHT_NORMAL);
    skillBtn->setBrightStyle(tabIndex == 3?BRIGHT_HIGHLIGHT:BRIGHT_NORMAL);
    continerLayer->removeAllChildren();
    
    int col = 1;
    int interval = 10;
    int itemH = 137;
    float numchidren =4;
    float num = float((numchidren)/1.0f);
    int scrollH =  ceil(num)*(itemH + interval);
    int scrollHeight = scroller->getContentSize().height;
    if(scrollH <= scrollHeight)
    {
        scrollH = scrollHeight;
    }
    continerLayer->setContentSize(Size(scroller->getContentSize().width,scrollH));
    
    for(int i = 0;i<4;i++)
    {
        
        ScrollerItem*item = new ScrollerItem(tabIndex,(i+1));
        continerLayer->addChild(item);
        int y =  scrollH - ((i/col)>>0)*(item->getContentSize().height + interval) - itemH;
        item->setPosition(Vec2(7,y));
    }
}
void GameLayer::adapter()
{
    int bgW = 750;
    int bgH = 1334;
    Size visibleSize = Director::getInstance()->getVisibleSize();
    float scaleX = visibleSize.width/bgW;
    float scaleY = visibleSize.height/bgH;
    map->setScale(scaleX,scaleY);
    map->setPosition(Vec2(map->getPositionX()*scaleX,map->getPositionY()*scaleY));
    topGroup->setPosition(Vec2(topGroup->getPositionX()*scaleX,topGroup->getPositionY()*scaleY));
    bottomGroup->setScale(scaleX, scaleY);
    mainGeneral->setPosition(Vec2(mainGeneral->getPositionX()*scaleX,mainGeneral->getPositionY()*scaleY));
    enemyNode->setPosition(Vec2(enemyNode->getPositionX()*scaleX,enemyNode->getPositionY()*scaleY));
    general2->setPosition(Vec2(general2->getPositionX()*scaleX,general2->getPositionY()*scaleY));
    general3->setPosition(Vec2(general3->getPositionX()*scaleX,general3->getPositionY()*scaleY));
    general4->setPosition(Vec2(general4->getPositionX()*scaleX,general4->getPositionY()*scaleY));
    general5->setPosition(Vec2(general5->getPositionX()*scaleX,general5->getPositionY()*scaleY));
}
void GameLayer::calculAttr()
{
    int atkValue = 0;
    int spd = 0;
    for(int i = 0;i<4;i++)
    {
        
        int value1 = i == 0?5:i == 1?10:i == 2?15:20;
        atkValue += (CF()->tab1Cfg.at(i))*value1;
        
        spd += (CF()->tab2Cfg.at(i))*5;
    }
    dmgLab->setString(StringUtils::format("%d",atkValue));
    dpsLab->setString(StringUtils::format("%d",spd));
    goldLab->setString(StringUtils::format("%d",CF()->gold));
    for(int i = 0;i<4;i++)
    {
        int num = CF()->tab2Cfg.at(i);
        if(num > 0)
        {
            if(!general2->isVisible() && i==0)
            {
                general2->setVisible(true);
            }
           if(!general3->isVisible() && i==1)
           {
               general3->setVisible(true);
           }
            if(!general4->isVisible() && i==2)
            {
                general4->setVisible(true);
            }
            if(!general5->isVisible() && i==3)
            {
                general5->setVisible(true);
            }
            
        }
    }
}
bool GameLayer::menuBegin(cocos2d::Touch* tTouch,cocos2d::Event* eEvent)
{
    Point p = this->convertToNodeSpace(tTouch->getLocation());
    if(bottomGroup->getBoundingBox().containsPoint(p) && !CF()->isFirst)
    {
        Point bottomP = bottomGroup->convertToNodeSpace(tTouch->getLocation());
        if(roleBtn->getBoundingBox().containsPoint(bottomP))
        {
            return true;
        }else if(partnerBtn->getBoundingBox().containsPoint(bottomP))
        {
            return true;
        }else if(skillBtn->getBoundingBox().containsPoint(bottomP))
        {
            return true;
        }
        Point scrollP = continerLayer->convertToNodeSpace(tTouch->getLocation());
        Vector<Node*> items = continerLayer->getChildren();
        for(auto c : items)
        {
            Point localP = c->convertToNodeSpace(tTouch->getLocation());
            Layout*button = static_cast<ScrollerItem*>(c)->upgradeBtn;
            CheckBox*rise10 = static_cast<ScrollerItem*>(c)->rise10;
            CheckBox*rise100 = static_cast<ScrollerItem*>(c)->rise100;
            if(button->getBoundingBox().containsPoint(localP))
            {
                button->setScale(0.9f);
                return true;
            }else if(rise10->getBoundingBox().containsPoint(localP))
            {
                return true;
            }else if(rise100->getBoundingBox().containsPoint(localP))
            {
                return true;
            }
        }
    }else if(returnBtn->getBoundingBox().containsPoint(p) && !CF()->isFirst)
    {
        return true;
    }else if(map->getBoundingBox().containsPoint(p) && !CF()->isFirst)
    {
        return true;
    }
    return false;
}
void GameLayer::menuEndCallback(cocos2d::Touch* tTouch,cocos2d::Event* eEvent)
{
    Point p = this->convertToNodeSpace(tTouch->getLocation());
   if(bottomGroup->getBoundingBox().containsPoint(p) && !CF()->isFirst)
   {
       Point bottomP = bottomGroup->convertToNodeSpace(tTouch->getLocation());
       if(roleBtn->getBoundingBox().containsPoint(bottomP))
       {
           tabIndex = 1;
           changeTab();
           return;
       }else if(partnerBtn->getBoundingBox().containsPoint(bottomP))
       {
           tabIndex = 2;
           changeTab();
           return;
       }else if(skillBtn->getBoundingBox().containsPoint(bottomP))
       {
           tabIndex = 3;
           changeTab();
           return;
       }
       Point scrollP = continerLayer->convertToNodeSpace(tTouch->getLocation());
       Vector<Node*> items = continerLayer->getChildren();
       int selectIndex = -1;
       for(auto c : items)
       {
           selectIndex += 1;
           Point localP = c->convertToNodeSpace(tTouch->getLocation());
           Layout*button = static_cast<ScrollerItem*>(c)->upgradeBtn;
           CheckBox*rise10 = static_cast<ScrollerItem*>(c)->rise10;
           CheckBox*rise100 = static_cast<ScrollerItem*>(c)->rise100;
           if(button->getBoundingBox().containsPoint(localP))
           {
               int cost = static_cast<ScrollerItem*>(c)->upgradeCost;
               button->setScale(1.0f);
               if(CF()->gold < cost)
               {
                   CommonUtil::floatFont("Not enough gold", this);
                   return;
               }
               CF()->gold -= cost;
               goldLab->setString(StringUtils::format("%d",CF()->gold));
               if(tabIndex == 1){CF()->tab1Cfg.at(selectIndex) += 1*static_cast<ScrollerItem*>(c)->multiple;}
               else if(tabIndex == 2){CF()->tab2Cfg.at(selectIndex) += 1*static_cast<ScrollerItem*>(c)->multiple;}
               else if(tabIndex == 3){CF()->tab3Cfg.at(selectIndex) += 1*static_cast<ScrollerItem*>(c)->multiple;}
               static_cast<ScrollerItem*>(c)->refreshItem();
               this->calculAttr();
               break;
           }else if(rise10->getBoundingBox().containsPoint(localP))
           {
               if(rise10->isSelected())
               {
                   rise10->setSelected(false);
                   static_cast<ScrollerItem*>(c)->multiple = 1;
               }else{
                   rise100->setSelected(false);
                   rise10->setSelected(true);
                   static_cast<ScrollerItem*>(c)->multiple = 10;
               }
               static_cast<ScrollerItem*>(c)->refreshItem();
               
           }else if(rise100->getBoundingBox().containsPoint(localP))
           {
               if(rise100->isSelected())
               {
                   rise100->setSelected(false);
                   static_cast<ScrollerItem*>(c)->multiple = 1;
               }else{
                   rise100->setSelected(true);
                   rise10->setSelected(false);
                   static_cast<ScrollerItem*>(c)->multiple = 100;
               }
               static_cast<ScrollerItem*>(c)->refreshItem();
           }
       }
   }else if(returnBtn->getBoundingBox().containsPoint(p) && !CF()->isFirst)
   {
       auto scene = HelloWorld::createScene();
       Director::getInstance()->replaceScene(scene);
   }else if(map->getBoundingBox().containsPoint(p) && !CF()->isFirst)
   {
       if(!mainAtk)
       {
           mainAtk = true;
           mainAct->gotoFrameAndPlay(0,40,0);
           createSkill(-1);
           showDamage(atoi(dmgLab->getString().c_str()));
           auto delay = DelayTime::create(0.5f);
           auto callback = CallFunc::create([&](){
               mainAtk = false;
               mainAct->gotoFrameAndPlay(40,75,true);
           });
           mainGeneral->runAction(Sequence::create(delay,callback, NULL));
       }
   }
}
void GameLayer::createSkill(int skillIndex)
{
    int endTime = skillIndex == -1?20:skillIndex == 1?65:skillIndex == 2?55:skillIndex == 3?50:70;
    std::string skillass = skillIndex == -1?"NormalSkill.csb":StringUtils::format("Skill%d.csb",skillIndex);
    auto skillNode = CSLoader::createNode(skillass);
    this->addChild(skillNode,9999);
    cocostudio::timeline::ActionTimeline *skillact = CSLoader::createTimeline(skillass);
    skillNode->runAction(skillact);
    skillNode->setPosition(enemyNode->getPosition());
    skillact->gotoFrameAndPlay(0,endTime,0);
    auto delay = DelayTime::create(1.0f);
    auto callback = CallFunc::create([=](){
        skillNode->removeFromParent();
    });
    skillNode->runAction(Sequence::create(delay,callback, NULL));
}
void GameLayer::showDamage(int dmg)
{
    auto myLabel = Label::createWithBMFont("assets/myFont.fnt", StringUtils::format("-%d",dmg));
    this->addChild(myLabel);
    myLabel->setScale(0.6f);
    myLabel->setPosition(enemyNode->getPosition());
    curHp -= dmg;
    if(curHp <= 0)
    {
        curHp = 0;
        for(int i = 0;i<=5;i++)
        {
            auto delay = DelayTime::create(i*0.2f);
            Sprite*goldsp = Sprite::create("assets/goldicon.png");
            this->addChild(goldsp);
            goldsp->setPosition(enemyNode->getPosition());
            float x = bottomGroup->getBoundingBox().size.width - 100;
            float y = bottomGroup->getBoundingBox().size.height;
            auto moveto = MoveTo::create(1.0f, Vec2(x,y));
            auto fadeout = FadeOut::create(1.0f);
            auto spawn = Spawn::createWithTwoActions(moveto, fadeout);
            auto callback = CallFunc::create([=](){
                goldsp->removeFromParent();
            });
            CF()->gold += 10;
            goldsp->runAction(Sequence::create(delay,spawn,callback, NULL));
        }
        goldLab->setString(StringUtils::format("%d",CF()->gold));
        this->nextLevel();
    }
    float percent = float(curHp)/float(totalHp)*100.0f;
    loadBar->setPercent(percent);
    
    auto move = MoveTo::create(1.0f, Vec2(myLabel->getPositionX(), myLabel->getPositionY() + 100));
    auto fadeout = FadeOut::create(1.0f);
    
    auto spwan = Spawn::createWithTwoActions(move, fadeout);
    auto callback = CallFunc::create([=](){
        myLabel->removeFromParent();
    });
    myLabel->runAction(Sequence::create(spwan,callback,NULL));
}
