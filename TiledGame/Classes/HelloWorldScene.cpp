#include "HelloWorldScene.h"

using namespace cocos2d;



CCScene* HelloWorld::scene()
{
    CCScene * scene = NULL;
    do 
    {
        // 'scene' is an autorelease object
        scene = CCScene::create();
        CC_BREAK_IF(! scene);

        // 'layer' is an autorelease object
        HelloWorld *layer = HelloWorld::create();
        CC_BREAK_IF(! layer);

		HelloWorldHud* layerHud = HelloWorldHud::create();
		CC_BREAK_IF(! layerHud);

		scene->addChild(layer);

		scene->addChild(layerHud);

		layer->_hud = layerHud;
    } while (0);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{

    bool bRet = false;
    do 
    {
		_numCollected=0;
		label = CCLabelTTF::create("","Artial", 32);
        //////////////////////////////////////////////////////////////////////////
        // super init first
        //////////////////////////////////////////////////////////////////////////

        CC_BREAK_IF(! CCLayer::init());
		this->setTouchEnabled(true);
        //////////////////////////////////////////////////////////////////////////
        // add your codes below...
        //////////////////////////////////////////////////////////////////////////

        // 1. Add a menu item with "X" image, which is clicked to quit the program.

        // Create a "close" menu item with close icon, it's an auto release object.
        CCMenuItemImage *pCloseItem = CCMenuItemImage::create(
            "CloseNormal.png",
            "CloseSelected.png",
            this,
            menu_selector(HelloWorld::menuCloseCallback));
        CC_BREAK_IF(! pCloseItem);

        // Place the menu item bottom-right conner.
        pCloseItem->setPosition(ccp(CCDirector::sharedDirector()->getWinSize().width - 20, 20));

        // Create a menu with the "close" menu item, it's an auto release object.
        CCMenu* pMenu = CCMenu::create(pCloseItem, NULL);
        pMenu->setPosition(CCPointZero);
        CC_BREAK_IF(! pMenu);

        // Add the menu to HelloWorld layer as a child layer.
   //     this->addChild(pMenu, 1);

		_tileMap = CCTMXTiledMap::create("TileGameResources/TileMap.tmx");
		_background = _tileMap->layerNamed("Background");
		_foreground = _tileMap->layerNamed("Foreground");
		_meta = _tileMap->layerNamed("Meta");
		_meta->setVisible(false);
		this->addChild(_tileMap, 1);	

		CCTMXObjectGroup *objects = _tileMap->objectGroupNamed("Objects");
		//CCAssert(objects != NULL, "'Objects' object group not found");
	//	CCStringToStringDictionary* spawnPoint = objects->objectNamed("SpawnPoint");
	//	CCAssert(spawnPoint != NULL, "Spawn Point not Found!");
		CCDictionary* spawnPoint = objects->objectNamed("SpawnPoint");
		float x = ((CCString*)spawnPoint->objectForKey("x"))->intValue();
        float y = ((CCString*)spawnPoint->objectForKey("y"))->intValue();
		_player = CCSprite::create("TileGameResources/Player.png");
		_player->setPosition(ccp(x,y));
		this->addChild( _player,2);
		this->setViewpointCenter(_player->getPosition());


        // 2. Add a label shows "Hello World".

        // Create a label and initialize with string "Hello World".
        CCLabelTTF* pLabel = CCLabelTTF::create("Hello World", "Arial", 24);
        CC_BREAK_IF(! pLabel);

        // Get window size and place the label upper. 
        CCSize size = CCDirector::sharedDirector()->getWinSize();
        pLabel->setPosition(ccp(size.width / 2, size.height - 50));

        // Add the label to HelloWorld layer as a child layer.
        this->addChild(pLabel, 1);

        // 3. Add add a splash screen, show the cocos2d splash image.
        CCSprite* pSprite = CCSprite::create("HelloWorld.png");
        CC_BREAK_IF(! pSprite);

        // Place the sprite on the center of the screen
        pSprite->setPosition(ccp(size.width/2, size.height/2));

        // Add the sprite to HelloWorld layer as a child layer.
        this->addChild(pSprite, 0);

        bRet = true;
    } while (0);

    return bRet;
}

void HelloWorld::setViewpointCenter(CCPoint position)
{
 
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
 
    int x = MAX(position.x, winSize.width / 2);
    int y = MAX(position.y, winSize.height / 2);
	x = MIN(x, (_tileMap->getMapSize().width * _tileMap->getMapSize().width) 
        - winSize.width / 2);
    y = MIN(y, (_tileMap->getMapSize().height * _tileMap->getMapSize().height) 
        - winSize.height/2);
    CCPoint actualPosition = ccp(x, y);
 
    CCPoint centerOfView = ccp(winSize.width/2, winSize.height/2);
    CCPoint viewPoint = ccpSub(centerOfView, actualPosition);
	this->setPosition(viewPoint);
 
}

void HelloWorld::ccTouchesEnded(cocos2d::CCSet* touches, cocos2d::CCEvent* event)
{
	CCTouch* touch = (CCTouch*)( touches->anyObject() );
	CCPoint touchLocation = touch->locationInView();
	touchLocation = CCDirector::sharedDirector()->convertToGL(touchLocation);
	touchLocation = this->convertToNodeSpace(touchLocation);

	CCPoint playerPos = _player->getPosition();
    CCPoint diff = ccpSub(touchLocation, playerPos);
    if (abs(diff.x) > abs(diff.y)) {
        if (diff.x > 0) {
			playerPos.x += _tileMap->getTileSize().width;
        } else {
            playerPos.x -= _tileMap->getTileSize().width; 
        }    
    } else {
        if (diff.y > 0) {
            playerPos.y += _tileMap->getTileSize().height;
        } else {
            playerPos.y -= _tileMap->getTileSize().height;
        }
    }
 
	if (playerPos.x <= (_tileMap->getMapSize().width * _tileMap->getMapSize().width) &&
        playerPos.y <= (_tileMap->getMapSize().height * _tileMap->getMapSize().height) &&
        playerPos.y >= 0 &&
        playerPos.x >= 0 ) 
    {
		setPlayerPosition(playerPos);
    }
 
	this->setViewpointCenter(playerPos);
}

CCPoint HelloWorld::tileCoordForPosition(CCPoint position)
{
	int x = position.x / _tileMap->getTileSize().width;
	int y = ((_tileMap->getMapSize().height * _tileMap->getTileSize().height) - position.y) / _tileMap->getTileSize().height;
    return ccp(x, y);
}

void HelloWorld::setPlayerPosition(CCPoint position)
{
	CCPoint tileCoord = this->tileCoordForPosition(position);
	int tileGid = _meta->tileGIDAt(tileCoord);
	if (tileGid) {
		CCDictionary *properties = _tileMap->propertiesForGID(tileGid);
		if (properties) {
			CCString *collision = (CCString*)properties->valueForKey("Collidable");
	        if (collision && collision->compare("True")>-1) {
				return;
			}
			CCString *collectable  = (CCString*)properties->valueForKey("Collectable");
	        if (collectable && collectable->compare("True")>-1) {
				_numCollected++;
				_hud->numCollectedChanged(_numCollected);
				_meta->removeTileAt(tileCoord);
				_foreground->removeTileAt(tileCoord);
			}
		}
	}
	_player->setPosition( position);
}

void HelloWorldHud::numCollectedChanged(int numCollected) {
	const char *score = new char;
    label->setString( "0");
}

void HelloWorld::menuCloseCallback(CCObject* pSender)
{
    // "close" menu item clicked
    CCDirector::sharedDirector()->end();
}
