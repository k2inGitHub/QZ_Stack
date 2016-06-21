//
//  BeginLayer.cpp
//  zuanshimicheng
//
//  Created by zhangrui on 14-2-12.
//
//



#include "BeginLayer.h"
#include "SaveIndex.h"
#include "HLAdManagerWrapper.hpp"

using namespace cocos2d;
using namespace CocosDenshion;
using namespace std;




CCScene* BeginLayer::scene()
{
    CCScene *scene = CCScene::create();
    BeginLayer *layer = BeginLayer::create();
    scene->addChild(layer,-100);
    return scene;
}

BeginLayer::BeginLayer()
{

}
BeginLayer::~BeginLayer(){
}

bool BeginLayer::init()
{
    if(!CCLayerColor::initWithColor(ccc4(255, 255, 255, 0))){
        return false;
    }
    
    
    CocosDenshion::SimpleAudioEngine::sharedEngine()->playBackgroundMusic("sound/bg.mp3", true);

    
    CCSprite* title=CCSprite::create("game/title.png");
    this->addChild(title);
    title->setScale(0.18*screenSize.height/title->getContentSize().height);
    title->setPosition(ccpPercent(50, 88));
    
    {
        float s=title->getScale();
        CCScaleTo* a1=CCScaleTo::create(1.0f, s*1.2f);
        CCScaleTo* a2=CCScaleTo::create(1.0f, s);
        CCRepeatForever* r=CCRepeatForever::create((CCSequence*)CCSequence::create(a1,a2,NULL));
        title->runAction(r);
    }
    
    
    {
        CCSprite* bg=CCSprite::create("game/bg.png");
        this->addChild(bg,-1);
        bg->setScaleX(screenSize.width/bg->getContentSize().width);
        bg->setScaleY(screenSize.height/bg->getContentSize().height);
        bg->setPosition(ccpPercent(50, 50));
    }
    
    CCSprite* ground=CCSprite::create("game/ground.png");
    this->addChild(ground);
    ground->setScaleX(screenSize.width/ground->getContentSize().width);
    ground->setScaleY(0.05*screenSize.height/ground->getContentSize().height);
    ground->setPosition(ccp(screenSize.width*0.5f, DOWN_HEIGHT+ground->boundingBox().size.height*0.5f));
    
    CCSprite* down=CCSprite::create("game/down.png");
    this->addChild(down);
    down->setScaleX(0.53*screenSize.width/down->getContentSize().width);
    down->setScaleY(0.1*screenSize.height/down->getContentSize().height);
    down->setPosition(ccp(screenSize.width*0.5f, ground->getPosition().y+ground->boundingBox().size.height*0.5f+down->boundingBox().size.height*0.5f));
    
    
    ccColor3B color[9]={ccc3(234, 138, 28),ccc3(252, 291, 0),ccc3(76, 176, 52),ccc3(242, 175, 169),ccc3(228, 102, 79),ccc3(238, 155, 53),ccc3(255, 234, 69),ccc3(207, 100, 144),ccc3(79, 187, 161)};
    
    for(int i=0; i<9; i++){
        
        
        CCMenuItemImage *button = CCMenuItemImage::create(
                                                          "game/block.png",
                                                          "game/block.png",
                                                          CC_CALLBACK_1(BeginLayer::enter_game, this)
                                                          );
        CCMenu* Menu = CCMenu::create(button, NULL);
        Menu->setPosition(CCPointZero);
        this->addChild(Menu,1);
        button->setScaleX((0.5-i*0.02)*screenSize.width/button->getContentSize().width);
        button->setScaleY(0.05*screenSize.height/button->getContentSize().height);
        button->setPosition(ccp(screenSize.width*0.5f, (down->getPosition().y+down->boundingBox().size.height*0.5f+button->boundingBox().size.height*0.6f)+i*button->boundingBox().size.height*1.1f));
        button->setTag(i);
        button->setColor(color[i]);
        
        
        //        Add_function::set_MenuItemImage_SelectedImage_Scale(button, 1.0f);
        
        if(i<6){
            char* name;
            if(i==0){name=(char*)"完美模式";}
            if(i==1){name=(char*)"多开宝模式";}
            if(i==2){name=(char*)"绿色模式";}
            if(i==3){name=(char*)"粉色模式";}
            if(i==4){name=(char*)"多开模式";}
            if(i==5){name=(char*)"双开模式";}
            CCLabelTTF* word = CCLabelTTF::create(name, "ttf/word.ttf", screenSize.height*0.1f);
            button->addChild(word,10);
            word->setAnchorPoint(ccp(0.5f,0.5f));
            word->setScale(0.8*button->getContentSize().height/word->getContentSize().height);
            word->setScaleX(word->getScaleY()*button->getScaleY()/button->getScaleX());
            word->setPosition(ccpContent(button->getContentSize(), 50, 50));
            word->setColor(ccc3(1, 1, 1));
        }
        
        {
            float movex=screenSize.width*(0.02+0.02*(CCRANDOM_0_1()*100/100.0f));
            if(i%2==0){
                movex=-movex;
            }
            if(i==0){movex=0;}
            button->setPosition(ccp(button->getPosition().x+movex, button->getPosition().y));
            
            float t=2+0.15f*CCRANDOM_0_1()*100/100.0f;
            movex=screenSize.width*(0.01+0.02*(CCRANDOM_0_1()*100/100.0f));
            if(i%2==0){
                movex=-movex;
            }
            CCMoveBy* a1=CCMoveBy::create(t, ccp(movex,0));
            CCMoveBy* a2=CCMoveBy::create(t, ccp(-movex,0));
            CCSequence* l=(CCSequence*)CCSequence::create(a1,a2,NULL);
            CCRepeatForever* r=CCRepeatForever::create(l);
            button->runAction(r);
        }
        
        if(i==8){
            button->setPosition(button->boundingBox().size.width*0.5f,button->getPosition().y);
            CCMoveTo* a1=CCMoveTo::create(2.0f, ccp(screenSize.width-button->boundingBox().size.width*0.5f,button->getPosition().y));
            CCMoveTo* a2=CCMoveTo::create(2.0f, ccp(button->boundingBox().size.width*0.5f,button->getPosition().y));
            CCSequence* l=(CCSequence*)CCSequence::create(a1,a2,NULL);
            CCRepeatForever* r=CCRepeatForever::create(l);
            button->runAction(r);
        }
    }

    HLAdManagerWrapper::showUnsafeInterstitial();
    
    return true;
}

