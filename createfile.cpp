#include "createfile.h"
#include "ui_createfile.h"

CreateFile::CreateFile(QWidget *parent, QString label) :
    QDialog(parent),
    ui(new Ui::CreateFile)
{
    ui->setupUi(this);
    ui->label->setText(label);
}

CreateFile::~CreateFile()
{
    delete ui;
}

void CreateFile::on_buttonBox_accepted()
{
    if(!ui->lineEdit->text().isEmpty())
        emit createFile(ui->lineEdit->text());
}


void CreateFile::on_actionEnter_triggered()
{
    on_buttonBox_accepted();
}

