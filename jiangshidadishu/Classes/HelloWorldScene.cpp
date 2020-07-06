#include "HelloWorldScene.h"

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
    rect = Director::getInstance()->getOpenGLView()->getVisibleRect();
    auto rootNode = CSLoader::createNode("MainScene.csb");

    addChild(rootNode);
    
    start_layout = static_cast<Layout*>(rootNode->getChildByName("start_layout"));
    game_layout = static_cast<Layout*>(rootNode->getChildByName("game_layout"));
    over_layout = static_cast<Layout*>(rootNode->getChildByName("over_layout"));
    hp_layout = static_cast<Layout*>(game_layout->getChildByName("hp_layout"));
    dong_layout = static_cast<Layout*>(game_layout->getChildByName("dong_layout"));
    start_bg = static_cast<ImageView*>(start_layout->getChildByName("start_bg"));
    game_bg = static_cast<ImageView*>(game_layout->getChildByName("game_bg"));
    game_title = static_cast<Sprite*>(start_layout->getChildByName("game_title"));
    hp_1 = static_cast<Sprite*>(hp_layout->getChildByName("hp_1"));
    hp_2 = static_cast<Sprite*>(hp_layout->getChildByName("hp_2"));
    hp_3 = static_cast<Sprite*>(hp_layout->getChildByName("hp_3"));
    start_btn = static_cast<Button*>(start_layout->getChildByName("start_btn"));
    over_btn = static_cast<Button*>(over_layout->getChildByName("over_btn"));
    score = static_cast<TextBMFont*>(game_layout->getChildByName("score"));
    bastText = static_cast<Text*>(over_layout->getChildByName("bastText"));
    scoreText = static_cast<Text*>(over_layout->getChildByName("scoreText"));
    
    over_layout->setPositionX(rect.size.width/2-over_layout->getContentSize().width/2);
    start_layout->setContentSize(Size(rect.size.width, rect.size.height));
    start_bg->setContentSize(Size(rect.size.width, rect.size.height));
    game_layout->setContentSize(Size(rect.size.width, rect.size.height));
    game_bg->setContentSize(Size(rect.size.width, rect.size.height));
    dong_layout->setPositionX(rect.size.width/2-dong_layout->getContentSize().width/2);
    hp_layout->setPositionX(rect.size.width/2-hp_layout->getContentSize().width/2*0.6);
    game_title->setPositionX(rect.size.width/2);
    start_btn->setPositionX(rect.size.width/2);
    
    start_btn->addTouchEventListener([&](Ref* sender, cocos2d::ui::Widget::TouchEventType type){
    switch (type)
    {
        case ui::Widget::TouchEventType::BEGAN:
            start_btn->setScale(0.9, 0.9);
            break;
        case ui::Widget::TouchEventType::ENDED:
            start_btn->setScale(1, 1);
            start_layout->setVisible(false);
            schedule(schedule_selector(HelloWorld::update), 0.7f);
            schedule(schedule_selector(HelloWorld::removeUpdate), 0.03f);
            break;
    }
    });
    start_btn->setPressedActionEnabled(true);
    
    over_btn->addTouchEventListener([&](Ref* sender, cocos2d::ui::Widget::TouchEventType type){
    switch (type)
    {
        case ui::Widget::TouchEventType::BEGAN:
            over_btn->setScale(0.9, 0.9);
            break;
        case ui::Widget::TouchEventType::ENDED:
            over_btn->setScale(1, 1);
            over_layout->setVisible(false);
            scoreNum = 0;
            hp = 3;
            schedule(schedule_selector(HelloWorld::update), 0.7f);
            schedule(schedule_selector(HelloWorld::removeUpdate), 0.03f);
            break;
    }
    });
    over_btn->setPressedActionEnabled(true);
    
    Vector<Node*> bg = dong_layout->getChildren();
    int _id = 0;
    for(auto a : bg)
    {
        Sprite* dong = static_cast<Sprite*>(a);
        dong->setName(std::to_string(_id));
        bgVec.pushBack(dong);
        _id++;
    }
    
    dong_layout->addTouchEventListener(CC_CALLBACK_2(HelloWorld::onSVTouch, this));
    
//    ScoreEffect* score0 = new ScoreEffect();
//    score0->moveScore(100);
//    score0->setPosition(rect.size.width/2, rect.size.height/2);
//    this->addChild(score0);

    return true;
}

void HelloWorld::update(float ft)
{
    timeNum++;
    if(timeNum <= 15)
    {
        monsterNum = 1;
    }else if(timeNum <= 30)
    {
        monsterNum = 2;
    }else
    {
        monsterNum = 2;
    }
    for(int i = 0; i < monsterNum; i++)
    {
        if(bgVec.size()>0)
        {
            int index = rand()%bgVec.size();
            this->createAni(bgVec.at(index)->getPositionX(), bgVec.at(index)->getPositionY() + 45, bgVec.at(index)->getName());
            chooseVec.pushBack(bgVec.at(index));
            bgVec.erase(index);
        }
    }
}

