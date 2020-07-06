//
//  MovieClip.cpp
//  wuxiaCocosProject-mobile
//
//  Created by 志强 on 2020/3/26.
// 
//

#include "MovieClip.h"

MovieClip::MovieClip()
{
    this->initialize();
}
MovieClip::~MovieClip()
{
    
}
void MovieClip::initialize()
{
    frameRate = 0.0f;
    curFrameTolIndex = 0;
    loopTCount = 0;
    loopCount = 0;
    plistName = "";
    curIndex = 0;
    isRemoved = false;
    this->setScale(1);
}
void MovieClip::playFile(const char*plistPath,const char*keyName,int startIndex,int endIndex,int loop,bool isRemove){
    const char*_plist = plistPath;
    if(_ani && _mc && !isRemove){
        if(_plist == plistName){
            //当前请求的动作已经在播放
            return;
        }
    }
    std::string _path(_plist);
    std::string _newpath = "ani/"+_path;
    plistName = _plist;
    //将纹理提前加入精灵针缓存
    if(!SpriteFrameCache::getInstance()->getSpriteFrameByName(_newpath))
    {
        SpriteFrameCache::getInstance() -> addSpriteFramesWithFile(_newpath.c_str());
    }
    changeAction(keyName, startIndex, endIndex,loop,isRemove);
    
}

void MovieClip::update(float dt)
{
    int index = _ani->getCurrentFrameIndex();
    if(curIndex == index){return;}
    curIndex = index;
    if(curIndex >= curFrameTolIndex){
        loopCount += 1;
        if(loopCount >= loopTCount && !isRemoved){
            isRemoved = true;
            this->releaseCache();
            if(mcPlayEndFunc){
                mcPlayEndFunc(this);
            }
        } 
    }
}
void MovieClip::changeAction(const char*keyName,int startIndex,int endIndex,int loop,bool isRemove){
    if(_mc && _ani){
        _spriteFrame.clear();
        _mc->stopAction(_ani);
        this->unscheduleUpdate();
    }
    //创建一个精灵用于显示动画
    if(!_mc){
//        char*firstStr;
//        sprintf(firstStr, "%d.png",startIndex);
        std::string firstStr = StringUtils::format("%d",startIndex)+".png";
        std::string startKey(keyName);
        startKey += firstStr;
        _mc = Sprite::createWithSpriteFrameName(startKey);
        this->addChild(_mc);
    }
    //建立
//    Vector<SpriteFrame*> mSprite;
    SpriteFrame* frame = nullptr;
//    char str[100] = {0};
//    int*si = startIndex;
    for(int i = startIndex;i<endIndex +1;i++){
        std::string key = keyName;
//        key +="%d.png";
//        sprintf(str, key.c_str(),i);
        key += StringUtils::format("%d",i)+".png";
        frame = SpriteFrameCache::getInstance() ->getSpriteFrameByName(key);
        _spriteFrame.pushBack(frame);
    }
    float _frameRate = 0.08f;
    if(abs(frameRate) > 0.000001)
    {
        _frameRate = frameRate;
    }
    auto mAnimation2 = Animation::createWithSpriteFrames(_spriteFrame,_frameRate);
    mAnimation2->setLoops(loop);//设置循环(-1表示无限)
    if(loop != -1 && isRemove){
        loopTCount = loop;
        loopCount = 0;
        curFrameTolIndex = endIndex - startIndex ;
        mAnimation2->setRestoreOriginalFrame(false);//设置是否在动画播放结束恢复到第一帧
        this->scheduleUpdate();
    }else{
        mAnimation2->setRestoreOriginalFrame(true);//设置是否在动画播放结束恢复到第一帧
    }
    _ani = Animate::create(mAnimation2);
    _mc ->runAction(_ani);
}
void MovieClip::releaseCache(){
//    AnimationCache::destroyInstance();
//    SpriteFrameCache::getInstance()->removeUnusedSpriteFrames();
    this->unscheduleUpdate();
//    _mc->release();
//    _frameCache->release();
    _spriteFrame.clear();
    if(_mc)
    {
        _mc->removeFromParent();
        _mc = nullptr;
    }
    SpriteFrameCache::getInstance()->removeSpriteFrameByName(plistName);
    _ani = nullptr;
//    _frameCache = nullptr;
    if(this->getParent())
    {
        this->removeFromParent();
    }
    
    
//    CC_SAFE_DELETE(_mc);
//    this->removeFromParent();
}
//void MovieClip::pauseAni()
//{
//    _mc->stopAction(_ani);
//}

