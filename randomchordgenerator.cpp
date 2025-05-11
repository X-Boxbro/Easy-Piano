#include "randomchordgenerator.h"

RandomChordGenerator::RandomChordGenerator(fluidsynth_initial* synth, QObject* parent)
    : QObject(parent)
    , mysynth(synth)
    ,virtualKeyboard(nullptr)
{
    initChords();
}

void RandomChordGenerator::initChords(){
    chordMap.clear();
    chordMap_v.clear();
    // 大三和弦
    chordMap["C# Major"] = {61, 65, 68};  // C# E# G#
    chordMap["D Major"] = {62, 66, 69};   // D F# A
    chordMap["D# Major"] = {63, 67, 70};  // D# G A#
    chordMap["E Major"] = {64, 68, 71};   // E G# B
    chordMap["F Major"] = {65, 69, 72};   // F A C
    chordMap["F# Major"] = {66, 70, 73};  // F# A# C#
    chordMap["G Major"] = {67, 71, 74};   // G B D
    chordMap["G# Major"] = {68, 72, 75};  // G# C D#
    chordMap["A Major"] = {69, 73, 76};   // A C# E
    chordMap["A# Major"] = {70, 74, 77};  // A# D F
    chordMap["B Major"] = {71, 75, 78};   // B D# F#
    // 小三和弦
    chordMap["C# Minor"] = {61, 64, 68};  // C# E G#
    chordMap["D Minor"] = {62, 65, 69};   // D F A
    chordMap["D# Minor"] = {63, 66, 70};  // D# F# A#
    chordMap["E Minor"] = {64, 67, 71};   // E G B
    chordMap["F Minor"] = {65, 68, 72};   // F G# C
    chordMap["F# Minor"] = {66, 69, 73};  // F# A C#
    chordMap["G Minor"] = {67, 70, 74};   // G A# D
    chordMap["G# Minor"] = {68, 71, 75};  // G# B D#
    chordMap["A Minor"] = {69, 72, 76};   // A C E
    chordMap["A# Minor"] = {70, 73, 77};  // A# C# F
    chordMap["B Minor"] = {71, 74, 78};   // B D F#
    // 属七和弦
    chordMap["C7"] = {60, 64, 67, 70};    // C E G Bb
    chordMap["C#7"] = {61, 65, 68, 71};   // C# E# G# B
    chordMap["D7"] = {62, 66, 69, 72};    // D F# A C
    chordMap["D#7"] = {63, 67, 70, 73};   // D# G A# C#
    chordMap["E7"] = {64, 68, 71, 74};    // E G# B D
    chordMap["F7"] = {65, 69, 72, 75};    // F A C Eb
    chordMap["F#7"] = {66, 70, 73, 76};   // F# A# C# E
    chordMap["G7"] = {67, 71, 74, 77};    // G B D F
    chordMap["G#7"] = {68, 72, 75, 78};   // G# C D# F#
    chordMap["A7"] = {69, 73, 76, 79};    // A C# E G
    chordMap["A#7"] = {70, 74, 77, 80};   // A# D F G#
    chordMap["B7"] = {71, 75, 78, 81};    // B D# F# A
    // 大七和弦
    chordMap["CMaj7"] = {60, 64, 67, 71};  // C E G B
    chordMap["C#Maj7"] = {61, 65, 68, 72}; // C# E# G# C
    chordMap["DMaj7"] = {62, 66, 69, 73};  // D F# A C#
    chordMap["D#Maj7"] = {63, 67, 70, 74}; // D# G A# D
    chordMap["EMaj7"] = {64, 68, 71, 75};  // E G# B D#
    chordMap["FMaj7"] = {65, 69, 72, 76};  // F A C E
    chordMap["F#Maj7"] = {66, 70, 73, 77}; // F# A# C# F
    chordMap["GMaj7"] = {67, 71, 74, 78};  // G B D F#
    chordMap["G#Maj7"] = {68, 72, 75, 79}; // G# C D# G
    chordMap["AMaj7"] = {69, 73, 76, 80};  // A C# E G#
    chordMap["A#Maj7"] = {70, 74, 77, 81}; // A# D F A
    chordMap["BMaj7"] = {71, 75, 78, 82};  // B D# F# A#
    // 小七和弦
    chordMap["Cm7"] = {60, 63, 67, 70};    // C Eb G Bb
    chordMap["C#m7"] = {61, 64, 68, 71};   // C# E G# B
    chordMap["Dm7"] = {62, 65, 69, 72};    // D F A C
    chordMap["D#m7"] = {63, 66, 70, 73};   // D# F# A# C#
    chordMap["Em7"] = {64, 67, 71, 74};    // E G B D
    chordMap["Fm7"] = {65, 68, 72, 75};    // F G# C Eb
    chordMap["F#m7"] = {66, 69, 73, 76};   // F# A C# E
    chordMap["Gm7"] = {67, 70, 74, 77};    // G A# D F
    chordMap["G#m7"] = {68, 71, 75, 78};   // G# B D# F#
    chordMap["Am7"] = {69, 72, 76, 79};    // A C E G
    chordMap["A#m7"] = {70, 73, 77, 80};   // A# C# F G#
    chordMap["Bm7"] = {71, 74, 78, 81};    // B D F# A
    // 减三和弦
    chordMap["Cdim"] = {60, 63, 66};    // C Eb Gb
    chordMap["C#dim"] = {61, 64, 67};   // C# E G
    chordMap["Ddim"] = {62, 65, 68};    // D F Ab
    chordMap["D#dim"] = {63, 66, 69};   // D# F# A
    chordMap["Edim"] = {64, 67, 70};    // E G Bb
    chordMap["Fdim"] = {65, 68, 71};    // F G# B
    chordMap["F#dim"] = {66, 69, 72};   // F# A C
    chordMap["Gdim"] = {67, 70, 73};    // G A# C#
    chordMap["G#dim"] = {68, 71, 74};   // G# B D
    chordMap["Adim"] = {69, 72, 75};    // A C Eb
    chordMap["A#dim"] = {70, 73, 76};   // A# C# E
    chordMap["Bdim"] = {71, 74, 77};    // B D F
    // 增三和弦
    chordMap["Caug"] = {60, 64, 68};    // C E G#
    chordMap["C#aug"] = {61, 65, 69};   // C# E# A
    chordMap["Daug"] = {62, 66, 70};    // D F# A#
    chordMap["D#aug"] = {63, 67, 71};   // D# G B
    chordMap["Eaug"] = {64, 68, 72};    // E G# C
    chordMap["Faug"] = {65, 69, 73};    // F A C#
    chordMap["F#aug"] = {66, 70, 74};   // F# A# D
    chordMap["Gaug"] = {67, 71, 75};    // G B D#
    chordMap["G#aug"] = {68, 72, 76};   // G# C E
    chordMap["Aaug"] = {69, 73, 77};    // A C# F
    chordMap["A#aug"] = {70, 74, 78};   // A# D F#
    chordMap["Baug"] = {71, 75, 79};    // B D# G

    //Map reverse
    // 大三和弦
    chordMap_v[{61, 65, 68}] = "C# Major";
    chordMap_v[{62, 66, 69}] = "D Major";
    chordMap_v[{63, 67, 70}] = "D# Major";
    chordMap_v[{64, 68, 71}] = "E Major";
    chordMap_v[{65, 69, 72}] = "F Major";
    chordMap_v[{66, 70, 73}] = "F# Major";
    chordMap_v[{67, 71, 74}] = "G Major";
    chordMap_v[{68, 72, 75}] = "G# Major";
    chordMap_v[{69, 73, 76}] = "A Major";
    chordMap_v[{70, 74, 77}] = "A# Major";
    chordMap_v[{71, 75, 78}] = "B Major";
    // 小三和弦
    chordMap_v[{61, 64, 68}] = "C# Minor";
    chordMap_v[{62, 65, 69}] = "D Minor";
    chordMap_v[{63, 66, 70}] = "D# Minor";
    chordMap_v[{64, 67, 71}] = "E Minor";
    chordMap_v[{65, 68, 72}] = "F Minor";
    chordMap_v[{66, 69, 73}] = "F# Minor";
    chordMap_v[{67, 70, 74}] = "G Minor";
    chordMap_v[{68, 71, 75}] = "G# Minor";
    chordMap_v[{69, 72, 76}] = "A Minor";
    chordMap_v[{70, 73, 77}] = "A# Minor";
    chordMap_v[{71, 74, 78}] = "B Minor";
    // 属七和弦
    chordMap_v[{60, 64, 67, 70}] = "C7";
    chordMap_v[{61, 65, 68, 71}] = "C#7";
    chordMap_v[{62, 66, 69, 72}] = "D7";
    chordMap_v[{63, 67, 70, 73}] = "D#7";
    chordMap_v[{64, 68, 71, 74}] = "E7";
    chordMap_v[{65, 69, 72, 75}] = "F7";
    chordMap_v[{66, 70, 73, 76}] = "F#7";
    chordMap_v[{67, 71, 74, 77}] = "G7";
    chordMap_v[{68, 72, 75, 78}] = "G#7";
    chordMap_v[{69, 73, 76, 79}] = "A7";
    chordMap_v[{70, 74, 77, 80}] = "A#7";
    chordMap_v[{71, 75, 78, 81}] = "B7";
    // 大七和弦
    chordMap_v[{60, 64, 67, 71}] = "CMaj7";
    chordMap_v[{61, 65, 68, 72}] = "C#Maj7";
    chordMap_v[{62, 66, 69, 73}] = "DMaj7";
    chordMap_v[{63, 67, 70, 74}] = "D#Maj7";
    chordMap_v[{64, 68, 71, 75}] = "EMaj7";
    chordMap_v[{65, 69, 72, 76}] = "FMaj7";
    chordMap_v[{66, 70, 73, 77}] = "F#Maj7";
    chordMap_v[{67, 71, 74, 78}] = "GMaj7";
    chordMap_v[{68, 72, 75, 79}] = "G#Maj7";
    chordMap_v[{69, 73, 76, 80}] = "AMaj7";
    chordMap_v[{70, 74, 77, 81}] = "A#Maj7";
    chordMap_v[{71, 75, 78, 82}] = "BMaj7";
    // 小七和弦
    chordMap_v[{60, 63, 67, 70}] = "Cm7";
    chordMap_v[{61, 64, 68, 71}] = "C#m7";
    chordMap_v[{62, 65, 69, 72}] = "Dm7";
    chordMap_v[{63, 66, 70, 73}] = "D#m7";
    chordMap_v[{64, 67, 71, 74}] = "Em7";
    chordMap_v[{65, 68, 72, 75}] = "Fm7";
    chordMap_v[{66, 69, 73, 76}] = "F#m7";
    chordMap_v[{67, 70, 74, 77}] = "Gm7";
    chordMap_v[{68, 71, 75, 78}] = "G#m7";
    chordMap_v[{69, 72, 76, 79}] = "Am7";
    chordMap_v[{70, 73, 77, 80}] = "A#m7";
    chordMap_v[{71, 74, 78, 81}] = "Bm7";
    // 减三和弦
    chordMap_v[{60, 63, 66}] = "Cdim";
    chordMap_v[{61, 64, 67}] = "C#dim";
    chordMap_v[{62, 65, 68}] = "Ddim";
    chordMap_v[{63, 66, 69}] = "D#dim";
    chordMap_v[{64, 67, 70}] = "Edim";
    chordMap_v[{65, 68, 71}] = "Fdim";
    chordMap_v[{66, 69, 72}] = "F#dim";
    chordMap_v[{67, 70, 73}] = "Gdim";
    chordMap_v[{68, 71, 74}] = "G#dim";
    chordMap_v[{69, 72, 75}] = "Adim";
    chordMap_v[{70, 73, 76}] = "A#dim";
    chordMap_v[{71, 74, 77}] = "Bdim";
    // 增三和弦
    chordMap_v[{60, 64, 68}] = "Caug";
    chordMap_v[{61, 65, 69}] = "C#aug";
    chordMap_v[{62, 66, 70}] = "Daug";
    chordMap_v[{63, 67, 71}] = "D#aug";
    chordMap_v[{64, 68, 72}] = "Eaug";
    chordMap_v[{65, 69, 73}] = "Faug";
    chordMap_v[{66, 70, 74}] = "F#aug";
    chordMap_v[{67, 71, 75}] = "Gaug";
    chordMap_v[{68, 72, 76}] = "G#aug";
    chordMap_v[{69, 73, 77}] = "Aaug";
    chordMap_v[{70, 74, 78}] = "A#aug";
    chordMap_v[{71, 75, 79}] = "Baug";
}

