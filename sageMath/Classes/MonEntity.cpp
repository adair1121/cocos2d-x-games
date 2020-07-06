//
//  MonEntity.cpp
//  sageMath-mobile
//
//  Created by 志强 on 2020/5/21.
//

#include "MonEntity.h"


MonEntity::MonEntity(int num)
{
    mc = new MovieClip();
    this->addChild(mc);
    srand(int(time(0))+ rand());//置随机数种子
    oper = CCRANDOM_0_1()*3;
    mc->setScaleX(-1.0f);
    showNum = num;
    int endIndex = 0;
    if(oper == 0)
    {
        ass = "demon";
        mc->playFile((ass + ".plist").c_str(), (ass + "_walk_").c_str(), 1, 14, -1, false);
    }else if(oper == 1)
    {
        ass = "dragon";
        mc->playFile((ass + ".plist").c_str(), (ass + "_walk_").c_str(), 1, 18, -1, false);
    }else if(oper == 2)
    {
        ass = "drake";
        mc->playFile((ass + ".plist").c_str(), (ass + "_walk_").c_str(), 1, 18, -1, false);
    }else if(oper == 3)
    {
        ass = "drake";
        mc->playFile((ass + ".plist").c_str(), (ass + "_walk_").c_str(), 1, 6, -1, false);
    }else if(oper == 4)
    {
        ass = "dragon";
        mc->playFile((ass + ".plist").c_str(), (ass + "_walk_").c_str(), 1, 6, -1, false);
    }else if(oper == 5)
    {
        ass = "demon";
        mc->playFile((ass + ".plist").c_str(), (ass + "_walk_").c_str(), 1, 4, -1, false);
        
    }
//    mc ->setScale(2.0f);
    Sprite*bg = Sprite::create("assets/icon_shield.png");
    this->addChild(bg);
//    bg->setScale(2.0f);
    bg->setPosition(Vec2(0,140));
    
    auto myLabel = Label::createWithBMFont("assets/myFont.fnt", StringUtils::format("%d",num));
    this->addChild(myLabel);
    myLabel->setScale(0.5f);
    myLabel->setPosition(Vec2(20,140));
}
void MonEntity::changeAction(std::string actionstr)
{
    if(actionstr == "dash")
    {
//        auto delay = DelayTime::create(0.5f);
//        auto callback = CallFunc::create([&](){
//            changeAction("walk");
//        });
//        mc->runAction(Sequence::create(delay,callback, NULL));
    }
    if(oper == 0)
    {
        int endindex = 0;
        int loop = -1;
        bool isRemove = false;
        if(actionstr == "dash"){return;}
        else if(actionstr == "dead"){endindex = 3;loop = 1;isRemove = true;}
        else if(actionstr == "walk"){endindex = 14;loop = -1;}
        mc->changeAction( (ass + "_"+actionstr+"_").c_str(), 1, endindex, loop, false);
    }else if(oper == 1)
    {
        int endindex = 0;
        int loop = -1;
        bool isRemove = false;
        if(actionstr == "dash"){return;}
        else if(actionstr == "dead"){endindex = 2;loop = 1;isRemove = true;}
        else if(actionstr == "walk"){endindex = 18;loop = -1;}
        mc->changeAction( (ass + "_"+actionstr+"_").c_str(), 1, endindex, loop, false);
    }else if(oper == 2)
    {
       int endindex = 0;
       int loop = -1;
        bool isRemove = false;
       if(actionstr == "dash"){return;}
       else if(actionstr == "dead"){endindex = 2;loop = 1;isRemove = true;}
       else if(actionstr == "walk"){endindex = 18;loop = -1;}
        mc->changeAction( (ass + "_"+actionstr+"_").c_str(), 1, endindex, loop, false);
    }else if(oper == 3)
    {
        int endindex = 0;
        int loop = -1;
         bool isRemove = false;
        if(actionstr == "dash"){return;}
        else if(actionstr == "dead"){endindex = 6;loop = 1;isRemove = true;}
        else if(actionstr == "walk"){endindex = 6;loop = -1;}
        mc->changeAction( (ass + "_"+actionstr+"_").c_str(), 1, endindex, loop, false);
    }else if(oper == 4)
    {
       int endindex = 0;
       int loop = -1;
        bool isRemove = false;
       if(actionstr == "dash"){return;}
       else if(actionstr == "dead"){endindex = 6;loop = 1;isRemove = true;}
       else if(actionstr == "walk"){endindex = 6;loop = -1;}
        mc->changeAction( (ass + "_"+actionstr+"_").c_str(), 1, endindex, loop, false);
    }else if(oper == 5)
    {
        int endindex = 0;
        int loop = -1;
         bool isRemove = false;
        if(actionstr == "dash"){return;}
        else if(actionstr == "dead"){endindex = 6;loop = 1;isRemove = true;}
        else if(actionstr == "walk"){endindex = 4;loop = -1;}
        mc->changeAction( (ass + "_"+actionstr+"_").c_str(), 1, endindex, loop, false);
    }
}
int MonEntity::getRemainNum()
{
    return showNum;
}
MonEntity::~MonEntity()
{
    
}
