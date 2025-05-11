#include "mainwindow.h"
#include "ui_mainwindow.h"
//添加使用的库
#include<string>
#include <QFileDialog>
#include <QFile>
#include <QStandardPaths>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , synth(nullptr)
    ,mypiano(nullptr)
    ,mymanager(nullptr)
    ,virtualKeyboard(nullptr)
    ,mygenerator(nullptr)
    ,mynoteanswer(nullptr)
    ,mygenerator2(nullptr)
    ,mychordanswer(nullptr)
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
    mygenerator = new RandomNoteGenerator(synth);
    mynoteanswer=new notesenseanswer(mygenerator,mypiano);
    mygenerator2 =new RandomChordGenerator(synth);
    mychordanswer=new chordsenseanswer(mygenerator2,mypiano);

    connect(ui->createMappingButton, &QPushButton::clicked, this, &MainWindow::onCreateMappingClicked);
    connect(ui->saveMappingButton, &QPushButton::clicked, this, &MainWindow::onSaveMappingClicked);
    connect(ui->loadMappingButton, &QPushButton::clicked, this, &MainWindow::onLoadMappingClicked);
    connect(ui->startdeletebutton,&QPushButton::clicked,this,&MainWindow::startDeleting);
    connect(ui->stopDeletebutton,&QPushButton::clicked,this,&MainWindow::stopDeleting);


    ui->earGameButton->hide();
    ui->freePlayButton->hide();
    ui->noteSenseButton->hide();
    ui->songSenseButton->hide();
    ui->returnfromfreebutton->hide();
    ui->returnfromnotebutton->hide();
    ui->returnfromchordbutton->hide();

    ui->randomstartbutton->hide();
    ui->noteanswerbutton->hide();
    ui->nextnotebutton->hide();

    ui->randombutton->hide();
    ui->chordanswerbutton->hide();
    ui->nextchordbutton->hide();

    connect(ui->modeButton, &QPushButton::clicked, this, &MainWindow::on_modeButton_clicked);
    connect(ui->earGameButton,&QPushButton::clicked,this,&MainWindow::on_earGameButton_clicked);
    connect(ui->freePlayButton,&QPushButton::clicked,this,&MainWindow::openFreeWindow);
    connect(ui->returnfromfreebutton,&QPushButton::clicked,this,&MainWindow::returnfromfreeWindow);
    connect(ui->noteSenseButton,&QPushButton::clicked,this,&MainWindow::openNoteSenseWindow);
    connect(ui->returnfromnotebutton,&QPushButton::clicked,this,&MainWindow::returnfromnoteWindow);
    connect(ui->songSenseButton,&QPushButton::clicked,this,&MainWindow::openChordWindow);
    connect(ui->returnfromchordbutton,&QPushButton::clicked,this,&MainWindow::returnfromChordWindow);
;}

