#include "createplayer.h"
#include "ui_createplayer.h"
#include <QDebug>
#include <string>
#include <QDir>

createPlayer::createPlayer(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::createPlayer)
{
    ui->setupUi(this);
}

createPlayer::~createPlayer()
{
    delete ui;
}



void createPlayer::on_pushButton_clicked()
{
    std::string filePath = QDir().absolutePath().toStdString() + "/player_stats_csv.csv";
    std::ofstream playerFile;
    playerFile.open(filePath, std::ios_base::app);
    if(!playerFile.is_open()) return;



    //LAST NAME
    playerLastName=ui->lastNameLineEdit->text();
    playerFile << playerLastName.toStdString()<<",";
    //FIRST NAME
    playerFirstName=ui->fristNameLineEdit->text();
    playerFile << playerFirstName.toStdString()<<",";
    //NICKNAME
    playerNickname=ui->nickNameLineEdit->text();
    playerFile << playerNickname.toStdString()<<",";
    //AVERAGE
    playerAverage=ui->averageScoreLineEdit->text();
    playerFile << playerAverage.toStdString()<<",";
//    playerAverageFloat=float(playerAverageString);
    qDebug()<<"player average "<<playerAverage;

    //NINEDARTERS
    playerNineDarters=ui->nineDartFinishesLineEdit->text();
    playerFile << playerNineDarters.toStdString()<<",";

    //WINPERCENTAGE
    playerWinPercentage=ui->winPercentageLineEdit->text();
    playerFile << playerWinPercentage.toStdString()<<",";
    //RANKING
    playerRanking=ui->rankingLineEdit->text();
    playerFile << playerRanking.toStdString()<<",";
    //TOTAL 180'S
    playerTotal180s=ui->total180sLineEdit->text();
    playerFile << playerTotal180s.toStdString()<<std::endl;
    //remove menu
    playerFile.close();
    hide();
}

