//
//  GameScene.cpp
//  Greedy-mobile
//
//  Created by 志强 on 2020/6/1.
//

#include "GameScene.hpp"
#include "CommonUtil.h"
#include "HelloWorldScene.h"
#include "Config.h"

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
    leftGroup = static_cast<Layout*>(rootNode->getChildByName("leftGroup"));
    rightGroup = static_cast<Layout*>(rootNode->getChildByName("rightGroup"));
    leftIcon = static_cast<Sprite*>(leftGroup->getChildByName("leftIcon"));
    rightIcon = static_cast<Sprite*>(rightGroup->getChildByName("rightIcon"));
    
    ownScoreLab = static_cast<Text*>(leftGroup->getChildByName("ownScoreLab"));
    comScoreLab = static_cast<Text*>(rightGroup->getChildByName("comScoreLab"));
    gameGroup = static_cast<Layout*>(rootNode->getChildByName("gameGroup"));
    
    returnBtn = static_cast<Sprite*>(rootNode->getChildByName("returnBtn"));
    greyLayer = static_cast<Layout*>(rootNode->getChildByName("greyLayer"));
    guidebg = static_cast<Sprite*>(rootNode->getChildByName("guidebg"));
    
    resultGroup = static_cast<Layout*>(rootNode->getChildByName("resultGroup"));
    winIcon = static_cast<Sprite*>(resultGroup->getChildByName("winIcon"));
    mainBtn = static_cast<Button*>(resultGroup->getChildByName("mainBtn"));
    goldGroup = static_cast<Layout*>(resultGroup->getChildByName("goldGroup"));
    rewardNum = static_cast<Text*>(goldGroup->getChildByName("rewardNum"));
    
    resultGroup->setVisible(false);
    
    greyLayer->setVisible(true);
    guidebg->setOpacity(0);
    guidebg->setScale(0.0f);
    
    std::string ass = StringUtils::format("assets/item%d.png",CF()->selectIndex);
    Sprite*right = Sprite::create(ass);
    leftIcon->setTexture(right->getTexture());
    
    auto delay = DelayTime::create(1.0f);
    auto delay2 = DelayTime::create(3.0f);
    auto scaleTo = ScaleTo::create(1.0f, 1.0f);
    auto fadein = FadeIn::create(1.0f);
    auto spwan = Spawn::createWithTwoActions(scaleTo, fadein);
    auto callback = CallFunc::create([&](){
        auto fadeout = FadeOut::create(1.0f);
        auto scaleOut = ScaleTo::create(1.0f, 0.0f);
        auto spawmOut = Spawn::createWithTwoActions(fadeout, scaleOut);
        auto callback2 = CallFunc::create([&](){
            guidebg->setVisible(false);
            greyLayer->setVisible(false);
        });
        guidebg->runAction(Sequence::create(spawmOut,callback2, NULL));
        
    });
    guidebg->runAction(Sequence::create(delay,spwan,delay2,callback, NULL));
    
    _listener=EventListenerTouchOneByOne::create();
    _listener->setSwallowTouches(true);
    _listener->onTouchBegan = CC_CALLBACK_2(GameScene::menuBegin,this);
    _listener->onTouchMoved = CC_CALLBACK_2(GameScene::menuMove,this);
    _listener->onTouchEnded = CC_CALLBACK_2(GameScene::menuEndCallback, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(_listener, this);
    this->resetData();
    this->adapter();
    return true;
}
void GameScene::resetData()
{
    battleCount = 0;
    Vector<Node*> childrens = gameGroup->getChildren();
    showIndex = 0;
    totalstr = "";
    battleState = false;
    ownScoreLab->setString("0");
    comScoreLab->setString("0");
    for(auto c : childrens)
    {
        if(c->getTag() == 1 || c->getTag() == 0 || c->getName() == "rewardRect")
        {
            c->removeFromParent();
        }
    }
    this->leftIcon->setVisible((battleCount%2 == 0));
    this->rightIcon->setVisible(!this->leftIcon->isVisible());
}
void GameScene::changePlayer()
{
    if(battleCount >= 41)
    {
        //游戏结束
        battleState = true;
        this->resultCalCul();
        return;
    }
    battleCount += 1;
    this->leftIcon->setVisible((battleCount%2 == 0));
    this->rightIcon->setVisible(!this->leftIcon->isVisible());
    if(this->rightIcon->isVisible())
    {
        this->execAiPosJudge();
    }
}
void GameScene::execAiPosJudge()
{
    std::vector<std::string> newpos = CommonUtil::split(newDropstr, "_");
    //先判断附近的3个位置可以落子
    int col = atoi(newpos.at(0).c_str());
    Point bottomP1 = getBottomPos(col-1);
    std::string str1 = bottomNamestr;
    Point bottomP2 = getBottomPos(col+1);
    std::string str2 = bottomNamestr;
    Point bottomP3 = getBottomPos(col);
    std::string str3 = bottomNamestr;
    Point dropP = Point(0,0);
    std::string namestr = "";
    if(bottomP1.x != 0 && bottomP1.y != 0){dropP = bottomP1;namestr = str1;}
    else if(bottomP2.x != 0 && bottomP2.y != 0){dropP = bottomP2;namestr = str2;}
    else if(bottomP3.x != 0 && bottomP3.y != 0){dropP = bottomP3;namestr = str3;}
    if(dropP.x == 0 && dropP.y == 0)
    {
        for(int i = 0;i<=6;i++)
        {
            Point p = getBottomPos(i);
            namestr = bottomNamestr;
            if(p.x != 0 && p.y != 0)
            {
                dropP = p;
                break;
            }
        }
    }
    Sprite*item = Sprite::create("assets/coinicon-sheet1.png");
    item->setName(namestr);
    item->setTag(0);
    gameGroup->addChild(item);
    
    item->setPosition(Vec2(dropP.x,Director::getInstance()->getVisibleSize().height + 100));
    auto moveTo = MoveTo::create(0.5f, Vec2(dropP.x,dropP.y));
    auto callback = CallFunc::create([&](){
        this->changePlayer();
    });
    item->runAction(Sequence::create(moveTo,callback, NULL));
     
}
void GameScene::resultCalCul()
{
    //结算
    std::string toplinestr;
    for(int j = 0;j<=6;j++)
    {
        Vector<Sprite*> tops;
        bool boo = false;
        for(int i = 0;i<=5;i++)
        {
            //从上到下
            boo = calculLine(j, i, tops);
            if(boo){break;}
        }
        if(tops.size() < 4){tops.clear();continue;}
        std::string str = "";
        for(auto c : tops)
        {
            str += c->getName()+"|";
        }
        toplinestr += str+"&";
    }
    std::string leftLinestr = "";
    for(int j = 0;j<=5;j++)
    {
        Vector<Sprite*> lefts;
        bool boo = false;
        for(int i = 0;i <= 6;i++)
        {
            //从左到右
            boo = calculLine(i, j, lefts);
            if(boo){break;}
        }
        if(lefts.size() < 4){lefts.clear();continue;}
        std::string str = "";
        for(auto c : lefts)
        {
            str += c->getName()+"|";
        }
        leftLinestr += str + "&";
    }
    //从左上到左下
    std::string poslinestr1 = "";
    std::vector<std::string> pos1;
    pos1.push_back("0_2");
    pos1.push_back("0_1");
    pos1.push_back("0_0");
    pos1.push_back("1_0");
    pos1.push_back("2_0");
    pos1.push_back("3_0");
    for(auto c : pos1)
    {
        Vector<Sprite*> vecs;
        bool boo = false;
        for(int i = 0;i<=5;i++)
        {
            std::vector<std::string> posinfo = CommonUtil::split(c, "_");
            boo = calculLine(atoi(posinfo.at(0).c_str()) + i, atoi(posinfo[1].c_str()) +i, vecs);
            if(boo){break;}
        }
        if(vecs.size() < 4){vecs.clear();continue;}
        std::string str = "";
        for(auto c : vecs)
        {
            str += c->getName()+"|";
        }
        poslinestr1 += str + "&";
    }
    std::vector<std::string> pos2;
    pos2.push_back("3_5");
    pos2.push_back("2_5");
    pos2.push_back("1_5");
    pos2.push_back("0_5");
    pos2.push_back("0_4");
    pos2.push_back("0_3");
    std::string poslinestr2 = "";
    for(auto c : pos2)
    {
        Vector<Sprite*> vecs;
        bool boo = false;
        for(int i = 0;i<=5;i++)
        {
            std::vector<std::string> posinfo = CommonUtil::split(c, "_");
            boo = calculLine(atoi(posinfo.at(0).c_str()) + i, atoi(posinfo[1].c_str()) - i, vecs);
            if(boo){break;}
        }
        if(vecs.size() < 4){vecs.clear();continue;}
        std::string str = "";
        for(auto c : vecs)
        {
            str += c->getName()+"|";
        }
        poslinestr2 += str + "&";
    }
    totalstr = toplinestr + leftLinestr + poslinestr1 + poslinestr2;
    
    this->showBlock();
}
void GameScene::showBlock()
{
    std::vector<std::string> combvec = CommonUtil::split(totalstr, "&");
    if(showIndex < combvec.size()){
        std::string blockstr = combvec.at(showIndex);
        showIndex += 1;
        std::vector<std::string> blockinfo = CommonUtil::split(blockstr, "|");
        for(auto c : blockinfo)
        {
            if(c == ""){continue;}
            Sprite*item = static_cast<Sprite*>(gameGroup->getChildByName(c));
            Sprite*rewardRect = Sprite::create("assets/pointsframe-sheet0.png");
            gameGroup->addChild(rewardRect);
            rewardRect->setPosition(item->getPosition());
            rewardRect->setOpacity(0);
            rewardRect->setName("rewardRect");
            auto fadein = FadeIn::create(1.0f);
            auto delay = DelayTime::create(1.0f);
            if(item->getTag() == 1)
            {
                ownScoreLab->setString(StringUtils::format("%d",atoi(ownScoreLab->getString().c_str()) + 100));
            }else if(item->getTag() == 0)
            {
                comScoreLab->setString(StringUtils::format("%d",atoi(comScoreLab->getString().c_str()) + 100));
            }
            auto callback = CallFunc::create([&](){
                Sprite*rewardRect = static_cast<Sprite*>(gameGroup->getChildByName("rewardRect"));
                rewardRect->removeFromParent();
            });
            rewardRect->runAction(Sequence::create(fadein,delay,callback, NULL));
        }
        auto delay = DelayTime::create(2.0f);
        auto callback = CallFunc::create([&](){
           showBlock();
        });
        this->runAction(Sequence::create(delay,callback, NULL));
    }else{
        //提示结算界面
        int ownScore = atoi(ownScoreLab->getString().c_str());
        int comScore = atoi(comScoreLab->getString().c_str());
        resultGroup->setVisible(true);
        winIcon->setVisible(true);
        if(ownScore > comScore)
        {
            //游戏胜利
            Texture2D*texture = Director::getInstance()->getTextureCache()->addImage("assets/winbg.png");
            winIcon->setTexture(texture);
            goldGroup->setVisible(true);
            srand(int(time(0))+ rand());//置随机数种子
            int num=CCRANDOM_0_1()*50+50;
            rewardNum->setString(StringUtils::format("%d",num));
            CF()->gold += num;
        }else{
            //游戏失败
            Texture2D*texture = Director::getInstance()->getTextureCache()->addImage("assets/failbg.png");
            winIcon->setTexture(texture);
            goldGroup->setVisible(false);
        }
    }
}
bool GameScene::calculLine(int x,int y,Vector<Sprite*> & vecs)
{
    std::string itemkey = StringUtils::format("%d_",x)+StringUtils::format("%d",y);
    Sprite*item = static_cast<Sprite*>(gameGroup->getChildByName(itemkey));
    
    if(item)
    {
        if(vecs.size() > 0)
        {
            Sprite*prevItem = vecs.at(vecs.size()-1);
            if(prevItem->getTag() != item->getTag())
            {
                if(vecs.size() < 4){vecs.clear();vecs.pushBack(item);}
                else{return true;}
            }else{
                vecs.pushBack(item);
            }
        }else{
            vecs.pushBack(item);
        }
    }else{
        if(vecs.size() < 4){vecs.clear();}
    }
    return false;
}
void GameScene::adapter()
{
    int bgW = bg->getContentSize().width;
    int bgH = bg->getContentSize().height;
    Size visibleSize = Director::getInstance()->getVisibleSize();
    float scaleX = visibleSize.width/bgW;
    float scaleY = visibleSize.height/bgH;
    bg->setScale(scaleX, scaleY);
    rightGroup->setPosition(Vec2(rightGroup->getPositionX()*scaleX,rightGroup->getPositionY()*scaleY));
    leftGroup->setPosition(Vec2(leftGroup->getPositionX()*scaleX,leftGroup->getPositionY()*scaleY));
    gameGroup->setScale(scaleX);
    gameGroup->setPosition(Vec2(gameGroup->getPositionX()*scaleX,gameGroup->getPositionY()*scaleY));
    greyLayer->setScale(scaleX,scaleY);
    guidebg->setPosition(Vec2(guidebg->getPositionX()*scaleX,guidebg->getPositionY()*scaleY));
    resultGroup->setScale(scaleX);
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
    if(gameGroup->getBoundingBox().containsPoint(localP) && !greyLayer->isVisible() && leftIcon->isVisible() && !battleState)
    {
        Point group = gameGroup->convertToNodeSpace(tTouch->getLocation());
        std::string ass = StringUtils::format("assets/item%d.png",CF()->selectIndex);
        virtualSp = Sprite::create(ass);
        virtualSp->setOpacity(128.0f);
        virtualSp->setTag(-1);
        gameGroup->addChild(virtualSp);
        showVirtualSpPos(group.x,group.y);
        return true;
    }else if(resultGroup->getBoundingBox().containsPoint(localP) && resultGroup->isVisible())
    {
        Point groupP = resultGroup->convertToNodeSpace(tTouch->getLocation());
        if(mainBtn->getBoundingBox().containsPoint(groupP))
        {
            mainBtn->setBrightStyle(BRIGHT_HIGHLIGHT);
            return true;
        }
    }else if(returnBtn->getBoundingBox().containsPoint(localP))
    {
        returnBtn->setScale(0.9f);
        return true;
    }
    return false;
}
void GameScene::showVirtualSpPos(float x,float y)
{
    if(virtualSp)
    {
        for(int i = 0;i<=6;i++)
        {
            for(int j = 0;j<=5;j++)
            {
                std::string asskey = StringUtils::format("item%d_",i) + StringUtils::format("%d",j);
                Layout*grid = static_cast<Layout*>(gameGroup->getChildByName(asskey));
                float gx = grid->getPositionX();
                if(x >= gx - (grid->getContentSize().width/2) && x <= (gx + (grid->getContentSize().width/2)))
                {
                    Point bottomP = getBottomPos(i);
                    if(bottomP.x != 0 || bottomP.y != 0)
                    {
                        virtualSp->setVisible(true);
                        virtualSp->setPosition(Vec2(bottomP.x,bottomP.y));
                    }
                    break;
                }
            }
        }
        if(virtualSp->getPositionX() == 0 && virtualSp->getPositionY() ==0)
        {
            virtualSp->setVisible(false);
        }
    }
}
Point GameScene::getBottomPos(int col)
{
    Vector<Node*> childrens = gameGroup->getChildren();
    Point bottomP = Point(0,0);
    bool isExist = false;
    bottomNamestr = "";
    for(int i = 5;i>=0;i--)
    {
        isExist = false;
        std::string asskey = StringUtils::format("%d_",col) + StringUtils::format("%d",i);
        for(auto c : childrens)
        {
            if(c->getName() == asskey && ( c->getTag() == 1 || c->getTag() == 0))
            {
                isExist = true;
                break;
            }
        }
        if(!isExist)
        {
            std::string gridkey = StringUtils::format("item%d_",col) + StringUtils::format("%d",i);
            Layout*grid = static_cast<Layout*>(gameGroup->getChildByName(gridkey));
            if(!grid)
            {
                return bottomP;
            }
            bottomP.x = grid->getPositionX();
            bottomP.y = grid->getPositionY();
            if(virtualSp)
            {
                virtualSp->setName(asskey);
            }
            bottomNamestr = asskey;
            break;
        }
    }
    return bottomP;
}
void GameScene::menuMove(cocos2d::Touch* tTouch,cocos2d::Event* eEvent)
{
    if(virtualSp)
    {
        Point group = gameGroup->convertToNodeSpace(tTouch->getLocation());
        showVirtualSpPos(group.x, group.y);
    }
}
void GameScene::menuEndCallback(cocos2d::Touch* tTouch,cocos2d::Event* eEvent)
{
    Point localP = this->convertToNodeSpace(tTouch->getLocation());
    if(gameGroup->getBoundingBox().containsPoint(localP) && !greyLayer->isVisible() && leftIcon->isVisible() && !battleState)
    {
        if(virtualSp->getPositionX() == 0 && virtualSp->getPositionY() == 0)
        {
            virtualSp->removeFromParent();
            virtualSp = NULL;
        }else{
            leftIcon->setVisible(false);
            std::string ass = StringUtils::format("assets/item%d.png",CF()->selectIndex);
            Sprite*item = Sprite::create(ass);
            item->setName(virtualSp->getName());
            newDropstr = virtualSp->getName();
            item->setTag(1);
            gameGroup->addChild(item);
            float x = virtualSp->getPositionX();
            float y = virtualSp->getPositionY();
            item->setPosition(Vec2(x,Director::getInstance()->getVisibleSize().height + 100));
            auto moveTo = MoveTo::create(0.5f, Vec2(x,y));
            auto callback = CallFunc::create([&](){
                this->changePlayer();
            });
            item->runAction(Sequence::create(moveTo,callback, NULL));
        }
        
    }else if(resultGroup->getBoundingBox().containsPoint(localP) && resultGroup->isVisible())
    {
        Point groupP = resultGroup->convertToNodeSpace(tTouch->getLocation());
        if(mainBtn->getBoundingBox().containsPoint(groupP))
        {
            mainBtn->setBrightStyle(BRIGHT_NORMAL);
            auto scene = HelloWorld::createScene();
            Director::getInstance()->replaceScene(TransitionFade::create(1, scene));
        }
    }else if(returnBtn->getBoundingBox().containsPoint(localP))
    {
        returnBtn->setScale(1.0f);
        auto scene = HelloWorld::createScene();
        Director::getInstance()->replaceScene(TransitionFade::create(1, scene));
    }
    if(virtualSp)
    {
        virtualSp->removeFromParent();
        virtualSp = NULL;
    }
    
}
