//
//  BeginLayer.cpp
//  zuanshimicheng
//
//  Created by zhangrui on 14-2-12.
//
//



#include "Block.h"
#include "GameBody.h"
#include "GameLayer.h"



using namespace cocos2d;
using namespace CocosDenshion;
using namespace std;

Block::Block()
{
    move_time=1.3f;
}
Block::~Block()
{
    
}

bool Block::init(CCNode* layer_, CCSprite* baseblock_)
{
    if(!CCSprite::init()){
        return false;
    }
    
    layer=layer_;
    baseblock=baseblock_;
    
    game_mode=((GameLayer*)layer->getParent())->game_mode;
    
    if(game_mode==MODE_XIANSHI){
        float gamescore=((GameLayer*)layer->getParent())->gamebody->score;
        move_time-=gamescore*0.03f;
        if(move_time<0.2f){
            move_time=0.2f;
        }
    }
    
    
    ccColor3B color[9]={ccc3(234, 138, 28),ccc3(252, 291, 0),ccc3(76, 176, 52),ccc3(242, 175, 169),ccc3(228, 102, 79),ccc3(238, 155, 53),ccc3(255, 234, 69),ccc3(207, 100, 144),ccc3(79, 187, 161)};

    int rom=CCRANDOM_0_1()*9;
    int lastcolor=CCUserDefault::sharedUserDefault()->getIntegerForKey("lastcolor");
    while (lastcolor==rom) {
        rom=CCRANDOM_0_1()*9;
    }
    CCUserDefault::sharedUserDefault()->setIntegerForKey("lastcolor", rom);
    CCUserDefault::sharedUserDefault()->flush();
    
    
    block=CCSprite::create("game/block.png");
    layer->addChild(block,1);
    block->setScale(layer->getContentSize().width/block->getContentSize().width*0.5f);
    block->setTextureRect(baseblock->getTextureRect());
    block->setColor(color[rom]);

    if(baseblock->getTag()==0){
        block->setTag(1);
        block->setPosition(ccp(layer->getContentSize().width-block->boundingBox().size.width*0.5f,baseblock->getPosition().y+baseblock->boundingBox().size.height*1.1f));
        CCMoveTo* a1=CCMoveTo::create(move_time, ccp(0+block->boundingBox().size.width*0.5f, block->getPosition().y));
        CCMoveTo* a2=CCMoveTo::create(move_time, ccp(layer->getContentSize().width-block->boundingBox().size.width*0.5f, block->getPosition().y));
        CCSequence* l=(CCSequence*)CCSequence::create(a1,a2,NULL);
        CCRepeatForever* r=CCRepeatForever::create(l);
        block->runAction(r);
    }
    else{
        block->setTag(0);
        block->setPosition(ccp(0+block->boundingBox().size.width*0.5f, baseblock->getPosition().y+baseblock->boundingBox().size.height*1.1f));
        CCMoveTo* a1=CCMoveTo::create(move_time, ccp(0+block->boundingBox().size.width*0.5f, block->getPosition().y));
        CCMoveTo* a2=CCMoveTo::create(move_time, ccp(layer->getContentSize().width-block->boundingBox().size.width*0.5f, block->getPosition().y));
        CCSequence* l=(CCSequence*)CCSequence::create(a2,a1,NULL);
        CCRepeatForever* r=CCRepeatForever::create(l);
        block->runAction(r);
    }
    
    return true;
}

