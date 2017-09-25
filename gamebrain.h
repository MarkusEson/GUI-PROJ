#ifndef GAMEBRAIN_H
#define GAMEBRAIN_H

#include <algorithm>
#include <QDebug>
#include <stdlib.h>
#include <time.h>

enum class WIPenum {
    rolldice = 0,
    ones,
    twos,
    threes,
    fours,
    fives,
    sixes,

    sum,
    bonus,

    onepair,
    twopairs,
    threeofakind,
    fourofakind,
    fullhouse,
    smallstraight,
    largestraight,
    yahzee,
    chance,
    yahzeebonus,

    total,
};

class Die {
public:
    Die();
    ~Die();

    void setValue(int newValue);
    int getValue();
    bool checkIsChecked();
    void checkDie();

private:
    int _value = 6;
    bool _isChecked = 0;

};

class GameBrain
{
public:
    GameBrain();
    ~GameBrain();

    void rollDice();
    Die *getDiceArray();
    void checkDie(int dieNumber);
    int oneToSix(int keyId);
    int xOfAKind(int player);
    int fullHouse();
    int smallLargeStraight(int player);
    //QString getScoreFromArray();
    QString getScoreFromArray(int keyID, int playerNr);
    QString calculateScoreBoard(int player, int sumBonusOrTotal);
    void resetChecked();
    void resetScoreBoard();
    int functionHandler(int keyId, int playerNr);
    void addScoreToArray(int keyId, int playerNr, int sumFromRound);
    QString endTurnChoice(int keyId, int playerNr);
    void calculateScoreFromChoice(int keyId, int playerNr);

private:
    Die _diceArray[5];

    int throwValue [5] = {2,3,4,5,6};           //testvariabler för funktionerna
    int _scoreArray[19][4];
};

#endif // GAMEBRAIN_H
