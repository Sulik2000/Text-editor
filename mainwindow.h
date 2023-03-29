#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "createfile.h"
#include "settingswindow.h"
#include <QMainWindow>
#include <QFileSystemModel>
#include <QFileDialog>
#include <qmap.h>
#include <QPlainTextEdit>
#include <QMessageBox>
#include <QJsonDocument>
#include <QJsonParseError>
#include <QJsonObject>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:

private slots:
    void setSettings(QVariantMap settings);

    void renameFile(QString name);

    void createFile(QString name);

    void createFolder(QString name);

    void on_actionOpen_Folder_triggered();

    void on_treeView_doubleClicked(const QModelIndex &index);

    void on_actionOpen_triggered();

    void on_actionSave_triggered();

    void on_actionSave_all_triggered();

    void on_actionClose_triggered();

    void on_actionExit_triggered();

    void on_treeView_customContextMenuRequested(const QPoint &pos);

    void on_actionSettings_triggered();

private:
    void parseSettings(QString path);
    void initializeSettings(QString path);
    QJsonDocument _settingsDoc;
    QJsonParseError *_settingsError;
    QFileInfo _newFile;
    QMap<QString, int> openedFiles;
    Ui::MainWindow *ui;
    QFileSystemModel *fileModel;
    QMenu *contextMenu;
    QVariantMap _settings;
};
#endif // MAINWINDOW_H
