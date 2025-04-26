#ifndef SCORERWINDOW_H
#define SCORERWINDOW_H
#include <QMainWindow>
#include "menuscreen.h"
#include <QDebug>
#include <QDateTime>
#include <QGraphicsView>
#include <QString>
#include <string>
#include <dartboard.h>
#include <QResizeEvent>
#include "dartgame.h"
#include "ui_ScorerWindow.h"
#include "mainmanager.h"
#include "createplayer.h"
#include "audiencegraphicsview.h"



QT_BEGIN_NAMESPACE
namespace Ui { class ScorerWindow; }
QT_END_NAMESPACE

class ScorerWindow : public QMainWindow
{
    Q_OBJECT

public:
    ScorerWindow(QWidget *parent = nullptr);
    ~ScorerWindow();
    void resizeEvent(QResizeEvent * event);

    DartGame dartGame;
    DartGame* mainGamePtr;

    void initGameDisplay();
    void updateDataDisplay();

private slots:

    // STACKED WIDGET
    void changeStackWidgetPage1();
    void changeStackWidgetPage2();

    //TABLE WIDGET
    void changeTableWidget();

    void display1a();
    void display2a();
    void display3a();
    void display4a();
    void display1b();
    void display2b();
    void display3b();
    void display4b();

    void submitBtn();
    void d1ResetBtn();
    void d2ResetBtn();
    void d3ResetBtn();
    void d1KnockBtn();
    void d2KnockBtn();
    void d3KnockBtn();

    void zoneDartHit(int zone);

    void on_menuButton_clicked();


    void on_createPlayerButton_clicked();

    void on_audienceView_clicked();

private:
    Ui::ScorerWindow *ui;
    MenuScreen *menscreen;
    createPlayer *createPlayerScreen;
    audienceGraphicsView *audienceViewScreen;


    void testInit();

};
#endif // SCORERWINDOW_H
