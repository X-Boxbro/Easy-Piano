#ifndef MAPPINGMANAGER_H
#define MAPPINGMANAGER_H

#include <QObject>
#include <QPushButton>
#include <QMap>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

#include "keyboard.h"
#include"pianokeys.h"
class mappingmanager:public QObject
{
    Q_OBJECT
    friend class MainWindow;
public:
    explicit mappingmanager(keyboard* virtualKeyboard, QObject* parent = nullptr);

    QString getCurrentMappingPath() const;
    void reloadMapping();
    void startMapping();
    void startDeleting();
    void deleteMapping();
    void stopDeleting();
    void stopMapping();
    void saveMappingToFile(const QString& filePath);
    void loadMappingFromFile(const QString& filePath);
    QPushButton* getPianoKeyForScanCode(int scanCode) const;
    void setPianoKeys(pianokeys* pianoKeys);
    void updateVirtualKeyText(QPushButton* virtualKey, QPushButton* pianoKey) ;
signals:
    void mappingCreated(int scanCode, QPushButton* pianoKey);
private slots:
    void onVirtualKeyClicked();
    void onPianoKeyClicked();
private:
    keyboard* virtualKeyboard;
    QMap<int, QPushButton*> keyToPianoMap;
    QPushButton* currentVirtualKey;
    QPushButton* currentPianoKey;
    bool isMappingMode;
    bool isDeletingMode;
    pianokeys* pianoKeys;
    QString currentMappingPath;
};

#endif // MAPPINGMANAGER_H
