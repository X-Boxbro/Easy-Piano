#ifndef CHORDSENSEANSWER_H
#define CHORDSENSEANSWER_H

#include"randomchordgenerator.h"
#include"pianokeys.h"
#include<QList>
#include<QMap>
#include<QTimer>
#include<QDebug>
#include"keyboard.h"

class chordsenseanswer:public QObject
{
public:
    chordsenseanswer(RandomChordGenerator*generator=nullptr,pianokeys*piano=nullptr);
    void getAnswer();
    void pianokeysanswer();
    void setchordMap_v();
    void setvirtualKeyboard(keyboard*virtualKeyboard);
private:
    QList<int> answerChord;
    RandomChordGenerator*generator;
    pianokeys*piano;
    QMap<int,QPushButton*>keyMap;
    QMap<QList<int>,QString> chordMap_v;
    bool isAnswering;
    keyboard*virtualKeyboard;
};

#endif // CHORDSENSEANSWER_H
