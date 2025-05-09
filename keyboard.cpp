#include "keyboard.h"
#include"mappingmanager.h"

#include <QSet>

// 构造函数
keyboard::keyboard(QWidget* parent) : QWidget(parent), mappingManager(nullptr) {
    // 确保虚拟键盘能够接收键盘事件
    setFocusPolicy(Qt::StrongFocus); // 确保这个控件可以获得焦点
}
// 设置映射管理器
void keyboard::setMappingManager(mappingmanager* manager) {
    mappingManager = manager;
}

// 自动初始化键盘映射
void keyboard::initializeKeyMapping() {
    // 定义虚拟键盘按钮名称列表
    QList<QString> buttonNames;
    for (int i = 1; i <= 104; ++i) {
        buttonNames.append(QString("keyboard_%1").arg(i));
    }
    // 定义对应的硬件扫描码列表（需要根据实际按键的扫描码填写）
    QList<int> scanCodes = {
        1,59,60,61,62,63,64,65,66,67,68,87,88,0,70,69,//第一行16
        41,2,3,4,5,6,7,8,9,10,11,12,13,14,57426,57415,57417,57413,57397,55,74,//第二行21
        15,16,17,18,19,20,21,22,23,24,25,26,27,43,57427,57423,57425,71,72,73,78,//第三行21
        58,30,31,32,33,34,35,36,37,38,39,40,28,75,76,77,//第四行16
        42,44,45,46,47,48,49,50,51,52,53,54,57416,79,80,81,57372,//第五行17
        29,57435,56,57,57400,0,0,57373,57419,57424,57421,82,83//第六行13
    };

    if (buttonNames.size() != scanCodes.size()) {
        qWarning() << "Error: buttonNames and scanCodes size mismatch!";
        return;
    }
    // 遍历按钮名称和扫描码列表，建立映射关系
    for (int i = 0; i < scanCodes.size(); ++i) {
        QString buttonName = buttonNames[i];
        int scanCode = scanCodes[i];
        // 查找按钮对象
        QPushButton* button = parent()->findChild<QPushButton*>(buttonName);
        if (button) {
            // 忽略 Fn 键（扫描码为 0 的情况）
            if (scanCode != 0) {
                scanCodeToButtonMap[scanCode] = button; // 建立映射

                // 连接按钮的点击信号，将焦点返回到虚拟键盘
                connect(button, &QPushButton::clicked, this, [this]() {
                    this->setFocus(); // 将焦点设置回虚拟键盘窗口
                });
            }
        }
    }
}


void keyboard::keyPressEvent(QKeyEvent* event) {
    int scanCode = event->nativeScanCode();  // 获取硬件扫描码

    // 检测是否为重复事件
    if (event->isAutoRepeat()) {
        return;  // 忽略键盘重复事件
    }

    // 禁用 Alt 和 Tab 键的系统默认功能
    if (scanCode == 56 ||  // Alt 键
        scanCode == 57400) {  // Tab 键
        return;  // 阻止事件传播，禁用默认行为
    }

    // 如果当前按键已经被按下，则忽略重复事件
    if (pressedKeys.contains(scanCode)) {
        return;
    }

    // 标记按键为已按下
    pressedKeys.insert(scanCode);

    // 设置虚拟键盘按钮为按下状态
    if (scanCodeToButtonMap.contains(scanCode)) {
        QPushButton* button = scanCodeToButtonMap.value(scanCode);
        if (button) {
            button->setDown(true);
        }
    }

    // 找到琴键按钮
    if (mappingManager) {
        QPushButton* pianoKey = mappingManager->getPianoKeyForScanCode(scanCode);
        if (pianoKey) {
            pianoKey->setDown(true);  // 设置琴键按钮为按下状态
            emit pianoKey->pressed();
        }
    }

    emit keyPressed(scanCode);
}

void keyboard::keyReleaseEvent(QKeyEvent* event) {
    int scanCode = event->nativeScanCode(); // 获取扫描码

    // 忽略键盘重复事件
    if (event->isAutoRepeat()) {
        return;
    }

    // 禁用 Alt 和 Tab 键的系统默认功能
    if (scanCode == 56 ||  // Alt 键
        scanCode == 57400) {  // Tab 键
        return; // 阻止事件传播，禁用默认行为
    }

    // 如果按键未被按下，则忽略释放事件
    if (!pressedKeys.contains(scanCode)) {
        return;
    }

    // 移除按键状态
    pressedKeys.remove(scanCode);

    // 恢复虚拟键盘按钮的状态
    if (scanCodeToButtonMap.contains(scanCode)) {
        QPushButton* button = scanCodeToButtonMap.value(scanCode);
        if (button) {
            button->setDown(false); // 恢复虚拟键盘原来状态
        }
    }

    // 找到琴键按钮
    if (mappingManager) {
        QPushButton* pianoKey = mappingManager->getPianoKeyForScanCode(scanCode);
        if (pianoKey) {
            pianoKey->setDown(false); // 恢复琴键按钮为松开状态
            emit pianoKey->released();
        }
    }
    emit keyReleased(scanCode);
}
// 禁用 Tab 键的默认焦点切换功能
bool keyboard::focusNextPrevChild([[maybe_unused]] bool next) {
    return false; // 返回 false 表示不处理焦点切换
}

//返回按钮列表的函数实现
QList<QPushButton*> keyboard::getAllButtons()const{
    return scanCodeToButtonMap.values(); // 返回所有按钮
}

const QMap<int, QPushButton*>& keyboard::getScanCodeToButtonMap() const {
    return scanCodeToButtonMap;
}
