#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "skyline.h"

#include <QString>
#include <QRandomGenerator>

#include <QMainWindow>
#include <QObject>

#include <QFileDialog>
#include <QFile>
#include <QTextStream>
#include <QMessageBox>

#include <QGraphicsScene>
#include <QGraphicsLineItem>
#include <QBrush>
#include <QPen>
#include <QPalette>
#include <QColor>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_loadData();
    void on_showBuildings();
    void on_calculateSkyline();
    void on_showSkyline();
    void on_reset();

private:
    Ui::MainWindow *ui;

    Skyline skyline;
    QVector<QVector<int>> buildings;
    QVector<QVector<int>> skyline_buildings;
    QGraphicsScene* scene;
    QGraphicsScene* scene2;

    void calculateBuildings();
    void calculateSkyline();

    void showBuildings();
    void showSkyline();
    void drawSkyline();
    void drawRoad(QGraphicsScene* s);
    void drawNightBackground();
};
#endif // MAINWINDOW_H
