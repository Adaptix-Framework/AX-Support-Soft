#ifndef AXCHECKER_DIALOGVIEW_H
#define AXCHECKER_DIALOGVIEW_H

#include <main.h>
#include <Classes/WidgetBuilder.h>

class DialogView : public QDialog
{
    WidgetBuilder* widgetBuilder    = nullptr;
    QSpacerItem*   horizontalSpacer = nullptr;
    QGridLayout*   mainGridLayout   = nullptr;
    QPushButton*   loadButton       = nullptr;
    QPushButton*   saveButton       = nullptr;
    QDialogButtonBox* buttonBox     = nullptr;

public:
    explicit DialogView(WidgetBuilder* builder);
    ~DialogView() override;

    QString GetData();

protected slots:
    void onButtonLoad();
    void onButtonSave();
};

#endif //AXCHECKER_DIALOGVIEW_H