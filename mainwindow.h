#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include"fluidsynth_initial.h"
#include"pianokeys.h"
#include"keyboard.h"
#include"mappingmanager.h"
#include"notesensesetting.h"
#include"notesenseanswer.h"
#include"randomnotegenerator.h"
#include<QString>
#include"chordsensesetting.h"
#include"randomchordgenerator.h"
#include"chordsenseanswer.h"
QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    QString loadSf2FromResource(const QString& resourcePath);
    void onCreateMappingClicked();
    void onSaveMappingClicked();
    void onLoadMappingClicked();
    void startDeleting();
    void stopDeleting();
    void on_modeButton_clicked();
    void on_earGameButton_clicked();
    void on_songSenseButton_clicked();
    void openFreeWindow();
    void returnfromfreeWindow();
    void openNoteSenseWindow();
    void returnfromnoteWindow();
    void openChordWindow();
    void returnfromChordWindow();
private:
    Ui::MainWindow *ui;
    fluidsynth_initial*synth;
    pianokeys*mypiano;
    mappingmanager*mymanager;
    keyboard* virtualKeyboard;
    RandomNoteGenerator*mygenerator;
    notesenseanswer*mynoteanswer;
    QString mappingFolderPath;
    RandomChordGenerator*mygenerator2;
    chordsenseanswer*mychordanswer;
};
#endif // MAINWINDOW_H
