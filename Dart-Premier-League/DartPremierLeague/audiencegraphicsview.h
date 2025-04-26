#ifndef AUDIENCEGRAPHICSVIEW_H
#define AUDIENCEGRAPHICSVIEW_H

#include <QDialog>
#include <QDebug>
#include <string>
#include "mainmanager.h"


namespace Ui {
class audienceGraphicsView;
}

class audienceGraphicsView : public QDialog
{
    Q_OBJECT

public:
    explicit audienceGraphicsView(QWidget *parent = nullptr);
    ~audienceGraphicsView();

    void updateInfo();
    int infoState1;
    int infoState2;

private slots:
    void on_closeAudienceViewButton_clicked();

private:
    Ui::audienceGraphicsView *ui;
};

#endif // AUDIENCEGRAPHICSVIEW_H
