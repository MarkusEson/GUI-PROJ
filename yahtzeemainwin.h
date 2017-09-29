#ifndef YAHTZEEMAINWIN_H
#define YAHTZEEMAINWIN_H

#include <QMainWindow>
#include <QLabel>
#include <QPushButton>
#include "gamebrain.h"

#define PLAYERONE 1
#define PLAYERTWO 2
#define PLAYERTHREE 3
#define PLAYERFOUR 4


namespace Ui {
class YahtzeeMainWin;
}

class YahtzeeMainWin : public QMainWindow
{
    Q_OBJECT

public:
    explicit YahtzeeMainWin(QWidget *parent = 0);
    ~YahtzeeMainWin();
    void showPlayerBlockersOnClick();
    void chooseAmountOfPlayers(int numOfPlayers);
    void setDieImage(QPushButton * button, Die die);
    void displayDiceOnScreen();
    void playerTurn(int _numOfPlayers);
    void resetScoreboardUI();
    void resetPlayerScoreboardUI();
    void lockDice();
    void unlockDice();
    void resetDice();
    std::map<QObject*, Key> _keyPressedFromUI;
    void uiScoreCalculator();
    void setPlayerScoreToUi();
    void openHelpWindow();
    void gameEnding();

private:
    Ui::YahtzeeMainWin *ui;
    int _numOfPlayers = 0;
    int _activePlayer = 1;
    int _timesRolled = 0;
    int _rounds = 0;
    int _winTimer = 0;
    int _winTimerEnd = 0;

    GameBrain _gameBrain;


private slots:
    void aButtonWasClicked();
    void aDiceWasClicked();
    void on_rollDiceButton_clicked();


    void on_threePlayerButton_triggered();
    void on_onePlayerButton_triggered();
    void on_twoPlayerButton_triggered();
    void on_fourPlayerButton_triggered();

    void on_actionGuide_triggered();
    void on_closeHelpButton_clicked();
};

#endif // YAHTZEEMAINWIN_H
