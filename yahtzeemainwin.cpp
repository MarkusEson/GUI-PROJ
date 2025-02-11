#include "yahtzeemainwin.h"
#include "ui_yahtzeemainwin.h"
#include <QDebug>
#include <QAbstractButton>
#include <unistd.h>
#include <QString>
#include <QSound>
#include <QGraphicsScene>



YahtzeeMainWin::YahtzeeMainWin(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::YahtzeeMainWin)
{
    ui->setupUi(this);
    ui->helpBrowser->hide();
    ui->closeHelpButton->hide();
    ui->gameOverLabel->hide();

    _keyPressedFromUI = {
        {ui->rollDiceButton, Key::rolldice},

        // Maps the keys in the 1st section of the grid to the FUNK ones
        {ui->A1, Key::ones},
        {ui->B1, Key::ones},
        {ui->C1, Key::ones},
        {ui->D1, Key::ones},

        {ui->A2, Key::twos},
        {ui->B2, Key::twos},
        {ui->C2, Key::twos},
        {ui->D2, Key::twos},

        {ui->A3, Key::threes},
        {ui->B3, Key::threes},
        {ui->C3, Key::threes},
        {ui->D3, Key::threes},

        {ui->A4, Key::fours},
        {ui->B4, Key::fours},
        {ui->C4, Key::fours},
        {ui->D4, Key::fours},

        {ui->A5, Key::fives},
        {ui->B5, Key::fives},
        {ui->C5, Key::fives},
        {ui->D5, Key::fives},

        {ui->A6, Key::sixes},
        {ui->B6, Key::sixes},
        {ui->C6, Key::sixes},
        {ui->D6, Key::sixes},

        {ui->A7, Key::sum},
        {ui->B7, Key::sum},
        {ui->C7, Key::sum},
        {ui->D7, Key::sum},

        {ui->A8, Key::bonus},
        {ui->B8, Key::bonus},
        {ui->C8, Key::bonus},
        {ui->D8, Key::bonus},

        {ui->A9, Key::onepair},
        {ui->B9, Key::onepair},
        {ui->C9, Key::onepair},
        {ui->D9, Key::onepair},

        {ui->A10, Key::twopairs},
        {ui->B10, Key::twopairs},
        {ui->C10, Key::twopairs},
        {ui->D10, Key::twopairs},

        {ui->A11, Key::threeofakind},
        {ui->B11, Key::threeofakind},
        {ui->C11, Key::threeofakind},
        {ui->D11, Key::threeofakind},

        {ui->A12, Key::fourofakind},
        {ui->B12, Key::fourofakind},
        {ui->C12, Key::fourofakind},
        {ui->D12, Key::fourofakind},

        {ui->A13, Key::fullhouse},
        {ui->B13, Key::fullhouse},
        {ui->C13, Key::fullhouse},
        {ui->D13, Key::fullhouse},

        {ui->A14, Key::smallstraight},
        {ui->B14, Key::smallstraight},
        {ui->C14, Key::smallstraight},
        {ui->D14, Key::smallstraight},

        {ui->A15, Key::largestraight},
        {ui->B15, Key::largestraight},
        {ui->C15, Key::largestraight},
        {ui->D15, Key::largestraight},

        {ui->A16, Key::yahzee},
        {ui->B16, Key::yahzee},
        {ui->C16, Key::yahzee},
        {ui->D16, Key::yahzee},

        {ui->A17, Key::chance},
        {ui->B17, Key::chance},
        {ui->C17, Key::chance},
        {ui->D17, Key::chance},

        {ui->A18, Key::yahzee},
        {ui->B18, Key::yahzee},
        {ui->C18, Key::yahzee},
        {ui->D18, Key::yahzee},

        {ui->A19, Key::total},
        {ui->B19, Key::total},
        {ui->C19, Key::total},
        {ui->D19, Key::total},

    };

    /*
     * A function that connects all the buttons in the grids A,B,C, and D.
     * Funktionen är inspirerad från GUI labbarna samt stackexchange sidan https://stackoverflow.com/questions/4065378/qt-get-children-from-layout
     */
    for(int i = 0; i < ui->Agrid->count(); i++){
        QWidget *button = ui->Agrid->itemAt(i)->widget();
        connect(button, SIGNAL(clicked()), this, SLOT(aButtonWasClicked()));
    }
    for(int i = 0; i < ui->Bgrid->count(); i++){
        QWidget *button = ui->Bgrid->itemAt(i)->widget();
        connect(button, SIGNAL(clicked()), this, SLOT(aButtonWasClicked()));
    }
    for(int i = 0; i < ui->Cgrid->count(); i++){
        QWidget *button = ui->Cgrid->itemAt(i)->widget();
        connect(button, SIGNAL(clicked()), this, SLOT(aButtonWasClicked()));
    }
    for(int i = 0; i < ui->Dgrid->count(); i++){
        QWidget *button = ui->Dgrid->itemAt(i)->widget();
        connect(button, SIGNAL(clicked()), this, SLOT(aButtonWasClicked()));
    }
    for(int i = 0; i < ui->diceButtonLayout->count(); i++){
        QWidget *button = ui->diceButtonLayout->itemAt(i)->widget();
        connect(button, SIGNAL(clicked()), this, SLOT(aDiceWasClicked()));
        button->setDisabled(true); // Disabled so that player can't lock the dice until rolled
    }
}

