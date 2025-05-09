#include "pianokeys.h"
#include<QPushButton>
#include<QWidget>
#include<QTimer>
#include<QDebug>
pianokeys::pianokeys(fluidsynth_initial *synth, QObject *parent)
    :QObject(parent),synth(synth) {
    Q_ASSERT(synth);
}



void pianokeys::addKeys(QPushButton *button,int midiNote){
    if(!button)
        return ;
    //将按钮与MIDI音符绑定
    keyMap[button]=midiNote;
    keyMap_v[midiNote] = button;

    //连接信号与槽函数
    connect(button,&QPushButton::pressed,this,&pianokeys::onKeyPressed);
    connect(button,&QPushButton::released,this,&pianokeys::onKeyReleased);

}

void pianokeys::onKeyPressed() {
    QPushButton *button = qobject_cast<QPushButton *>(sender());
    if (button && keyMap.contains(button)) {
        int midiNote = keyMap[button];
        if (activeNotes.contains(midiNote)) {
            return; // 如果音符已经在播放，则忽略
        }
        activeNotes.insert(midiNote); // 标记音符为正在播放
        synth->playNote(0, midiNote, 110); // 播放音符，通道 0，力度 127
    }
}


void pianokeys::onKeyReleased() {
    QPushButton *button = qobject_cast<QPushButton *>(sender());
    if (button && keyMap.contains(button)) {
        int midiNote = keyMap[button];
        if (activeNotes.contains(midiNote)) {
            activeNotes.remove(midiNote); // 移除音符标记
        }
        // 使用QTimer实现延音效果而不破坏主线程
        QTimer::singleShot(1000,[this,midiNote](){
            synth->stopNote(0, midiNote); // 停止音符
        });
    }
}


void pianokeys::bindKeysFromUI(QWidget *parentWidget) {
    for (int i = 1; i <= 88; ++i) {
        // 使用 QString::arg 动态构建按钮名称，例如 keys_1, keys_2, ..., keys_88
        QString buttonName = QString("keys_%1").arg(i);

        // 查找按钮
        QPushButton *button = parentWidget->findChild<QPushButton *>(buttonName);
        if (button) {
            // MIDI 音符从 21(A0) 到 108(C8)
            button->setFocusPolicy(Qt::NoFocus); // 禁止钢琴键按钮抢占焦点
            int midiNote = 20 + i; // MIDI 音符从 21 开始
            addKeys(button, midiNote);
        } else {
            qDebug() << "Button " << buttonName.toStdString() << " not found!" ;
        }
    }
}


QList<QPushButton*> pianokeys::getAllButtons() const {
    return keyMap.keys(); // 返回所有琴键按钮
}

int pianokeys::getMidiNoteForButton(QPushButton* button) const {
    if (keyMap.contains(button)) {
        return keyMap.value(button);
    }
    return -1; // 如果找不到对应的按钮，返回 -1
}
