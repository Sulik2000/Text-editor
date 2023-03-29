#ifndef SETTINGSWINDOW_H
#define SETTINGSWINDOW_H

#include <QDialog>
#include <QFileDialog>

namespace Ui {
class SettingsWindow;
}

class SettingsWindow : public QDialog
{
    Q_OBJECT

public:
    explicit SettingsWindow(QVariantMap settings, QWidget *parent = nullptr);
    ~SettingsWindow();

signals:
    void acceptedSettings(QVariantMap settings);

private slots:
    void on_buttonBox_accepted();

    void on_pushButton_clicked();

private:
    QVariantMap _settings;
    Ui::SettingsWindow *ui;
};

#endif // SETTINGSWINDOW_H
