#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"

#include "Box2D/Box2D.h"

class HelloWorldHud : public cocos2d::CCLayer
{
	public:
		cocos2d::CCLabelTTF *label;
		void numCollectedChanged(int numCollected);

		CREATE_FUNC(HelloWorldHud);
};

class HelloWorld : public cocos2d::CCLayer
{

public:
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
	cocos2d::CCTMXLayer *_meta;
	cocos2d::CCTMXTiledMap *_tileMap;
	cocos2d::CCTMXLayer *_background;
	cocos2d::CCSprite *_player;
	cocos2d::CCTMXLayer *_foreground;
	int _numCollected;
	HelloWorldHud *_hud;

    virtual bool init();  
	void setViewpointCenter(cocos2d::CCPoint position);
	cocos2d::CCPoint tileCoordForPosition(cocos2d::CCPoint position);
	void setPlayerPosition(cocos2d::CCPoint position);
	//void registerWithTouchDispatcher();
	void ccTouchesEnded(cocos2d::CCSet* touches, cocos2d::CCEvent* event);

    // there's no 'id' in cpp, so we recommand to return the exactly class pointer
    static cocos2d::CCScene* scene();
    
    // a selector callback
    void menuCloseCallback(CCObject* pSender);

    // implement the "static node()" method manually
    CREATE_FUNC(HelloWorld);
};

#endif  // __HELLOWORLD_SCENE_H__