#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->actionAdd_file, &QAction::triggered, this, [this] (){
       fileName = QFileDialog::getOpenFileName(this, tr("Open file"));
       QMediaPlayer* player = new QMediaPlayer;
       player->setMedia(QUrl::fromLocalFile(fileName));
       player->setVolume(50);
       player->play();
    });
}

MainWindow::~MainWindow()
{
    delete ui;
}
