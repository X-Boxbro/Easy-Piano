#include "chordsenseanswer.h"

chordsenseanswer::chordsenseanswer(RandomChordGenerator*input,pianokeys*input2)
    :generator(input)
    ,piano(input2)
    ,isAnswering(false)
{
    keyMap=input2->keyMap_v;
    setchordMap_v();
}

void chordsenseanswer::getAnswer(){
    if(isAnswering==true)
    {
        qDebug()<<"You are getting answers now!";
        return;
    }
    pianokeysanswer();
    virtualKeyboard->setFocus();
}

void chordsenseanswer::setchordMap_v(){
    chordMap_v=generator->chordMap_v;
}

void chordsenseanswer::pianokeysanswer(){
    answerChord=generator->currentChord;
    isAnswering=true;

    QList<QPushButton*> pressedButtons;
    for (int note : answerChord) {
        if (keyMap.contains(note)) {
            QPushButton* button = keyMap[note];
            button->setDown(true);
            emit button->pressed();
            pressedButtons.append(button);
        }
    }
    qDebug()<<chordMap_v[answerChord];
    QTimer::singleShot(500, this, [this, pressedButtons]() mutable {
        for (QPushButton* button : pressedButtons) {
            button->setDown(false);
            emit button->released();
        }
        isAnswering = false;
    });
}
void chordsenseanswer::setvirtualKeyboard(keyboard*input){
    virtualKeyboard=input;
}
