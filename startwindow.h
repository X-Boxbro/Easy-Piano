#ifndef STARTWINDOW_H
#define STARTWINDOW_H

#include <QWidget>
#include<QPushButton>
namespace Ui {
class startwindow;
}

class startwindow : public QWidget
{
    Q_OBJECT

public:
    explicit startwindow(QWidget *parent = nullptr);
    ~startwindow();
    void openMainWindow();

private:
    Ui::startwindow *ui;
};

#endif // STARTWINDOW_H
