#include "scorerwindow.h"
#include "ui_ScorerWindow.h"

int on=0;
int onstackWidget=0;


ScorerWindow::ScorerWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::ScorerWindow)
{
    ui->setupUi(this);
    //String Notation Connection
    //connect(reference of object you are trying to capture signals for, specify the signal you are trying to capture,
    //  specify object affected by signal, specify a method on that object)
    connect(ui->menuButton,SIGNAL(clicked()),this,SLOT(changeStackWidgetPage1()));
    connect(ui->historyButton,SIGNAL(clicked()),this,SLOT(changeStackWidgetPage2()));
    connect(ui->createPlayerButton,SIGNAL(clicked()),this,SLOT(changeTableWidget()));
    connect(ui->submitButton,SIGNAL(clicked()),this,SLOT(submitBtn()));
    connect(ui->Dart1Knock,SIGNAL(clicked()), this, SLOT(d1KnockBtn()));
    connect(ui->Dart2Knock,SIGNAL(clicked()), this, SLOT(d2KnockBtn()));
    connect(ui->Dart3Knock,SIGNAL(clicked()), this, SLOT(d3KnockBtn()));
    connect(ui->Dart1Reset,SIGNAL(clicked()), this, SLOT(d1ResetBtn()));
    connect(ui->Dart2Reset,SIGNAL(clicked()), this, SLOT(d2ResetBtn()));
    connect(ui->Dart3Reset,SIGNAL(clicked()), this, SLOT(d3ResetBtn()));
    connect(ui->pushButton,SIGNAL(clicked()), this, SLOT(display1a()));
    connect(ui->pushButton_3,SIGNAL(clicked()), this, SLOT(display2a()));
    connect(ui->pushButton_4,SIGNAL(clicked()), this, SLOT(display3a()));
    connect(ui->pushButton_2,SIGNAL(clicked()), this, SLOT(display4a()));
    connect(ui->pushButton_5,SIGNAL(clicked()), this, SLOT(display1b()));
    connect(ui->pushButton_6,SIGNAL(clicked()), this, SLOT(display2b()));
    connect(ui->pushButton_7,SIGNAL(clicked()), this, SLOT(display3b()));
    connect(ui->pushButton_8,SIGNAL(clicked()), this, SLOT(display4b()));

    QGraphicsScene* scene = new QGraphicsScene(0, 0, 400, 400);
    ui->DartBoardGraphicsView->setScene(scene);
    //scene->setBackgroundBrush(Qt::black);
    DartBoard* dart_board = new DartBoard;
    scene->addItem(dart_board);
    dart_board->setTransformOriginPoint(200,200);
    dart_board->setPos(0.0, 0.0);


    mainGamePtr = &MainManager::mainGame;


    dart_board->setScale(1.1);
    //ui->DartBoardGraphicsView->fitInView(dart_board, Qt::KeepAspectRatio);
    ui->DartBoardGraphicsView->centerOn(0.0,0.0);

    connect(dart_board, SIGNAL(zoneClicked(int)), this, SLOT(zoneDartHit(int)));


    updateDataDisplay();
}

ScorerWindow::~ScorerWindow()
{
    delete ui;
}

void ScorerWindow::resizeEvent(QResizeEvent * event)
{

    QMainWindow::resizeEvent(event);
    float heightRatio = ui->DartBoardGraphicsView->height();
    float widthRatio = ui->DartBoardGraphicsView->width();

    float scaleRatio = 0;
    if(widthRatio < heightRatio) scaleRatio = widthRatio;
    else scaleRatio = heightRatio;
    scaleRatio/=400;
    qDebug()<<"scaleRatio: " << scaleRatio;

    ui->DartBoardGraphicsView->scene()->items()[0]->setScale(scaleRatio);
    ui->DartBoardGraphicsView->centerOn(0.0,0.0);

}

///Stacked widget = History, etc..
void ScorerWindow::changeStackWidgetPage1(){
    qDebug()<<"Page1";
//    if (on==1){
//        ui->stackedWidget->setCurrentIndex(1);
//        onstackWidget=0;
//        qDebug()<<"1";
//    }
//    else if(on==0){
//        ui->stackedWidget->setCurrentIndex(0);
//        onstackWidget=1;
//        qDebug()<<"0";
//    }
    mainGamePtr->printDartHistory();
    ui->stackedWidget->setCurrentIndex(0);
}
void ScorerWindow::changeStackWidgetPage2()
{
    qDebug()<<"History";
    ui->stackedWidget->setCurrentIndex(1);
    mainGamePtr->printDartHistory();
}


void ScorerWindow::display1a()
{
    audienceViewScreen->infoState1 = 0;
    updateDataDisplay();
}
void ScorerWindow::display2a()
{
    audienceViewScreen->infoState1 = 1;
    updateDataDisplay();
}
void ScorerWindow::display3a()
{
    audienceViewScreen->infoState1 = 2;
    updateDataDisplay();
}
void ScorerWindow::display4a()
{
    audienceViewScreen->infoState1 = 3;
    updateDataDisplay();
}
void ScorerWindow::display1b()
{
    audienceViewScreen->infoState2 = 0;
    updateDataDisplay();
}
void ScorerWindow::display2b()
{
    audienceViewScreen->infoState2 = 1;
    updateDataDisplay();
}
void ScorerWindow::display3b()
{
    audienceViewScreen->infoState2 = 2;
    updateDataDisplay();
}
void ScorerWindow::display4b()
{
    audienceViewScreen->infoState2 = 3;
    updateDataDisplay();
}

void ScorerWindow::zoneDartHit(int zone)
{
    if(MainManager::mainGame.activeLeg != nullptr) MainManager::mainGame.activeLeg->throwDart(zone);
    updateDataDisplay();
    qDebug() << zone;
}