void BeginLayer::tick(float dt){
    

    
    
}

void BeginLayer::enter_game(Ref* obj){
    int tag=((Node *)obj)->getTag();
    if(tag<6){
        CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("sound/button.mp3", false);
        CCUserDefault::sharedUserDefault()->setIntegerForKey("gamemode", tag);
        CCUserDefault::sharedUserDefault()->flush();
        CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(1.0f, GameLayer::scene(), ccWHITE));
        
        CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("sound/button.mp3", false);
    }
}

void BeginLayer::enter_gamecenter(){
    CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("sound/button.mp3", false);
}

void BeginLayer::edit_sound(){
//    CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("sound/button.mp3", false);
//    if(CocosDenshion::SimpleAudioEngine::sharedEngine()->getBackgroundMusicVolume()==1){
//        CocosDenshion::SimpleAudioEngine::sharedEngine()->setBackgroundMusicVolume(0);
//        CocosDenshion::SimpleAudioEngine::sharedEngine()->setEffectsVolume(0);
//        
//        Add_function::setTextureByFile((CCSprite*)sound_button->getNormalImage(), "begin/nosound.png");
//        //      Add_function::setTextureByFile((CCSprite*)sound_button->getSelectedImage(), "ui/begin/nosound.png");
//    }
//    else{
//        CocosDenshion::SimpleAudioEngine::sharedEngine()->setBackgroundMusicVolume(1);
//        CocosDenshion::SimpleAudioEngine::sharedEngine()->setEffectsVolume(1);
//        
//        Add_function::setTextureByFile((CCSprite*)sound_button->getNormalImage(), "begin/sound.png");
//        //      Add_function::setTextureByFile((CCSprite*)sound_button->getSelectedImage(), "ui/begin/sound1.png");
//    }
}


