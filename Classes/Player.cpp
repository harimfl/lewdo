//
//  Player.cpp
//  LudoGame
//
//  Created by Hari Krishna on 1/10/17.
//
//

#include "Player.hpp"
#include "GameScene.hpp"


Player* Player::createPlayer(int idx, cocos2d::Node *node) {
    Player* p = new Player();
    p->init(idx, node);
    p->autorelease();
    return p;
}

void Player::init(int idx, cocos2d::Node *node) {
    mainNode = node;
    index = idx;
    Color3B color = GameScene::playerColors.at(idx);
    auto children = mainNode->getChildren();
    for (int i = 1 ; i <= 4 ; i++) {
        auto child = mainNode->getChildByName("p" + std::to_string(i));
//        child->setColor(color);
        Sprite* markerSprite = Sprite::createWithSpriteFrameName("pointer.png");
        Label* text = Label::createWithSystemFont(std::to_string(i), "Helvetica", 24);
        markerSprite->addChild(text);
        markerSprite->setLocalZOrder(999);
        text->setPosition(Point(20, 40));
        markerSprite->setColor(color);
        child->addChild(markerSprite);
        markerSprite->setScale(0.75);
        markerSprite->setAnchorPoint(Point::ANCHOR_MIDDLE_BOTTOM);
        markerSprite->setPosition(child->getContentSize() / 2);
        markerNodes.push_back(child);
        markerImages.push_back(markerSprite);
        curPosition.push_back(-1);
    }
}

int Player::getMovableMarkerIndex(int dice) {
    for (int i = 0; i < curPosition.size(); i++) {
        int marker = curPosition.at(i);
        if (marker < 57) {
            if (marker == -1 && dice == 6) {
                return i;
            }
            if (marker > -1 && (marker + dice) < 57) {
                return i;
            }
        }
    }
    return -1;
}

bool Player::isValidIndex(int idx, int dice) {
    int marker = curPosition.at(idx);
    if (marker < 57) {
        if (marker == -1 && dice == 6) {
            return true;
        }
        if (marker > -1 && (marker + dice) < 57) {
            return true;
        }
    }
    return false;
}

bool Player::canMove(int dice) {
    int possibleMoves = 0;
    for (auto marker : curPosition) {
        if (marker < 57) {
            if (marker == -1 && dice == 6) {
                possibleMoves++;
                continue;
            }
            if (marker > -1 && (marker + dice) < 57) {
                possibleMoves++;
                continue;
            }
        }
    }
    return (possibleMoves > 0);
}

bool Player::hasMultipleOptions(int dice) {
    int possibleMoves = 0;
    for (auto marker : curPosition) {
        if (marker < 57) {
            if (marker == -1 && dice == 6) {
                possibleMoves++;
                continue;
            }
            if (marker > -1 && (marker + dice) < 57) {
                possibleMoves++;
                continue;
            }
        }
    }
    return (possibleMoves > 1);
}

void Player::moveMarker(int dice, int idx) {
    int curIndex = curPosition.at(idx);
    if (curIndex == -1) {
        curPosition[idx] = 0;
        return;
    }
    curPosition[idx] =  curIndex + dice;
    if (curPosition[idx] > 57){
        curPosition[idx] = 57;
    }
}
