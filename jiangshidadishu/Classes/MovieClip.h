//
//  MovieClip.hpp
//  wuxiaCocosProject-mobile
//
//  Created by 志强 on 2020/3/26.
//

#ifndef MovieClip_h
#define MovieClip_h

#include <stdio.h>
#include "cocos2d.h"

USING_NS_CC;

class MovieClip : public Node{

private:
    const char*plistName;
    Sprite*_mc = nullptr;
    Animate*_ani = nullptr;
    Vector<SpriteFrame*> _spriteFrame;
    int curFrameTolIndex;
    int loopTCount;
    int loopCount;
    int curIndex;
    bool isRemoved = false;
    
public:
    MovieClip();
    ~MovieClip();
public:
    float frameRate = 0.0f;
    
    
    std::function<void(MovieClip*)>  mcPlayEndFunc;
//    McPlayEndFunc*playEndFunc;
    
    void playFile(const char*plistPath,const char*keyName,int startIndex,int endIndex,int loop,bool isRemove);
    
    void changeAction(const char*keyName,int startIndex,int endIndex,int loop,bool isRemove);
    
    void releaseCache();
    
    void update(float dt);
    
    void initialize();
    
    void pauseAni();
    
};

#endif /* MovieClip_hpp */
