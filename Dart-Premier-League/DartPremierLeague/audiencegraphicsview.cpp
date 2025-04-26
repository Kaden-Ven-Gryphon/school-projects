#include "audiencegraphicsview.h"
#include "ui_audiencegraphicsview.h"

audienceGraphicsView::audienceGraphicsView(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::audienceGraphicsView)
{
    ui->setupUi(this);

    infoState1 = 0;
    infoState2 = 0;

    ui->currentPlayer2_2->setText(QString::fromStdString(MainManager::mainGame.player1.name));
    ui->currentPlayer2->setText(QString::fromStdString(MainManager::mainGame.player2.name));
    ui->legsPlayingUntil->setText(QString::fromStdString(std::to_string((MainManager::mainGame.numberOfLegsPerMatch / 2) +1)+ " Legs and " +std::to_string((MainManager::mainGame.numberOfMatches /2) +1) + " Matches"));

    //update
    ui->label_5->setText((QString::fromStdString("Leg " + std::to_string(MainManager::mainGame.legIndex) + " out of " + std::to_string(MainManager::mainGame.numberOfLegsPerMatch) + "\nMatch " + std::to_string(MainManager::mainGame.matchIndex) + " out of " + std::to_string(MainManager::mainGame.numberOfMatches))));
    ui->currentPlayer1LegsWon->setText(QString::fromStdString(std::to_string(MainManager::mainGame.p1LegWins) + " Legs\n" + std::to_string(MainManager::mainGame.p1MatchWins) + " Matches"));
    ui->currentPlayer2LegsWon->setText(QString::fromStdString(std::to_string(MainManager::mainGame.p2LegWins) + " Legs\n" + std::to_string(MainManager::mainGame.p2MatchWins) + " Matches"));
    ui->pointsRemainingPlayer1->setText((QString::fromStdString(std::to_string(MainManager::mainGame.activeLeg->playerOneEffectiveScore))));
    ui->pointsRemainingPlayer2->setText((QString::fromStdString(std::to_string(MainManager::mainGame.activeLeg->playerTwoEffectiveScore))));
    switch(infoState1)
    {
    case 0:
        ui->bestShotsPlayer1->setText("");
        break;
    }
    switch(infoState2)
    {
    case 0:
        ui->bestShotsPlayer2->setText("");
    }

}

audienceGraphicsView::~audienceGraphicsView()
{
    delete ui;
}

void audienceGraphicsView::on_closeAudienceViewButton_clicked()
{
    hide();
}

void audienceGraphicsView::updateInfo()
{
    //update
    ui->label_5->setText((QString::fromStdString("Leg " + std::to_string(MainManager::mainGame.legIndex) + " out of " + std::to_string(MainManager::mainGame.numberOfLegsPerMatch) + "\nMatch " + std::to_string(MainManager::mainGame.matchIndex) + " out of " + std::to_string(MainManager::mainGame.numberOfMatches))));
    ui->currentPlayer1LegsWon->setText(QString::fromStdString(std::to_string(MainManager::mainGame.p1LegWins) + " Legs\n" + std::to_string(MainManager::mainGame.p1MatchWins) + " Matches"));
    ui->currentPlayer2LegsWon->setText(QString::fromStdString(std::to_string(MainManager::mainGame.p2LegWins) + " Legs\n" + std::to_string(MainManager::mainGame.p2MatchWins) + " Matches"));
    ui->pointsRemainingPlayer1->setText((QString::fromStdString(std::to_string(MainManager::mainGame.activeLeg->playerOneEffectiveScore))));
    ui->pointsRemainingPlayer2->setText((QString::fromStdString(std::to_string(MainManager::mainGame.activeLeg->playerTwoEffectiveScore))));
    switch(infoState1)
    {
    case 1:
        ui->bestShotsPlayer1->setText(QString::fromStdString(MainManager::mainGame.activeLeg->calc3DartOut(MainManager::mainGame.activeLeg->playerOneEffectiveScore)));
        break;
    case 2:
        ui->bestShotsPlayer1->setText((QString::fromStdString(("Average Turn: " + std::to_string(MainManager::mainGame.p1TurnAverage) + "\nLowest Turn: " +std::to_string(MainManager::mainGame.p1LowestTurnScore)))));
        break;
    case 3:
        ui->bestShotsPlayer1->setText((QString::fromStdString("Number of 180s: " + std::to_string(MainManager::mainGame.p1num180s))));
        break;
    case 0:
    default:
        ui->bestShotsPlayer1->setText("");
        break;
    }
    switch(infoState2)
    {
    case 1:
        ui->bestShotsPlayer2->setText(QString::fromStdString(MainManager::mainGame.activeLeg->calc3DartOut(MainManager::mainGame.activeLeg->playerTwoEffectiveScore)));
        break;
    case 2:
        ui->bestShotsPlayer2->setText((QString::fromStdString(("Average Turn: " + std::to_string(MainManager::mainGame.p2TurnAverage) + "\nLowest Turn: " +std::to_string(MainManager::mainGame.p2LowestTurnScore)))));
        break;
    case 3:
        ui->bestShotsPlayer2->setText((QString::fromStdString("Number of 180s: " + std::to_string(MainManager::mainGame.p2num180s))));
        break;
    case 0:
    default:
        ui->bestShotsPlayer2->setText("");
        break;
    }
}
