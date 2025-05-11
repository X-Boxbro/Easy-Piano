#include "chordsensesetting.h"
#include "ui_chordsensesetting.h"

chordsensesetting::chordsensesetting(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::chordsensesetting)
{
    ui->setupUi(this);
    for (int i = 0; i < ui->treeWidgetChords->topLevelItemCount(); ++i) {
        QTreeWidgetItem* parent = ui->treeWidgetChords->topLevelItem(i);

        // 顶层项可勾选（如果需要）
        parent->setFlags(parent->flags() | Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);
        parent->setCheckState(0, Qt::Unchecked);

        for (int j = 0; j < parent->childCount(); ++j) {
            QTreeWidgetItem* child = parent->child(j);
            child->setFlags(child->flags() | Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);
            child->setCheckState(0, Qt::Unchecked);
        }
    }
    connect(ui->treeWidgetChords, &QTreeWidget::itemChanged, this, [=](QTreeWidgetItem* item, int column) {
        // 如果是顶层项（即有子项）
        if (item->childCount() > 0) {
            Qt::CheckState state = item->checkState(0);
            for (int i = 0; i < item->childCount(); ++i) {
                QTreeWidgetItem* child = item->child(i);
                child->setCheckState(0, state);
            }
        }
    });
}

chordsensesetting::~chordsensesetting()
{
    delete ui;
}

QStringList chordsensesetting::getSelectedChords() const {
    QStringList selectedChords;

    for (int i = 0; i < ui->treeWidgetChords->topLevelItemCount(); ++i) {
        QTreeWidgetItem* parent = ui->treeWidgetChords->topLevelItem(i);
        for (int j = 0; j < parent->childCount(); ++j) {
            QTreeWidgetItem* child = parent->child(j);
            if (child->checkState(0) == Qt::Checked) {
                selectedChords << child->text(0);  // 假设第0列是名称，如 "C7"、"F#dim"
            }
        }
    }

    return selectedChords;
}
