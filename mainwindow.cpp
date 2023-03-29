#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    fileModel = new QFileSystemModel();
    fileModel->setFilter(QDir::Filter::AllEntries | QDir::Filter::NoDot);

    ui->treeView->setContextMenuPolicy(Qt::CustomContextMenu);

    // Parsing settings from json file
    _settingsError = new QJsonParseError();
    QString pathOfSettings = qApp->applicationDirPath() + '/' + "settings.json";
    if(!QFile::exists(pathOfSettings))
        initializeSettings(pathOfSettings);
    else
        parseSettings(pathOfSettings);

    _settings = _settingsDoc.object().toVariantMap();
}

MainWindow::~MainWindow()
{
    delete ui;
    delete fileModel;
    delete _settingsError;
}

void MainWindow::setSettings(QVariantMap settings)
{
    _settings = settings;
    QString pathOfSettings = qApp->applicationDirPath() + '/' + "settings.json";
    QFile file(pathOfSettings);
    file.open(QFile::WriteOnly);
    qDebug() << settings["path"];
    _settingsDoc.setObject(QJsonObject::fromVariantMap(_settings));
    file.write(_settingsDoc.toJson(QJsonDocument::Compact));
    file.close();
}

void MainWindow::renameFile(QString name)
{
    if(_newFile.isFile()){
        QString path = _newFile.absoluteFilePath();
        if(!QFile::rename(_newFile.absoluteFilePath(), _newFile.absoluteDir().path() + "/" + name))
            QMessageBox::critical(this, "Error", "File name already exists");
    }
    else{
        QDir dir {_newFile.absoluteDir()};

        QString oldName = _newFile.absoluteFilePath();
        QString newName = dir.absolutePath() + '/' + name;
        dir.rename(oldName, newName);
    }
}

void MainWindow::createFile(QString name)
{
    if(_newFile.isFile())
        return;
    qDebug() << _newFile.absoluteFilePath() + '/' + name;

    QFile file(_newFile.absoluteFilePath() + '/' + name);
    if(!file.open(QFile::OpenModeFlag::NewOnly))
        QMessageBox::critical(this, "Error", "File already exists");
    file.close();
}

void MainWindow::createFolder(QString name)
{
    if(_newFile.isFile())
        return;
    QDir dir(_newFile.absoluteFilePath());
    if(!dir.mkdir(name))
        qDebug() << "Error cannot create folder";
}

void MainWindow::on_actionOpen_Folder_triggered()
{
    QString path = QFileDialog::getExistingDirectory(this, "Open folder", "C:/");
    if(path.isEmpty())
        return;
    fileModel->setRootPath(path);
    ui->treeView->setModel(fileModel);
    ui->treeView->setRootIndex(fileModel->index(path));
    for(int i = 1; i < fileModel->columnCount(); i++)
        ui->treeView->hideColumn(i);
}


void MainWindow::on_treeView_doubleClicked(const QModelIndex &index)
{
    if(!index.isValid())
        return;

    QFileInfo fileInfo = fileModel->fileInfo(index);
    if(fileInfo.isDir())
        return;

    QString filePath = fileInfo.absoluteFilePath();

    QFile file(filePath);

    if(openedFiles.value(filePath, -1) != -1){
        ui->tabWidget->setCurrentIndex(openedFiles[filePath]);
        return;
    }

    file.open(QFile::OpenModeFlag::ReadOnly);
    QString text = file.readAll();
    file.close();
    QPlainTextEdit *edit = new QPlainTextEdit(text, ui->tabWidget);
    edit->setProperty("path", filePath);
    openedFiles[filePath] = ui->tabWidget->addTab(edit, fileInfo.fileName());
}


void MainWindow::on_actionOpen_triggered()
{
    QString path = QFileDialog::getOpenFileName(this, "Select file", "C:/");
    if(path.isEmpty())
        return;
    if(openedFiles.value(path, -1) != -1){
        ui->tabWidget->setCurrentIndex(openedFiles.value(path, -1));
        return;
    }
    QFile file(path);

    file.open(QFile::OpenModeFlag::ReadOnly | QFile::OpenModeFlag::ExistingOnly); // Open file

    QString text = file.readAll();

    QPlainTextEdit *edit = new QPlainTextEdit(text, ui->tabWidget);
    edit->setProperty("path", path);

    openedFiles[path] = ui->tabWidget->addTab(edit, file.fileName());
}

void MainWindow::on_actionSave_triggered()
{
    if(ui->tabWidget->count() == 0)
        return;

    QPlainTextEdit* edit = (QPlainTextEdit*)ui->tabWidget->currentWidget();
    QString path = edit->property("path").toString();
    QFile file(path);
    if(!file.open(QFile::OpenModeFlag::WriteOnly))
        qDebug() << "Error occured: didn't open file";
    QString text = edit->toPlainText();
    if(file.write(text.toStdString().c_str()) == -1)
        qDebug() << "Error occured: didn't write file";
    file.close();
}


