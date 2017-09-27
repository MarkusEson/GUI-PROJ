#include "yahtzeemainwin.h"
#include "ui_yahtzeemainwin.h"
#include <QDebug>
#include <QAbstractButton>
#include <unistd.h>
#include <cstdlib>
#include <QString>
#include <QSound>

YahtzeeMainWin::YahtzeeMainWin(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::YahtzeeMainWin)
{
    ui->setupUi(this);
    ui->A18->setEnabled(false);
    ui->B18->setEnabled(false);
    ui->C18->setEnabled(false);
    ui->D18->setEnabled(false);

    _keyPressedFromUI = {
        {ui->rollDiceButton, WIPenum::rolldice},

        // Maps the keys in the 1st section of the grid to the FUNK ones
        {ui->A1, WIPenum::ones},
        {ui->B1, WIPenum::ones},
        {ui->C1, WIPenum::ones},
        {ui->D1, WIPenum::ones},

        {ui->A2, WIPenum::twos},
        {ui->B2, WIPenum::twos},
        {ui->C2, WIPenum::twos},
        {ui->D2, WIPenum::twos},

        {ui->A3, WIPenum::threes},
        {ui->B3, WIPenum::threes},
        {ui->C3, WIPenum::threes},
        {ui->D3, WIPenum::threes},

        {ui->A4, WIPenum::fours},
        {ui->B4, WIPenum::fours},
        {ui->C4, WIPenum::fours},
        {ui->D4, WIPenum::fours},

        {ui->A5, WIPenum::fives},
        {ui->B5, WIPenum::fives},
        {ui->C5, WIPenum::fives},
        {ui->D5, WIPenum::fives},

        {ui->A6, WIPenum::sixes},
        {ui->B6, WIPenum::sixes},
        {ui->C6, WIPenum::sixes},
        {ui->D6, WIPenum::sixes},

        {ui->A7, WIPenum::sum},
        {ui->B7, WIPenum::sum},
        {ui->C7, WIPenum::sum},
        {ui->D7, WIPenum::sum},

        {ui->A8, WIPenum::bonus},
        {ui->B8, WIPenum::bonus},
        {ui->C8, WIPenum::bonus},
        {ui->D8, WIPenum::bonus},

        {ui->A9, WIPenum::onepair},
        {ui->B9, WIPenum::onepair},
        {ui->C9, WIPenum::onepair},
        {ui->D9, WIPenum::onepair},

        {ui->A10, WIPenum::twopairs},
        {ui->B10, WIPenum::twopairs},
        {ui->C10, WIPenum::twopairs},
        {ui->D10, WIPenum::twopairs},

        {ui->A11, WIPenum::threeofakind},
        {ui->B11, WIPenum::threeofakind},
        {ui->C11, WIPenum::threeofakind},
        {ui->D11, WIPenum::threeofakind},

        {ui->A12, WIPenum::fourofakind},
        {ui->B12, WIPenum::fourofakind},
        {ui->C12, WIPenum::fourofakind},
        {ui->D12, WIPenum::fourofakind},

        {ui->A13, WIPenum::fullhouse},
        {ui->B13, WIPenum::fullhouse},
        {ui->C13, WIPenum::fullhouse},
        {ui->D13, WIPenum::fullhouse},

        {ui->A14, WIPenum::smallstraight},
        {ui->B14, WIPenum::smallstraight},
        {ui->C14, WIPenum::smallstraight},
        {ui->D14, WIPenum::smallstraight},

        {ui->A15, WIPenum::largestraight},
        {ui->B15, WIPenum::largestraight},
        {ui->C15, WIPenum::largestraight},
        {ui->D15, WIPenum::largestraight},

        {ui->A16, WIPenum::yahzee},
        {ui->B16, WIPenum::yahzee},
        {ui->C16, WIPenum::yahzee},
        {ui->D16, WIPenum::yahzee},

        {ui->A17, WIPenum::chance},
        {ui->B17, WIPenum::chance},
        {ui->C17, WIPenum::chance},
        {ui->D17, WIPenum::chance},

        {ui->A18, WIPenum::yahzeebonus},
        {ui->B18, WIPenum::yahzeebonus},
        {ui->C18, WIPenum::yahzeebonus},
        {ui->D18, WIPenum::yahzeebonus},

        {ui->A19, WIPenum::total},
        {ui->B19, WIPenum::total},
        {ui->C19, WIPenum::total},
        {ui->D19, WIPenum::total},

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
// Varför? Låste upp sum, bonus och total
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
    showPlayerBlockersOnClick();
    gameBrain.resetChecked();
    gameBrain.resetScoreBoard();    // resets the score array
    resetScoreboardUI();            // resets the UI scores
    resetDice();

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
    Die * arrayWithDice = gameBrain.getDiceArray();
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
     * calculateScoreBoard(<the players index in _scoreArray>, <the scoreIndexer>)
     *
     * The function then advances to next player. it also locks the score button so that it cannot be clicked agin.
     * - Markus
     */

    WIPenum keyValue = _keyPressedFromUI[theButton];
        int keyId = intFromKey(keyValue);
        dynamic_cast<QPushButton*>(sender())->setText(gameBrain.endTurnChoice(keyId, _activePlayer)); // theButton ist för dynamic cast?

    if(theButton){
        dynamic_cast<QPushButton*>(sender())->setEnabled(false); // theButton ist för dynamic cast?
        theButton->setStyleSheet("color: rgb(0, 0, 0);");

        if(_activePlayer == PLAYERONE){
            ui->A7->setText(gameBrain.calculateScoreBoard(_activePlayer, 0));
            ui->A8->setText(gameBrain.calculateScoreBoard(_activePlayer, 1));
            ui->A19->setText(gameBrain.calculateScoreBoard(_activePlayer, 2));
        }
        if(_activePlayer == PLAYERTWO){
            ui->B7->setText(gameBrain.calculateScoreBoard(_activePlayer, 0));
            ui->B8->setText(gameBrain.calculateScoreBoard(_activePlayer, 1));
            ui->B19->setText(gameBrain.calculateScoreBoard(_activePlayer, 2));
        }
        if(_activePlayer == PLAYERTHREE){
            ui->C7->setText(gameBrain.calculateScoreBoard(_activePlayer, 0));
            ui->C8->setText(gameBrain.calculateScoreBoard(_activePlayer, 1));
            ui->C19->setText(gameBrain.calculateScoreBoard(_activePlayer, 2));
        }
        if(_activePlayer == PLAYERFOUR){
            ui->D7->setText(gameBrain.calculateScoreBoard(_activePlayer, 0));
            ui->D8->setText(gameBrain.calculateScoreBoard(_activePlayer, 1));
            ui->D19->setText(gameBrain.calculateScoreBoard(_activePlayer, 2));
        }
        setPlayerScoreToUi();
        _activePlayer++;
        gameBrain.resetChecked();
        playerTurn(_numOfPlayers); // player func that changes turns to next player.
        displayDiceOnScreen();
        lockDice();
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
        gameBrain.checkDie(1);
    else if (theButton == ui->dice2Button)
        gameBrain.checkDie(2);
    else if (theButton == ui->dice3Button)
        gameBrain.checkDie(3);
    else if (theButton == ui->dice4Button)
        gameBrain.checkDie(4);
    else if (theButton == ui->dice5Button)
        gameBrain.checkDie(5);

    displayDiceOnScreen();


    // shows and hides playerblockers accordingly.
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
        gameBrain.rollDice();
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
    Die *resetDiceArray = gameBrain.getDiceArray();

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
}

void YahtzeeMainWin::on_twoPlayerButton_triggered()
{
    chooseAmountOfPlayers(2);
}

void YahtzeeMainWin::on_threePlayerButton_triggered()
{
    chooseAmountOfPlayers(3);
}

void YahtzeeMainWin::on_fourPlayerButton_triggered()
{
    chooseAmountOfPlayers(4);
}

void YahtzeeMainWin::uiScoreCalculator()
{
    if(_activePlayer==1){
    for(int i = 1; i < ui->Agrid->count(); i++){
                QWidget *button = ui->Agrid->itemAtPosition(i-1, 0)->widget();
                QPushButton *theButton = dynamic_cast<QPushButton*>(button);
                if(theButton->isEnabled()) {
                    theButton->setText(gameBrain.getPossibleScores(i));
                    theButton->setStyleSheet("color: rgb(94, 255, 97);");
                }
    }
    }

    else if(_activePlayer==2){
        for(int i = 1; i < ui->Bgrid->count(); i++){
                QWidget *button = ui->Bgrid->itemAtPosition(i-1, 0)->widget();
                QPushButton *theButton = dynamic_cast<QPushButton*>(button);
                if(theButton->isEnabled()) {
                    theButton->setText(gameBrain.getPossibleScores(i));
                    theButton->setStyleSheet("color: rgb(94, 255, 97);");
                }
            }
    }
   else if(_activePlayer==3){
        for(int i = 1; i < ui->Cgrid->count(); i++){
                QWidget *button = ui->Cgrid->itemAtPosition(i-1, 0)->widget();
                QPushButton *theButton = dynamic_cast<QPushButton*>(button);
                qDebug() << theButton;
                if(theButton->isEnabled()) {
                    theButton->setText(gameBrain.getPossibleScores(i));
                    theButton->setStyleSheet("color: rgb(94, 255, 97);");
                }
            }
    }
    else{
        for(int i = 1; i < ui->Dgrid->count(); i++){
                QWidget *button = ui->Dgrid->itemAtPosition(i-1, 0)->widget();
                QPushButton *theButton = dynamic_cast<QPushButton*>(button);
                qDebug() << theButton;
                if(theButton->isEnabled()) {
                    theButton->setText(gameBrain.getPossibleScores(i));
                    theButton->setStyleSheet("color: rgb(94, 255, 97);");
                }
            }
    }
}

void YahtzeeMainWin::setPlayerScoreToUi()
{
    if(_activePlayer == 1){
        for(int i = 1; i < ui->Agrid->count(); i++){
                QWidget *button = ui->Agrid->itemAtPosition(i-1, 0)->widget();
                QPushButton *theButton = dynamic_cast<QPushButton*>(button);
                theButton->setText(gameBrain.putPlayerScoreToUi(i,_activePlayer));
        }
    }
    else if(_activePlayer == 2){
        for(int i = 1; i < ui->Bgrid->count(); i++){
                QWidget *button = ui->Bgrid->itemAtPosition(i-1, 0)->widget();
                QPushButton *theButton = dynamic_cast<QPushButton*>(button);
                theButton->setText(gameBrain.putPlayerScoreToUi(i,_activePlayer));
        }
    }
    else if(_activePlayer == 3){
        for(int i = 1; i < ui->Cgrid->count(); i++){
                QWidget *button = ui->Cgrid->itemAtPosition(i-1, 0)->widget();
                QPushButton *theButton = dynamic_cast<QPushButton*>(button);
                theButton->setText(gameBrain.putPlayerScoreToUi(i,_activePlayer));
        }
    }
    else{
        for(int i = 1; i < ui->Dgrid->count(); i++){
                QWidget *button = ui->Dgrid->itemAtPosition(i-1, 0)->widget();
                QPushButton *theButton = dynamic_cast<QPushButton*>(button);
                theButton->setText(gameBrain.putPlayerScoreToUi(i,_activePlayer));
        }
    }

}
