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
        {ui->A1, WIPenum::ones}, {ui->B1, WIPenum::ones}, {ui->C1, WIPenum::ones}, {ui->D1, WIPenum::ones},

        {ui->A2, WIPenum::twos},
        {ui->B2, WIPenum::twos},
        {ui->C2, WIPenum::twos},
        {ui->D2, WIPenum::twos},

        {ui->A3, WIPenum::threes},
        {ui->A3, WIPenum::threes},
        {ui->A3, WIPenum::threes},
        {ui->A3, WIPenum::threes},

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
        {ui->A7, WIPenum::sum},
        {ui->A7, WIPenum::sum},
        {ui->A7, WIPenum::sum},

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
        {ui->A19, WIPenum::total},
        {ui->A19, WIPenum::total},
        {ui->A19, WIPenum::total},

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

void YahtzeeMainWin::chooseAmountOfPlayers()
{
    // First turns on the player blocker windows, then hides them according to how many players are playing.
    _activePlayer = PLAYERONE;
    showPlayerBlockersOnClick();
    ui->playerBlockerA->hide();
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
            ui->playerBlockerA->hide();
            break;

        case 2:
            showPlayerBlockersOnClick();
            ui->playerBlockerB->hide();
            break;

        case 3:
            showPlayerBlockersOnClick();
            ui->playerBlockerC->hide();
            break;

        case 4:
            showPlayerBlockersOnClick();
            ui->playerBlockerD->hide();
            break;
        }
}

void YahtzeeMainWin::aButtonWasClicked()
{
    QPushButton *theButton = dynamic_cast<QPushButton*>(sender());

    if(theButton == ui->A16)
        ui->A18->setEnabled(true);
    if(theButton == ui->B16)
        ui->B18->setEnabled(true);
    if(theButton == ui->C16)
        ui->C18->setEnabled(true);
    if(theButton == ui->D16)
        ui->D18->setEnabled(true);




    /*
     * Calls the function calculateScoreBoard, which is called every time a player clicks the score board.
     * This Function updates the ui scoreboard to show the updated points in "score", "bonus", and "total score"
     * calculateScoreBoard(<the players index in _scoreArray>, <the scoreIndexer>)
     *
     * The function then advances to next player. it also locks the score button so that it cannot be clicked agin.
     * - Markus
     */
    if(theButton)
    {
        //dynamic_cast<QPushButton*>(sender())->setText("12");
        //dynamic_cast<QPushButton*>(sender())->setEnabled(false);

        if(_activePlayer == PLAYERONE){
            ui->A7->setText(GameBrain::calculateScoreBoard(_activePlayer, 0));
            ui->A8->setText(GameBrain::calculateScoreBoard(_activePlayer, 1));
            ui->A19->setText(GameBrain::calculateScoreBoard(_activePlayer, 2));
        }
        if(_activePlayer == PLAYERTWO){
            ui->B7->setText(GameBrain::calculateScoreBoard(_activePlayer, 0));
            ui->B8->setText(GameBrain::calculateScoreBoard(_activePlayer, 1));
            ui->B19->setText(GameBrain::calculateScoreBoard(_activePlayer, 2));
        }
        if(_activePlayer == PLAYERTHREE){
            ui->C7->setText(GameBrain::calculateScoreBoard(_activePlayer, 0));
            ui->C8->setText(GameBrain::calculateScoreBoard(_activePlayer, 1));
            ui->C19->setText(GameBrain::calculateScoreBoard(_activePlayer, 2));
        }
        if(_activePlayer == PLAYERFOUR){
            ui->D7->setText(GameBrain::calculateScoreBoard(_activePlayer, 0));
            ui->D8->setText(GameBrain::calculateScoreBoard(_activePlayer, 1));
            ui->D19->setText(GameBrain::calculateScoreBoard(_activePlayer, 2));
        }

        _activePlayer++;
        gameBrain.resetChecked();
        playerTurn(_numOfPlayers); // player func that changes turns to next player.
        displayDiceOnScreen();

        for(int i = 0; i < ui->diceButtonLayout->count() - 1; i++){
            QWidget *button = ui->diceButtonLayout->itemAt(i)->widget();
                button->setDisabled(true);
        }
        dynamic_cast<QPushButton*>(sender())->setEnabled(false);
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
}

void YahtzeeMainWin::on_rollDiceButton_clicked() // Added rollDice func
{
    QSound::play(":/new/pictures/shakedice.wav");
    if(_timesRolled <= 2 )
        gameBrain.rollDice();
        displayDiceOnScreen();
    if(_timesRolled == 2)
        ui->rollDiceButton->setDisabled(true);

    for(int i = 0; i < ui->diceButtonLayout->count(); i++){
        QWidget *button = ui->diceButtonLayout->itemAt(i)->widget();
            if(!button->isEnabled())
                button->setEnabled(true);
    }
    _timesRolled++;
}

void YahtzeeMainWin::on_onePlayerButton_triggered()
{
    ui->rollDiceButton->setEnabled(true);
    _numOfPlayers = 1;
    chooseAmountOfPlayers();
    GameBrain::resetScoreBoard();
    ui->helpLabel->setText("Press New Game to restart!");
}

void YahtzeeMainWin::on_twoPlayerButton_triggered()
{
    ui->rollDiceButton->setEnabled(true);
    _numOfPlayers = 2;
    chooseAmountOfPlayers();
    GameBrain::resetScoreBoard();
    ui->helpLabel->setText("Press New Game to restart!");
}

void YahtzeeMainWin::on_threePlayerButton_triggered()
{
    ui->rollDiceButton->setEnabled(true);
    _numOfPlayers = 3;
    chooseAmountOfPlayers();
    GameBrain::resetScoreBoard();
    ui->helpLabel->setText("Press New Game to restart!");
}

void YahtzeeMainWin::on_fourPlayerButton_triggered()
{
    ui->rollDiceButton->setEnabled(true);
    _numOfPlayers = 4;
    chooseAmountOfPlayers();
    GameBrain::resetScoreBoard();
    ui->helpLabel->setText("Press New Game to restart!");
}
