#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "skyline.h"

#include <QString>
#include <QRandomGenerator>
#include <iostream>

#include <QMainWindow>

#include <QFileDialog>
#include <QFile>
#include <QTextStream>
#include <QMessageBox>

#include <QGraphicsScene>
#include <QGraphicsLineItem>
#include <QBrush>
#include <QPen>

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
    void on_calculateSkyline();
    void on_showBuildings();
    void on_showSkyline();

private:
    Ui::MainWindow *ui;

    Skyline skyline;
    std::vector<std::vector<int>> buildings;
    std::vector<std::vector<int>> skyline_buildings;
    QGraphicsScene* scene;
    QGraphicsScene* scene2;

    void calculateBuildings();
    void calculateSkyline();

    void showBuildings();
    void showSkyline();
    void drawRoad(QGraphicsScene* s, const QColor& color);
    void drawNightBackground();

};
#endif // MAINWINDOW_H