void HelloWorld::removeUpdate(float ft)
{
    for(int i = 0; i < monsterVec.size(); i++)
    {
        if(monsterVec.at(i)->drop)
        {
            if(monsterVec.at(i)->touch)
            {
                if(monsterVec.at(i)->monster_id == 1 || monsterVec.at(i)->monster_id == 2 || monsterVec.at(i)->monster_id == 3)
                {
                    //加分
                    printf("加分");
                    scoreNum+=100;
                    this->addScoreEffect(100, monsterVec.at(i)->getPositionX(), monsterVec.at(i)->getPositionY());
                }else if(monsterVec.at(i)->monster_id == 4)
                {
                    //减分减血
                    printf("减分减血");
                    MovieClip* effect = new MovieClip();
                    effect->playFile("garra.plist", "garra_sheet", 0, 2, 1, true);
                    effect->setAnchorPoint(Vec2(0.5,0.5));
                    effect->setPosition(rect.size.width/2, rect.size.height/2);
                    this->addChild(effect);
                    hp--;
                    scoreNum-=100;
                    this->addScoreEffect(-100, monsterVec.at(i)->getPositionX(), monsterVec.at(i)->getPositionY());
                }else if(monsterVec.at(i)->monster_id == 5)
                {
                    //加血
                    printf("加血");
                    hp++;
                    scoreNum+=100;
                    this->addScoreEffect(100, monsterVec.at(i)->getPositionX(), monsterVec.at(i)->getPositionY());
                }
            }else
            {
                if(monsterVec.at(i)->monster_id == 1 || monsterVec.at(i)->monster_id == 2 || monsterVec.at(i)->monster_id == 3)
                {
                    //减血
                    printf("减血");
                    MovieClip* effect = new MovieClip();
                    effect->playFile("garra.plist", "garra_sheet", 0, 2, 1, true);
                    effect->setAnchorPoint(Vec2(0.5,0.5));
                    effect->setPosition(rect.size.width/2, rect.size.height/2);
                    this->addChild(effect);
                    hp--;
                    scoreNum-=100;
                    this->addScoreEffect(-100, monsterVec.at(i)->getPositionX(), monsterVec.at(i)->getPositionY());
                }
            }
            for(int j = 0; j<chooseVec.size(); j++)
            {
                if(monsterVec.at(i)->bg_id == chooseVec.at(j)->getName())
                {
                    bgVec.pushBack(chooseVec.at(j));
                    chooseVec.erase(j);
                    break;
                }
            }
            monsterVec.at(i)->releasePage();
            monsterVec.erase(i);
            i--;
        }
    }
    if(scoreNum<=0)
    {
        scoreNum = 0;
    }
    score->setString("score:" + std::to_string(scoreNum));
    
    if(hp <= 0)
    {
        //结束
        hp_1->setVisible(false);
        hp_2->setVisible(false);
        hp_3->setVisible(false);
        for(int i = 0; i < monsterVec.size(); i++)
        {
            monsterVec.at(i)->releasePage();
        }
        monsterVec.clear();
        unschedule(schedule_selector(HelloWorld::update));
        unschedule(schedule_selector(HelloWorld::removeUpdate));
        if(scoreMax<scoreNum)
        {
            scoreMax = scoreNum;
        }
        scoreText->setString(std::to_string(scoreNum));
        bastText->setString(std::to_string(scoreMax));
        over_layout->setVisible(true);
    }else if(hp<=1)
    {
        hp_1->setVisible(true);
        hp_2->setVisible(false);
        hp_3->setVisible(false);
    }else if(hp<=2)
    {
        hp_1->setVisible(true);
        hp_2->setVisible(true);
        hp_3->setVisible(false);
    }else if(hp<=3)
    {
        hp_1->setVisible(true);
        hp_2->setVisible(true);
        hp_3->setVisible(true);
    }else
    {
        hp = 3;
        hp_1->setVisible(true);
        hp_2->setVisible(true);
        hp_3->setVisible(true);
    }
}

void HelloWorld::createAni(float _x, float _y, std::string bg_id)
{
    Monster* monster = new Monster();
    monster->setPosition(_x, _y);
    int num = rand()%100;
    if(num<50)
    {
        monster->setAni(rand()%3 + 1, bg_id);
    }else if(num<95)
    {
        monster->setAni(4, bg_id);
    }else
    {
        monster->setAni(5, bg_id);
    }
    
    monsterVec.pushBack(monster);
    dong_layout->addChild(monster);
}

void HelloWorld::onSVTouch(Ref *object, cocos2d::ui::Widget::TouchEventType type)
{
    if (type == cocos2d::ui::Widget::TouchEventType::ENDED)
    {
        auto pos = dong_layout->getTouchEndPosition();
        for(auto b : monsterVec)
        {
            Point point = dong_layout->convertToNodeSpace(pos);
            if(abs(point.x - b->getPositionX())<=70&&point.y - (b->getPositionY() - 45)<=60)
            {
                if(point.y>(b->getPositionY() - 45))
                {
                    b->touch = true;
                    b->drop = true;
                }
            }
        }
    }
    if (type == cocos2d::ui::Widget::TouchEventType::BEGAN)
    {
        auto pos = dong_layout->getTouchBeganPosition();
    }
    if (type==cocos2d::ui::Widget::TouchEventType::MOVED)
    {
        auto pos = dong_layout->getTouchMovePosition();
    }
}

void HelloWorld::addScoreEffect(int _score, float _x, float _y)
{
    TextBMFont* text0 = TextBMFont::create();
    if(_score > 0)
    {
        text0->setString("+" + std::to_string(_score));
    }else
    {
        text0->setString(std::to_string(_score));
    }
    text0->setFntFile("font/scoreFont.fnt");
    dong_layout->addChild(text0);
    text0->Node::setPosition(_x, _y);
    auto moveTo = MoveTo::create(0.2, Vec2(text0->getPositionX(), text0->getPositionY()+50));
    auto func = CallFunc::create([text0]()
    {
        text0->removeFromParent();
        text0->release();
    });
    auto seq = Sequence::create(moveTo, func, nullptr);
    text0->runAction(seq);
}
