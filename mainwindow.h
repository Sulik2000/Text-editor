#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "createfile.h"
#include <QMainWindow>
#include <QFileSystemModel>
#include <QFileDialog>
#include <qmap.h>
#include <QPlainTextEdit>
#include <QMessageBox>

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

private:
    QString pathNewFile;
    QMap<QString, int> openedFiles;
    Ui::MainWindow *ui;
    QFileSystemModel *fileModel;
    QMenu *contextMenu;
};
#endif // MAINWINDOW_H
