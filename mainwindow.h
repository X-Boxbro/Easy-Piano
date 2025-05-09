#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include"fluidsynth_initial.h"
#include"pianokeys.h"
#include"keyboard.h"
#include"mappingmanager.h"
#include<QString>
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
private:
    Ui::MainWindow *ui;
    fluidsynth_initial*synth;
    pianokeys*mypiano;
    keyboard* virtualKeyboard;
    mappingmanager*mymanager;
    QString mappingFolderPath;
};
#endif // MAINWINDOW_H