YahtzeeMainWin::~YahtzeeMainWin()
{
    delete ui;
}

void YahtzeeMainWin::showPlayerBlockersOnClick()
{
    // Shows the playerBlockers on click.
    ui->playerBlockerA->show();
    ui->playerBlockerB->show();
    ui->playerBlockerC->show();
    ui->playerBlockerD->show();
}

void YahtzeeMainWin::chooseAmountOfPlayers(int num)
{

    /*
     * This function checks how many players are playing.
     * It enables all the pushbuttons.
     * It enables the rollDice button, sets _numOfPlayers accordingly.
     * resets scoreboard to get fresh clean start
     * sets texlabels to show how many players are playing.
     */
    // forloops unlocks all buttons except for the sum,bonus and totals
    for(int i = 0; i < ui->Agrid->count(); i++){
        QWidget *button = ui->Agrid->itemAt(i)->widget();
        if(button == ui->A7 || button == ui->A8 || button == ui->A19)
            button->setEnabled(false);
        else
            button->setEnabled(true);
    }
    for(int i = 0; i < ui->Bgrid->count(); i++){
        QWidget *button = ui->Bgrid->itemAt(i)->widget();
        if(button == ui->B7 || button == ui->B8 || button == ui->B19)
            button->setEnabled(false);
        else
            button->setEnabled(true);
    }
    for(int i = 0; i < ui->Cgrid->count(); i++){
        QWidget *button = ui->Cgrid->itemAt(i)->widget();
        if(button == ui->C7 || button == ui->C8 || button == ui->C19)
            button->setEnabled(false);
        else
            button->setEnabled(true);
    }
    for(int i = 0; i < ui->Dgrid->count(); i++){
        QWidget *button = ui->Dgrid->itemAt(i)->widget();
        if(button == ui->D7 || button == ui->D8 || button == ui->D19)
            button->setEnabled(false);
        else
            button->setEnabled(true);
    }

    _activePlayer = PLAYERONE;
    ui->gameOverLabel->hide();      // hides the game over/ win label when you restart new game
    showPlayerBlockersOnClick();
    _gameBrain.resetChecked();
    _gameBrain.resetScoreBoard();    // resets the score array
    resetScoreboardUI();            // resets the UI scores
    resetDice();

    /*
     * This code changes the playertexts to show how many players are actively plaing.
     * also changes background to visually show how many are playing.
     */
    if(num == 1){
        ui->gameBackground->setStyleSheet("background-image: url(:/new/pictures/backgroundplayer1test.png);");
        ui->rollDiceButton->setEnabled(true);
        _numOfPlayers = 1;
        ui->player1Label->setText("Player 1");
        ui->player2Label->setText("");
        ui->player3Label->setText("");
        ui->player4Label->setText("");
    }
    else if(num == 2){
        ui->gameBackground->setStyleSheet("background-image: url(:/new/pictures/backgroundplayer2test.png);");
        ui->rollDiceButton->setEnabled(true);
        _numOfPlayers = 2;
        ui->player1Label->setText("Player 1");
        ui->player2Label->setText("Player 2");
        ui->player3Label->setText("");
        ui->player4Label->setText("");
    }
    else if(num == 3){
        ui->gameBackground->setStyleSheet("background-image: url(:/new/pictures/backgroundplayer3test.png);");
        ui->rollDiceButton->setEnabled(true);
        _numOfPlayers = 3;
        ui->player1Label->setText("Player 1");
        ui->player2Label->setText("Player 2");
        ui->player3Label->setText("Player 3");
        ui->player4Label->setText("");
    }
    else{
        ui->gameBackground->setStyleSheet("background-image: url(:/new/pictures/backgroundplayer4.png);");
        ui->rollDiceButton->setEnabled(true);
        _numOfPlayers = 4;
        ui->player1Label->setText("Player 1");
        ui->player2Label->setText("Player 2");
        ui->player3Label->setText("Player 3");
        ui->player4Label->setText("Player 4");
    }
}

