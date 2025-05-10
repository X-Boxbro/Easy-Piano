#include "notesensesetting.h"
#include "ui_notesensesetting.h"

notesensesetting::notesensesetting(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::notesensesetting)
{
    ui->setupUi(this);
    for (int i = 0; i < ui->listWidgetScales->count(); ++i) {
        QListWidgetItem* item = ui->listWidgetScales->item(i);
        item->setFlags(Qt::ItemIsEnabled | Qt::ItemIsUserCheckable | Qt::ItemIsSelectable);
        item->setCheckState(Qt::Unchecked);
    }
    connect(ui->listWidgetScales, &QListWidget::itemChanged, this, [this](QListWidgetItem* item) {
    });

}

notesensesetting::~notesensesetting()
{
    delete ui;
}
// 获取用户设置的值
int notesensesetting::noteCount() const {
    return ui->spinBoxNoteCount->value();
}

int notesensesetting::speed() const {
    return ui->spinBoxSpeed->value();
}
QList<int>notesensesetting::selectedScales()const{
    QList<int> scales;
    scales.clear();
    for (int i = 0; i < ui->listWidgetScales->count(); ++i) {
        QListWidgetItem* item = ui->listWidgetScales->item(i);

        if (item->checkState() == Qt::Checked) {
            QString text = item->text();
            int index = text.indexOf('(');
            if (index > 0) {
                QString scaleStr = text.left(index);
                bool ok;
                int scale = scaleStr.toInt(&ok);
                if (ok && scale >= 1 && scale <= 7) {
                    scales.append(scale);
                    qDebug() << "  Added scale:" << scale;
                } else {
                    qDebug() << "  Invalid scale string:" << scaleStr;
                }
            } else {
                qDebug() << "  No '(' found in text:" << text;
            }
        }
    }
    return scales;
}
void notesensesetting::on_buttonBox_accepted() {
    accept();
}

void notesensesetting::on_buttonBox_rejected() {
    reject();
}
