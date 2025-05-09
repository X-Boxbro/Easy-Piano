#ifndef PIANOKEYS_H
#define PIANOKEYS_H


#include "fluidsynth_initial.h"
#include<QObject>
#include<QPushButton>
#include<QMap>
#include <QSet>

class pianokeys:public QObject
{
    Q_OBJECT
    friend class notesenseanswer;
    friend class mappingmanager;
public:
    explicit pianokeys(fluidsynth_initial*synth,QObject *parent =nullptr);
    void addKeys(QPushButton*button,int midiNote);
    void bindKeysFromUI(QWidget*parentWidget);
    QList<QPushButton*> getAllButtons() const;
    int getMidiNoteForButton(QPushButton* button) const;
private slots:
    void onKeyPressed();
    void onKeyReleased();
private:
    QMap<QPushButton*,int>keyMap;
    QMap<int,QPushButton*>keyMap_v;
    fluidsynth_initial*synth;
    QSet<int> activeNotes;
};

#endif // PIANOKEYS_H
