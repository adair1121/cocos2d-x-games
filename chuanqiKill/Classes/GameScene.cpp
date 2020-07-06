//
//  GameScene.cpp
//  ChuanqiKill-mobile
//
//  Created by 志强 on 2020/5/16.
//

#include "GameScene.h"
#include "ResultScene.h"
#include "Date.h"
#include "SelectLevel.hpp"
#include "SoundCtl.h"
#include "CommonUtil.h"
#include  "iostream"
#include "MovieClip.h"
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
    
    leftGroup = static_cast<Layout*>(rootNode->getChildByName("leftGroup"));
    timeBar = static_cast<LoadingBar*>(leftGroup->getChildByName("timeBar"));
    scoreBar = static_cast<LoadingBar*>(leftGroup->getChildByName("scoreBar"));
    scoreLab = static_cast<TextBMFont*>(leftGroup->getChildByName("scoreLab"));
    levelLab = static_cast<TextBMFont*>(leftGroup->getChildByName("levelLab"));
    goldLab = static_cast<TextBMFont*>(leftGroup->getChildByName("goldLab"));
     
    gamebg = static_cast<Sprite*>(rootNode->getChildByName("gamebg"));
    menuBtn = static_cast<Sprite*>(rootNode->getChildByName("menuBtn"));
    container = static_cast<Layout*>(rootNode->getChildByName("container"));
    boxContainer = static_cast<Layout*>(rootNode->getChildByName("boxContainer"));
    effCon = static_cast<Layout*>(rootNode->getChildByName("effCon"));
    bg = static_cast<Sprite*>(rootNode->getChildByName("bg"));
    
    skillGroup = static_cast<Layout*>(rootNode->getChildByName("skillGroup"));
    num1Lab = static_cast<TextBMFont*>(skillGroup->getChildByName("num1Lab"));
    num2Lab = static_cast<TextBMFont*>(skillGroup->getChildByName("num2Lab"));
    num3Lab = static_cast<TextBMFont*>(skillGroup->getChildByName("num3Lab"));
    skillGroup1 = static_cast<Layout*>(skillGroup->getChildByName("skillGroup1"));
    skillGroup2 = static_cast<Layout*>(skillGroup->getChildByName("skillGroup2"));
    skillGroup3 = static_cast<Layout*>(skillGroup->getChildByName("skillGroup3"));
    
    num1Lab->setString(StringUtils::format("%d",DT()->skill1num));
    num2Lab->setString(StringUtils::format("%d",DT()->skill2num));
    num3Lab->setString(StringUtils::format("%d",DT()->skill3num));
     
    _listener=EventListenerTouchOneByOne::create();
    _listener->setSwallowTouches(true);
    _listener->onTouchBegan = CC_CALLBACK_2(GameScene::menuBegin,this);
    _listener->onTouchEnded = CC_CALLBACK_2(GameScene::menuEndCallback, this);
    _listener->onTouchMoved = CC_CALLBACK_2(GameScene::menuMove, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(_listener, this);
    
    this->resetData();
    this->adapter();
    return true;
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
void GameScene::adapter()
{
    int bgW = bg->getContentSize().width;
    int bgH = bg->getContentSize().height;
    Size visibleSize = Director::getInstance()->getVisibleSize();
    float scaleX = visibleSize.width/bgW;
    float scaleY = visibleSize.height/bgH;
    bg->setScale(scaleX, scaleY);
    container->setScale(scaleX);
    container->setPosition(Vec2(container->getPositionX()*scaleX,container->getPositionY()*scaleY));
    boxContainer->setScale(scaleX);
    boxContainer->setPosition(Vec2(boxContainer->getPositionX()*scaleX,boxContainer->getPositionY()*scaleY));
    gamebg->setScale(scaleX);
    gamebg->setPosition(Vec2(gamebg->getPositionX()*scaleX,gamebg->getPositionY()*scaleY));
    effCon->setScale(scaleX);
    effCon->setPosition(Vec2(effCon->getPositionX()*scaleX,effCon->getPositionY()*scaleY));
    skillGroup->setScale(scaleX);
    skillGroup->setPosition(Vec2(skillGroup->getPositionX()*scaleX,skillGroup->getPositionY()*scaleY));
    
//    timeBar->setPosition(Vec2(timeBar->getPositionX()*scaleX,timeBar->getPositionY()*scaleY));
//    float timeBatScaleX = 0.93/1334*visibleSize.width;
//    timeBar->setScaleY(timeBatScaleX);
//    levelLab->setPosition(Vec2(levelLab->getPositionX()*scaleX,levelLab->getPositionY()*scaleY));
   
//    scoreBar->setPositionX(scoreBar->getPositionX()*scaleX);
    leftGroup->setScale(scaleX);
    leftGroup->setPosition(Vec2(leftGroup->getPositionX()*scaleX,leftGroup->getPositionY()*scaleY));
    menuBtn->setPosition(Vec2(menuBtn->getPositionX()*scaleX,menuBtn->getPositionY()*scaleY));
}
void GameScene::resetData()
{
    timeBar->setPercent(100);
    scoreBar->setPercent(0);
    scoreLab->setString("0");
    levelLab->setString(StringUtils::format("%d",DT()->curLevel));
    goldLab->setString(StringUtils::format("%d",DT()->gold));
    //生成随机的item组显示
    container->removeAllChildren();
    linePoints.clear();
    count = 0;
    totalCount = 60;
    scoreNum = 0;
    int col = 8;//列
    int row = 6;//行
   
    for(int i = 0;i<row;i++)
    {
        for(int j = 0;j<col;j++)
        {
            this->createItem(i,j);
        }
    }
    this->schedule(schedule_selector(GameScene::updateTime), 1.0f);
}
Sprite* GameScene::createItem(int i,int j)
{
    int intervalY = 27;
    int intervalX = 28;
    srand(int(time(0))+ rand());//置随机数种子
    int ran=CCRANDOM_0_1()*10 + 1;
    int tag = 0;
    if(ran == 1 || ran == 6){tag = 1;}
    else if(ran == 2 || ran == 7){tag = 2;}
    else if(ran == 3 || ran == 8){tag = 3;}
    else if(ran == 4 || ran == 9){tag = 4;}
    else if(ran == 5 || ran == 10){tag = 5;}
    Sprite*item = Sprite::create(StringUtils::format("assets/item%d.png",ran));
    item->setTag(tag);
    //x_y
    std::string nameinfostr = StringUtils::format("%d_",j)+StringUtils::format("%d",i);
    item->setName(nameinfostr);
    container->addChild(item);
//    item->setScale(0.45f);
    int x = j* (item->getContentSize().width + intervalX) + 40;
    int y = container->getContentSize().height - i* (item->getContentSize().height + intervalY) -45 ;
    
    Sprite*boxbg = Sprite::create("assets/itembox.png");
    boxContainer->addChild(boxbg);
    boxbg->setPosition(Vec2(x,y));
    x += 10;
    y -= 10;
    item->setPosition(Vec2(x,y));
    
    
    return item;
}
void GameScene::lineConnectExec(Vec2 posVec)
{
    //执行连线逻辑判断
    Sprite*focusItem = NULL;
    Vector<Node*> childrens = container->getChildren();
    for(auto c : childrens)
    {
       if(c->getBoundingBox().containsPoint(posVec))
       {
           focusItem = static_cast<Sprite*>(c);
           break;
       }
    }
    if(focusItem)
    {
        //当前有焦点。需要判断焦点是否是同类型的
        for(auto c : linePoints)
        {
            if(c->getName() == focusItem->getName())
            {
                //说明已经在当前连线中了
                return;
            }
        }
        //此时 说明不在连线中 需要判断是否为同一个类型
        Sprite*lastItem = linePoints.at(linePoints.size()- 1);
        int tag1 = focusItem->getTag();
        int tag2 = lastItem->getTag();
        if(tag1 == tag2)
        {
            //说明当前是一个类型的item 此时需要判断是否在最后一个连线位置的9宫格内
            std::string posInfoStr = lastItem->getName();
            std::vector<std::string> posInfo = CommonUtil::split(posInfoStr, "_");
            int x = atoi(posInfo.at(0).c_str());
            int y = atoi(posInfo.at(1).c_str());
            //获取九宫格边界坐标信息
            int topX = (x - 1) < 0?x:(x - 1);
            int topY = (y - 1) < 0?y:(y - 1);
            int bottomX = (x + 1) > 7?x:(x+1);
            int bottomY = (y + 1) > 5?y:(y+1);
            for(int i = topX;i<=bottomX;i++)
            {
                for(int j = topY;j<=bottomY;j++)
                {
                    std::string key = StringUtils::format("%d_",i)+StringUtils::format("%d",j);
                    if(strcmp(focusItem->getName().c_str(), key.c_str()) == 0)
                    {
                        //当前是在九宫格范围内;
                        linePoints.pushBack(focusItem);
                        focusItem->setScale(1.5f);
                        break;
                    }
                }
            }
        }
    }
}
bool GameScene::menuBegin(cocos2d::Touch* tTouch,cocos2d::Event* eEvent)
{
    Point localP = this->convertToNodeSpace(tTouch->getLocation());
    
    if(menuBtn->getBoundingBox().containsPoint(localP) && !battleEnd){
       menuBtn->setScale(0.9f);
       return true;
    }else if(skillGroup->getBoundingBox().containsPoint(localP) && !battleEnd)
    {
        Point skillP = skillGroup->convertToNodeSpace(tTouch->getLocation());
        if(skillGroup1->getBoundingBox().containsPoint(skillP) && !battleEnd)
        {
            return true;
        }else if(skillGroup2->getBoundingBox().containsPoint(skillP) && !battleEnd)
        {
            return true;
        }else if(skillGroup3->getBoundingBox().containsPoint(skillP) && !battleEnd)
        {
            return true;
        }
    }
    Vector<Node*> childrens = container->getChildren();
    Point groupP = container->convertToNodeSpace(tTouch->getLocation());
    for(auto c : childrens)
    {
        if(c->getBoundingBox().containsPoint(groupP) && !battleEnd)
        {
            linePoints.pushBack(static_cast<Sprite*>(c));
            c->setScale(1.5f);
            return true;
        }
    }
    return false;
}
void GameScene::touchSkill(int skillIndex)
{
    if(skillIndex == 1)
    {
        if(DT()->skill1num <= 0)
        {
            this->floatFont("Lack of skills");
        }else{
            DT()->skill1num -= 1;
            num1Lab->setString(StringUtils::format("%d",DT()->skill1num));
            
            srand(int(time(0))+ rand());//置随机数种子
            Vector<Node*> childrens = container->getChildren();
            int index = CCRANDOM_0_1()*childrens.size();

            int randomType=childrens.at(index)->getTag();

            int changeType = CCRANDOM_0_1()*5+1;
           
            for(auto c : childrens)
            {
                Sprite*item = static_cast<Sprite*>(c);
                if(item->getTag() == randomType)
                {
                    item->setTag(changeType);
                    Texture2D*texture = Director::getInstance()->getTextureCache()->addImage(StringUtils::format("assets/item%d.png",changeType));
                    item->setTexture(texture);
                    MovieClip*mc = new MovieClip();
                    mc->playFile("change.plist", "frame", 1, 6, 1, true);
                    effCon->addChild(mc);
                    mc->setPosition(Vec2(item->getPositionX(),item->getPositionY()));
                }
            }
        }
    }else if(skillIndex == 2)
    {
        if(DT()->skill2num <= 0)
        {
            this->floatFont("Lack of skills");
        }else{
            DT()->skill2num -= 1;
            num2Lab->setString(StringUtils::format("%d",DT()->skill2num));
            srand(int(time(0))+ rand());//置随机数种子
            Vector<Node*> childrens = container->getChildren();
            int index = CCRANDOM_0_1()*childrens.size();
            int randomType=childrens.at(index)->getTag();
            for(auto c : childrens)
            {
                Sprite*item = static_cast<Sprite*>(c);
                if(item->getTag() == randomType)
                {
                    linePoints.pushBack(item);
                }
            }
            execEliminate();
        }
    }else if(skillIndex == 3)
    {
        if(DT()->skill3num <= 0)
        {
            this->floatFont("Lack of skills");
        }else{
            auto scene = GameScene::createScene();
            
            Director::getInstance()->replaceScene(TransitionProgressRadialCW::create(1, scene));
        }
    }
}
void GameScene::menuEndCallback(cocos2d::Touch* tTouch,cocos2d::Event* eEvent)
{
    Point localP = this->convertToNodeSpace(tTouch->getLocation());
    Point skillP = skillGroup->convertToNodeSpace(tTouch->getLocation());
    if(menuBtn->getBoundingBox().containsPoint(localP) && !battleEnd){
        menuBtn->setScale(1.0f);
        SoundCtl::getInstance()->playEffect("sound/click.m4a");
        auto scene = SelectLevel::createScene();
        
        Director::getInstance()->replaceScene(TransitionProgressRadialCW::create(1, scene));
        return;
    }else if(skillGroup->getBoundingBox().containsPoint(localP) && !battleEnd)
    {
        Point skillP = skillGroup->convertToNodeSpace(tTouch->getLocation());
        if(skillGroup1->getBoundingBox().containsPoint(skillP) && !battleEnd)
        {
            this->touchSkill(1);
        }else if(skillGroup2->getBoundingBox().containsPoint(skillP) && !battleEnd)
        {
            this->touchSkill(2);
        }else if(skillGroup3->getBoundingBox().containsPoint(skillP) && !battleEnd)
        {
            this->touchSkill(3);
        }
        return;
    }
    this->execEliminate();
    
}
void GameScene::execEliminate()
{
    if(linePoints.size() > 0)
    {
        if(linePoints.size() >= 3)
        {
            //执行消除操作
            for(auto c : linePoints)
            {
                MovieClip*mc = new MovieClip();
                mc->setScale(0.8f);
                mc->frameRate = 0.03f;
                mc->playFile("atk.plist", "atk", 1, 16, 1, true);
                effCon->addChild(mc);
                mc->setPosition(Vec2(c->getPositionX(),c->getPositionY()));
                c->removeFromParent();
            }
            //判断是否有元素下方为空缺 需要执行移动 即y 上面的元素都需要像下移动
            //然后最上方需要生成新的物品 填补空缺
            int len = linePoints.size();
            for(int i = 0;i<len;i++)
            {
                std::string posInfoStr = linePoints.at(i)->getName();
                std::vector<std::string> posInfo = CommonUtil::split(posInfoStr, "_");
                int x = atoi(posInfo.at(0).c_str());
                int y = atoi(posInfo.at(1).c_str());
                //根据x 定位到对应消除的y轴元素
                Vector<Node*> childrens = container->getChildren();
                Vector<Sprite*> operSps;
                for(auto c : childrens)
                {
                    Sprite*item = static_cast<Sprite*>(c);
                    std::string infostr = item->getName();
                    std::vector<std::string> info = CommonUtil::split(infostr, "_");
                    int cx = atoi(info.at(0).c_str());
                    if(cx == x)
                    {
                        //定为到当前消除后的 剩余y轴所有的元素
                        operSps.pushBack(item);
                    }
                }
//                childrens.clear();
                //下面为执行移动 y值最高者先移动 即最下面的先移动 修改数据空出格子位置
                
                int startIndex = operSps.size() - 1;
                int topJ = 1;
                for(int o = startIndex ;o>=0;o--)
                {
                    //倒叙遍历。从底部开始移动
                    std::string infostr = operSps.at(o)->getName();
                    std::vector<std::string> info = CommonUtil::split(infostr, "_");
                    for(int j = 5;j>=0;j--)
                    {
                        int prevY = atoi(info.at(1).c_str());
                        if(j > prevY)
                        {
                            std::string namestr = info.at(0) + "_" + StringUtils::format("%d",j);
                            Sprite*item = static_cast<Sprite*>(container->getChildByName(namestr));
                            if(!item)
                            {
                                topJ = j;
                                //此时这个itemn不存在 需要像这里移动
                                int mx = operSps.at(o)->getPositionX();
                                operSps.at(o)->setName(namestr);
                                int my = container->getContentSize().height - j* (operSps.at(o)->getContentSize().height + 27) -55 ;
                                auto move = MoveTo::create(0.5f, Vec2(mx,my));
                                auto move_ease_in = EaseExponentialOut::create(move->clone() );
                                operSps.at(o)->runAction(move_ease_in);
                                break;
                            }
                        }
                        
                    }
                }
                //获取当前需要补充的数据
                int topY = 0;
                if(operSps.size() == 0){topY = 6;}
                if(operSps.size() > 0)
                {
                    topY = 6 - operSps.size();
                }
                operSps.clear();
                //到此 一条y上的集合已经移动完毕 填补数据
                Size visibleSize = Director::getInstance()->getVisibleSize();
                int offY = visibleSize.height + 100;
                for(int r = 0;r<topY;r++)
                {
                    Sprite*item = this->createItem(r, x);
                    int my = item->getPositionY();
                    int mx = item->getPositionX();
                    item->setPositionY(offY + r*(item->getContentSize().height));
                    auto move = MoveTo::create(0.5f, Vec2(mx,my));
                    auto move_ease_in = EaseExponentialOut::create(move->clone() );
                    item->runAction(move_ease_in);
                }
            }
            //以上为移动 填补 逻辑执行完毕
            scoreNum += linePoints.size()*10;
            scoreLab -> setString(StringUtils::format("%d",scoreNum));
            
            float exper = float(scoreNum)/(float(DT()->curLevel)*500.0f)*100.0f;
            if(exper >= 100.0f)
            {
                battleEnd = true;
                exper = 100.0f;
                this->unschedule(schedule_selector(GameScene::updateTime));
                DT()->battleState = true;
                if(DT()->curLevel == DT()->level)
                {
                    DT()->level += 1;
                }
                srand(int(time(0))+ rand());//置随机数种子
                int goldnum=CCRANDOM_0_1()*100 + 100;
                DT()->rewardGold = goldnum;
                ResultScene*result = new ResultScene();
                this->addChild(result);
            }
//            int expPer = scoreNum/4500.0f*100.0f;
//            if(expPer >= 100){expPer = 0;level+=1;levelLab->setString(StringUtils::format("%d",level));}
            scoreBar->setPercent(exper);
            linePoints.clear();
        }else{
            //取消
            for(auto c : linePoints)
            {
                c->setScale(1.0f);
            }
            linePoints.clear();
        }
    }
}
int GameScene::getMinIndex(int x,int y)
{
    //获取移动最低值;
    std::string key = StringUtils::format("%d_",x)+StringUtils::format("%d",y);
    
}
void GameScene::menuMove(cocos2d::Touch* tTouch,cocos2d::Event* eEvent)
{
    if(linePoints.size() > 0)
    {
        Point groupP = container->convertToNodeSpace(tTouch->getLocation());
        Vec2 posVec = Vec2(groupP.x,groupP.y);
        this->lineConnectExec(posVec);
    }
}

void GameScene::updateTime(float dt)
{
    count +=1;
    float num = count/60.0f*100.0f;
    timeBar->setPercent(100-num);
    if(count >= 60)
    {
        battleEnd = true;
        Date::getInstance()->gameScore = scoreNum;
        this->unschedule(schedule_selector(GameScene::updateTime));
        //切换结算界面
        DT()->battleState = false;
        srand(int(time(0))+ rand());//置随机数种子
        int goldnum=CCRANDOM_0_1()*20 + 20;
        DT()->rewardGold = goldnum;
        ResultScene*result = new ResultScene();
        this->addChild(result);
    }
}
void GameScene::onExit()
{
    Layer::onExit();
    _eventDispatcher->removeEventListener(_listener);
}
