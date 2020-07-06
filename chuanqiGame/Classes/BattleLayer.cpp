//
//  BattleLayer.cpp
//  chuanqiGame-mobile
//
//  Created by 志强 on 2020/6/11.
//

#include "BattleLayer.hpp"
#include "Config.h"
#include "ModelEntity.hpp"
#include "ResultLayer.hpp"
Scene* BattleLayer::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = BattleLayer::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool BattleLayer::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
    
    auto rootNode = CSLoader::createNode("BattleLayer.csb");

    addChild(rootNode);
    
    bg = static_cast<Sprite*>(rootNode->getChildByName("bg"));
    gameCon = static_cast<Layout*>(rootNode->getChildByName("gameCon"));
    Node*node = static_cast<Node*>(gameCon->getChildByName("roleNode"));
    leftArrow = static_cast<Sprite*>(rootNode->getChildByName("leftArrow"));
    rightArrow = static_cast<Sprite*>(rootNode->getChildByName("rightArrow"));
    killLab = static_cast<TextBMFont*>(rootNode->getChildByName("killLab"));
    blood = static_cast<Sprite*>(rootNode->getChildByName("blood"));
    blood->setVisible(false);
    
    visibleSize = Director::getInstance()->getVisibleSize();
    node->setVisible(false);
    std::string roleass = CF()->sex == 1?"manNode.csb":"womanNode.csb";
    roleNode = CSLoader::createNode(roleass);
    gameCon->addChild(roleNode);
    roleNode->setPosition(node->getPosition());
    killLab->setString(StringUtils::format("Kill:0/%d",CF()->c_chapter*20));
    
    roleAction = CSLoader::createTimeline(roleass);
    roleNode->runAction(roleAction);
    roleAction->gotoFrameAndPlay(0,15,true);
    
    this->adapter();
    roleTitle = new RoleTitleLayer();
    this->addChild(roleTitle);
    roleTitle->setPosition(Vec2(0,visibleSize.height - 140));
    
    _listener=EventListenerTouchOneByOne::create();
    _listener->setSwallowTouches(true);
    _listener->onTouchBegan = CC_CALLBACK_2(BattleLayer::menuBegin,this);
    _listener->onTouchEnded = CC_CALLBACK_2(BattleLayer::menuEndCallback, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(_listener, this);
    
    this->schedule(schedule_selector(BattleLayer::createEneity), 1.0f);
    this->schedule(schedule_selector(BattleLayer::createSkillIcon), 5.0f);
    return true;
}
void BattleLayer::createSkillIcon(float dt)
{
    std::vector<int> lockIndex;
    if(CF()->skillCfgs.size())
    {
        for(auto c : CF()->skillCfgs)
        {
            if(!c->isUnlock)
            {
                lockIndex.push_back(c->skillIndex);
            }
        }
    }else{
        lockIndex.push_back(1);
    }
    
    srand(int(time(0))+ rand());//置随机数种子
    int skillIndex=CCRANDOM_0_1()*lockIndex.size();
    std::string skillass = StringUtils::format("battle/battle_skill_%d.png",lockIndex.at(skillIndex));
    Sprite*skillIcon = Sprite::create(skillass);
    gameCon->addChild(skillIcon);
    skillIcon->setName("skill");
    skillIcon->setTag(lockIndex.at(skillIndex));
    float x = CCRANDOM_0_1()*(gameCon->getContentSize().width - 200) + 100;
    float y = gameCon->getContentSize().height + 100;
    skillIcon->setPosition(Vec2(x,y));
    
    float my = -100;
    auto moveto = MoveTo::create(5.0f, Vec2(x,my));
    auto callback = CallFunc::create([=](){
        skillIcon->removeFromParent();
    });
    skillIcon->runAction(Sequence::create(moveto,callback, NULL));
}
void BattleLayer::createEneity(float dt)
{
    srand(int(time(0))+ rand());//置随机数种子
    int monindex = CF()->c_chapter >= 4?4:CF()->c_chapter;
    int type=CCRANDOM_0_1()*monindex + 1;
    ModelEntity*entity = new ModelEntity(type);
    entity->setScale(0.7f);
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
        entity->execAtkAct();
        entity->atkState = true;
    });
    entity->runAction(Sequence::create(moveto,callback, NULL));
    Vector<Node*> childrens = gameCon->getChildren();
    for(auto c : childrens)
    {
        if(c->getName() == "mon" && static_cast<ModelEntity*>(c)->atkState){
            static_cast<ModelEntity*>(c)->atkState = false;
            static_cast<ModelEntity*>(c)->execAtkAct();
            auto delay = DelayTime::create(0.5f);
            CF()->curHp -= CF()->c_chapter*5;
            roleTitle->refreshInfo();
            if(CF()->curHp <= 0)
            {
                CF()->curHp = 0;
                roleTitle->refreshInfo();
                CF()->curHp = CF()->totalHp;
                //游戏结束
                this->gameEnd();
                ResultLayer*result = new ResultLayer(0);
                this->addChild(result);
                return;
            }
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
void BattleLayer::judgeAtk(bool skillAtk)
{
    Vector<Node*> childrens = gameCon->getChildren();
    bool atkboo = false;
    
    for(auto c : childrens)
    {
        if(c->getName() == "mon")
        {
            int dic = roleNode->getScaleX() > 0?1:-1;
            float distance = sqrt(pow(c->getPositionX()-roleNode->getPositionX(),2)+pow(c->getPositionY()-roleNode->getPositionY(),2));
            if(((dic > 0 && distance <= 180 && c->getPositionX() > roleNode->getPositionX()) || (dic < 0 && distance <= 200 && c->getPositionX() < roleNode->getPositionX())) || skillAtk)
            {
                atkboo = true;
                ModelEntity*item = static_cast<ModelEntity*>(c);
                int dmg = 50*CF()->t_chapter;
                if(skillAtk)
                {
                    dmg = releaseIndex*100;
                }
                item->reduceHp(dmg);
                if(item->curHp <= 0)
                {
                    auto delay = DelayTime::create(0.3f);
                    killnum += 1;
                    std::string killstr = StringUtils::format("Kill:%d/",killnum) + StringUtils::format("%d",CF()->c_chapter*20);
                    killLab->setString(killstr);
                    if(killnum >= CF()->c_chapter*20)
                    {
                        this->gameEnd();
                        if(CF()->c_chapter == CF()->t_chapter)
                        {
                            CF()->t_chapter += 1;
                            CF()->totalHp += 200;
                        }
                        CF()->curHp = CF()->totalHp;
                        ResultLayer*result = new ResultLayer(1);
                        this->addChild(result);
                        return;
                    }
                    auto callback = CallFunc::create([=](){
                        float halfW = Director::getInstance()->getVisibleSize().width/2;
                        float x = c->getPositionX() > halfW?c->getPositionX() + 100:c->getPositionX() - 100;
                        auto jumpto = JumpTo::create(1.0f, Vec2(x, -300), 400, 1);
                        auto jumpcall = CallFunc::create([=](){
                            c->stopAllActions();
                            c->removeFromParent();
                        });
                        c->runAction(Sequence::create(jumpto,jumpcall, NULL));
                    });
                    c->runAction(Sequence::create(delay,callback, NULL));
                }
                
            }
        }
    }
    if(skillAtk){return;}
    if(atkboo)
    {
        
        auto delay = DelayTime::create(0.3f);
        auto callback2 = CallFunc::create([&](){
            rolePlay = false;
            roleAction->gotoFrameAndPlay(0,15,true);
        });
        gameCon->runAction(Sequence::create(delay,callback2, NULL));
    }else{
        shakeNode(this);
        
        auto delay = DelayTime::create(1.0f);
        auto callback = CallFunc::create([&](){
             roleAction->gotoFrameAndPlay(0,15,true);
             rolePlay = false;
        });
        gameCon->runAction(Sequence::create(delay,callback, NULL));
    }
}
void BattleLayer::gameEnd()
{
    this->unschedule(schedule_selector(BattleLayer::createEneity));
    this->unschedule(schedule_selector(BattleLayer::createSkillIcon));
    gameState = true;
}
void BattleLayer::shakeNode(cocos2d::Node *node)
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
void BattleLayer::adapter()
{
    int bgW = 750;
    int bgH = 1334;
    Size visibleSize = Director::getInstance()->getVisibleSize();
    float scaleX = visibleSize.width/bgW;
    float scaleY = visibleSize.height/bgH;
    bg->setScale(scaleX);
    gameCon->setContentSize(Size(visibleSize.width,visibleSize.height));
    gameCon->setPosition(Vec2(0,0));
    bg->setPosition(Vec2(bg->getPositionX()*scaleX,bg->getPositionY()*scaleY));
    roleNode->setPosition(Vec2(roleNode->getPositionX()*scaleX,roleNode->getPositionY()*scaleY));
    leftArrow->setPosition(Vec2(leftArrow->getPositionX()*scaleX,leftArrow->getPositionY()*scaleY));
    rightArrow->setPosition(Vec2(rightArrow->getPositionX()*scaleX,rightArrow->getPositionY()*scaleY));
    killLab->setPosition(Vec2(killLab->getPositionX()*scaleX,killLab->getPositionY()*scaleY));
}
bool BattleLayer::menuBegin(cocos2d::Touch* tTouch,cocos2d::Event* eEvent)
{
    Point p = this->convertToNodeSpace(tTouch->getLocation());
    if(leftArrow->getBoundingBox().containsPoint(p) && !rolePlay && !gameState)
    {
        leftArrow->setScale(0.9f);
        return true;
    }else if(rightArrow->getBoundingBox().containsPoint(p) && !rolePlay && !gameState)
    {
        rightArrow->setScale(0.9f);
        return true;
    }
    Vector<Node*> childrens = gameCon->getChildren();
    for(auto c : childrens)
    {
        if(c->getName() == "skill" && c->getBoundingBox().containsPoint(p))
        {
           
            return true;
        }
    }
    return false;
}
void BattleLayer::menuEndCallback(cocos2d::Touch* tTouch,cocos2d::Event* eEvent)
{
    Point p = this->convertToNodeSpace(tTouch->getLocation());
    if(leftArrow->getBoundingBox().containsPoint(p) && !rolePlay && !gameState)
    {
        leftArrow->setScale(1.0f);
        roleNode->setScaleX(-1.0f);
        rolePlay = true;
        roleAction->gotoFrameAndPlay(20,45,0);
        judgeAtk(false);
        return;
    }else if(rightArrow->getBoundingBox().containsPoint(p) && !rolePlay && !gameState)
    {
        rightArrow->setScale(1.0f);
        roleNode->setScaleX(1.0f);
        rolePlay = true;
        roleAction->gotoFrameAndPlay(20,45,0);
        judgeAtk(false);
        return;
    }
    Vector<Node*> childrens = gameCon->getChildren();
    for(auto c : childrens)
    {
        if(c->getName() == "skill" && c->getBoundingBox().containsPoint(p))
        {
            
            int skillIndex = c->getTag();
            releaseIndex = skillIndex;
            std::string skilleff = StringUtils::format("Skill%d.csb",skillIndex);
            for(int i = 0;i <= 10;i++)
            {
                srand(int(time(0))+ rand());//置随机数种子
                int dic=CCRANDOM_0_1()*100 > 50?1:-1;
                float x = CCRANDOM_0_1()*(visibleSize.width - 200) + 100;
                float y = roleNode->getPositionY() + CCRANDOM_0_1()*100*dic;
                Node*skillNode = CSLoader::createNode(skilleff);
                gameCon->addChild(skillNode);
                skillNode->setPosition(Vec2(x,y));
                cocostudio::timeline::ActionTimeline *skillAction = CSLoader::createTimeline(skilleff);
                skillNode->runAction(skillAction);
                int endnum = skillIndex == 1?64:skillIndex == 2?45:skillIndex == 3?80:skillIndex == 4?55:95;
                skillAction->gotoFrameAndPlay(0,endnum,0);
                auto callback = CallFunc::create([=](){
                    skillNode->stopAllActions();
                    skillNode->removeFromParent();
                });
                auto delay = DelayTime::create(0.5f);
                skillNode->runAction(Sequence::create(delay,callback, NULL));
            }
            c->removeFromParent();
            judgeAtk(true);
            break;
        }
    }
}
