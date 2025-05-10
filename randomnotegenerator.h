#ifndef RANDOMNOTEGENERATOR_H
#define RANDOMNOTEGENERATOR_H
#include <QObject>
#include <QList>
#include <QMap>
#include <QTimer>
#include "fluidsynth_initial.h"
#include"keyboard.h"

class RandomNoteGenerator : public QObject
{
    Q_OBJECT
    friend class notesenseanswer;
public:
    RandomNoteGenerator(fluidsynth_initial* synth, QObject *parent = nullptr);
    void setNoteCount(int count);
    void setSpeed(int ms);
    void setScales(const QList<int>& zones);
    void start();
    void nextNote();
    void setvirtualKeyboard(keyboard*input);

private:
    fluidsynth_initial* mysynth;
    int noteCount;
    int speedMs;
    int currentIndex;
    QList<int> scaleZones;
    QList<int> allAvailableNotes;
    QMap<int, QList<int>> scaleMap;
    keyboard*virtualKeyboard;
    void initScaleMap();
    void generateNotes();
    void playNotes();
};

#endif // RANDOMNOTEGENERATOR_H
