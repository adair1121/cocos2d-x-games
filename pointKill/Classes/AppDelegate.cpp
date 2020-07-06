#include "AppDelegate.h"
#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;
using namespace CocosDenshion;
AppDelegate::AppDelegate() {

}

AppDelegate::~AppDelegate() 
{
}

//if you want a different context,just modify the value of glContextAttrs
//it will takes effect on all platforms
void AppDelegate::initGLContextAttrs()
{
    //set OpenGL context attributions,now can only set six attributions:
    //red,green,blue,alpha,depth,stencil
    GLContextAttrs glContextAttrs = {8, 8, 8, 8, 24, 8};

    GLView::setGLContextAttrs(glContextAttrs);
}

bool AppDelegate::applicationDidFinishLaunching() {
    // initialize director
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();
    if(!glview) {
        glview = GLViewImpl::createWithRect("pointKill", Rect(0, 0, 750, 1334));
        director->setOpenGLView(glview);
    }
    auto winSize = Size(750, 1334);//设计分辨率
    auto screenSize = glview->getFrameSize();//屏幕分辨率，
    
    float widthRate = screenSize.width / winSize.width;
    float heightRate = screenSize.height / winSize.height;
    if (widthRate < heightRate)
    {
        //说明设计分辨率宽度偏大 横屏
        //这时候我们让高度适配，宽度裁剪掉。也就是图片宽度会超出屏幕外
        winSize.width = screenSize.width/heightRate;
        director->getOpenGLView()->setDesignResolutionSize(winSize.width, winSize.height, ResolutionPolicy::FIXED_HEIGHT);
    }
    else
    {
        //说明设计分辨率高度偏大 竖屏
        //这时候我们让宽度适配，高度裁剪掉。也就是图片高度会超出屏幕外
        winSize.height = screenSize.height/widthRate;
        director->getOpenGLView()->setDesignResolutionSize(winSize.width, winSize.height, ResolutionPolicy::FIXED_WIDTH);
    }
//    director->getOpenGLView()->setDesignResolutionSize(750, 1334, ResolutionPolicy::fix);

    // turn on display FPS
//    director->setDisplayStats(true);

    // set FPS. the default value is 1.0/60 if you don't call this
    director->setAnimationInterval(1.0 / 60);

    FileUtils::getInstance()->addSearchPath("res");

    // create a scene. it's an autorelease object
    auto scene = HelloWorld::createScene();

    // run
    director->runWithScene(scene);

    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground() {
    Director::getInstance()->stopAnimation();

    // if you use SimpleAudioEngine, it must be pause
     SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
    Director::getInstance()->startAnimation();

    // if you use SimpleAudioEngine, it must resume here
     SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
}
