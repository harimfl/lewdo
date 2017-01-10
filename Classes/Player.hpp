//
//  Player.hpp
//  LudoGame
//
//  Created by Hari Krishna on 1/10/17.
//
//

#ifndef Player_hpp
#define Player_hpp

#include <stdio.h>
#include "cocos2d.h"

USING_NS_CC;

class Player : public Node {
public:
    int index;
    std::vector<int> curPosition;
    std::vector<Node*> markerNodes;
    std::vector<Node*> markerImages;
    Node* mainNode;


    void init(int idx, Node* node);
    static Player* createPlayer(int idx, Node* node);
    bool canMove(int dice);
    bool isValidIndex(int idx, int dice);
    bool hasMultipleOptions(int dice);
    void moveMarker(int dice);
    int getMovableMarkerIndex(int dice);
    void moveMarker(int dice, int idx);
    void setMarker(int val, int idx);

};

#endif /* Player_hpp */
