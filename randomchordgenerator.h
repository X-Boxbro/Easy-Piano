#ifndef RANDOMCHORDGENERATOR_H
#define RANDOMCHORDGENERATOR_H

#include<QObject>
#include <QTimer>
#include "fluidsynth_initial.h"
#include"keyboard.h"
#include <QList>
#include <QMap>
#include<QString>
#include<QVector>
#include<QRandomGenerator>
class RandomChordGenerator:public QObject
{
    friend class chordsenseanswer;
    Q_OBJECT
public:
    RandomChordGenerator(fluidsynth_initial* synth, QObject* parent=nullptr);
    void setAvailableChordList(const QStringList& chordNames);
    QVector<int> getRandomChord() const;
    void start();
    void setvirtualKeyboard(keyboard*input);
    ~RandomChordGenerator();
    void playChord();
    void nextChord();
private:
    fluidsynth_initial* mysynth;
    QMap<QString,QList<int>> chordMap;
    QMap<QList<int>,QString> chordMap_v;
    QList<int> currentChord;
    void initChords();
    QVector<QVector<int>> availableChords;
    keyboard*virtualKeyboard;
};

#endif // RANDOMCHORDGENERATOR_H
