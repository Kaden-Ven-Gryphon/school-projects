#ifndef MENUSCREEN_H
#define MENUSCREEN_H
#include <QDialog>
#include <QDebug>
#include <string>

#include <QDateTime>
#include "dartgame.h"
//#include "scorerwindow.h"
#include "playerparser.h"

namespace Ui {
class MenuScreen;
}

class MenuScreen : public QDialog
{
    Q_OBJECT

public:
    explicit MenuScreen(QWidget *parent = nullptr);
    ~MenuScreen();


private slots:
    void on_menuBeginButton_clicked();

private:
    Ui::MenuScreen *ui;
    //competitor
    QString firstCompetitor;
    QString secondCompetitor;

    // player parser
    PlayerParser *theParser;

    //leg starting value
    int legStartValueIndex;
    int legStartValue;

    //number of legs
    int numberOfLegs;

    //number of matches
    int numberOfMatches;

    //date
    QDateTime dateValue;

    //officials
    QString currentOfficial;

    //location
    QString currentLocation;



};

#endif // MENUSCREEN_H
