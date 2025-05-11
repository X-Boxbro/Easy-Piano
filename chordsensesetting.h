#ifndef CHORDSENSESETTING_H
#define CHORDSENSESETTING_H

#include <QDialog>

namespace Ui {
class chordsensesetting;
}

class chordsensesetting : public QDialog
{
    Q_OBJECT

public:
    explicit chordsensesetting(QWidget *parent = nullptr);
    ~chordsensesetting();
    QStringList getSelectedChords() const;

private:
    Ui::chordsensesetting *ui;

private slots:

};

#endif // CHORDSENSESETTING_H
