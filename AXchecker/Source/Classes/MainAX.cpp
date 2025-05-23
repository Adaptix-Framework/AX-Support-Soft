#include <Classes/MainAX.h>

MainAX::MainAX()
{

    this->setStyle();

    this->createUI();

    connect(selectButton, &QPushButton::clicked, this, [&](){SelectFile();});
    connect(buildButton, &QPushButton::clicked, this, [&](){BuildForm();});
    connect(editButton, &QPushButton::clicked, this, [&](){EditForm();});
}

MainAX::~MainAX()
{
    delete controlLayout;
    delete widgetBuilder;
};

void MainAX::createUI()
{
    this->setWindowTitle("AX Checker");

    mainLayout = new QVBoxLayout(this);

    configTextarea = new QTextEdit(this);

    pathInput = new QLineEdit(this);
    pathInput->setReadOnly(true);

    selectButton = new QPushButton("Select", this);
    buildButton = new QPushButton("Build", this);
    buildButton->setEnabled(false);
    editButton = new QPushButton("Edit", this);
    editButton->setEnabled(false);

    controlLayout = new QHBoxLayout();
    controlLayout->addWidget(pathInput);
    controlLayout->addWidget(selectButton);
    controlLayout->addWidget(buildButton);
    controlLayout->addWidget(editButton);

    mainLayout->addWidget(configTextarea);
    mainLayout->addLayout(controlLayout);
}

void MainAX::setStyle()
{
    QGuiApplication::setWindowIcon( QIcon( ":/LogoLin" ) );

    int FontID = QFontDatabase::addApplicationFont( ":/fonts/DroidSansMono" );
    QString FontFamily = QFontDatabase::applicationFontFamilies( FontID ).at( 0 );
    auto Font = QFont( FontFamily );

    Font.setPointSize( 10 );
    QApplication::setFont( Font );

    QString theme = ":/themes/Dark";

    bool result = false;
    QString style = ReadFileString(theme, &result);
    if (result) {
        auto *app = qobject_cast<QApplication*>(QCoreApplication::instance());
        app->setStyleSheet(style);
    }
}

void MainAX::SelectFile()
{
    QString filePath = QFileDialog::getOpenFileName(this, "Select JSON file", "", "JSON Files (*.json)");
    if (!filePath.isEmpty()) {
        pathInput->setText(filePath);
        buildButton->setEnabled(true);
    }
}

void MainAX::BuildForm()
{
    QString filePath = pathInput->text();
    if (!filePath.isEmpty()) {
        bool result = false;
        QByteArray jsonData = ReadFileBytearray(filePath, &result);
        if (result) {
            widgetBuilder = new WidgetBuilder(jsonData);
            if ( widgetBuilder->GetError().isEmpty() ) {
                widgetBuilder->BuildWidget(false);
                if (widgetBuilder->valid) {
                    dialogView = new DialogView(widgetBuilder);
                    if (dialogView->exec() == QDialog::Accepted) {
                        QString resultData = dialogView->GetData();
                        configTextarea->setText(resultData);
                        editButton->setEnabled(true);
                    }
                }
            }
            else {
                configTextarea->setText(widgetBuilder->GetError());
            }
        }
    }
}

void MainAX::EditForm()
{
    QString jsonString = configTextarea->toPlainText();
    widgetBuilder->FillData(jsonString);
    dialogView = new DialogView(widgetBuilder);
    if (dialogView->exec() == QDialog::Accepted) {
        QString resultData = dialogView->GetData();
        configTextarea->setText(resultData);
        editButton->setEnabled(true);
    }
}

void MainAX::Start() {
    this->show();
    QApplication::exec();
}