void YahtzeeMainWin::setDieImage(QPushButton * button, Die die)
{
    // Med inspiration av grupp mars
    if (!die.checkIsChecked())
    {
        QString string = "QPushButton {border-image: url(:/new/pictures/" + QString::number(die.getValue()) + "dice.png) }";
        button->setStyleSheet(string);
    }
    else
    {
        QString string = "QPushButton {border-image: url(:/new/pictures/" + QString::number(die.getValue()) + "diceClicked.png) }";
        button->setStyleSheet(string);
    }
}

void YahtzeeMainWin::displayDiceOnScreen() // Removed rollDice func
{
    Die * arrayWithDice = _gameBrain.getDiceArray();
    setDieImage(ui->dice1Button, arrayWithDice[0]);
    setDieImage(ui->dice2Button, arrayWithDice[1]);
    setDieImage(ui->dice3Button, arrayWithDice[2]);
    setDieImage(ui->dice4Button, arrayWithDice[3]);
    setDieImage(ui->dice5Button, arrayWithDice[4]);

    // removed delete, it does not work in mac, crashed application
    // does work on windows tho, for some reason
    //delete arrayWithDice;
}

void YahtzeeMainWin::playerTurn(int numplayers)
{
    _timesRolled = 0;                           // resets _timesRolled, so next player can now roll again.
    ui->rollDiceButton->setEnabled(true);       // sets the rollDice button to enabled, so it can be clicked.

        if(_activePlayer > numplayers)
            _activePlayer = PLAYERONE;

        switch (_activePlayer){
        case 1:
            showPlayerBlockersOnClick();
            ui->helpLabel->setText("Player 1:s turn!");
            break;

        case 2:
            showPlayerBlockersOnClick();
            ui->helpLabel->setText("Player 2:s turn!");
            break;

        case 3:
            showPlayerBlockersOnClick();
            ui->helpLabel->setText("Player 3:s turn!");
            break;

        case 4:
            showPlayerBlockersOnClick();
            ui->helpLabel->setText("Player 4:s turn!");
            break;
        }
}

void YahtzeeMainWin::resetScoreboardUI()
{
    /*
     * Goes through the scoreboard Grids to reset the text on the PusshButtons.
     * sets all text to "".
     * this is only visual, i.e. UI. Another function resets the _scoreArray
     */
    for(int i = 0; i < ui->Agrid->count(); i++){
        QWidget *button = ui->Agrid->itemAt(i)->widget();
        QPushButton *resetButton = dynamic_cast<QPushButton*>(button);
        resetButton->setText("");
    }
    for(int i = 0; i < ui->Bgrid->count(); i++){
        QWidget *button = ui->Bgrid->itemAt(i)->widget();
        QPushButton *resetButton = dynamic_cast<QPushButton*>(button);
        resetButton->setText("");
    }
    for(int i = 0; i < ui->Cgrid->count(); i++){
        QWidget *button = ui->Cgrid->itemAt(i)->widget();
        QPushButton *resetButton = dynamic_cast<QPushButton*>(button);
        resetButton->setText("");
    }
    for(int i = 0; i < ui->Dgrid->count(); i++){
        QWidget *button = ui->Dgrid->itemAt(i)->widget();
        QPushButton *resetButton = dynamic_cast<QPushButton*>(button);
        resetButton->setText("");
    }
}

