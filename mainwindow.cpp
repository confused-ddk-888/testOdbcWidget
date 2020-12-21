#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QMessageBox>
#include <QSqlError>
#include <QDesktopWidget>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //重置窗体大小
    //this->resize(850, 400);

    //屏幕置中
    QDesktopWidget *desktop = QApplication::desktop();
    move((desktop->width()-this->width())/2,(desktop->height()-this->height())/2);

    //连接button事件
    connect(ui->btnConn, SIGNAL(clicked()), this, SLOT(ClickOnConnect()));
    connect(ui->btnExit, SIGNAL(clicked()), this, SLOT(ClickOnExit()));
    connect(ui->btnExec, SIGNAL(clicked()), this, SLOT(ClickOnExec()));
    connect(ui->btnClear, SIGNAL(clicked()), this, SLOT(ClickOnClear()));


    //设置预定义DSN
    ui->tbDSN->clear();
    ui->tbDSN->setText("PostgreSQL30");
    ui->tbLog->clear();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::ClickOnConnect()
{
    if (ui->tbDSN->toPlainText().length() == 0)
    {
        QMessageBox::warning(this, "warning", "DSN is NULL, please input dsn string!");

        ui->tbLog->append("DSN is NULL, please input dsn string!");
        return;
    }

    db = QSqlDatabase::addDatabase("QODBC");

    ui->tbLog->append("支持的驱动：" + db.drivers().join(" "));
    ui->tbLog->append(QString::fromUtf16(u"开始连接数据库....."));

    db.setDatabaseName(ui->tbDSN->toPlainText());

    if(!db.open())
    {
        QMessageBox::information(this, "Warning", db.lastError().text());

        ui->tbLog->append(db.lastError().text());

        ui->tbLog->append("Can't Connect to DB !");

        //qDebug() << "Can't Connect to DB !";
    }
    else
    {
        ui->tbLog->append("连接数据库成功！");
        //qDebug() << "Connected Successfully to DB !";

        ui->tbLog->append("创建customer_t1....");

        QSqlQuery query;
        query.prepare("drop table IF EXISTS customer_t1");
        if (!query.exec())
        {
            ui->tbLog->append("创建失败 step 1.." + query.lastError().text());
        }
        else
        {
            ui->tbLog->append("删除 customer_t1.... 成功.");

            query.clear();

            query.prepare("CREATE TABLE customer_t1(id INTEGER, name VARCHAR(32));");
            if(!query.exec())
            {
                ui->tbLog->append("创建失败 step 2.." + query.lastError().text());
            }
            else
            {
                ui->tbLog->append("创建 customer_t1.... 成功.");

                query.clear();
                query.prepare("insert into customer_t1(id, name) values(1, 'ddk')");
                if (!query.exec())
                {
                    ui->tbLog->append("插入数据.." + query.lastError().text());
                }
                else
                {
                    ui->tbLog->append("插入 customer_t1.... 成功.");

                    query.clear();
                    query.prepare("SELECT * FROM customer_t1 WHERE ID = 1");
                    if(!query.exec())
                    {
                        ui->tbLog->append("读取数据.." + query.lastError().text());
                    }
                    else
                    {
                        ui->tbLog->append("读取 customer_t1.... 成功.");
                        while(query.next())
                        {
                            ui->tbLog->append("col 1:" + query.value(0).toString());
                            ui->tbLog->append("col 2:" + query.value(1).toString());

                        }
                    }
                }
            }
        }
    }
    db.close();
}

void MainWindow::ClickOnExit()
{
    qApp->exit();
}

void MainWindow::ClickOnExec()
{
    if (ui->tbDSN->toPlainText().length() == 0)
    {
        QMessageBox::warning(this, "warning", "DSN is NULL, please input dsn string!");

        ui->tbLog->append("DSN is NULL, please input dsn string!");
        return;
    }

    if (ui->tbSQL->toPlainText().length() == 0)
    {
        QMessageBox::warning(this, "Error", "Please input Sql statement!");

        ui->tbLog->append("SQL is NULL, Please input Sql statement!");
        return;
    }

    db = QSqlDatabase::addDatabase("QODBC");

    ui->tbLog->append("支持的驱动：" + db.drivers().join(" "));
    ui->tbLog->append("开始连接数据库.....");

    db.setDatabaseName(ui->tbDSN->toPlainText());

    if(!db.open())
    {
        QMessageBox::information(this, "Warning", db.lastError().text());

        ui->tbLog->append(db.lastError().text());

        ui->tbLog->append("Can't Connect to DB !");

        //qDebug() << "Can't Connect to DB !";
    }
    else
    {
        ui->tbLog->append("连接数据库成功！");

        ui->tbLog->append("开始执行SQL....");

        QSqlQuery query;
        query.prepare(ui->tbSQL->toPlainText());
        if (!query.exec())
        {
            ui->tbLog->append("执行 SQL 失败:" + query.lastError().text());
        }
        else
        {
            ui->tbLog->append("执行 SQL 成功.");
        }
    }
    db.close();

}

void MainWindow::ClickOnClear()
{
    ui->tbLog->clear();
}
