//
//  BeginLayer.h
//  zuanshimicheng
//
//  Created by zhangrui on 14-2-12.
//
//

#ifndef __zuanshimicheng__Block__
#define __zuanshimicheng__Block__

#include "cocos2d.h"
#include "GameHelper.h"
#include "Add_function.h"
#include "SaveIndex.h"

class GameData;
class Block : public CCSprite
{
public:
    
    virtual bool init(CCNode* layer_, CCSprite* baseblock_);
    Block();
    ~Block();
    
    
    int block_break();
    
public:

    CCNode* layer;

    
    CCSprite* baseblock;
    CCSprite* block;
    
    
    GAME_MODE game_mode;
    
    float move_time=1.3f;
};

#endif /* defined(__zuanshimicheng__BeginLayer__) */
