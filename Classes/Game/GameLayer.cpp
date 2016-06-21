//
//  BeginLayer.cpp
//  zuanshimicheng
//
//  Created by zhangrui on 14-2-12.
//
//



#include "GameLayer.h"
#include "BeginLayer.h"
#include "OverLayer.h"


using namespace cocos2d;
using namespace CocosDenshion;
using namespace std;




CCScene* GameLayer::scene()
{
    CCScene *scene = CCScene::create();
    GameLayer *layer = GameLayer::create();
    scene->addChild(layer,-100);
    return scene;
}

GameLayer::GameLayer()
{
    int mode=CCUserDefault::sharedUserDefault()->getIntegerForKey("gamemode");
    if(mode==0){game_mode=MODE_WANMEI;}
    if(mode==1){game_mode=MODE_SHANDIAN;}
    if(mode==2){game_mode=MODE_NIUZHUAN;}
    if(mode==3){game_mode=MODE_SHUANGZI;}
    if(mode==4){game_mode=MODE_XIANSHI;}
    if(mode==5){game_mode=MODE_JINGDIAN;}
}

GameLayer::~GameLayer(){
}

bool GameLayer::init()
{
    if(!CCLayerColor::initWithColor(ccc4(255, 255, 255, 0))){
        return false;
    }
    this->setTouchEnabled(true);
    this->schedule(schedule_selector(GameLayer::tick));
    CCDirector::sharedDirector()->getScheduler()->setTimeScale(1.0f);
    
    auto touchListener = EventListenerTouchAllAtOnce::create();
    touchListener->onTouchesBegan = CC_CALLBACK_2(GameLayer::onTouchesBegan, this);
    
    
    CocosDenshion::SimpleAudioEngine::sharedEngine()->playBackgroundMusic("sound/bg.mp3", true);
//    CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("sound/begin.mp3", false);
    
    {
        CCSprite* bg=CCSprite::create("game/bg.png");
        this->addChild(bg,-1);
        bg->setScaleX(screenSize.width/bg->getContentSize().width);
        bg->setScaleY(screenSize.height/bg->getContentSize().height);
        bg->setPosition(ccpPercent(50, 50));
    }

    gamebody=new GameBody();
    gamebody->init(this, ccpPercent(50, 50));
    

    
    if(game_mode==MODE_SHUANGZI){
        gamebody->pane->setPosition(ccpPercent(25, 50));
        gamebody->pane->setScaleX(0.5f*screenSize.width/gamebody->pane->getContentSize().width);
        gamebody->setTag(1);
        
        gamebody2=new GameBody();
        gamebody2->init(this, ccpPercent(50, 50));
        gamebody2->pane->setPosition(ccpPercent(75, 50));
        gamebody2->pane->setScaleX(0.5f*screenSize.width/gamebody2->pane->getContentSize().width);
        gamebody2->setTag(2);
        
        CCSprite* line=CCSprite::create("game/black.jpg");
        this->addChild(line,50);
        line->setScaleX(0.002f*screenSize.width/line->getContentSize().width);
        line->setScaleY(screenSize.height/line->getContentSize().height);
        line->setPosition(ccpPercent(50, 50));
        line->setOpacity(200);
    }
    
    
    
    {
        CCSprite* numberback=CCSprite::create("game/scoreback.png");
        this->addChild(numberback,100);
        numberback->setScale(0.08*screenSize.height/numberback->getContentSize().height);
        numberback->setPosition(ccpPercent(50, 95));
        
        CCLabelTTF* word = CCLabelTTF::create("0", "ttf/word.ttf", numberback->getContentSize().height*0.5f);
        numberback->addChild(word, 100);
        word->setPosition(ccpContent(numberback->getContentSize(), 50, 50));
        word->setColor(ccRED);
        scoreshow=word;
    }
    
    {
        CCSprite* numberback=CCSprite::create("game/scoreback.png");
        this->addChild(numberback,100);
        numberback->setScale(0.08*screenSize.height/numberback->getContentSize().height);
        numberback->setPosition(ccpPercent(50, 95));
        
        CCLabelTTF* word = CCLabelTTF::create("0", "ttf/word.ttf", screenSize.height*0.025f);
        numberback->addChild(word, 100);
        word->setPosition(ccpContent(numberback->getContentSize(), 50, 50));
        word->setColor(ccGRAY);
        timeshow=word;
        numberback->setVisible(false);
        
        if(game_mode==MODE_XIANSHI){
            numberback->setVisible(true);
            numberback->setPosition(ccpPercent(65, 95));
            scoreshow->getParent()->setPosition(ccpPercent(35, 95));
        }
    }
    
    {
        tap=CCSprite::create("game/tap.png");
        this->addChild(tap,100);
        tap->setScale(0.25f*screenSize.width/tap->getContentSize().width);
        tap->setPosition(ccpPercent(50, 55));
    }
    
    return true;
}

void GameLayer::tick(float dt){
    char number[200];
    sprintf(number, "%.1f秒", gamebody->gametime);
    timeshow->setString(number);
}

void GameLayer::scoreadd(){
    {
        char number[200];
        sprintf(number, "%d",gamebody->score);
        if(game_mode==MODE_SHUANGZI){
            sprintf(number, "%d",gamebody->score+gamebody2->score);
        }
        scoreshow->setString(number);
        
        float s=scoreshow->getScale();
        CCScaleTo* a1=CCScaleTo::create(0.1f, s*1.2f);
        CCScaleTo* a2=CCScaleTo::create(0.1f, s*1.0f);
        CCScaleTo* a3=CCScaleTo::create(0.1f, s*1.1f);
        CCScaleTo* a4=CCScaleTo::create(0.1f, s*1.0f);
        CCSequence* l=(CCSequence*)CCSequence::create(a1,a2,a3,a4,NULL);
        scoreshow->runAction(l);
    }
}

void GameLayer::replaceScene(Ref* obj){

    CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("sound/button.mp3", false);
    CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(0.0f, GameLayer::scene(), ccBLACK));
    
}

#pragma mark-
#pragma mark 点击
void GameLayer::onTouchesBegan(const std::vector<cocos2d::Touch*>& pTouches,Event *pEvent){
    tap->setVisible(false);
    
    for(auto &item:pTouches){
        auto touch = item;
        CCPoint Point = this->convertTouchToNodeSpace(touch);
        
        gamebody->to_touch(Point);
        if(game_mode==MODE_SHUANGZI){
            gamebody2->to_touch(Point);
        }
    }
}
