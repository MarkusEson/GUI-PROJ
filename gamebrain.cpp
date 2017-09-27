#include "gamebrain.h"
#define gridOnes 1
#define gridTwos 2
#define gridthrees 3
#define gridFours 4
#define gridFives 5
#define gridSixes 6
#define gridPairs 9
#define gridTwoPairs 10
#define gridThreeOfAkind 11
#define gridFourOfAkind 12
#define gridFullHouse 13
#define gridSmallStraight 14
#define gridLargeStraight 15
#define gridYahzee 16
#define gridChance 17

GameBrain::GameBrain()
{
}

GameBrain::~GameBrain()
{

}

void GameBrain::rollDice()
{
    for (unsigned int i = 0; i < sizeof(_diceArray)/sizeof(_diceArray[0]); i++)
    {
        if (!_diceArray[i].checkIsChecked())
        {
            _diceArray[i].setValue(rand() % 6 + 1);
            qDebug() << "Dice nr " << (i + 1) << " fick värdet" << _diceArray[i].getValue();
        }
        else
            qDebug() << "Dice nr " << (i + 1) << " har fortfarande värdet" << _diceArray[i].getValue();
    }
    std::sort(_diceArray, _diceArray + 5, [](Die & a, Die & b) -> bool { return a.getValue() < b.getValue(); });
    qDebug() << "Sorterat!";
    // Här har vi använt kod inspirerad av https://stackoverflow.com/questions/12823573/c-sorting-class-array
    // Lambakoden förklarar för sort att objekten ska sorteras efter värdena som returneras från "getValue()"
    // Tänkte att det var nog bäst att ha lambakod eftersom det bara är i rollDice() den är relevant

}

Die * GameBrain::getDiceArray() // Returns copy of array
{
    Die * pointerToDiceArrayCopy = new Die[5];
    for (int i = 0; i < 5; i++)
        pointerToDiceArrayCopy[i] = _diceArray[i];
    return pointerToDiceArrayCopy;
}

void GameBrain::checkDie(int dieNumber)
{
    _diceArray[dieNumber - 1].checkDie();
    qDebug() << "Tärning nr: " << dieNumber << "blev checkad/avcheckad";
}

int GameBrain::pairs()
{
    int sum = 0;
    for(int i=0; i<4; i++){
     if(_diceArray[i].getValue()==_diceArray[i+1].getValue()){
             sum = _diceArray[i].getValue() + _diceArray[i+1].getValue();
         }
    }
     qDebug() <<"pair sum: " << sum;
     return sum;
}

int GameBrain::twoPairs()
{
    int countPairs=0;
    int sum = 0;
    for(int i=0; i<4; i++){
     if(_diceArray[i].getValue()==_diceArray[i+1].getValue()){
             sum += _diceArray[i].getValue() + _diceArray[i+1].getValue();
             countPairs++;
             i++;
         }
    }
    if(countPairs==2)
    {
        qDebug() << "firstPairSum:" << sum;
        return sum;
    }
    else
        return false;

    qDebug() << "firstPair false";
}

int GameBrain::xOfAKind(int keyId)
{
    int xOfAKindValue = 0;
    if(keyId == gridThreeOfAkind){xOfAKindValue = 3;}
    else if(keyId == gridFourOfAkind){xOfAKindValue = 4;}
    else if(keyId== gridYahzee){xOfAKindValue = 5;}

    int sum = 0;
    for(int j=1; j<7; j++)  // Ändrade 6 -> 7 så att den räknar med 6or
    {
        int count = 0;
        for( int i = 0; i < 5; i++ )
        {
            if( _diceArray[i].getValue() == j )
                count +=1;
            if(count >= xOfAKindValue)
              sum = j * xOfAKindValue;
        }
    }
    return sum;
}

int GameBrain::oneToSix(int keyId)
{
    int sum = 0;
      for( int i = 0; i < 5; i++ )
      {
        if( _diceArray[i].getValue() == keyId )
            sum += keyId;
      }
      for (unsigned int i = 0; i < sizeof(_diceArray)/sizeof(_diceArray[0]); i++)
          qDebug() << "Dice nummer " << (i + 1) << " har värdet" << _diceArray[i].getValue();
      return sum;
      qDebug() << "Sum i oneToSix: " << sum;
}

int GameBrain::fullHouse()
{
    int sum = 0;
    if( (((_diceArray[0].getValue() == _diceArray[1].getValue()) && (_diceArray[1].getValue() == _diceArray[2].getValue())) && // Three of a Kind
        (_diceArray[3].getValue() == _diceArray[4].getValue()) && // Two of a Kind
        (_diceArray[2].getValue() != _diceArray[3].getValue())) ||
        ((_diceArray[0].getValue() == _diceArray[1].getValue()) && // Two of a Kind
        ((_diceArray[2].getValue() == _diceArray[3].getValue()) && (_diceArray[3].getValue() == _diceArray[4].getValue())) && // Three of a Kind
        (_diceArray[1].getValue() != _diceArray[2].getValue())) )
    {
        sum = _diceArray[0].getValue() + _diceArray[1].getValue() + _diceArray[2].getValue() + _diceArray[3].getValue() + _diceArray[4].getValue();
    }
    return  sum;

    qDebug() << "Sum i fullhouse:" << sum;
}

