//
//  GameScene.hpp
//  LudoGame
//
//  Created by Hari Krishna on 1/10/17.
//
//

#ifndef GameScene_hpp
#define GameScene_hpp

#include <stdio.h>
#include "cocos2d.h"
#include "cocos-ext.h"
#include "editor-support/cocosbuilder/CocosBuilder.h"

#include "Player.hpp"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace cocosbuilder;

#define CC_CREATE_LOADER_FOR_CCBI(loaderClassName, className)\
class loaderClassName : public LayerLoader {\
public:\
CCB_STATIC_NEW_AUTORELEASE_OBJECT_METHOD(loaderClassName, loader);\
protected:\
CCB_VIRTUAL_NEW_AUTORELEASE_CREATECCNODE_METHOD(className);\
};

enum class GameState {
    ROLL = 0,
    WAITING = 1,
    DONE = 2,
    OVER = 3
};

class GameScene : public cocos2d::Layer, public CCBMemberVariableAssigner,
    public CCBSelectorResolver {
    private:
        static NodeLoaderLibrary* _nodeLoaderLibrary;
        std::map<int, Player*> players;
        int playerCount;
        int curPlayer;
        int curDiceValue;
        void advancePlayer();
        bool processPlayerWithDiceValue(int player, int dice);
        bool rollDice();
        bool onTurnEnd();
        void performMove(Player* p, int dice, int index);
        bool hasMoreRoll();
        GameState gameState;
    public:
        static std::vector<int> specialPoints;
        static std::vector<int> startPoints;
        static std::vector<std::vector<int>> movementPoints;
        static int homePlaceCount;
        static std::vector<Color3B> playerColors;
        
        static NodeLoaderLibrary* GameNodeLoaderLibrary();
        static Node* CCBNodeFromFile(std::string ccbName);

        static Scene* createScene();
        virtual bool init() override;
        void initEverything(int playerCount);
        void onDiceRoll(Ref*, Control::EventType);
        void onMarker1(Ref*, Control::EventType);
        void onMarker2(Ref*, Control::EventType);
        void onMarker3(Ref*, Control::EventType);
        void onMarker4(Ref*, Control::EventType);
        void onMarkerSelected(int marker);
        void animateDice(std::string color);

        GameScene();
        ~GameScene();
        CCB_STATIC_NEW_AUTORELEASE_OBJECT_WITH_INIT_METHOD(GameScene, create);
        virtual SEL_MenuHandler onResolveCCBCCMenuItemSelector(Ref *pTarget, const char* pSelectorName) override;
        virtual Control::Handler onResolveCCBCCControlSelector(Ref *pTarget, const char* pSelectorName) override;
        virtual bool onAssignCCBMemberVariable(Ref* pTarget, const char* pMemberVariableName, Node* pNode) override;
        virtual bool onAssignCCBCustomProperty(Ref* target, const char* pMemberVariableName, const cocos2d::Value& value) override;
};


#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
namespace std {
    template <typename T> inline string to_string(T value) {
        ostringstream os;
        os << value;
        return os.str();
    }
}
#endif


CC_CREATE_LOADER_FOR_CCBI(GameSceneLoader, GameScene);
#endif /* GameScene_hpp */
