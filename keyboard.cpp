#include "keyboard.h"
#include"mappingmanager.h"

#include <QSet>


keyboard::keyboard(QWidget* parent) : QWidget(parent), mappingManager(nullptr) {
    setFocusPolicy(Qt::StrongFocus);
}

void keyboard::setMappingManager(mappingmanager* manager) {
    mappingManager = manager;
}

void keyboard::initializeKeyMapping() {
    QList<QString> buttonNames;
    for (int i = 1; i <= 104; ++i) {
        buttonNames.append(QString("keyboard_%1").arg(i));
    }
    QList<int> scanCodes = {
        1,59,60,61,62,63,64,65,66,67,68,87,88,0,70,69,
        41,2,3,4,5,6,7,8,9,10,11,12,13,14,57426,57415,57417,57413,57397,55,74,
        15,16,17,18,19,20,21,22,23,24,25,26,27,43,57427,57423,57425,71,72,73,78,
        58,30,31,32,33,34,35,36,37,38,39,40,28,75,76,77,
        42,44,45,46,47,48,49,50,51,52,53,54,57416,79,80,81,57372,
        29,57435,56,57,57400,0,0,57373,57419,57424,57421,82,83
    };

    if (buttonNames.size() != scanCodes.size()) {
        qWarning() << "Error: buttonNames and scanCodes size mismatch!";
        return;
    }
    for (int i = 0; i < scanCodes.size(); ++i) {
        QString buttonName = buttonNames[i];
        int scanCode = scanCodes[i];
        QPushButton* button = parent()->findChild<QPushButton*>(buttonName);
        if (button) {

            if (scanCode != 0) {
                scanCodeToButtonMap[scanCode] = button;
                connect(button, &QPushButton::clicked, this, [this]() {
                    this->setFocus();
                });
            }
        }
    }
}


void keyboard::keyPressEvent(QKeyEvent* event) {
    int scanCode = event->nativeScanCode();
    if (event->isAutoRepeat()) {
        return;
    }
    if (scanCode == 56 ||
        scanCode == 57400) {
        return;
    }
    if (pressedKeys.contains(scanCode)) {
        return;
    }
    pressedKeys.insert(scanCode);
    if (scanCodeToButtonMap.contains(scanCode)) {
        QPushButton* button = scanCodeToButtonMap.value(scanCode);
        if (button) {
            button->setDown(true);
        }
    }
    if (mappingManager) {
        QPushButton* pianoKey = mappingManager->getPianoKeyForScanCode(scanCode);
        if (pianoKey) {
            pianoKey->setDown(true);
            emit pianoKey->pressed();
        }
    }
    emit keyPressed(scanCode);
}

void keyboard::keyReleaseEvent(QKeyEvent* event) {
    int scanCode = event->nativeScanCode();
    if (event->isAutoRepeat()) {
        return;
    }
    if (scanCode == 56 ||
        scanCode == 57400) {
        return;
    }
    if (!pressedKeys.contains(scanCode)) {
        return;
    }
    pressedKeys.remove(scanCode);
    if (scanCodeToButtonMap.contains(scanCode)) {
        QPushButton* button = scanCodeToButtonMap.value(scanCode);
        if (button) {
            button->setDown(false);
        }
    }
    if (mappingManager) {
        QPushButton* pianoKey = mappingManager->getPianoKeyForScanCode(scanCode);
        if (pianoKey) {
            pianoKey->setDown(false);
            emit pianoKey->released();
        }
    }
    emit keyReleased(scanCode);
}

bool keyboard::focusNextPrevChild([[maybe_unused]] bool next) {
    return false;
}

QList<QPushButton*> keyboard::getAllButtons()const{
    return scanCodeToButtonMap.values();
}

const QMap<int, QPushButton*>& keyboard::getScanCodeToButtonMap() const {
    return scanCodeToButtonMap;
}
