//
//  BeginLayer.cpp
//  zuanshimicheng
//
//  Created by zhangrui on 14-2-12.
//
//



#include "GameBody.h"
#include "GameLayer.h"
#include "OverMenu.h"

using namespace cocos2d;
using namespace CocosDenshion;
using namespace std;

GameBody::GameBody()
{
    score=0;
    gameover=false;
    gametime=15;
    gamebegin=false;
}
GameBody::~GameBody()
{
    
}

bool GameBody::init(CCNode* layer_, CCPoint p)
{
    if(!CCLayer::init()){
        return false;
    }
    this->setTouchEnabled(true);
    
    layer=layer_;
    layer->addChild(this);
    
    game_mode=((GameLayer*)layer)->game_mode;
    
    pane=CCSprite::create("game/pane.jpg");
    layer->addChild(pane);
    pane->setPosition(p);
    pane->setScale(screenSize.width/pane->getContentSize().width);
    pane->setOpacity(0);
    
    if(game_mode==MODE_NIUZHUAN){
        pane->setScaleY(pane->getScaleY()*-1);
    }
    
    CCSprite* ground=CCSprite::create("game/ground.png");
    pane->addChild(ground);
    ground->setScaleX(pane->getContentSize().width/ground->getContentSize().width);
    ground->setScaleY(0.05*pane->getContentSize().height/ground->getContentSize().height);
    ground->setPosition(ccp(pane->getContentSize().width*0.5f, pane->convertToNodeSpace(ccp(0,DOWN_HEIGHT)).y+ground->boundingBox().size.height*0.5f));
    
    CCSprite* down=CCSprite::create("game/down.png");
    pane->addChild(down);
    down->setScaleX(0.525f*pane->getContentSize().width/down->getContentSize().width);
    down->setScaleY(0.1*pane->getContentSize().height/down->getContentSize().height);
    down->setPosition(ccp(pane->getContentSize().width*0.5f, ground->getPosition().y+ground->boundingBox().size.height*0.5f+down->boundingBox().size.height*0.5f));
    
    
    CCSprite* baseblock=CCSprite::create("game/block.png");
    pane->addChild(baseblock,1);
    baseblock->setScale(pane->getContentSize().width/baseblock->getContentSize().width*0.5f);
    baseblock->setPosition(ccp(pane->getContentSize().width*0.5f, down->getPosition().y+down->boundingBox().size.height*0.5f-baseblock->boundingBox().size.height*0.5f));
    baseblock->setColor(ccc3(CCRANDOM_0_1()*255, CCRANDOM_0_1()*255, CCRANDOM_0_1()*255));
    baseblock->setTag(0);
    baseblock->setOpacity(0);
    
    
    Block* block=new Block();
    block->init(pane, baseblock);
    current_block=block;
    
    
    //闪电
    {
        rank=CCSprite::create("game/black.jpg");
        layer->addChild(rank,200);
        rank->setPosition(ccpPercent(50, 50));
        rank->setOpacity(0);
        
        if(game_mode==MODE_SHANDIAN){
            CCDelayTime* d=CCDelayTime::create(5.0f);
            CCCallFunc * f=CCCallFunc::create(this, callfunc_selector(GameBody::add_rank));
            CCSequence* l=(CCSequence*)CCSequence::create(d,f,NULL);
            rank->runAction(l);
        }
    }
    
    
    this->schedule(schedule_selector(GameBody::tick));

    return true;
}

void GameBody::tick(float dt){
    if(pane->getScaleY()>0)
    {
        CCPoint p=pane->convertToWorldSpace(current_block->block->getPosition());
        if(p.y>screenSize.height*0.7f){
            pane->setPosition(ccp(pane->getPosition().x,pane->getPosition().y-screenSize.height*0.002f));
        }
    }
    else{
        CCPoint p=pane->convertToWorldSpace(current_block->block->getPosition());
        if(p.y<screenSize.height*0.3f){
            pane->setPosition(ccp(pane->getPosition().x,pane->getPosition().y+screenSize.height*0.002f));
        }
    }
    
    if(game_mode==MODE_XIANSHI&&gamebegin==true){
        if(gameover==false){
            gametime-=dt;
            if(gametime<0){
                gametime=0;
                if(gameover==false){
                    gameover=true;
                    CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("sound/fail.mp3", false);
                    OverMenu* overmenu=new OverMenu();
                    overmenu->init(layer);
                }
            }
        }
    }
}


#pragma mark-
#pragma mark 闪电
void GameBody::add_rank(){
    
    rank->setOpacity(0);
    CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("sound/rank.mp3", false);
    
    if(gameover==false){
        CCFadeTo* a1=CCFadeTo::create(0.7f, 255);
        CCDelayTime* d=CCDelayTime::create(1.0f);
        CCFadeTo* a2=CCFadeTo::create(0.08f, 0);
        CCFadeTo* a3=CCFadeTo::create(0.08f, 255);
        CCRepeat* r=CCRepeat::create((CCSequence*)CCSequence::create(a2,a3,NULL), 5);
        CCFadeTo* a4=CCFadeTo::create(0.08f, 0);
        CCDelayTime* d1=CCDelayTime::create(2.0f);
        CCCallFunc * f=CCCallFunc::create(this, callfunc_selector(GameBody::add_rank));
        CCSequence* l=(CCSequence*)CCSequence::create(a1,d,r,a4,d1,f,NULL);
        rank->runAction(l);
    }
    
}


#pragma mark-
#pragma mark 点击


void GameBody::to_touch(CCPoint Point){
    gamebegin=true;
    
    if(gameover==true){
        return;
    }

            
    if(game_mode==MODE_SHUANGZI){
        if(Point.x<=screenSize.width*0.5f&&this->getTag()==2){
            return;
        }
        if(Point.x>screenSize.width*0.5f&&this->getTag()==1){
            return;
        }
    }
    
    
    int addscore=current_block->block_break();
    if(addscore!=0&&gametime>0){
        Block* block=new Block();
        block->init(pane, current_block->block);
        current_block=block;
        
        score+=addscore;
        ((GameLayer*)layer)->scoreadd();
    }
    else{
        CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("sound/fail.mp3", false);
        OverMenu* overmenu=new OverMenu();
        overmenu->init(layer);
        
        gameover=true;
    }
}
