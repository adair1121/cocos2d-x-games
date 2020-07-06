//
//  GameScene.cpp
//  Puzzle-mobile
//
//  Created by 志强 on 2020/5/19.
//

#include "GameScene.h"
#include "Config.h"
#include "SoundCtl.h"
#include "HelloWorldScene.h"
#include "CommonUtil.h"
#include "TipPop.hpp"
#include "iostream"
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
    closeBtn = static_cast<Sprite*>(rootNode->getChildByName("closeBtn"));
    resetBtn = static_cast<Sprite*>(rootNode->getChildByName("resetBtn"));
    timeLab = static_cast<TextBMFont*>(rootNode->getChildByName("timeLab"));
    previewBtn = static_cast<Button*>(rootNode->getChildByName("previewBtn"));
    frameGroup = static_cast<Layout*>(rootNode->getChildByName("frameGroup"));
    frameGroup2 = static_cast<Layout*>(rootNode->getChildByName("frameGroup2"));
    frameGroup3 = static_cast<Layout*>(rootNode->getChildByName("frameGroup3"));
    if(Config::getInstance()->level == 1)
    {
        frameGroup2->setVisible(false);
        frameGroup3->setVisible(false);
    }else if(Config::getInstance()->level == 2)
    {
        frameGroup->setVisible(false);
        frameGroup3->setVisible(false);
    }else if(Config::getInstance()->level == 3)
    {
        frameGroup->setVisible(false);
        frameGroup2->setVisible(false);
    }
    
