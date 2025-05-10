#include "notesenseanswer.h"

notesenseanswer::notesenseanswer(RandomNoteGenerator*input,pianokeys*input2)
    :generator(input)
    ,piano(input2)
    ,isAnswering(false)
{currentIndex=0;
keyMap=input2->keyMap_v;
speedMs=generator->speedMs;
}

void notesenseanswer::getAnswer(){
    if(isAnswering==true)
    {
        qDebug()<<"You are getting answers now!";
        return;
    }
    currentIndex=0;
    pianokeysanswer();
}
void notesenseanswer::pianokeysanswer(){
    answerNotes=generator->allAvailableNotes;
    isAnswering=true;
    if (currentIndex >= answerNotes.size()) {
        qDebug()<<"answer have all been output!";
        currentIndex=0;
        isAnswering=false;
        return;
    }
        int index=answerNotes[currentIndex];
        QPushButton* button =keyMap[index];
        button->setDown(true);
        emit button->pressed();
        QTimer::singleShot(speedMs, this, [this,button]() {
            button->setDown(false);
            emit button->released();
            currentIndex++;
            if (currentIndex < answerNotes.size()) {
                pianokeysanswer();
            } else {
                isAnswering = false;
            }
        });
}
