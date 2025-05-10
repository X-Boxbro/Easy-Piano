#ifndef NOTESENSESETTING_H
#define NOTESENSESETTING_H

#include <QDialog>
#include<QList>
namespace Ui {
class notesensesetting;
}

class notesensesetting : public QDialog
{
    Q_OBJECT

public:
    explicit notesensesetting(QWidget *parent = nullptr);
    ~notesensesetting();
    int noteCount() const;
    int speed() const;
    QList<int> selectedScales() const;
private slots:
    void on_buttonBox_accepted();
    void on_buttonBox_rejected();
private:
    Ui::notesensesetting *ui;
};

#endif // NOTESENSESETTING_H
