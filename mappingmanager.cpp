#include "mappingmanager.h"
#include "qdir.h"
#include<QDebug>


mappingmanager::mappingmanager(keyboard* virtualKeyboard, QObject* parent)
    : QObject(parent), virtualKeyboard(virtualKeyboard), currentVirtualKey(nullptr), currentPianoKey(nullptr) {
    // 连接虚拟键盘按钮点击信号
    for (QPushButton* button : virtualKeyboard->getAllButtons()) {
        connect(button, &QPushButton::clicked, this, &mappingmanager::onVirtualKeyClicked);
    }
}


void mappingmanager::setPianoKeys(pianokeys* pianoKeys) {
    this->pianoKeys = pianoKeys;
}

void mappingmanager::updateVirtualKeyText(QPushButton* virtualKey, QPushButton* pianoKey) {
    if (!virtualKey || !pianoKey) return;

    QString pianoKeyText = pianoKey->text();
    virtualKey->setText(pianoKeyText);

    qDebug() << "Updated virtual key text. VirtualKey:" << virtualKey->objectName()
             << ", Text:" << pianoKeyText;
}

void mappingmanager::startMapping() {
    isMappingMode = true;
    isDeletingMode=false;
    currentVirtualKey = nullptr;
    currentPianoKey = nullptr;
    qDebug() << "Mapping mode started. Please click a virtual key and then a piano key to create a mapping.";
}

void mappingmanager::startDeleting() {
    isDeletingMode=true;
    isMappingMode=false;
    currentVirtualKey = nullptr;
    currentPianoKey = nullptr;
    qDebug() << "Deleting mode started. Please click a virtual key and then the deleting key to delete a mapping between a virtual key and a piano key.";
}

void mappingmanager::deleteMapping(){
    if (isDeletingMode==true){
    currentVirtualKey = qobject_cast<QPushButton*>(sender());
    if(!currentVirtualKey){
        qDebug()<<"currentVirtualKey not found";
        return;}

    int scanCode=virtualKeyboard->getScanCodeToButtonMap().key(currentVirtualKey,-1);
    if(scanCode==-1||scanCode==0){
        qWarning()<<"Virtual key not found in scanCodeToButtonMap.";
        return ;
    }
    if(keyToPianoMap.contains(scanCode)){
        QPushButton* temp =keyToPianoMap[scanCode];
        qDebug()<<"deleting virtualKeyScanCode:"<<scanCode<<" "<<"with PianoKey:"<<temp->text();
        keyToPianoMap.remove(scanCode);
        currentVirtualKey->setText("");
    }else {
        qWarning() << "No mapping found for ScanCode:" << scanCode;
    }
    }
}

void mappingmanager::stopDeleting(){
    isDeletingMode=false;
    isMappingMode=true;
    qDebug()<<"Deleting Mode stopped.";
    currentVirtualKey = nullptr;
}

void mappingmanager::stopMapping() {
    isMappingMode = false;
    qDebug() << "Mapping mode stopped.";
}

void mappingmanager::onVirtualKeyClicked() {
    if (!isMappingMode) return;

    currentVirtualKey = qobject_cast<QPushButton*>(sender());
    if (currentVirtualKey && currentPianoKey) {
        int scanCode = virtualKeyboard->getScanCodeToButtonMap().key(currentVirtualKey);
        qDebug() << "Virtual key pressed. ScanCode:" << scanCode;

        if (currentPianoKey) {
            keyToPianoMap[scanCode] = currentPianoKey;
            updateVirtualKeyText(currentVirtualKey, currentPianoKey);
            qDebug() << "Mapping created: ScanCode =" << scanCode
                     << ", PianoKey =" << currentPianoKey->objectName();
            currentVirtualKey = nullptr;
            currentPianoKey = nullptr;
        }
    }
}

void mappingmanager::onPianoKeyClicked() {
    if (!isMappingMode) return;

    currentPianoKey = qobject_cast<QPushButton*>(sender());
    if (currentVirtualKey && currentPianoKey) {
        qDebug() << "Piano key pressed. Name:" << currentPianoKey->objectName();
        if (currentVirtualKey) {
            int scanCode = virtualKeyboard->getScanCodeToButtonMap().key(currentVirtualKey);
            keyToPianoMap[scanCode] = currentPianoKey;
            updateVirtualKeyText(currentVirtualKey, currentPianoKey);
            qDebug() << "Mapping created: ScanCode =" << scanCode
                     << ", PianoKey =" << currentPianoKey->objectName();
            currentVirtualKey = nullptr;
            currentPianoKey = nullptr;
        }
    }
}

void mappingmanager::saveMappingToFile(const QString& filePath) {
    QJsonArray mappingArray;
    for (auto it = keyToPianoMap.begin(); it != keyToPianoMap.end(); ++it) {
        QJsonObject mapping;
        mapping["scanCode"] = it.key();
        mapping["pianoKey"] = it.value()->objectName();
        mappingArray.append(mapping);
    }

    QJsonObject root;
    root["mappings"] = mappingArray;

    QFile file(filePath);
    if (file.open(QIODevice::WriteOnly)) {
        file.write(QJsonDocument(root).toJson());
        file.close();
    }
}

void mappingmanager::loadMappingFromFile(const QString& filePath) {
    currentMappingPath=filePath;
    for (QPushButton* button : virtualKeyboard->getAllButtons()) {
        if (button) {
            button->setText("");
        }
    }
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly)) {
        qWarning() << "Failed to open mapping file:" << filePath;
        return;
    }

    QByteArray data = file.readAll();
    file.close();

    QJsonDocument doc = QJsonDocument::fromJson(data);
    if (!doc.isObject()) {
        qWarning() << "Invalid mapping file format";
        return;
    }

    QJsonObject root = doc.object();
    QJsonArray mappingArray = root["mappings"].toArray();
    keyToPianoMap.clear();
    for (const QJsonValue& value : mappingArray) {
        QJsonObject mapping = value.toObject();
        int scanCode = mapping["scanCode"].toInt();
        QString pianoKeyName = mapping["pianoKey"].toString();

        // 查找琴键按钮
        QPushButton* pianoKey = virtualKeyboard->parent()->findChild<QPushButton*>(pianoKeyName);
        QPushButton* virtualKey = virtualKeyboard->getScanCodeToButtonMap().value(scanCode);

        if (pianoKey) {
            keyToPianoMap[scanCode] = pianoKey;
            if (virtualKey) {
                virtualKey->setText(pianoKey->text());
            }

        } else {
            qWarning() << "Mapping failed: Piano key not found for objectName:" << pianoKeyName;
        }
    }
}

QPushButton* mappingmanager::getPianoKeyForScanCode(int scanCode) const {
    return keyToPianoMap.value(scanCode, nullptr);
}
QString mappingmanager::getCurrentMappingPath() const {
    return currentMappingPath;
}

void mappingmanager::reloadMapping() {
    if (!currentMappingPath.isEmpty()) {
        loadMappingFromFile(currentMappingPath);
    }
}
