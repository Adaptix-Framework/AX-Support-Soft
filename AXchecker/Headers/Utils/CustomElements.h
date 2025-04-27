#ifndef CUSTOMELEMENTS_H
#define CUSTOMELEMENTS_H

#include <main.h>

class SpinTable : public QWidget {
    Q_OBJECT
    public:
    QGridLayout*  layout      = nullptr;
    QTableWidget* table       = nullptr;
    QPushButton*  buttonAdd   = nullptr;
    QPushButton*  buttonClear = nullptr;

    SpinTable(int rows, int clomuns, QWidget* parent);

    explicit SpinTable(QWidget* parent = nullptr) { SpinTable(0,0,parent); }
    ~SpinTable() override = default;
};



class FileSelector : public QWidget
{
    Q_OBJECT

    public:
    QVBoxLayout* layout = nullptr;
    QLineEdit*   input  = nullptr;
    QPushButton* button = nullptr;

    QString content;

    explicit FileSelector(QWidget* parent = nullptr);
    ~FileSelector() override = default;
};

#endif //CUSTOMELEMENTS_H