void YahtzeeMainWin::resetPlayerScoreboardUI()
{
    if(_activePlayer == 1){
            for(int i = 1; i < ui->Agrid->count(); i++){
                    QWidget *button = ui->Agrid->itemAtPosition(i-1, 0)->widget();
                    QPushButton *theButton = dynamic_cast<QPushButton*>(button);
                    theButton->setText("");
            }
        }
        else if(_activePlayer == 2){
            for(int i = 1; i < ui->Bgrid->count(); i++){
                    QWidget *button = ui->Bgrid->itemAtPosition(i-1, 0)->widget();
                    QPushButton *theButton = dynamic_cast<QPushButton*>(button);
                    theButton->setText("");
            }
        }
        else if(_activePlayer == 3){
            for(int i = 1; i < ui->Cgrid->count(); i++){
                    QWidget *button = ui->Cgrid->itemAtPosition(i-1, 0)->widget();
                    QPushButton *theButton = dynamic_cast<QPushButton*>(button);
                    theButton->setText("");
            }
        }
        else{
            for(int i = 1; i < ui->Dgrid->count(); i++){
                    QWidget *button = ui->Dgrid->itemAtPosition(i-1, 0)->widget();
                    QPushButton *theButton = dynamic_cast<QPushButton*>(button);
                        theButton->setText("");
            }
        }
}


void YahtzeeMainWin::lockDice()
{
    for(int i = 0; i < ui->diceButtonLayout->count() - 1; i++){
        QWidget *button = ui->diceButtonLayout->itemAt(i)->widget();
            button->setDisabled(true);
    }
}

void YahtzeeMainWin::unlockDice()
{
    for(int i = 0; i < ui->diceButtonLayout->count(); i++){
        QWidget *button = ui->diceButtonLayout->itemAt(i)->widget();
            if(!button->isEnabled())
                button->setEnabled(true);
    }
}

void YahtzeeMainWin::aButtonWasClicked()
{
    QPushButton *theButton = dynamic_cast<QPushButton*>(sender());

    /*
     * Calls the function calculateScoreBoard, which is called every time a player clicks the score board.
     * This Function updates the ui scoreboard to show the updated points in "score", "bonus", and "total score"
     * calculateScoreBoard(<the player>, <the scoreIndexer>)
     *
     * The function then advances to next player. it also locks the score button so that it cannot be clicked agin.
     * - Markus
     */

    Key keyValue = _keyPressedFromUI[theButton];
        int keyId = intFromKey(keyValue);
        dynamic_cast<QPushButton*>(sender())->setText(_gameBrain.endTurnChoice(keyId, _activePlayer)); // theButton ist för dynamic cast?

    if(theButton){
        dynamic_cast<QPushButton*>(sender())->setEnabled(false); // theButton ist för dynamic cast?
        theButton->setStyleSheet("color: rgb(0, 0, 0);");

        if(_activePlayer == PLAYERONE){
            ui->A7->setText(_gameBrain.calculateScoreBoard(_activePlayer, 0));
            ui->A8->setText(_gameBrain.calculateScoreBoard(_activePlayer, 1));
            ui->A19->setText(_gameBrain.calculateScoreBoard(_activePlayer, 2));
        }
        if(_activePlayer == PLAYERTWO){
            ui->B7->setText(_gameBrain.calculateScoreBoard(_activePlayer, 0));
            ui->B8->setText(_gameBrain.calculateScoreBoard(_activePlayer, 1));
            ui->B19->setText(_gameBrain.calculateScoreBoard(_activePlayer, 2));
        }
        if(_activePlayer == PLAYERTHREE){
            ui->C7->setText(_gameBrain.calculateScoreBoard(_activePlayer, 0));
            ui->C8->setText(_gameBrain.calculateScoreBoard(_activePlayer, 1));
            ui->C19->setText(_gameBrain.calculateScoreBoard(_activePlayer, 2));
        }
        if(_activePlayer == PLAYERFOUR){
            ui->D7->setText(_gameBrain.calculateScoreBoard(_activePlayer, 0));
            ui->D8->setText(_gameBrain.calculateScoreBoard(_activePlayer, 1));
            ui->D19->setText(_gameBrain.calculateScoreBoard(_activePlayer, 2));
        }

        /*
         * Sets the possible scores on the screen, in green.
         * increments player, so that it is next players turn.
         */
        resetPlayerScoreboardUI();
        setPlayerScoreToUi();
        _winTimer ++;       // advances the winTimer, when at max the game ends
        _activePlayer++;
        _gameBrain.resetChecked();
        playerTurn(_numOfPlayers); // player func that changes turns to next player.
        displayDiceOnScreen();
        lockDice();

        if(_winTimer == _winTimerEnd)
            gameEnding();
    }
}

