#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFile>
#include <QPushButton>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QLabel>
#include <QDialog>
#include <QDir>
#include <QModelIndex>
#include <QDebug>
//#include <ActiveQt/QAxWidget>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    model = new QStandardItemModel(0,7,this);
    QStringList headers={"Фамилия","Имя","Отчество","Телефон","Должность","Доп.Параметр1","Доп.Параметр2"};

    for (int j=0;j<headers.length();j++){
        model->setHeaderData(j,Qt::Horizontal,headers.at(j));
    }
    ui->tableView->setSelectionMode(QAbstractItemView::SingleSelection);

    Reload();
    ui->tableView->setModel(model);
    //qDebug()<<"MainWindow End";


}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_comboBox_activated(const QString &arg1)
{
    //ReloadFile(arg1);
}

void MainWindow::AddNewGroup()
{

    QDialog Dial;
    dialogWidgets.clear();
    QVBoxLayout* layout = new QVBoxLayout(&Dial);
    QPushButton* btnCrt = new QPushButton("Создать",&Dial);
    QPushButton* btnExt = new QPushButton("Выйти",&Dial);
    QObject::connect(btnExt,SIGNAL(clicked()),&Dial,SLOT(close()));
    QObject::connect(btnCrt,SIGNAL(clicked()),SLOT(CreateNewGroup()));
#define ADDMAC(NAME)  layout->addWidget(NAME); dialogWidgets.append(NAME);
    QLineEdit* lned = new QLineEdit(&Dial);
    QLineEdit* lned1 = new QLineEdit(&Dial);
    QLineEdit* lned2 = new QLineEdit(&Dial);
    QLabel* lbl = new QLabel("Название группы",&Dial);
    QLabel* lbl1 = new QLabel("Доп. параметр 1",&Dial);
    QLabel* lbl2 = new QLabel("Доп. параметр 2",&Dial);
    layout->addWidget(lbl);
    ADDMAC(lned)
    layout->addWidget(lbl1);
    ADDMAC(lned1)
    layout->addWidget(lbl2);
    ADDMAC(lned2)

#undef ADDMAC
    layout->addWidget(btnCrt);
    layout->addWidget(btnExt);
    Dial.setLayout(layout);
    Dial.exec();
    //ui->comboBox->addItem();
}

void MainWindow::AddNewPerson()
{
    QDialog Dial;
    dialogWidgets.clear();
    QVBoxLayout* layout = new QVBoxLayout(&Dial);
    QPushButton* btnCrt = new QPushButton("Создать",&Dial);
    QPushButton* btnExt = new QPushButton("Выйти",&Dial);
    QObject::connect(btnExt,SIGNAL(clicked()),&Dial,SLOT(close()));
    QObject::connect(btnCrt,SIGNAL(clicked()),SLOT(CreateNewPerson()));
#define ADDMAC(NAME)  layout->addWidget(NAME); dialogWidgets.append(NAME);
    QLineEdit** lneds=new QLineEdit*[model->columnCount()];
    QLabel** lbls=new QLabel*[model->columnCount()];
    for (int j=0;j<model->columnCount();j++){
        lneds[j] = new QLineEdit(&Dial);
        lbls[j] = new QLabel(model->headerData(j,Qt::Horizontal).toString(),&Dial);
        layout->addWidget(lbls[j]);
        ADDMAC(lneds[j])
    }
#undef ADDMAC
    layout->addWidget(btnCrt);
    layout->addWidget(btnExt);
    Dial.setLayout(layout);

    Dial.exec();
    for (int i=model->columnCount()-1;i>=0;i--)
    {
        delete lneds[i];
        delete lbls[i];
    }
    delete [] lneds;
    delete [] lbls;
}

void MainWindow::SaveTabl()
{
    QFile file( QApplication::applicationDirPath()+"/data/"+ui->comboBox->currentText()+".tabl");
    file.remove();
    file.open(QFile::WriteOnly);
    QTextStream stream(&file);
    stream.setCodec("UTF-8");
    stream<<QString("Фамилия Имя Отчество Телефон Должность "+model->headerData(5,Qt::Horizontal).toString()+" "+model->headerData(5,Qt::Horizontal).toString()).toUtf8()<<endl;
    int j;
    QModelIndex id;
    for (int i=0;i<model->rowCount();i++)
    {
        for (j=0;j<model->columnCount();j++)
        {
            id = model->index(i,j);
            stream<<model->data(id).toString().toUtf8()<<" ";
        }
        stream<<endl;
    }
}