int Block::block_break(){
    block->stopAllActions();
    

    
    float x=baseblock->getPosition().x-block->getPosition().x;
    if(sqrtf((x/block->getScale())*(x/block->getScale()))>=block->getTextureRect().size.width){
        
        if(x>0){
            if(game_mode==MODE_SHUANGZI){
                block->runAction(CCMoveBy::create(0.5f, ccp(0, -block->boundingBox().size.height*2.0f)));
            }
            else{
                block->runAction(CCRotateTo::create(0.5f, -90));
            }
        }
        else{
            if(game_mode==MODE_SHUANGZI){
                block->runAction(CCMoveBy::create(0.5f, ccp(0, -block->boundingBox().size.height*2.0f)));
            }
            else{
                block->runAction(CCRotateTo::create(0.5f, 90));
            }
        }
        block->runAction(CCFadeTo::create(0.5f, 0));
        
        return 0;
    }
    if(sqrtf(x*x)<=layer->getContentSize().width*0.01f){
        
        block->setPosition(ccp(baseblock->getPosition().x, block->getPosition().y));
        
        {
            CCLabelTTF* word = CCLabelTTF::create("完美！", "ttf/word.ttf", block->boundingBox().size.height*1.0f);
            word->setAnchorPoint(ccp(0.5f,0.0f));
            layer->addChild(word, 10);
            word->setPosition(ccp(block->getPosition().x, block->getPosition().y+block->boundingBox().size.height*0.5f));
            word->setColor(block->getColor());
            if(game_mode==MODE_SHUANGZI){
                word->setScaleX(word->getScaleX()*2);
            }
            
            if(layer->getScaleY()<0){
                word->setScaleY(-1*word->getScaleY());
            }
            
            word->runAction(CCMoveBy::create(2.0f, ccp(0, word->getContentSize().height*6.0f)));
            word->runAction(CCFadeTo::create(2.0f, 0));
//            word->runAction(ActionRemove::create(2.0f));
            Add_function::removeDelay(word, 2.0f);
        }
        
        CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("sound/perfect.mp3", false);
        
        return 2;
    }
    
    CCSprite* blockpart=CCSprite::create("game/block.png");
    layer->addChild(blockpart,1);
    blockpart->setScale(block->getScale());
    blockpart->setTextureRect(block->getTextureRect());
    blockpart->setColor(block->getColor());
    CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("sound/good.mp3", false);
    
    if(x>0){
        block->setTextureRect(CCRect(block->getTextureRect().origin.x+x/block->getScale(), 0, block->getTextureRect().size.width-x/block->getScale(), block->getContentSize().height));
        block->setPosition(ccp(block->getPosition().x+x*0.5f,block->getPosition().y));
        
        blockpart->setTextureRect(CCRect(blockpart->getTextureRect().origin.x, 0, x/block->getScale(), blockpart->getContentSize().height));
        blockpart->setAnchorPoint(ccp(1, 0.0f));
        blockpart->setPosition(ccp(block->getPosition().x-block->boundingBox().size.width*0.5f, block->getPosition().y-block->boundingBox().size.height*0.5f));
        
        if(game_mode==MODE_SHUANGZI){
            blockpart->runAction(CCMoveBy::create(0.5f, ccp(0, -blockpart->boundingBox().size.height*2.0f)));
        }
        else{
            blockpart->runAction(CCRotateTo::create(0.5f, -90));
        }
        blockpart->runAction(CCFadeTo::create(0.5f, 0));
        
    }
    else if(x<0){
        block->setTextureRect(CCRect(block->getTextureRect().origin.x, 0, block->getTextureRect().size.width+x/block->getScale(), block->getContentSize().height));
        block->setPosition(ccp(block->getPosition().x+x*0.5f,block->getPosition().y));
        
        blockpart->setTextureRect(CCRect(block->getTextureRect().origin.x+block->getTextureRect().size.width, 0, -x/block->getScale(), blockpart->getContentSize().height));
        blockpart->setAnchorPoint(ccp(0.0f, 0.0f));
        blockpart->setPosition(ccp(block->getPosition().x+block->boundingBox().size.width*0.5f, block->getPosition().y-block->boundingBox().size.height*0.5f));
        
        if(game_mode==MODE_SHUANGZI){
            blockpart->runAction(CCMoveBy::create(0.5f, ccp(0, -blockpart->boundingBox().size.height*2.0f)));
        }
        else{
            blockpart->runAction(CCRotateTo::create(0.5f, 90));
        }
        blockpart->runAction(CCFadeTo::create(0.5f, 0));
    }
    
    if(game_mode==MODE_WANMEI){
        return 0;
    }
    return 1;
}
