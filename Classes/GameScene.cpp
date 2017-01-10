//
//  GameScene.cpp
//  LudoGame
//
//  Created by Hari Krishna on 1/10/17.
//
//

#include "GameScene.hpp"

NodeLoaderLibrary* GameScene::_nodeLoaderLibrary = nullptr;

NodeLoaderLibrary* GameScene::GameNodeLoaderLibrary() {
    NodeLoaderLibrary* ccNodeLoaderLibrary = nullptr;
    if(ccNodeLoaderLibrary == nullptr) {
        ccNodeLoaderLibrary = NodeLoaderLibrary::newDefaultNodeLoaderLibrary();

        ccNodeLoaderLibrary->registerNodeLoader("GameScene", GameSceneLoader::loader());
    }
    return ccNodeLoaderLibrary;
}

Node* GameScene::CCBNodeFromFile(std::string ccbName) {
    Node* ret = nullptr;
    if (_nodeLoaderLibrary == nullptr) {
        _nodeLoaderLibrary = GameNodeLoaderLibrary();
        _nodeLoaderLibrary->retain();
    }
    CCBReader* reader = new CCBReader(_nodeLoaderLibrary);
    reader->autorelease();
    ret = reader->readNodeGraphFromFile(ccbName.c_str());
    return ret;
}

int GameScene::homePlaceCount = 6;
std::vector<int> GameScene::specialPoints =  {12, 25, 38, 51};
std::vector<int> GameScene::startPoints =  {4, 17, 30, 43};
std::vector<std::vector<int>> GameScene::movementPoints = {
    {4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 1, 2, 53, 54, 55, 56, 57, 58},
    {17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 59, 60, 61, 62, 63, 64},
    {30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 65, 66, 67, 68, 69, 70},
    {43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 71, 72, 73, 74, 75, 76}
};
std::vector<Color3B> GameScene::playerColors = {
    Color3B(255,   0,   0),
    Color3B(0,   255,   0),
    Color3B(255,   255,   0),
    Color3B(0,   0,   255)
};

Scene* GameScene::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();

    // 'layer' is an autorelease object
    auto layer = GameScene::CCBNodeFromFile("ccbi/GameScene.ccbi");

    GameScene* mainLayer = static_cast<GameScene*>(layer);
    mainLayer->initEverything(2);

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

GameScene::GameScene() {

}

GameScene::~GameScene() {

}

bool GameScene::init() {
    return Layer::init();
}

void GameScene::initEverything(int pCount) {
    for (int i = 1; i < 77; i++) {
        std::string str = std::to_string(i);
        Node* child = this->getChildByName(str);
        if (child == nullptr) {
            CCLOG("not found %s", str.c_str());
        }
    }

    Node* centerNodes = this->getChildByName("centerNodes");
    for (auto child : centerNodes->getChildren()) {
        child->setColor(Color3B::GRAY);
    }

    for (int j = 0; j < movementPoints.size(); j++) {
        std::vector<int> points = movementPoints.at(j);
        Color3B color = playerColors.at(j);
        int startPoint = points.at(0);
        auto child = this->getChildByName(std::to_string(startPoint));
        child->setColor(color);
        Sprite *starSprite = Sprite::createWithSpriteFrameName("star.png");
        starSprite->setOpacity(180);
        child->addChild(starSprite);
        starSprite->setScale(0.75);
        starSprite->setPosition(child->getContentSize() / 2);

        child = this->getChildByName(std::to_string(specialPoints.at(j)));
        child->setColor(Color3B::GRAY);
        starSprite = Sprite::createWithSpriteFrameName("star.png");
        child->addChild(starSprite);
        starSprite->setScale(0.75);
        starSprite->setPosition(child->getContentSize() / 2);
        for (int i = 1; i < homePlaceCount; i++) {
            auto child = this->getChildByName(std::to_string(points.at(points.size() - 1 - i)));
            child->setColor(color);
        }
    }

    for (int i = 0 ; i < 4; i++) {
        auto playerNode = this->getChildByName("playerNode" + std::to_string(i));
        Player* player = Player::createPlayer(i, playerNode);
        players[i] = player;
        player->retain();
    }
    playerCount = pCount;
    curPlayer = 0;
    gameState = GameState::ROLL;

    switch (playerCount) {
        case 2:
            players[1]->mainNode->setVisible(false);
            players[3]->mainNode->setVisible(false);
            break;
        case 3:
            players[3]->mainNode->setVisible(false);
            break;
        default:
            break;
    }

    auto btn  = static_cast<ControlButton*>(this->getChildByName("rollBtn"));
    btn->setZoomOnTouchDown(false);
}

void GameScene::onDiceRoll(Ref*, Control::EventType) {
    if (gameState != GameState::ROLL) return;
    std::vector<std::string> colors = {"blue", "red", "yellow", "green"};
    animateDice(colors.at(cocos2d::random(0, 3)));

    rollDice();
}

bool GameScene::hasMoreRoll() {
    if (curDiceValue == 6) {
        Player* curP = players.at(curPlayer);
        if (curP->canMove(curDiceValue)) {
            return true; // has rolled 6 and can move pawns
        }
    }
    return false;
}

