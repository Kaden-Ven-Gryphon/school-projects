#ifndef CREATEPLAYER_H
#define CREATEPLAYER_H

#include <QDialog>
#include <QDebug>
#include <string>
#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>

namespace Ui {
class createPlayer;
}

class createPlayer : public QDialog
{
    Q_OBJECT

public:
    explicit createPlayer(QWidget *parent = nullptr);
    ~createPlayer();

private slots:
    void on_pushButton_clicked();

private:
    Ui::createPlayer *ui;

    //LAST NAME
    QString playerLastName;

    //FIRST NAME
    QString playerFirstName;

    //NICKNAME
    QString playerNickname;

    //AVERAGE
    QString playerAverage;

    //NINEDARTERS
    QString playerNineDarters;

    //WINPERCENTAGE
    QString playerWinPercentage;

    //RANKING
    QString playerRanking;

    //TOTAL 180'S
    QString playerTotal180s;
};

#endif // CREATEPLAYER_H