MainWindow::~MainWindow()
{
    delete synth;
    delete mypiano;
    delete mymanager;
    delete virtualKeyboard;
    delete mygenerator;
    delete mygenerator2;
    delete mynoteanswer;
    delete mychordanswer;
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


void MainWindow::openFreeWindow() {

    ui->createMappingButton->hide();
    ui->startdeletebutton->hide();
    ui->saveMappingButton->hide();
    ui->stopDeletebutton->hide();
    ui->loadMappingButton->hide();
    ui->createMappingButton->hide();
    ui->modeButton->hide();
    ui->earGameButton->hide();
    ui->freePlayButton->hide();
    ui->returnfromfreebutton->setVisible(true);
    virtualKeyboard->setFocus();
}

void MainWindow::returnfromfreeWindow(){
    ui->createMappingButton->setVisible(true);
    ui->startdeletebutton->setVisible(true);
    ui->saveMappingButton->setVisible(true);
    ui->stopDeletebutton->setVisible(true);
    ui->loadMappingButton->setVisible(true);
    ui->modeButton->setVisible(true);
    ui->earGameButton->setVisible(true);
    ui->freePlayButton->setVisible(true);
    ui->returnfromfreebutton->hide();
    virtualKeyboard->setFocus();
}

void MainWindow::openNoteSenseWindow(){

    notesensesetting dialog(this);
    if (dialog.exec() == QDialog::Accepted) {
        ui->createMappingButton->hide();
        ui->startdeletebutton->hide();
        ui->saveMappingButton->hide();
        ui->stopDeletebutton->hide();
        ui->loadMappingButton->hide();
        ui->createMappingButton->hide();
        ui->modeButton->hide();
        ui->earGameButton->hide();
        ui->freePlayButton->hide();
        ui->noteSenseButton->hide();
        ui->songSenseButton->hide();
        ui->returnfromnotebutton->setVisible(true);
        ui->randomstartbutton->setVisible(true);
        ui->noteanswerbutton->setVisible(true);
        ui->nextnotebutton->setVisible(true);
        virtualKeyboard->setFocus();

        int noteCount = dialog.noteCount();
        int speed = dialog.speed();
        QList<int> scales = dialog.selectedScales();

        mygenerator->setNoteCount(noteCount);
        mygenerator->setSpeed(speed);
        mygenerator->setScales(scales);
        mygenerator->setvirtualKeyboard(virtualKeyboard);
        mynoteanswer->setvirtualKeyboard(virtualKeyboard);

        connect(ui->randomstartbutton,&QPushButton::clicked,mygenerator,&RandomNoteGenerator::start);
        connect(ui->nextnotebutton,&QPushButton::clicked,mygenerator,&RandomNoteGenerator::nextNote);
        connect(ui->noteanswerbutton,&QPushButton::clicked,mynoteanswer,&notesenseanswer::getAnswer);
    }
}
void MainWindow::returnfromnoteWindow(){
    ui->createMappingButton->setVisible(true);
    ui->startdeletebutton->setVisible(true);
    ui->saveMappingButton->setVisible(true);
    ui->stopDeletebutton->setVisible(true);
    ui->loadMappingButton->setVisible(true);
    ui->modeButton->setVisible(true);
    ui->earGameButton->setVisible(true);
    ui->freePlayButton->setVisible(true);
    ui->noteSenseButton->setVisible(true);
    ui->songSenseButton->setVisible(true);
    ui->returnfromnotebutton->hide();
    ui->randomstartbutton->hide();
    ui->noteanswerbutton->hide();
    ui->nextnotebutton->hide();
    virtualKeyboard->setFocus();
}
void MainWindow::openChordWindow(){
    chordsensesetting dialog(this);
    if(dialog.exec() == QDialog::Accepted){
    ui->createMappingButton->hide();
    ui->startdeletebutton->hide();
    ui->saveMappingButton->hide();
    ui->stopDeletebutton->hide();
    ui->loadMappingButton->hide();
    ui->createMappingButton->hide();
    ui->modeButton->hide();
    ui->earGameButton->hide();
    ui->freePlayButton->hide();
    ui->earGameButton->hide();
    ui->freePlayButton->hide();
    ui->returnfromchordbutton->setVisible(true);
    ui->randombutton->setVisible(true);
    ui->chordanswerbutton->setVisible(true);
    ui->nextchordbutton->setVisible(true);
    virtualKeyboard->setFocus();

    QStringList selectedChordNames = dialog.getSelectedChords();
    qDebug()<<"You had seleted :"<<selectedChordNames;

    mygenerator2->setAvailableChordList(selectedChordNames);
    mygenerator2->setvirtualKeyboard(virtualKeyboard);
    mychordanswer->setvirtualKeyboard(virtualKeyboard);

    connect(ui->randombutton,&QPushButton::clicked,mygenerator2,&RandomChordGenerator::start);
    connect(ui->nextchordbutton,&QPushButton::clicked,mygenerator2,&RandomChordGenerator::nextChord);
    connect(ui->chordanswerbutton,&QPushButton::clicked,mychordanswer,&chordsenseanswer::getAnswer);
    }
}
void MainWindow::returnfromChordWindow(){
    ui->createMappingButton->setVisible(true);
    ui->startdeletebutton->setVisible(true);
    ui->saveMappingButton->setVisible(true);
    ui->stopDeletebutton->setVisible(true);
    ui->loadMappingButton->setVisible(true);
    ui->modeButton->setVisible(true);
    ui->earGameButton->setVisible(true);
    ui->freePlayButton->setVisible(true);
    ui->earGameButton->setVisible(true);
    ui->freePlayButton->setVisible(true);
    ui->returnfromchordbutton->hide();
    ui->randombutton->hide();
    ui->chordanswerbutton->hide();
    ui->nextchordbutton->hide();
    virtualKeyboard->setFocus();
};

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

void MainWindow::on_modeButton_clicked() {
    ui->earGameButton->setVisible(!ui->earGameButton->isVisible());
    ui->freePlayButton->setVisible(!ui->freePlayButton->isVisible());
    virtualKeyboard->setFocus();

    if (!ui->earGameButton->isVisible()) {
        ui->noteSenseButton->hide();
        ui->songSenseButton->hide();
    }
}


void MainWindow::on_earGameButton_clicked() {
    ui->noteSenseButton->setVisible(!ui->noteSenseButton->isVisible());
    ui->songSenseButton->setVisible(!ui->songSenseButton->isVisible());
    virtualKeyboard->setFocus();
}

