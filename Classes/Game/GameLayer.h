//
//  BeginLayer.h
//  zuanshimicheng
//
//  Created by zhangrui on 14-2-12.
//
//

#ifndef __zuanshimicheng__GameLayer__
#define __zuanshimicheng__GameLayer__

#include "cocos2d.h"
#include "GameHelper.h"
#include "Add_function.h"
#include "GameBody.h"
#include "SaveIndex.h"


class GameLayer : public cocos2d::CCLayerColor
{
public:
    static cocos2d::CCScene* scene();
    
    virtual bool init();
    GameLayer();
    ~GameLayer();
    
    void tick(float dt);
    
    void scoreadd();

public:
    
    CREATE_FUNC(GameLayer);
    void replaceScene(Ref* obj);
    
//    void ccTouchesBegan(CCSet *pTouches,CCEvent *pEvent);
    virtual void onTouchesBegan(const std::vector<cocos2d::Touch*>& pTouches,Event *pEvent);

    
public:
    GameBody* gamebody;
    
    GameBody* gamebody2;
    
    CCLabelTTF* scoreshow;
    CCLabelTTF* timeshow;
    
    GAME_MODE game_mode;
    
    CCSprite* tap;
};

#endif /* defined(__zuanshimicheng__BeginLayer__) */