void MainWindow::CreateNewGroup()
{
    QFile NewGroup( QApplication::applicationDirPath()+"/data/"+(((QLineEdit*)dialogWidgets[0])->text())+".tabl");
    NewGroup.open(QFile::WriteOnly);
    QTextStream stream(&NewGroup);
    stream.setCodec("UTF-8");

    QString param1,param2;
    if (( !(((QLineEdit*)dialogWidgets[1])->text()).contains(' '))&&(!(((QLineEdit*)dialogWidgets[1])->text()=="")))
        param1=((QLineEdit*)dialogWidgets[1])->text();
    else
        param1='"'+((QLineEdit*)dialogWidgets[1])->text()+'"';

    if (( !(((QLineEdit*)dialogWidgets[2])->text()).contains(' '))&&(!(((QLineEdit*)dialogWidgets[2])->text()=="")))
        param2=((QLineEdit*)dialogWidgets[2])->text();
    else
        param2='"'+((QLineEdit*)dialogWidgets[2])->text()+'"';

    stream<<QString("Фамилия Имя Отчество Телефон Должность "+param1+" "+param2).toUtf8()<<endl;
    NewGroup.close();
    ui->comboBox->addItem( ((QLineEdit*)dialogWidgets[0])->text() );
    QDir GroupDir(( QApplication::applicationDirPath()+"/data/"));
    GroupDir.mkdir(((QLineEdit*)dialogWidgets[0])->text());
    qDebug()<<"AddPerson End";


}

void MainWindow::CreateNewPerson()
{

    qDebug()<<"CreatePerson";
    model->setRowCount(model->rowCount()+1);
    QModelIndex id;
    QFile Group( QApplication::applicationDirPath()+"/data/"+ui->comboBox->currentText()+".tabl");
    if (!Group.open(QFile::WriteOnly|QFile::ReadOnly)) qDebug()<<"NOT OPEN CreateNewPerson";
    QTextStream stream(&Group);
    stream.setCodec("UTF-8");
    stream.readAll();
    for (int j=0;j<model->columnCount();j++){
        id=model->index(model->rowCount()-1,j);
        model->setData(id,((QLineEdit*)dialogWidgets[j])->text());
        if (((QLineEdit*)dialogWidgets[j])->text()=="")
            stream<<"- ";
        else if (((QLineEdit*)dialogWidgets[j])->text().contains(' '))
            stream<<"\""<<((QLineEdit*)dialogWidgets[j])->text()<< "\" ";
        else
            stream<<((QLineEdit*)dialogWidgets[j])->text()<< " ";
    }
    stream<<endl;
    Group.close();
    QDir GroupDir(( QApplication::applicationDirPath()+"/data/"+ui->comboBox->currentText()+"/"));
    GroupDir.mkdir(((QLineEdit*)dialogWidgets[0])->text()+" "+((QLineEdit*)dialogWidgets[1])->text()+" "+((QLineEdit*)dialogWidgets[2])->text());
    qDebug()<<"CreatePerson End";
}

void MainWindow::Reload()
{
    qDebug()<<"Reload";
    QDir dir(QApplication::applicationDirPath()+"/data");
    if (!dir.exists())
    {
        dir.setPath(QApplication::applicationDirPath());
        dir.mkdir("data");
        dir.setPath(dir.currentPath()+"/data");
    }
    QStringList tabls = dir.entryList(QStringList("*.tabl"));
    for (int i=0;i<tabls.length();i++){
        ui->comboBox->addItem(tabls[i].mid(0,tabls[i].length()-5));
    }
    if (tabls.length()>0)
        ReloadFile(tabls[0].mid(0,tabls[0].length()-5));
}

void MainWindow::on_pushButton_clicked()
{
    AddNewGroup();
}

void MainWindow::on_pushButton_3_clicked()
{
    AddNewPerson();
}

void MainWindow::on_comboBox_currentIndexChanged(const QString &arg1)
{
    ReloadFile(arg1);
}

