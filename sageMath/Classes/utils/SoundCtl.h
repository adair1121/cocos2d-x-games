//
//  SoundCtl.hpp
//  wuxiaCocosProject-mobile
//
//  Created by 志强 on 2020/3/28.
//

#ifndef SoundCtl_h
#define SoundCtl_h
#include "cocos2d.h"
#include <stdio.h>
#define SC() SoundCtl::getInstance()

USING_NS_CC;

class SoundCtl{
public:
    bool musicBool;
public:
    static SoundCtl *getInstance();
    
    void stopBg();
    
    void playBg(const char*bgPath,bool isloop);
    
    void playEffect(const char*effPath);
    
    void preloadBgMusic(cocos2d::Vector<char*> list);
    
    void preloadEffMusic(cocos2d::Vector<char*> list);
    
    void unloadAss(const char* path);
    
    void setBackgroundMusicVolume(float volume);
    
    void setEffMusicVolume(float volume);
};

#endif /* SoundCtl_hpp */