void RandomChordGenerator::setvirtualKeyboard(keyboard*input){
    virtualKeyboard=input;
}

void RandomChordGenerator::setAvailableChordList(const QStringList& chordNames) {
    availableChords.clear();

    for (const QString& name : chordNames) {
        if (chordMap.contains(name)) {
            availableChords.append(chordMap[name]);  // QVector<int>
        }
    }
}

void RandomChordGenerator::start()
{
    if(!currentChord.isEmpty()){
        playChord();
    }else{
        nextChord();
        playChord();
    }
    virtualKeyboard->setFocus();
}

void RandomChordGenerator::playChord(){

    for(int note:currentChord){
        mysynth->playNote(0,note,110);
    }

    QTimer::singleShot(500, this, [this]() {
        for (int note : currentChord) {
            mysynth->stopNote(0, note);
        }
    });
}

//更新抽取
void RandomChordGenerator::nextChord(){
    currentChord=getRandomChord();
    virtualKeyboard->setFocus();
}

//抽取一个和弦
QVector<int> RandomChordGenerator::getRandomChord() const {
    if (availableChords.isEmpty())
        return {};

    int index = QRandomGenerator::global()->bounded(availableChords.size());
    return availableChords[index];
}

RandomChordGenerator::~RandomChordGenerator() {
}