bool GameScene::onTurnEnd() {
    gameState = GameState::ROLL;
    if (hasMoreRoll()) {
        rollDice();
    } else {
        advancePlayer();
    }
}

bool GameScene::rollDice() {
    gameState = GameState::WAITING;
    int diceRoll = cocos2d::random(1, 6);
    curDiceValue = diceRoll;
    CCLOG("dice roll %d", diceRoll);
    return processPlayerWithDiceValue(curPlayer, diceRoll);
}

void GameScene::performMove(Player *p, int dice, int index) {
    if (index == -1) return;
    int markerPosBefore = p->curPosition.at(index);
    p->moveMarker(dice, index);
    int markerPosAfter = p->curPosition.at(index);

    Node* marker = p->markerImages.at(index);
    int toIndex = movementPoints.at(p->index).at(markerPosAfter);
    CCLOG("moving node to %d", markerPosAfter);
    marker->retain();
    marker->removeFromParentAndCleanup(false);
    this->getChildByName(std::to_string(toIndex))->addChild(marker);
    marker->release();

    if (markerPosAfter == 56 && markerPosBefore != 56) {
        rollDice(); // finished one marker to the finish
    } else {
        onTurnEnd();
    }
}

bool GameScene::processPlayerWithDiceValue(int player, int dice) {
    Player* curP = players.at(player);
    if (curP->canMove(dice)) {
        if (curP->hasMultipleOptions(dice)) {
            CCLOG("wait for input of player for selecting option");
        } else {
            performMove(curP, dice, curP->getMovableMarkerIndex(dice));
        }
    } else {
        onTurnEnd();
    }
    return false;
}

void GameScene::advancePlayer() {
    CCLOG("Ended Turn");
    switch (playerCount) {
        case 1:
            CCLOG("cant play the game with one player");
            CCASSERT(false, "wth");
        case 2:
            curPlayer = (curPlayer + 2) % 4;
            break;
        case 3:
            curPlayer = (curPlayer + 1) % 4;
            if (curPlayer == 3)
                curPlayer = 0;
            break;
        case 4:
            curPlayer = (curPlayer + 1) % 4;
        default:
            break;
    }
}

void GameScene::onMarker1(cocos2d::Ref *, Control::EventType) {
    if (gameState != GameState::WAITING) return;
    onMarkerSelected(0);
}

void GameScene::onMarker2(cocos2d::Ref *, Control::EventType) {
    if (gameState != GameState::WAITING) return;
    onMarkerSelected(1);
}


void GameScene::onMarker3(cocos2d::Ref *, Control::EventType) {
    if (gameState != GameState::WAITING) return;
    onMarkerSelected(2);
}

void GameScene::onMarker4(cocos2d::Ref *, Control::EventType) {
    if (gameState != GameState::WAITING) return;
    onMarkerSelected(3);
}


void GameScene::onMarkerSelected(int marker) {
    if (gameState != GameState::WAITING) return;

    Player* p = this->players.at(curPlayer);
    if (p->isValidIndex(marker, curDiceValue)) {
        performMove(p, curDiceValue, marker);
    } else {
        CCLOG("invalid index");
    }
}

void GameScene::animateDice(std::string color) {
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("dice.plist");
    Node* diceLayer = this->getChildByName("diceLayer");
    diceLayer->removeAllChildren();

    Vector<SpriteFrame*> animFrames;
    char str[25];
    std::string tempsString;
    tempsString = "dice/" + color + "roll%01d.png";
    for(int i = 1; i <= 6; i++) {
        sprintf(str, tempsString.c_str(), i);
        animFrames.pushBack(SpriteFrameCache::getInstance()->getSpriteFrameByName(str));
    }
    auto image = Sprite::createWithSpriteFrame(animFrames.front());
    auto animation = Animation::createWithSpriteFrames(animFrames, 1.0f/12);
    image->runAction(Repeat::create(Animate::create(animation), CC_REPEAT_FOREVER));
    image->setAnchorPoint(Point(0.5,0.5));
    image->setPosition(diceLayer->getContentSize() / 2);
    diceLayer->addChild(image);
}

SEL_MenuHandler GameScene::onResolveCCBCCMenuItemSelector(Ref * pTarget, const char* pSelectorName) {

    //assign all menu item handlers to the respective methods here:

    //CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "onMenuTestClicked", ViewController::onMenuTestClicked);


    return NULL;
}

Control::Handler GameScene::onResolveCCBCCControlSelector(Ref * pTarget, const char* pSelectorName) {

    //assign all the control handlers to the respective methods here:
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onDiceRoll", GameScene::onDiceRoll);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onMarker1", GameScene::onMarker1);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onMarker2", GameScene::onMarker2);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onMarker3", GameScene::onMarker3);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onMarker4", GameScene::onMarker4);


    return NULL;
}

bool GameScene::onAssignCCBMemberVariable(cocos2d::Ref* pTarget, const char* pMemberVariableName, cocos2d::Node* pNode) {

    //assign all the instance variables to the respective variables of the class:

    //CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mBurstSprite", Sprite *, this->mBurstSprite);

    return false;
}

bool GameScene::onAssignCCBCustomProperty(cocos2d::Ref* target, const char* memberVariableName, const cocos2d::Value& value) {
    return false;
}
