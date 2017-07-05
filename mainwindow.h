#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStandardItemModel>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_comboBox_activated(const QString &arg1);
    void CreateNewGroup();
    void CreateNewPerson();

    void on_pushButton_clicked();

    void on_pushButton_3_clicked();

    void on_comboBox_currentIndexChanged(const QString &arg1);

    void on_pushButton_2_clicked();

private:
    Ui::MainWindow *ui;
    QList<QWidget*> dialogWidgets;
    QStandardItemModel *model;
    void Reload();
    bool ReloadFile(const QString& str);
    //bool Reload(QString& way,const QString& str);
    void AddNewGroup();
    void AddNewPerson();
    void WriteBack(QStringList strLst);

};

#endif // MAINWINDOW_H