//    tipPic = static_cast<Sprite*>(frameGroup->getChildByName("tipPic"));
//    showImg = static_cast<Sprite*>(frameGroup->getChildByName("showImg"));
//    levelmap1 = static_cast<Layout*>(frameGroup->getChildByName("levelmap1"));
    
    Texture2D*texture = Director::getInstance()->getTextureCache()->addImage(StringUtils::format("sprites/image_%d.png",Config::getInstance()->picIndex));
    
    static_cast<Sprite*>(frameGroup->getChildByName("tipPic"))->setTexture(texture);
    static_cast<Sprite*>(frameGroup2->getChildByName("tipPic"))->setTexture(texture);
    static_cast<Sprite*>(frameGroup3->getChildByName("tipPic"))->setTexture(texture);
    
    _listener=EventListenerTouchOneByOne::create();
    _listener->setSwallowTouches(true);
    _listener->onTouchBegan = CC_CALLBACK_2(GameScene::menuBegin,this);
    _listener->onTouchEnded = CC_CALLBACK_2(GameScene::menuEndCallback, this);
    _listener->onTouchMoved = CC_CALLBACK_2(GameScene::menuMove, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(_listener, this);
    this->adapter();
    return true;
}
void GameScene::adapter()
{
    int bgW = bg->getContentSize().width;
    int bgH = bg->getContentSize().height;
    Size visibleSize = Director::getInstance()->getVisibleSize();
    float scaleX = visibleSize.width/bgW;
    float scaleY = visibleSize.height/bgH;
    bg->setScale(scaleX, scaleY);
    frameGroup->setScale(frameGroup->getScaleX()* scaleX);
    frameGroup2->setScale(frameGroup2->getScaleX()* scaleX);
    frameGroup3->setScale(frameGroup3->getScaleX()* scaleX);
    resetBtn->setPositionX(resetBtn->getPositionX()*scaleX);
    closeBtn->setPositionX(closeBtn->getPositionX()*scaleX);
    previewBtn->setPositionX(previewBtn->getPositionX()*scaleX);
    timeLab->setPositionX(timeLab->getPositionX()*scaleX);
    frameGroup->setPositionX(frameGroup->getPositionX()*scaleX);
    frameGroup2->setPositionX(frameGroup2->getPositionX()*scaleX);
    frameGroup3->setPositionX(frameGroup3->getPositionX()*scaleX);
}
bool GameScene::menuBegin(cocos2d::Touch* tTouch,cocos2d::Event* eEvent)
{
    if(battleEnd){return false;}
    Point localP = this->convertToNodeSpace(tTouch->getLocation());
    if(closeBtn->getBoundingBox().containsPoint(localP) && startClick && !battleEnd){
       closeBtn->setScale(0.9f);
       return true;
    }else if(resetBtn->getBoundingBox().containsPoint(localP)  && startClick && !battleEnd)
    {
        resetBtn->setScale(0.9f);
        return true;
    }else if(previewBtn->getBoundingBox().containsPoint(localP) && startClick && !battleEnd)
    {
        previewBtn->setScale(0.9f);
        return true;
    }
    Point groupP = frameGroup->convertToNodeSpace(tTouch->getLocation());
    if(Config::getInstance()->level == 2)
    {
        groupP = frameGroup2->convertToNodeSpace(tTouch->getLocation());
    }else if(Config::getInstance()->level == 3)
    {
        groupP = frameGroup3->convertToNodeSpace(tTouch->getLocation());
    }
    
//    Point point = levelmap1->convertToNodeSpace(groupP);
    Vector<Layout*> childrens;
    for(auto c : items)
    {
        float distance=sqrt(pow(c->getPositionX()-groupP.x,2)+pow(c->getPositionY()-groupP.y,2));;
        if(distance<=100  && startClick)
        {
            childrens.pushBack(c);
        }
    }
    int tag = -1;
    if(childrens.size())
    {
        for(auto c : childrens)
        {
            
            int num = c->getTag();
            if(num > tag)
            {
                tag = num;
                dragItem = c;
            }
        }
        orderIndex += 1;
        if(dragItem){return true;}
    }
    return false;
}
void GameScene::menuEndCallback(cocos2d::Touch* tTouch,cocos2d::Event* eEvent)
{
    if(battleEnd){return;}
    Point localP = this->convertToNodeSpace(tTouch->getLocation());
    
    if(closeBtn->getBoundingBox().containsPoint(localP)){
       closeBtn->setScale(1.0f);
       SoundCtl::getInstance()->playEffect("sound/click.m4a");
       auto scene = HelloWorld::createScene();
       Director::getInstance()->replaceScene(TransitionCrossFade::create(1, scene));
        return;
    }else if(resetBtn->getBoundingBox().containsPoint(localP))
    {
        resetBtn->setScale(1.0f);
        SoundCtl::getInstance()->playEffect("sound/click.m4a");
        this->resetData();
        return;
    }else if(previewBtn->getBoundingBox().containsPoint(localP))
    {
        previewBtn->setScale(1.0f);
        SoundCtl::getInstance()->playEffect("sound/click.m4a");
        Sprite*prevPic;
        if(Config::getInstance()->level == 1)
        {
            prevPic = static_cast<Sprite*>(frameGroup->getChildByName("tipPic"));
        }else if(Config::getInstance()->level == 2)
        {
            prevPic = static_cast<Sprite*>(frameGroup2->getChildByName("tipPic"));
        }else if(Config::getInstance()->level == 3)
        {
            prevPic = static_cast<Sprite*>(frameGroup3->getChildByName("tipPic"));
        }
        if(prevPic->isVisible())
        {
            prevPic->setVisible(false);
            previewBtn->setBrightStyle(BRIGHT_NORMAL);
        }else{
            prevPic->setVisible(true);
            previewBtn->setBrightStyle(BRIGHT_HIGHLIGHT);
        }
        return;
    }
    if(dragItem)
    {
        std::string namestr = dragItem->getName();
        std::vector<std::string> posInfo = CommonUtil::split(namestr, "_");
        int posx = atoi(posInfo.at(0).c_str());
        int posy = atoi(posInfo.at(1).c_str());
        std::string topname = posy - 1 >= 0?StringUtils::format("%d_",posx)+StringUtils::format("%d",(posy-1)):"top";
        std::string leftname = posx - 1 >= 0?StringUtils::format("%d_",(posx - 1))+StringUtils::format("%d",posy):"left";
        std::string rightname = posx + 1 <= 3?StringUtils::format("%d_",(posx + 1))+StringUtils::format("%d",posy):"right";
        std::string bottomname = posy + 1 <= 3?StringUtils::format("%d_",posx)+StringUtils::format("%d",(posy+1)):"bottom";
        
        Layout*topitem = static_cast<Layout*>(frameGroup->getChildByName(topname));
        Layout*leftitem = static_cast<Layout*>(frameGroup->getChildByName(leftname));
        Layout*rightitem = static_cast<Layout*>(frameGroup->getChildByName(rightname));
        Layout*bottomitem = static_cast<Layout*>(frameGroup->getChildByName(bottomname));
        Sprite*standDragItem = static_cast<Sprite*>(frameGroup->getChildByName("item"+dragItem->getName()));
        if(Config::getInstance()->level == 2)
        {
            topitem = static_cast<Layout*>(frameGroup2->getChildByName(topname));
            leftitem = static_cast<Layout*>(frameGroup2->getChildByName(leftname));
            rightitem = static_cast<Layout*>(frameGroup2->getChildByName(rightname));
            bottomitem = static_cast<Layout*>(frameGroup2->getChildByName(bottomname));
            standDragItem = static_cast<Sprite*>(frameGroup2->getChildByName("item"+dragItem->getName()));
        }else if(Config::getInstance()->level == 3)
        {
            topitem = static_cast<Layout*>(frameGroup3->getChildByName(topname));
            leftitem = static_cast<Layout*>(frameGroup3->getChildByName(leftname));
            rightitem = static_cast<Layout*>(frameGroup3->getChildByName(rightname));
            bottomitem = static_cast<Layout*>(frameGroup3->getChildByName(bottomname));
            standDragItem = static_cast<Sprite*>(frameGroup3->getChildByName("item"+dragItem->getName()));
        }
        if(topitem)
        {
            int disY = topitem->getPositionY() - dragItem->getPositionY();
            int disX = abs(topitem->getPositionX() - dragItem->getPositionX());
            int offValue = abs(topitem->getContentSize().height + dragItem->getContentSize().height)/2;
            if(disY > 0  && disY <= offValue && disX <= 30)
            {
                Sprite*standTopitem = static_cast<Sprite*>(frameGroup->getChildByName("item"+topname));
                if(Config::getInstance()->level == 2)
                {
                    standTopitem = static_cast<Sprite*>(frameGroup2->getChildByName("item"+topname));
                }else if(Config::getInstance()->level == 3)
                {
                    standTopitem = static_cast<Sprite*>(frameGroup3->getChildByName("item"+topname));
                }
                int offY = standTopitem->getPositionY() - standDragItem->getPositionY();
                int offX = standTopitem->getPositionX() - standDragItem->getPositionX();
                Point prevp = Point(dragItem->getPositionX(),dragItem->getPositionY());
                dragItem->setPosition(Vec2(topitem->getPositionX() - offX,topitem->getPositionY() - offY));
                Point offP = Point(dragItem->getPositionX() - prevp.x,dragItem->getPositionY() - prevp.y);
                this->resetBlockPos(offP.x, offP.y);
                this->enterBlock(topname, namestr);
            }
        }
        if(bottomitem)
        {
            int disY = dragItem->getPositionY() - bottomitem->getPositionY();
            int disX = abs(bottomitem->getPositionX() - dragItem->getPositionX());
            int offValue = abs(bottomitem->getContentSize().height + dragItem->getContentSize().height)/2;
            if(disY > 0  && disY <= offValue && disX <= 30)
            {
                Sprite*standBottomitem = static_cast<Sprite*>(frameGroup->getChildByName("item"+bottomname));
                if(Config::getInstance()->level == 2)
                {
                    standBottomitem = static_cast<Sprite*>(frameGroup2->getChildByName("item"+bottomname));
                }else if(Config::getInstance()->level == 3)
                {
                    standBottomitem = static_cast<Sprite*>(frameGroup3->getChildByName("item"+bottomname));
                }
                int offY = standBottomitem->getPositionY() - standDragItem->getPositionY();
                int offX = standBottomitem->getPositionX() - standDragItem->getPositionX();
                Point prevp = Point(dragItem->getPositionX(),dragItem->getPositionY());
                dragItem->setPosition(Vec2(bottomitem->getPositionX() - offX,bottomitem->getPositionY() - offY));
                Point offP = Point(dragItem->getPositionX() - prevp.x,dragItem->getPositionY() - prevp.y);
                this->resetBlockPos(offP.x, offP.y);
                this->enterBlock(bottomname, namestr);
            }
        }
        if(leftitem)
        {
            int disX = dragItem->getPositionX() - leftitem->getPositionX() ;
            int disY = abs(leftitem->getPositionY() - dragItem->getPositionY());
            int offValue = abs(leftitem->getContentSize().width + dragItem->getContentSize().width)/2;
            if(disX > 0 && disX <= offValue && disY <= 30)
            {
                Sprite*standLeftitem = static_cast<Sprite*>(frameGroup->getChildByName("item"+leftname));
                if(Config::getInstance()->level == 2)
                {
                    standLeftitem = static_cast<Sprite*>(frameGroup2->getChildByName("item"+leftname));
                }else if(Config::getInstance()->level == 3)
                {
                    standLeftitem = static_cast<Sprite*>(frameGroup3->getChildByName("item"+leftname));
                }
                int offY = standLeftitem->getPositionY() - standDragItem->getPositionY();
                int offX = standLeftitem->getPositionX() - standDragItem->getPositionX();
                Point prevp = Point(dragItem->getPositionX(),dragItem->getPositionY());
                dragItem->setPosition(Vec2(leftitem->getPositionX() - offX,leftitem->getPositionY() - offY));
                Point offP = Point(dragItem->getPositionX() - prevp.x,dragItem->getPositionY() - prevp.y);
                this->resetBlockPos(offP.x, offP.y);
                this->enterBlock(leftname, namestr);
            }
        }
        if(rightitem)
        {
            int disX = rightitem->getPositionX() - dragItem->getPositionX()  ;
            int disY = abs(rightitem->getPositionY() - dragItem->getPositionY());
            int offValue = abs(rightitem->getContentSize().width + dragItem->getContentSize().width)/2;
            if(disX > 0 && disX <= offValue && disY <= 30)
            {
                Sprite*standRightitem = static_cast<Sprite*>(frameGroup->getChildByName("item"+rightname));
                if(Config::getInstance()->level == 2)
                {
                    standRightitem = static_cast<Sprite*>(frameGroup2->getChildByName("item"+rightname));
                }else if(Config::getInstance()->level == 3)
                {
                    standRightitem = static_cast<Sprite*>(frameGroup3->getChildByName("item"+rightname));
                }
                int offY = standRightitem->getPositionY() - standDragItem->getPositionY();
                int offX = standRightitem->getPositionX() - standDragItem->getPositionX();
                Point prevp = Point(dragItem->getPositionX(),dragItem->getPositionY());
                dragItem->setPosition(Vec2(rightitem->getPositionX() - offX,rightitem->getPositionY() - offY));
                Point offP = Point(dragItem->getPositionX() - prevp.x,dragItem->getPositionY() - prevp.y);
                this->resetBlockPos(offP.x, offP.y);
                this->enterBlock(rightname, namestr);
            }
        }
//        this->lineBlockMove(Point(dragItem->getPositionX(),dragItem->getPositionY()));
        dragItem = NULL;
    }
}
void GameScene::resetBlockPos(float offx,float offy)
{
    std::string containDragstr = "";
    for(auto c : connectBlock)
    {
      
        std::vector<std::string> blockitems = CommonUtil::split(c, "|");
        for(auto b : blockitems)
        {
            //找到了之前存在的块儿
            if(b == dragItem->getName()){containDragstr = c;}
        }
    }
    if(containDragstr != "")
    {
        std::vector<std::string> blockitems = CommonUtil::split(containDragstr, "|");
        for(auto b : blockitems)
        {
            //找到了之前存在的块儿
            if(b != dragItem->getName()){
                Layout*item = static_cast<Layout*>(frameGroup->getChildByName(b));
                if(Config::getInstance()->level == 2)
                {
                    item = static_cast<Layout*>(frameGroup2->getChildByName(b));
                }else if(Config::getInstance()->level == 3)
                {
                    item = static_cast<Layout*>(frameGroup3->getChildByName(b));
                }
                item->setPosition(Vec2(item->getPositionX() + offx,item->getPositionY() + offy));
            }
        }
    }
}
void GameScene::enterBlock(std::string blockstr , std::string dragstr)
{
    int blockindex = -1;
    int dragIndex = -1;
    int index = -1;
    std::string containBlockstr = "";
    std::string containDragstr = "";
    for(auto c : connectBlock)
    {
        index += 1;
        std::vector<std::string> blockitems = CommonUtil::split(c, "|");
        if(blockitems.size() >= Config::getInstance()->allNum)
        {
            this->unschedule(schedule_selector(GameScene::updateTime));
            //游戏结束
            if(Config::getInstance()->picIndex >= Config::getInstance()->openLevel)
            {
                Config::getInstance()->openLevel += 1;
            }
            TipPop*result = new TipPop();
            this->addChild(result);
            battleEnd = true;
        }
        for(auto b : blockitems)
        {
            //找到了之前存在的块儿
            if(b == blockstr){containBlockstr = c;blockindex = index;}
            if(b == dragstr){containDragstr = c;dragIndex = index;}
        }
    }
    if(containDragstr != "" && containBlockstr != "")
    {
        if(containDragstr == containBlockstr)
        {
            //说明是在一个块儿里面 并且已经都存在了直接return
            return;
        }else{
            //进行2个块儿的合并
            containBlockstr += "|"+containDragstr;
            connectBlock.at(blockindex) = containBlockstr;
            connectBlock.at(dragIndex) = "";
        }
        return;
    }
    if(containDragstr == "" && containBlockstr == "")
    {
        //这两个元素都不被块儿包含
        std::string valuestr = blockstr+"|"+dragstr;
        connectBlock.push_back(valuestr);
    }else{
        //其中一个 被块儿包含
        if(containBlockstr != "")
        {
            //说明 blockstr 被块儿包含。dragstr是单个元素
            containBlockstr += "|"+dragstr;
            connectBlock.at(blockindex) = containBlockstr;
        }else{
            containDragstr += "|" + blockstr;
            connectBlock.at(dragIndex) = containDragstr;
        }
    }
}
void GameScene::menuMove(cocos2d::Touch* tTouch,cocos2d::Event* eEvent)
{
    if(dragItem)
    {
        Point group = frameGroup->convertToNodeSpace(tTouch->getLocation());
        if(Config::getInstance()->level == 2)
        {
            group = frameGroup2->convertToNodeSpace(tTouch->getLocation());
        }else if(Config::getInstance()->level == 3)
        {
            group = frameGroup3->convertToNodeSpace(tTouch->getLocation());
        }
        this->lineBlockMove(group);
        dragItem->setPosition(Vec2(group.x,group.y));
        dragItem->setLocalZOrder(orderIndex);
    }
}
void GameScene::lineBlockMove(Point draItemP)
{
    
    std::string namestr = dragItem->getName();
    std::string blockstr = "";//0_0|1_1
    for(auto c : connectBlock)
    {
        std::vector<std::string> blockitems = CommonUtil::split(c, "|");
        for(auto b : blockitems)
        {
            if(b == namestr)
            {
                //存在块关系
                blockstr = c;
                break;
            }
        }
        if(blockstr != ""){break;}
    }
    if(blockstr != "")
    {
        std::vector<std::string> blockitems = CommonUtil::split(blockstr, "|");
        float offx = draItemP.x - dragItem->getPositionX();
        float offy = draItemP.y - dragItem->getPositionY();
        for(auto b : blockitems)
        {
            if(b == dragItem->getName()){continue;}
            Layout*item = static_cast<Layout*>(frameGroup->getChildByName(b));
            if(Config::getInstance()->level == 2)
            {
                item = static_cast<Layout*>(frameGroup2->getChildByName(b));
            }else if(Config::getInstance()->level == 3)
            {
                item = static_cast<Layout*>(frameGroup3->getChildByName(b));
            }
            orderIndex += 1;
            item->setLocalZOrder(orderIndex);
            item->setPosition(Vec2(item->getPositionX() + offx,item->getPositionY() + offy));
        }
    }
}
void GameScene::onEnter()
{
    Layer::onEnter();
    this->resetData();
}
void GameScene::startGame(float dt)
{
    static_cast<Sprite*>(frameGroup->getChildByName("showImg"))->setVisible(false);
    static_cast<Sprite*>(frameGroup2->getChildByName("showImg"))->setVisible(false);
    static_cast<Sprite*>(frameGroup3->getChildByName("showImg"))->setVisible(false);
//    showImg->setVisible(false);
    this->unschedule(schedule_selector(GameScene::startGame));
    srand(int(time(0))+ rand());//置随机数种子
    int leftNum=Config::getInstance()->allNum/2;
    int rightNum = leftNum;
    random_shuffle(items.begin(),items.end());
    for(int i = 0;i<items.size();i++)
    {
        float x = (i <= leftNum - 1)?-150:(frameGroup->getContentSize().width + 150);
        if(Config::getInstance()->level == 2)
        {
            x = (i <= leftNum - 1)?-150:(frameGroup2->getContentSize().width + 150);
        }else if(Config::getInstance()->level == 3)
        {
            x = (i <= leftNum - 1)?-150:(frameGroup3->getContentSize().width + 150);
        }
        float y = items.at(i)->getPositionY() - 50;
        auto moveTo = MoveTo::create(0.5f, Vec2(x,y));
        auto move_ease_in = EaseSineOut::create(moveTo->clone());
        items.at(i)->runAction(move_ease_in);
    }
    this->schedule(schedule_selector(GameScene::updateTime),1.0f);
    startDrag = true;
    startClick = true;
}
void GameScene::updateTime(float dt)
{
    timeCount += 1;
    this->refreshTimeShow();
}
void GameScene::resetData()
{
    for(auto c : items)
    {
        c->removeFromParent();
    }
    items.clear();
    if(Config::getInstance()->level == 1)
    {
        static_cast<Sprite*>(frameGroup->getChildByName("showImg"))->setVisible(true);
    }else if(Config::getInstance()->level == 2)
    {
        static_cast<Sprite*>(frameGroup2->getChildByName("showImg"))->setVisible(true);
    }else if(Config::getInstance()->level == 3)
    {
        static_cast<Sprite*>(frameGroup3->getChildByName("showImg"))->setVisible(true);
    }
//    showImg->setVisible(true);
    int row = Config::getInstance()->row;
    int col = Config::getInstance()->col;
    int level = Config::getInstance()->level;
    Layout*container = new Layout();

    this->unschedule(schedule_selector(GameScene::updateTime));
    this->unschedule(schedule_selector(GameScene::startGame));
    std::string baselevestr = StringUtils::format("sprites/level%d/",level);
    for(int i = 0;i<row;i++)
    {
        for(int j = 0;j<col;j++)
        {
            Layout*container = new Layout();
            container->setTag(i+j);
            std::string itemKey = StringUtils::format("item%d_",i) + StringUtils::format("%d",j);
            Sprite * item = static_cast<Sprite*>(frameGroup->getChildByName(itemKey));
            if(Config::getInstance()->level == 2)
            {
                item = static_cast<Sprite*>(frameGroup2->getChildByName(itemKey));
            }else if(Config::getInstance()->level == 3)
            {
                item = static_cast<Sprite*>(frameGroup3->getChildByName(itemKey));
            }
            std::string key = baselevestr + StringUtils::format("%d_",i) + StringUtils::format("%d_mark.png",j);
            Sprite * mask = Sprite::create(key);
            container->setContentSize(mask->getContentSize());

            Sprite *cal=Sprite::create(StringUtils::format("sprites/image_%d.png",Config::getInstance()->picIndex));


            Sprite * maskSp = maskedSpriteWithSprite(cal, mask,Vec2(item->getPositionX(),item->getPositionY()));
            
            container->addChild(maskSp);
          

            std::string bottomkey = baselevestr +  StringUtils::format("%d_",i) + StringUtils::format("%d.png",j);
            Sprite*bitem = Sprite::create(bottomkey);
            container->addChild(bitem);
            
            if(Config::getInstance()->level == 1)
            {
                
                frameGroup->addChild(container);
            }else if(Config::getInstance()->level == 2)
            {
                frameGroup2->addChild(container);
            }else if(Config::getInstance()->level == 3)
            {
                frameGroup3->addChild(container);
            }
            
            container->setPosition(Vec2(item->getPositionX(),item->getPositionY()));
            
            container->setName(StringUtils::format("%d_",i)+StringUtils::format("%d",j));
            
            items.pushBack(container);
        }
    }
    this->scheduleOnce(schedule_selector(GameScene::startGame), 3.0f);
    startDrag = false;
    startClick = false;
    timeCount = 0;
    orderIndex = 10;
    connectBlock.clear();
    this->refreshTimeShow();
}
void GameScene::refreshTimeShow()
{
    int minute = timeCount/60;
    std::string minutestr = minute < 10 ?StringUtils::format("0%d",minute):StringUtils::format("%d",minute);
    int second = timeCount - minute*60;
    std::string secondstr = second < 10 ?StringUtils::format("0%d",second):StringUtils::format("%d",second);
    timeLab->setString("TIME "+minutestr + ":" + secondstr);
}
void GameScene::onExit()
{
    Layer::onExit();
    _eventDispatcher->removeEventListener(_listener);
}