void ScorerWindow::submitBtn()
{
    mainGamePtr->submitTurn();
    updateDataDisplay();
}

void ScorerWindow::d1KnockBtn()
{
    if(MainManager::mainGame.activeLeg == nullptr) return;
    MainManager::mainGame.activeLeg->knockOut(0);
    updateDataDisplay();
}

void ScorerWindow::d2KnockBtn()
{
    if(MainManager::mainGame.activeLeg == nullptr) return;
    mainGamePtr->activeLeg->knockOut(1);
    updateDataDisplay();
}

void ScorerWindow::d3KnockBtn()
{
    if(MainManager::mainGame.activeLeg == nullptr) return;
    mainGamePtr->activeLeg->knockOut(2);
    updateDataDisplay();
}

void ScorerWindow::d1ResetBtn()
{
    if(MainManager::mainGame.activeLeg == nullptr) return;
    mainGamePtr->activeLeg->resetDart(0);
    updateDataDisplay();
}

void ScorerWindow::d2ResetBtn()
{
    if(MainManager::mainGame.activeLeg == nullptr) return;
    mainGamePtr->activeLeg->resetDart(1);
    updateDataDisplay();
}

void ScorerWindow::d3ResetBtn()
{
    if(MainManager::mainGame.activeLeg == nullptr) return;
    mainGamePtr->activeLeg->resetDart(2);
    updateDataDisplay();
}

void ScorerWindow::updateDataDisplay()
{
    if(MainManager::mainGame.activeLeg == nullptr) return;
    ui->currentPlayerLabel->setText(QString::fromStdString("Player: " + mainGamePtr->activeLeg->players[mainGamePtr->activeLeg->activePlayer].name));
    ui->sectionDart1Label->setText(QString::fromStdString(mainGamePtr->activeLeg->activeThreeDarts[0].boardHit));
    ui->pointsDart1Label->setText(QString::fromStdString(std::to_string(mainGamePtr->activeLeg->activeThreeDarts[0].points)));
    ui->sectionDart2Label->setText(QString::fromStdString(mainGamePtr->activeLeg->activeThreeDarts[1].boardHit));
    ui->pointsDart1Label_2->setText(QString::fromStdString(std::to_string(mainGamePtr->activeLeg->activeThreeDarts[1].points)));
    ui->sectionDart3Label->setText(QString::fromStdString(mainGamePtr->activeLeg->activeThreeDarts[2].boardHit));
    ui->pointsDart1Label_3->setText(QString::fromStdString(std::to_string(mainGamePtr->activeLeg->activeThreeDarts[2].points)));

    if(mainGamePtr->activeLeg->activePlayer == 0)
    {
        ui->currentPlayerPoints->setText(QString::fromStdString("Points: " + std::to_string(mainGamePtr->activeLeg->playerOneEffectiveScore)));
        ui->label_4->setText(QString::fromStdString(mainGamePtr->activeLeg->calc3DartOut(mainGamePtr->activeLeg->playerOneEffectiveScore)));
    }
    else
    {
        ui->currentPlayerPoints->setText(QString::fromStdString("Points: " + std::to_string(mainGamePtr->activeLeg->playerTwoEffectiveScore)));
        ui->label_4->setText(QString::fromStdString(mainGamePtr->activeLeg->calc3DartOut(mainGamePtr->activeLeg->playerTwoEffectiveScore)));
    }

    int dart3Total = mainGamePtr->activeLeg->activeThreeDarts[0].points + mainGamePtr->activeLeg->activeThreeDarts[1].points + mainGamePtr->activeLeg->activeThreeDarts[2].points;

    ui->pointsTotal->setText(QString(QString::fromStdString((std::to_string(dart3Total)))));
    ui->playersPlayingLabel->setText(QString::fromStdString(mainGamePtr->player1.name + " vs. " + mainGamePtr->player2.name));
    ui->LocationLabel->setText(QString::fromStdString(mainGamePtr->location));

    ui->matchesCompletedLabel->setText(QString::fromStdString("Match Number: " + std::to_string(mainGamePtr->activeLeg->matchNumber + 1)));
    ui->legNumberLabel->setText((QString::fromStdString("Leg: " + std::to_string(mainGamePtr->activeLeg->legNumber +1))));

    audienceViewScreen->updateInfo();
}

void ScorerWindow::initGameDisplay()
{
    ui->playersPlayingLabel->setText(QString::fromStdString(mainGamePtr->player1.name + " vs. " + mainGamePtr->player2.name));
    ui->LocationLabel->setText(QString::fromStdString(mainGamePtr->location));
}

void ScorerWindow::testInit()
{
    mainGamePtr->player1.name = "Wright";
    mainGamePtr->player2.name = "Van Gerwen";
    mainGamePtr->location = "Huntsville";
    mainGamePtr->activeLeg = new Leg(dartGame.player1, dartGame.player2, 0, 1, 1, 501);

    ui->playersPlayingLabel->setText(QString::fromStdString(mainGamePtr->player1.name + " vs. " + mainGamePtr->player2.name));
    ui->LocationLabel->setText(QString::fromStdString(mainGamePtr->location));
}



void ScorerWindow::on_menuButton_clicked()
{
//    MenuScreen menuScreen;
//    menuScreen.setModal(true);
//    menuScreen.exec();
    //hide();
    menscreen =  new MenuScreen(this);
    menscreen->show();
}


void ScorerWindow::on_createPlayerButton_clicked()
{
    createPlayerScreen = new createPlayer(this);
    createPlayerScreen->show();
}


void ScorerWindow::on_audienceView_clicked()
{
    audienceViewScreen= new audienceGraphicsView(this);
    audienceViewScreen->show();
}

