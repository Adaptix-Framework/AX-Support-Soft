#include <Classes/DialogView.h>

DialogView::DialogView(WidgetBuilder *builder)
{
    mainGridLayout = new QGridLayout(this);

    horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

    loadButton = new QPushButton(this);
    loadButton->setText("Load");

    saveButton = new QPushButton(this);
    saveButton->setText("Save");

    if(builder && builder->valid) {
        widgetBuilder = builder;
        mainGridLayout->addWidget(widgetBuilder->GetWidget(), 0, 0, 1, 4);
    }

    buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);

    mainGridLayout->addItem(horizontalSpacer, 1, 0, 1, 1);
    mainGridLayout->addWidget(loadButton, 1, 1, 1, 1);
    mainGridLayout->addWidget(saveButton, 1, 2, 1, 1);
    mainGridLayout->addWidget(buttonBox, 1, 3, 1, 1);
    this->setLayout(mainGridLayout);

    connect(loadButton, &QPushButton::clicked, this, &DialogView::onButtonLoad );
    connect(saveButton, &QPushButton::clicked, this, &DialogView::onButtonSave );

    connect(buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
}

DialogView::~DialogView()= default;

QString DialogView::GetData()
{
    if(widgetBuilder && widgetBuilder->valid) {
        return widgetBuilder->CollectData();
    }
    return {""};
}

void DialogView::onButtonLoad()
{
    QString filePath = QFileDialog::getOpenFileName(this, "Select JSON file", "", "JSON Files (*.json)");
    if (filePath.isEmpty())
        return;

    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly))
        return;

    QByteArray fileData = file.readAll();
    file.close();

    QString jsonString = QString(fileData);
    widgetBuilder->FillData(jsonString);
}

void DialogView::onButtonSave()
{
    auto configData = QString();
    if(widgetBuilder && widgetBuilder->valid) {
        configData = widgetBuilder->CollectData();
    }

    QString tmpFilename = "widget_data.json";
    QString filePath = QFileDialog::getSaveFileName( nullptr, "Save File", tmpFilename, "All Files (*.json)" );
    if ( filePath.isEmpty())
        return;

    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly)) {
        return;
    }

    file.write( configData.toUtf8() );
    file.close();

    QInputDialog inputDialog;
    inputDialog.setWindowTitle("Save config");
    inputDialog.setLabelText("File saved to:");
    inputDialog.setTextEchoMode(QLineEdit::Normal);
    inputDialog.setTextValue(filePath);
    inputDialog.adjustSize();
    inputDialog.move(QGuiApplication::primaryScreen()->geometry().center() - inputDialog.geometry().center());
    inputDialog.exec();
}