Sprite* GameScene::maskedSpriteWithSprite(Sprite* textureSprite, Sprite* maskSprite,Vec2 maskP)
{
    //以mask为原点进行渲染 不会移动 智能移动被遮罩元素
    RenderTexture * rt = RenderTexture::create( maskSprite->getContentSize().width,

                                               maskSprite->getContentSize().height,kCCTexture2DPixelFormat_RGBA8888);

    
    float x = maskSprite->getContentSize().width/2;
    float y = maskSprite->getContentSize().height/2;
    
    maskSprite->setPosition(x,y);
    
//    textureSprite->setPosition(Vec2(0,0));
    float xx = textureSprite->getContentSize().width/2;
    float yy = textureSprite->getContentSize().height/2;
    float offx =  x - maskP.x + 8;
    float offy = y - maskP.y + 3;
    textureSprite->setPosition(xx+offx,yy+offy);
   
  

    maskSprite->setBlendFunc( BlendFunc{GL_ONE, GL_ZERO} );

    textureSprite->setBlendFunc( BlendFunc{GL_DST_ALPHA, GL_ZERO} );
    
    rt->begin();

    maskSprite->visit();

    textureSprite->visit();

    rt->end();

    Sprite *retval = Sprite::createWithTexture(rt->getSprite()->getTexture());

    retval->setFlippedY(true);

    return retval;
}
