#include "pianokeys.h"
#include<iostream>
#include<QPushButton>
#include<QWidget>
#include<QTimer>

pianokeys::pianokeys(fluidsynth_initial *synth, QObject *parent)
    :QObject(parent),synth(synth) {
    Q_ASSERT(synth);
}



void pianokeys::addKeys(QPushButton *button,int midiNote){
    if(!button)
        return ;

    keyMap[button]=midiNote;
    keyMap_v[midiNote] = button;

    connect(button,&QPushButton::pressed,this,&pianokeys::onKeyPressed);
    connect(button,&QPushButton::released,this,&pianokeys::onKeyReleased);

}

void pianokeys::onKeyPressed() {
    QPushButton *button = qobject_cast<QPushButton *>(sender());
    if (button && keyMap.contains(button)) {
        int midiNote = keyMap[button];
        if (activeNotes.contains(midiNote)) {
            return;
        }
        activeNotes.insert(midiNote);
        synth->playNote(0, midiNote, 110);
    }
}


void pianokeys::onKeyReleased() {
    QPushButton *button = qobject_cast<QPushButton *>(sender());
    if (button && keyMap.contains(button)) {
        int midiNote = keyMap[button];
        if (activeNotes.contains(midiNote)) {
            activeNotes.remove(midiNote);
        }

        QTimer::singleShot(1000,[this,midiNote](){
            synth->stopNote(0, midiNote);
        });
    }
}


void pianokeys::bindKeysFromUI(QWidget *parentWidget) {
    for (int i = 1; i <= 88; ++i) {
        QString buttonName = QString("keys_%1").arg(i);

        // 查找按钮
        QPushButton *button = parentWidget->findChild<QPushButton *>(buttonName);
        if (button) {
            button->setFocusPolicy(Qt::NoFocus);
            int midiNote = 20 + i;
            addKeys(button, midiNote);
        } else {
            std::cerr << "Button " << buttonName.toStdString() << " not found!" << std::endl;
        }
    }
}


QList<QPushButton*> pianokeys::getAllButtons() const {
    return keyMap.keys();
}

int pianokeys::getMidiNoteForButton(QPushButton* button) const {
    if (keyMap.contains(button)) {
        return keyMap.value(button);
    }
    return -1;
}
