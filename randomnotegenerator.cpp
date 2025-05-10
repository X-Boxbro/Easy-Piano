#include "randomnotegenerator.h"
#include <QDebug>
#include <algorithm>
#include <random>

RandomNoteGenerator::RandomNoteGenerator(fluidsynth_initial* synth, QObject* parent)
    : QObject(parent), mysynth(synth), noteCount(1), speedMs(500), currentIndex(0),
    allAvailableNotes(QList<int>()), scaleZones(QList<int>()),virtualKeyboard(nullptr)
{
    initScaleMap();
}

void RandomNoteGenerator::initScaleMap()
{
    scaleMap.clear();
    scaleMap[1] = QList<int>() << 21 << 22 << 23 << 24 << 25 << 26 << 27 << 28 << 29 << 30 << 31 << 32 << 33 << 34 << 35;
    scaleMap[2] = QList<int>() << 36 << 37 << 38 << 39 << 40 << 41 << 42 << 43 << 44 << 45 << 46 << 47;
    scaleMap[3] = QList<int>() << 48 << 49 << 50 << 51 << 52 << 53 << 54 << 55 << 56 << 57 << 58 << 59;
    scaleMap[4] = QList<int>() << 60 << 61 << 62 << 63 << 64 << 65 << 66 << 67 << 68 << 69 << 70 << 71;
    scaleMap[5] = QList<int>() << 72 << 73 << 74 << 75 << 76 << 77 << 78 << 79 << 80 << 81 << 82 << 83;
    scaleMap[6] = QList<int>() << 84 << 85 << 86 << 87 << 88 << 89 << 90 << 91 << 92 << 93 << 94 << 95;
    scaleMap[7] = QList<int>() << 96 << 97 << 98 << 99 << 100 << 101 << 102 << 103 << 104 << 105 << 106 << 107 << 108;
}

void RandomNoteGenerator::setNoteCount(int count)
{
    noteCount = qBound(1, count, 5);
}
void RandomNoteGenerator::setvirtualKeyboard(keyboard*input){
    virtualKeyboard=input;
}
void RandomNoteGenerator::setSpeed(int ms)
{
    speedMs = qBound(300, ms, 1000);
}

void RandomNoteGenerator::setScales(const QList<int>& zones)
{
    scaleZones = zones;
}

void RandomNoteGenerator::start()
{
    if(!allAvailableNotes.isEmpty()){
        currentIndex = 0;
        playNotes();
    }else{
        nextNote();
        playNotes();
    }
    virtualKeyboard->setFocus();
}


void RandomNoteGenerator::nextNote()
{
    currentIndex = 0;
    generateNotes();
    virtualKeyboard->setFocus();
}

void RandomNoteGenerator::generateNotes()
{
    allAvailableNotes.clear();
    for (int zone : scaleZones) {
        allAvailableNotes.append(scaleMap[zone]);
    }

    if (allAvailableNotes.isEmpty()) {
        qDebug() << "[RandomNoteGenerator] 没有可用音符！";
        return;
    }

    // 打乱并选出所需数量
    std::shuffle(allAvailableNotes.begin(), allAvailableNotes.end(), std::mt19937{std::random_device{}()});
    allAvailableNotes = allAvailableNotes.mid(0, qMin(noteCount, allAvailableNotes.size()));
}

void RandomNoteGenerator::playNotes()
{
    if (currentIndex >= allAvailableNotes.size()) {
        return;
    }

    int note = allAvailableNotes[currentIndex];
    mysynth->playNote(0, note, 110);

    QTimer::singleShot(speedMs, this, [this,note]() {
        mysynth->stopNote(0, note);
        currentIndex++;
        if (currentIndex < allAvailableNotes.size()) {
            playNotes();
        }
    });
}
