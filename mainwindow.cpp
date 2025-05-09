#include "mainwindow.h"
#include "ui_mainwindow.h"
#include<string>
#include <QFileDialog>
#include <QFile>
#include <QStandardPaths>
#include <QDebug>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    ,synth(nullptr)
    ,mypiano(nullptr)
    ,virtualKeyboard(nullptr)
    ,mymanager(nullptr)
{
    ui->setupUi(this);
    QString mappingDirPath = QDir::currentPath() + "/mymapping";
    QDir dir;
    if (!dir.exists(mappingDirPath)) {
        if (dir.mkpath(mappingDirPath)) {
            qDebug() << "Mapping folder created at:" << mappingDirPath;
        } else {
            qWarning() << "Failed to create mapping folder.";
        }
    }
    this->mappingFolderPath = mappingDirPath;
    QString sf2Path = loadSf2FromResource(":/GeneralUser-GS.sf2");
    if (sf2Path.isEmpty()) {
        qDebug() << "Failed to load SoundFont";
        return;
    }

    std::string mypath = sf2Path.toStdString();
    synth =new fluidsynth_initial(mypath);
    mypiano=new pianokeys(synth,this);
    mypiano->bindKeysFromUI(this);
    virtualKeyboard = new keyboard(this);
    virtualKeyboard->initializeKeyMapping();
    virtualKeyboard->setFocus();
    mymanager = new mappingmanager(virtualKeyboard, this);
    virtualKeyboard->setMappingManager(mymanager);

    connect(ui->createMappingButton, &QPushButton::clicked, this, &MainWindow::onCreateMappingClicked);
    connect(ui->saveMappingButton, &QPushButton::clicked, this, &MainWindow::onSaveMappingClicked);
    connect(ui->loadMappingButton, &QPushButton::clicked, this, &MainWindow::onLoadMappingClicked);
    connect(ui->startdeletebutton,&QPushButton::clicked,this,&MainWindow::startDeleting);
    connect(ui->stopDeletebutton,&QPushButton::clicked,this,&MainWindow::stopDeleting);
}

MainWindow::~MainWindow()
{
    delete ui;
}

QString MainWindow::loadSf2FromResource(const QString& resourcePath) {
    QFile sf2File(resourcePath);
    if (!sf2File.open(QIODevice::ReadOnly)) {
        qWarning() << "Cannot open soundfont resource:" << resourcePath;
        return "";
    }

    QString tempPath = QStandardPaths::writableLocation(QStandardPaths::TempLocation) + "/temp_soundfont.sf2";
    QFile tempFile(tempPath);
    if (!tempFile.open(QIODevice::WriteOnly)) {
        qWarning() << "Cannot write temp soundfont file:" << tempPath;
        return "";
    }

    tempFile.write(sf2File.readAll());
    tempFile.close();
    sf2File.close();

    return tempPath;
}

void MainWindow::onCreateMappingClicked() {
    for (QPushButton* button : mypiano->getAllButtons()) {
        connect(button, &QPushButton::clicked, mymanager, &mappingmanager::onPianoKeyClicked);
    }
    mymanager->startMapping();
    virtualKeyboard->setFocus();
}


void MainWindow::onSaveMappingClicked() {
    QString filePath = QFileDialog::getSaveFileName(this, "保存映射文件", mappingFolderPath + "/mapping.json", "JSON Files (*.json)");
    if (!filePath.isEmpty()) {
        mymanager->saveMappingToFile(filePath);
        virtualKeyboard->setFocus();
    }
}

void MainWindow::onLoadMappingClicked() {
    QString filePath = QFileDialog::getOpenFileName(this, "打开映射文件",mappingFolderPath, "JSON Files (*.json)");
    if (!filePath.isEmpty()) {
        mymanager->loadMappingFromFile(filePath);
        virtualKeyboard->setFocus();
    }
}

void MainWindow::startDeleting() {
    for (QPushButton* button : virtualKeyboard->getAllButtons()) {
        connect(button, &QPushButton::clicked, mymanager, &mappingmanager::deleteMapping);
    }
    mymanager->startDeleting();
    virtualKeyboard->setFocus();
}

void MainWindow::stopDeleting(){
    mymanager->stopDeleting();
}
