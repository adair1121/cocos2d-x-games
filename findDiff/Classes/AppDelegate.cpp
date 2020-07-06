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
    
    //获得实际屏幕的大小
    Size frameSize = glview->getFrameSize();
    Size winSize=Size(1334,750);
//    Size visibleSize = Director::getInstance()->getVisibleSize();
    //将宽和高做一个比，通过这个比，来具体的调整逻辑分辨率的大小
    float widthRate = frameSize.width/winSize.width;
    float heightRate = frameSize.height/winSize.height;
    
    float width = winSize.width;
    float height = winSize.height;
    if (widthRate > heightRate)
    {
        //逻辑的高度有点大了，把逻辑的高缩小到和宽度一样的比率
        width = winSize.width;
        height = winSize.height*heightRate/widthRate;
    }else{
        width = winSize.width*widthRate/heightRate;
        height = winSize.height;
    }
    
    if(!glview) {
        glview = GLViewImpl::createWithRect("FindDiff", Rect(0, 0, width, height));
        director->setOpenGLView(glview);
    }
    
    director->getOpenGLView()->setDesignResolutionSize(1334, 750, ResolutionPolicy::FIXED_HEIGHT);
    
    // turn on display FPS
    director->setDisplayStats(false);

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
