#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <QObject>
#include <QWidget>
#include <QPushButton>
#include <QMap>
#include <QKeyEvent>

class mappingmanager;

class keyboard:public QWidget
{
    Q_OBJECT
    friend class mappingmanager;
public:
    explicit keyboard(QWidget* parent = nullptr);
    void initializeKeyMapping();
    void keyPressEvent(QKeyEvent* event) override;
    void keyReleaseEvent(QKeyEvent* event) override;
    bool focusNextPrevChild(bool next) override;
    QList<QPushButton*> getAllButtons()const;
    const QMap<int, QPushButton*>& getScanCodeToButtonMap() const;
    void setMappingManager(mappingmanager* manager);
signals:
    void keyPressed(int scanCode);
    void keyReleased(int scanCode);
private:
    QSet<int> pressedKeys;
    QMap<int, QPushButton*> scanCodeToButtonMap;
    mappingmanager* mappingManager;
};


#endif // KEYBOARD_H