bool MainWindow::ReloadFile(const QString &str)
{
    qDebug()<<"ReloadFile";
    QFile group( QApplication::applicationDirPath()+"/data/"+str+".tabl");
    if (!group.open(QFile::WriteOnly |QFile::ReadOnly)) return false;
    qDebug()<<"OPENED ReloadFile";
    QString strg;
    QTextStream stream(&group);
    stream.setCodec("UTF-8");
    int k=0,i=0,rememberLastI=0;
    bool inside=false;
    strg=stream.readLine();
    {
        for (i=0;i<strg.length();i++)
        {
            qDebug()<<"I:"<<i<<" Symb: "<<strg[i]<<" inside:"<<(int)inside;
            if ((strg[i]!=' ')&&(((strg[i+1]==' ')) || (i+1>=strg.length())))
            {


                if (!inside)
                {
                    k++;
                    qDebug()<<"K:"<<k;
                    if (k>4) {
                        if (k>5){
                            model->setHeaderData(k-1,Qt::Horizontal,strg.mid(rememberLastI,i-rememberLastI+1));
                            qDebug()<<strg.mid(rememberLastI,i-rememberLastI+1);
                        }
                    }
                    if (k==model->columnCount()) break;
                } else
                {
                    if ((strg[i]=='"')||(i+1>=strg.length())) {
                        inside=false;
                        qDebug()<<"End of Inside I: "<<i;
                        k++;
                        qDebug()<<"K:"<<k;
                        if (k>5){
                           model->setHeaderData(k-1,Qt::Horizontal,strg.mid(rememberLastI,i-rememberLastI));
                           qDebug()<<strg.mid(rememberLastI,i-rememberLastI);
                        }
                        if (k==model->columnCount()) break;
                    }
                }
            }

            if  ((strg[i]==' ')&&(strg[i+1]!=' '))
            {
                if (!inside){
                    rememberLastI=i+1;
                    if (strg[i+1]=='"')
                    {
                        rememberLastI++;
                        inside=true;
                    }
                }
            }
        }
    }
    QModelIndex id;
    i=0;
    int j;
    inside=false;

    if (model->rowCount()!=0)
        model->removeRows(0,model->rowCount());
    while (!stream.atEnd())
    {
        k=0;
        rememberLastI=0;
        model->setRowCount(model->rowCount()+1);
        strg = stream.readLine(512);
        for (j=0;j<strg.length();j++)
        {
            if ((strg[j]!=' ')&& ((strg[j+1]==' ')|| (j+1==strg.length())) )
            {
                if (!inside){
                    id=model->index(i,k);
                    if (strg.mid(rememberLastI,j-rememberLastI+1)!="-")
                        model->setData(id,strg.mid(rememberLastI,j-rememberLastI+1));
                    k++;
                }
                else
                {
                    if (strg[j]=='"'){
                        inside=false;
                        id=model->index(i,k);
                        model->setData(id,strg.mid(rememberLastI,j-rememberLastI));
                        k++;

                    }
                }
            }
            if  ((strg[j]==' ')&&(strg[j+1]!=' '))
            {
                if (!inside)
                    rememberLastI=j+1;
                if ((strg[j+1]=='"')&&(!inside))
                {
                    rememberLastI++;
                    inside=true;
                }
            }
        }
        i++;
    }
    group.close();
    qDebug()<<"End ReloadFile";
    return true;
}



void MainWindow::on_pushButton_2_clicked()
{
    QFile file(QApplication::applicationDirPath()+"/data/"+ui->comboBox->currentText()+".tabl");
    file.remove();
    QDir Dir(QApplication::applicationDirPath()+"/data/"+ui->comboBox->currentText()+"/");
    if (!Dir.exists())
    {
        return;
    }
    Dir.removeRecursively();
    ui->comboBox->removeItem(ui->comboBox->currentIndex());
    if (ui->comboBox->count()==0)
    {
        setStandartHeaders();
    }


}

void MainWindow::on_pushButton_4_clicked()
{

    if (ui->tableView->selectionModel()->selectedIndexes().length()==1)
    {
        model->removeRow(ui->tableView->selectionModel()->currentIndex().row());
        SaveTabl();
    }
}

void MainWindow::on_pushButton_5_clicked()
{
    SaveTabl();
}

void MainWindow::setStandartHeaders()
{

    QStringList headers={"Фамилия","Имя","Отчество","Телефон","Должность","Доп.Параметр1","Доп.Параметр2"};

    for (int j=0;j<headers.length();j++){
        model->setHeaderData(j,Qt::Horizontal,headers.at(j));
    }
}