void MainWindow::on_actionSave_all_triggered()
{
    for(int i = 0; i < ui->tabWidget->count(); i++){
        QPlainTextEdit *edit = (QPlainTextEdit*)ui->tabWidget->widget(i);
        QFile file(edit->property("path").toString());

        if(!file.open(QFile::OpenModeFlag::WriteOnly))
            qDebug() << "Error occured didn't open file";

        if(file.write(edit->toPlainText().toStdString().c_str()) == -1)
            qDebug() << "Error occured didn't write to file";

        file.close();
    }
}


void MainWindow::on_actionClose_triggered()
{
    int index = ui->tabWidget->currentIndex();
    if(index == -1)
        return;

    ui->tabWidget->removeTab(index);
    openedFiles.clear();

    for(int i = 0; i < ui->tabWidget->count(); i++)
        openedFiles[ui->tabWidget->widget(i)->property("path").toString()] = i;
}


void MainWindow::on_actionExit_triggered()
{
    QApplication::exit();}

void MainWindow::on_treeView_customContextMenuRequested(const QPoint &pos)
{
    contextMenu = new QMenu(this);
    contextMenu->addAction("New file");
    contextMenu->addAction("New folder");
    contextMenu->addAction("Delete");
    contextMenu->addAction("Rename");
    QPoint globalPos = ui->treeView->mapToGlobal(pos);
    QAction* action = contextMenu->exec(globalPos);
    if(action){
        if(action->text() == "New file"){
            QModelIndex index = ui->treeView->currentIndex();
            if(!index.isValid())
                return;

            QFileInfo info = fileModel->fileInfo(index);
            _newFile = info;

            if(info.fileName() == ".."){
                info.setFile(fileModel->rootPath());
                _newFile = info;
            }

            qDebug() << info.absoluteFilePath();

            CreateFile *menu = new CreateFile(this);
            connect(menu, &CreateFile::createFile, this, &MainWindow::createFile);
            menu->show();
        }

        if(action->text() == "New folder"){
            qDebug() << "Create new folder";
            QModelIndex index = ui->treeView->currentIndex();
            if(!index.isValid())
                return;

            QFileInfo info = fileModel->fileInfo(index);
            if(info.isFile())
                return;
            if(info.fileName() == "..")
                _newFile.setFile(fileModel->rootPath());
            else
                _newFile = info;

            CreateFile *createFile = new CreateFile(this, "Create folder");
            connect(createFile, &CreateFile::createFile, this, &MainWindow::createFolder);
            createFile->show();
        }

        else if(action->text() == "Delete"){

            QModelIndex index = ui->treeView->currentIndex();
            if(!index.isValid())
                return;
            QFileInfo info = fileModel->fileInfo(index);

            if(info.isFile()){
                QFile file(info.absoluteFilePath());
                if(file.exists())
                    if(!file.remove())
                        qDebug() << "Error cannot delete file";
            }
            else if(info.isDir()){
                QDir dir(info.absoluteFilePath());
                if(dir.exists()){
                    if(!dir.removeRecursively())
                        qDebug() << "Error cannot delete folder";
                }
            }

        }

        else if(action->text() == "Rename"){
            QFileInfo info = fileModel->fileInfo(ui->treeView->currentIndex());

            _newFile = info;

            CreateFile *createFile = new CreateFile(this, "Name your folder");
            connect(createFile, &CreateFile::createFile, this, &MainWindow::renameFile);
            createFile->show();
        }
    }
}


void MainWindow::on_actionSettings_triggered()
{
    SettingsWindow *window = new SettingsWindow(_settings, this);
    connect(window, &SettingsWindow::acceptedSettings, this, &MainWindow::setSettings);
    window->show();
}

// Parsing settings from json files
void MainWindow::parseSettings(QString path)
{
    QFile file(path);
    file.open(QFile::ReadOnly);
    _settingsDoc = QJsonDocument::fromJson(file.readAll(), _settingsError);
    if(_settingsDoc.isNull()){
        qDebug() << "Error: cannot parse settings from Json file";
        QMessageBox::critical(this, "Error", "Wrong syntax of json file which contains settings of application. Application automatically creates new file with default settings");
        file.remove();
        initializeSettings(path);
    }
}

void MainWindow::initializeSettings(QString path)
{
    QFile file(path);
    if(!file.open(QFile::WriteOnly)){
        qDebug() << "Error: cannot create file of settings";
        return;
    }

    file.write("{\n\t\"path\":\"C:/MinGW/bin/\"\n}");
    file.close();
    _settingsDoc = QJsonDocument::fromJson(QString("{\n\t\"path\":\"C:/MinGW\"\n}").toUtf8(), _settingsError);
    if(_settingsDoc.isNull())
        qDebug() << "Error: cannot parse information from file of settings";
}

