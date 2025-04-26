#include "menuscreen.h"
#include "ui_menuscreen.h"
#include <QDebug>
#include "mainmanager.h"
#include <string>
#include <QDir>

using namespace std;

MenuScreen::MenuScreen(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MenuScreen)
{
    ui->setupUi(this);
    // set up the parser
    qDebug() << QDir().absolutePath();
    theParser = new PlayerParser(QDir().absolutePath().toStdString() + "/player_stats_csv.csv");

    // loop through player records to load names into the comboBox menu
    for (int i=0;i<theParser->getRecordsSize(); i++)
    {
        string fullName;
        fullName = theParser->players[i].firstName + " " + theParser->players[i].lastName;
        ui->competitorOneBox->addItem(QString::fromStdString(fullName));
        ui->competitorTwoBox->addItem(QString::fromStdString(fullName));
    }
}

MenuScreen::~MenuScreen()
{
    delete ui;
}

void MenuScreen::on_menuBeginButton_clicked()
{
//    MenuScreen menscreen;
//    menscreen.setModal(true);

    //COMPETITORS
    //THIS IS TEMPURARY NEEDS TO GET THIS INFO FROM THE PLAYER MANAGER
   firstCompetitor=ui->competitorOneBox->currentText();
   MainManager::mainGame.player1.name = firstCompetitor.toStdString();
   qDebug()<<"first one is "<<firstCompetitor;
   secondCompetitor=ui->competitorTwoBox->currentText();
   MainManager::mainGame.player2.name= secondCompetitor.toStdString();
   qDebug()<<"second one is "<<secondCompetitor;

//   dartGame.player1.name = firstCompetitor.toStdString();
//   Ui_ScorerWindow scorerWind;
//   scorerWind.playersPlayingLabel->setText(firstCompetitor);

   //THIS MIGHT also have to be changed to be any number not just 301 501 801

   //LEG START VALUE
   legStartValueIndex=ui->legStartValueBox->currentIndex();
   if(legStartValueIndex==0){
       qDebug()<<"legStartValue Index="<<legStartValueIndex;
       legStartValue=301;
       qDebug()<<"legStartValue="<<legStartValue;
   }
   else if(legStartValueIndex==1){
       qDebug()<<"legStartValue Index="<<legStartValueIndex;
       legStartValue=501;
       qDebug()<<"legStartValue="<<legStartValue;
   }
   else if(legStartValueIndex==2){
       qDebug()<<"legStartValue Index="<<legStartValueIndex;
       legStartValue=801;
       qDebug()<<"legStartValue="<<legStartValue;
   }

   //NUMBER OF LEGS
   numberOfLegs=ui->numberOfLegsSpinBox->value();
   qDebug()<<"number of legs= "<<numberOfLegs;

   //NUMBER OF MATCHES
   numberOfMatches=ui->numberOfMatchesSpinBox->value();
   qDebug()<<"number of matches= "<<numberOfMatches;

   MainManager::mainGame.initGame(numberOfLegs, numberOfMatches, legStartValue);
   //DATE
   dateValue = ui->dateTimeMenu->dateTime();
   qDebug() << "Date Selected: " << dateValue;

   //OFFICIALS
   currentOfficial=ui->officialSpinBox->currentText();
   qDebug()<<"official: "<<currentOfficial;

   //LOCATION
   currentLocation = ui->locationPickBox->currentText();
   qDebug()<<"location is: "<<currentLocation;

    MainManager::mainGame.location = currentLocation.toStdString();

   //remove menu
   hide();

}