void YahtzeeMainWin::aDiceWasClicked()
{
    /*
     * If you click a dice then it gets checked, so that it does not roll again.
     * Then it calls the function to display the new dice image.
     */
    QPushButton *theButton = dynamic_cast<QPushButton*>(sender());

    if (theButton == ui->dice1Button)
        _gameBrain.checkDie(1);
    else if (theButton == ui->dice2Button)
        _gameBrain.checkDie(2);
    else if (theButton == ui->dice3Button)
        _gameBrain.checkDie(3);
    else if (theButton == ui->dice4Button)
        _gameBrain.checkDie(4);
    else if (theButton == ui->dice5Button)
        _gameBrain.checkDie(5);

    displayDiceOnScreen();

    // shows and hides playerblockers according to who'sturn it is.
    // unblocks scorerows when you click the Roll button by the dice.
    // Roll button is part of the DiceLayout
    if(_activePlayer == 1){
        showPlayerBlockersOnClick();
        ui->playerBlockerA->hide();
    }
    else if(_activePlayer == 2){
        showPlayerBlockersOnClick();
        ui->playerBlockerB->hide();
    }
    else if(_activePlayer == 3){
        showPlayerBlockersOnClick();
        ui->playerBlockerC->hide();
    }
    else{
        showPlayerBlockersOnClick();
        ui->playerBlockerD->hide();
    }

}

void YahtzeeMainWin::on_rollDiceButton_clicked() // Added rollDice func
{
    _timesRolled++;
    if(_timesRolled <= 3 )
    {
        _gameBrain.rollDice();
        displayDiceOnScreen();
        QSound::play(":/new/pictures/dicethrowshort.wav");
        uiScoreCalculator();
    }
    if(_timesRolled == 3)
        ui->rollDiceButton->setEnabled(false);

    unlockDice();
    // Lägg till enkel counter under roll-knappen med ex _timesRolled << "/3"?
}

void YahtzeeMainWin::resetDice()
{
    Die *resetDiceArray = _gameBrain.getDiceArray();

    for(int i = 0; i < 5; i++)
        resetDiceArray[i].setValue(6);

    setDieImage(ui->dice1Button, resetDiceArray[0]);
    setDieImage(ui->dice2Button, resetDiceArray[1]);
    setDieImage(ui->dice3Button, resetDiceArray[2]);
    setDieImage(ui->dice4Button, resetDiceArray[3]);
    setDieImage(ui->dice5Button, resetDiceArray[4]);

    // removed delete, it does not work in mac, crashed application
    // does work on windows tho, for some reason
    //delete resetDiceArray;
}

void YahtzeeMainWin::on_onePlayerButton_triggered()
{
    chooseAmountOfPlayers(1);
    _winTimer = 0;
    _winTimerEnd = 16;
}

void YahtzeeMainWin::on_twoPlayerButton_triggered()
{
    chooseAmountOfPlayers(2);
    _winTimer = 0;
    _winTimerEnd = 32;
}

void YahtzeeMainWin::on_threePlayerButton_triggered()
{
    chooseAmountOfPlayers(3);
    _winTimer = 0;
    _winTimerEnd = 48;
}

void YahtzeeMainWin::on_fourPlayerButton_triggered()
{
    chooseAmountOfPlayers(4);
    _winTimer = 0;
    _winTimerEnd = 64;
}

