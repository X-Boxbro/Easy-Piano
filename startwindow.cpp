#include "startwindow.h"
#include "ui_startwindow.h"
#include"mainwindow.h"

startwindow::startwindow(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::startwindow)
{
    ui->setupUi(this);
    QPushButton*settingbtn=this->findChild<QPushButton*>("settingbutton");
    if (settingbtn) {
        connect(settingbtn, &QPushButton::clicked, this, &startwindow::openMainWindow);
    }
}

startwindow::~startwindow()
{
    delete ui;
}

void startwindow::openMainWindow() {
    this->hide();

    MainWindow* mainWindow = new MainWindow();
    mainWindow->show();
}
