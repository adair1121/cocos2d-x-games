//
//  SoundCtl.cpp
//  wuxiaCocosProject-mobile
//
//  Created by 志强 on 2020/3/28.
//

#include "SoundCtl.h"
#include "SimpleAudioEngine.h"
#include "GlobalData.h"

using namespace CocosDenshion;
USING_NS_CC;

char*_bgPath;
SimpleAudioEngine*_audio = SimpleAudioEngine::getInstance();
SimpleAudioEngine*_effaudio = SimpleAudioEngine::getInstance();
static SoundCtl *s_pInstance = NULL; // pointer to singleton
SoundCtl* SoundCtl::getInstance()
{
    if (s_pInstance == NULL)
    {
        s_pInstance = new SoundCtl();
    }
    return s_pInstance;
}
void SoundCtl::preloadBgMusic(cocos2d::Vector<char*> list){
   //预加载背景音乐
    for(auto c : list){
        _audio->preloadBackgroundMusic(c);
    }
}
void SoundCtl::preloadEffMusic(cocos2d::Vector<char*> list){
    //预加载音效
    for(auto c : list){
        _effaudio->preloadEffect(c);
    }
}
void SoundCtl::unloadAss(const char* path){
    //从缓存中卸载声音。 如果完成声音并不再在游戏中使用它。 卸载以释放
    _audio->unloadEffect(path);
}
void SoundCtl::playBg(const char*bgPath,bool isloop){
    if(_bgPath){
        if(_bgPath == _bgPath){
            return;
        }
        _audio->stopBackgroundMusic();
    }
    _bgPath = _bgPath;
    _audio->playBackgroundMusic(bgPath, isloop);
//    GlobalData::getInstance()->musicBool = true;
}

void SoundCtl::playEffect(const char*effPath){
    if(GlobalData::getInstance()->soundBool)
    {
        _effaudio->playEffect(effPath, false, 1.0f, 1.0f, 1.0f);
    }
}
void SoundCtl::setBackgroundMusicVolume(float volume)
{
    _audio->setBackgroundMusicVolume(volume);
}
void SoundCtl::setEffMusicVolume(float volume)
{
    _effaudio->setEffectsVolume(volume);
}
void SoundCtl::stopBg()
{
//    GlobalData::getInstance()->musicBool = false;
    _audio->stopBackgroundMusic();
}

