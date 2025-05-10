#ifndef NOTESENSEANSWER_H
#define NOTESENSEANSWER_H
#include"randomnotegenerator.h"
#include"pianokeys.h"
#include<QList>
#include<QMap>
#include<QTimer>
#include<QDebug>

class notesenseanswer:public QObject
{
public:
    notesenseanswer(RandomNoteGenerator*generator=nullptr,pianokeys*piano=nullptr);
    void getAnswer();
    void pianokeysanswer();
private:
    QList<int> answerNotes;
    RandomNoteGenerator*generator;
    pianokeys*piano;
    int speedMs;
    int currentIndex;
    QMap<int,QPushButton*>keyMap;
    bool isAnswering;
};

#endif // NOTESENSEANSWER_H