int GameBrain::smallLargeStraight(int keyId)
{
    int firstStraight = 0;
    int sum = 0;

    if(keyId == gridSmallStraight){
        firstStraight = 1;
    }
    if(keyId == gridLargeStraight){
        firstStraight = 2;
    }

    if( ((_diceArray[0].getValue() == firstStraight) &&
     (_diceArray[1].getValue() == firstStraight +1) &&
     (_diceArray[2].getValue() == firstStraight +2) &&
     (_diceArray[3].getValue() == firstStraight +3) &&
     (_diceArray[4].getValue() == firstStraight +4)))
    {
        int straightNumber = firstStraight;
        int straightValue = firstStraight;
        for( int i = 0; i <4; i++ )
        {
            straightNumber++;
            straightValue += straightNumber;
        }
        sum = straightValue;
    }
    return sum;
}

int GameBrain::chance()
{
    int sum = 0;
    for(int i=0; i<5; i++){
        sum += _diceArray[i].getValue();
    }
    qDebug() << "chance sum: " << sum;
    return sum;
}

QString GameBrain::calculateScoreBoard(int player, int sumBonusOrTotal)
{
    /*
     * A function that takes the active player, and on click sums all the players currents points and returns these to be displayed.
     * Calculates the Sum, Bonus, and Total scores.
     */
    int score = 0;
    switch(sumBonusOrTotal){
        case 0:
        {                                            // sums up the first scores.
            score = 0;
            for(int i = 0; i <= 6; i++)
                score += _scoreArray[i][player];

            _scoreArray[7][player] = score;
            break;
        }
        case 1:
        {                                            // adds bonus scores based on the sum.
            score = 0;
            if(_scoreArray[7][player] >= 63)
                _scoreArray[8][player] = 35;

            else
                _scoreArray[8][player] = 0;

            score = _scoreArray[8][player];
            break;
        }
        case 2:
        {                                            // sums upp the total score.
            for(int i = 6; i < 18; i++)
                score = 0;

            for(int i = 7; i <= 18; i++)
                score += _scoreArray[i][player];
            break;
        }
    }
    QString scoreToReturn = QString::number(score);
    return scoreToReturn;
}

void GameBrain::resetChecked()
{
    for(int i = 0; i < 5; i++){
        if(_diceArray[i].checkIsChecked())
            _diceArray[i].checkDie();
    }
}

void GameBrain::resetScoreBoard()
{
    for(int i = 0; i < 19; i++)
        for(int j = 0; j < 4; j++)
            _scoreArray[i][j] = 0;
}

int GameBrain::functionHandler(int keyId)
{
    int functionId = keyId;
    int summa = 0;

    if (functionId >= gridOnes && functionId <= gridSixes ){
        summa = oneToSix(keyId);
    }

    else if (functionId == gridPairs ){
        summa = pairs();
        qDebug() << summa;
        qDebug() << "pairs";
    }

    else if (functionId == gridTwoPairs ){
        summa = twoPairs();
        qDebug() << summa;
        qDebug() << "twoPairs";
    }

    else if (functionId == gridThreeOfAkind || functionId == gridFourOfAkind || functionId == gridYahzee ){
        summa = xOfAKind(keyId);
        qDebug() << summa;
        qDebug() << "345ofakind";
    }

    else if (functionId == gridFullHouse ){
        summa = fullHouse();
        qDebug() << summa;
        qDebug() << "fullhouse";
    }

    else if (functionId >=gridSmallStraight && functionId <=gridLargeStraight){
        summa = smallLargeStraight(keyId);
        qDebug() << summa;
        qDebug() << "straight";
    }
    else if (functionId == gridChance ){
        summa = chance();
        qDebug() << summa;
        qDebug() << "chance";
    }
    return summa;

}

QString GameBrain::getScoreFromArray(int keyID, int playerNr)   //inlagd
{
    QString mystr = QString::number(_scoreArray[keyID][playerNr]);

    return mystr;
}

void GameBrain::addScoreToArray(int keyId, int playerNr, int sumFromRound)
{
    int sum = sumFromRound;
    _scoreArray[keyId][playerNr] = sum;
}

QString GameBrain::endTurnChoice(int keyId, int playerNr)
{
    calculateScoreFromChoice(keyId,playerNr);

    QString score = getScoreFromArray(keyId,playerNr);
    qDebug() << score << "Du är BÄST!!!";
    return score;
}

void GameBrain::calculateScoreFromChoice(int keyId, int playerNr)
{
    int score = 0;
    score = functionHandler(keyId);
    addScoreToArray(keyId,playerNr,score);
}

// ------ Die - koden -----------------

Die::Die()
{

}

Die::~Die()
{

}

void Die::setValue(int newValue)
{
    _value = newValue;
}

int Die::getValue()
{
    return _value;
}

bool Die::checkIsChecked()
{
    return _isChecked;
}

void Die::checkDie()
{
    if (_isChecked)
        _isChecked = false;
    else
        _isChecked = true;
}
