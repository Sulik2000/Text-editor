#include "settingswindow.h"
#include "ui_settingswindow.h"

SettingsWindow::SettingsWindow(QVariantMap settings, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SettingsWindow)
{
    _settings = settings;
    ui->setupUi(this);
    ui->lineEdit->setText(settings.value("path").toString());
}

SettingsWindow::~SettingsWindow()
{
    delete ui;
}

void SettingsWindow::on_buttonBox_accepted()
{
    emit acceptedSettings(_settings);
    close();
}


void SettingsWindow::on_pushButton_clicked()
{
    QString filePath = QFileDialog::getOpenFileName(this, "Select your compiler", "C:/", "Executable file (*.exe)");
    if(filePath.isEmpty())
        return;
    ui->lineEdit->setText(filePath);
    _settings["path"] = filePath;
}

