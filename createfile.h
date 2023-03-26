#ifndef CREATEFILE_H
#define CREATEFILE_H

#include <QDialog>

namespace Ui {
class CreateFile;
}

class CreateFile : public QDialog
{
    Q_OBJECT

public:
    explicit CreateFile(QWidget *parent = nullptr, QString label = "Create file");
    ~CreateFile();
signals:
    void createFile(QString name);
private slots:
    void on_buttonBox_accepted();

    void on_actionEnter_triggered();

private:
    Ui::CreateFile *ui;
};

#endif // CREATEFILE_H
