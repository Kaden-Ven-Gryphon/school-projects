#include "scorerwindow.h"

#include <QApplication>

#include <QGraphicsView>
#include <QGraphicsScene>
#include <dartboard.h>
#include "mainmanager.h"

DartGame MainManager::mainGame;


int main(int argc, char *argv[])
{
    //*

    QApplication a(argc, argv);
    ScorerWindow w;
    w.show();
    w.resize(1000,600);
    return a.exec();
    //*/



}
