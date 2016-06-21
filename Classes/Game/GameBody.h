//
//  BeginLayer.h
//  zuanshimicheng
//
//  Created by zhangrui on 14-2-12.
//
//

#ifndef __zuanshimicheng__GameBody__
#define __zuanshimicheng__GameBody__

#include "cocos2d.h"
#include "GameHelper.h"
#include "Add_function.h"


#include "Block.h"

class GameBody : public CCLayer
{
public:
    
    virtual bool init(CCNode* layer_, CCPoint p);
    GameBody();
    ~GameBody();
    
    
    void tick(float dt);
    
    
    void add_rank();
    

//    void ccTouchesBegan(CCSet *pTouches,CCEvent *pEvent);
    
    void to_touch(CCPoint Point);
    
public:

    
    CCNode* layer;
    CCSprite* pane;
    
    Block* current_block;
    
    int score=0;
    float gametime;
    
    bool gameover;
    
    GAME_MODE game_mode;
    
    CCSprite* rank;
    
    bool gamebegin;
    
};

#endif /* defined(__zuanshimicheng__BeginLayer__) */