void YahtzeeMainWin::uiScoreCalculator()
{
    /*
     * Shows the possible scores you can add to the board, it does this by printing the scores on the UI, and changing the text to a green color
     * the green color differentiates the scores that CAN be placed, with the black scores that you have already placed.
     * the green POSSIBLE scores are then removed after you are done.
     */
    if(_activePlayer==1){
    for(int i = 1; i < ui->Agrid->count(); i++){
                QWidget *button = ui->Agrid->itemAtPosition(i-1, 0)->widget();
                QPushButton *theButton = dynamic_cast<QPushButton*>(button);
                if(theButton->isEnabled()) {
                    theButton->setText(_gameBrain.getPossibleScores(i));
                    theButton->setStyleSheet("color: rgb(30, 217, 4);");
                }
    }
    }

    else if(_activePlayer==2){
        for(int i = 1; i < ui->Bgrid->count(); i++){
                QWidget *button = ui->Bgrid->itemAtPosition(i-1, 0)->widget();
                QPushButton *theButton = dynamic_cast<QPushButton*>(button);
                if(theButton->isEnabled()) {
                    theButton->setText(_gameBrain.getPossibleScores(i));
                    theButton->setStyleSheet("color: rgb(30, 217, 4);");
                }
            }
    }
   else if(_activePlayer==3){
        for(int i = 1; i < ui->Cgrid->count(); i++){
                QWidget *button = ui->Cgrid->itemAtPosition(i-1, 0)->widget();
                QPushButton *theButton = dynamic_cast<QPushButton*>(button);
                if(theButton->isEnabled()) {
                    theButton->setText(_gameBrain.getPossibleScores(i));
                    theButton->setStyleSheet("color: rgb(30, 217, 4);");
                }
            }
    }
    else{
        for(int i = 1; i < ui->Dgrid->count(); i++){
                QWidget *button = ui->Dgrid->itemAtPosition(i-1, 0)->widget();
                QPushButton *theButton = dynamic_cast<QPushButton*>(button);
                if(theButton->isEnabled()) {
                    theButton->setText(_gameBrain.getPossibleScores(i));
                    theButton->setStyleSheet("color: rgb(30, 217, 4);");
                }
            }
    }
}

void YahtzeeMainWin::setPlayerScoreToUi()
{
    if(_activePlayer == PLAYERONE){
        for(int i = 1; i < ui->Agrid->count(); i++){
                QWidget *button = ui->Agrid->itemAtPosition(i-1, 0)->widget();
                QPushButton *theButton = dynamic_cast<QPushButton*>(button);
                if(!theButton->isEnabled())
                    theButton->setText(_gameBrain.putPlayerScoreToUi(i,_activePlayer));
        }
    }
    else if(_activePlayer == PLAYERTWO){
        for(int i = 1; i < ui->Bgrid->count(); i++){
                QWidget *button = ui->Bgrid->itemAtPosition(i-1, 0)->widget();
                QPushButton *theButton = dynamic_cast<QPushButton*>(button);
                if(!theButton->isEnabled())
                    theButton->setText(_gameBrain.putPlayerScoreToUi(i,_activePlayer));
        }
    }
    else if(_activePlayer == PLAYERTHREE){
        for(int i = 1; i < ui->Cgrid->count(); i++){
                QWidget *button = ui->Cgrid->itemAtPosition(i-1, 0)->widget();
                QPushButton *theButton = dynamic_cast<QPushButton*>(button);
                if(!theButton->isEnabled())
                    theButton->setText(_gameBrain.putPlayerScoreToUi(i,_activePlayer));
        }
    }
    else{
        for(int i = 1; i < ui->Dgrid->count(); i++){
                QWidget *button = ui->Dgrid->itemAtPosition(i-1, 0)->widget();
                QPushButton *theButton = dynamic_cast<QPushButton*>(button);
                if(!theButton->isEnabled())
                    theButton->setText(_gameBrain.putPlayerScoreToUi(i,_activePlayer));
        }
    }

}

void YahtzeeMainWin::openHelpWindow()
{
    ui->helpBrowser->show();
    ui->closeHelpButton->show();
}

void YahtzeeMainWin::gameEnding()
{
    // Simple function that prints the winner of the game
    // -markus

    ui->gameOverLabel->show();

    if(_gameBrain.checkWinner() == PLAYERONE)
        ui->gameOverLabel->setStyleSheet("background-image: url(:/new/pictures/winplayer1.png);");
    else if(_gameBrain.checkWinner() == PLAYERTWO)
        ui->gameOverLabel->setStyleSheet("background-image: url(:/new/pictures/winplayer2.png);");
    else if(_gameBrain.checkWinner() == PLAYERTHREE)
        ui->gameOverLabel->setStyleSheet("background-image: url(:/new/pictures/winplayer3.png);");
    else if(_gameBrain.checkWinner() == PLAYERFOUR)
        ui->gameOverLabel->setStyleSheet("background-image: url(:/new/pictures/winplayer4.png);");

}

void YahtzeeMainWin::on_actionGuide_triggered()
{
    openHelpWindow();
}

void YahtzeeMainWin::on_closeHelpButton_clicked()
{
    ui->helpBrowser->hide();
    ui->closeHelpButton->hide();
}
