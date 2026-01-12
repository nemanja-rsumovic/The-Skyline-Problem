#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    scene = new QGraphicsScene(this);
    ui->graphicsView->setScene(scene);
    ui->graphicsView->setRenderHint(QPainter::Antialiasing);
    ui->graphicsView->scale(1,-1);


    scene2 = new QGraphicsScene(this);
    ui->graphicsView_2->setScene(scene2);
    ui->graphicsView_2->setRenderHint(QPainter::Antialiasing);
    ui->graphicsView_2->scale(1,-1);

    connect(ui->pb_loadData, &QPushButton::clicked, this, &MainWindow::on_loadData);
    connect(ui->pb_calculateSkyline, &QPushButton::clicked, this, &MainWindow::on_calculateSkyline);
    connect(ui->pb_showBuildings, &QPushButton::clicked, this, &MainWindow::on_showBuildings);
    connect(ui->pb_showSkyline, &QPushButton::clicked, this, &MainWindow::on_showSkyline);
    connect(ui->pb_reset, &QPushButton::clicked, this, &MainWindow::on_reset);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_loadData()
{
    ui->te_buildings->clear();
    ui->te_skyline->clear();
    scene->clear();
    scene2->clear();
    scene2->setBackgroundBrush(QBrush());

    QString fileName = QFileDialog::getOpenFileName(this, "Load buildings data", "../TheSkylineProblem/examples", "Text files (*.txt)");

    if (fileName.isEmpty())
        return;

    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QMessageBox::critical(this, "Error", "Cannot open file.");
        return;
    }

    buildings.clear();
    skyline_buildings.clear();

    QTextStream in(&file);
    while(!in.atEnd())
    {
        int l, r, h;
        in >> l >> r >> h;

        if (in.status() != QTextStream::Ok)
            break;

        buildings.push_back({l,r,h});

    }

    file.close();

    if (buildings.empty())
    {
        QMessageBox::warning(this, "Warning", "File is empty or invalid.");
        return;
    }

    calculateBuildings();

    ui->pb_showBuildings->setEnabled(true);
    ui->pb_calculateSkyline->setEnabled(true);
}

void MainWindow::on_showBuildings()
{
    ui->pb_showBuildings->setDisabled(true);
    if(!ui->te_skyline->toPlainText().isEmpty())
        ui->pb_showSkyline->setEnabled(true);

    showBuildings();
    showSkyline();
}

void MainWindow::on_calculateSkyline()
{
    ui->pb_calculateSkyline->setDisabled(true);
    if (!ui->pb_showBuildings->isEnabled())
        ui->pb_showSkyline->setEnabled(true);

    skyline_buildings = skyline.getSkyline(buildings);
    calculateSkyline();
}

void MainWindow::on_showSkyline()
{
    ui->pb_showSkyline->setDisabled(true);
    drawSkyline();
}

void MainWindow::on_reset()
{
    ui->pb_showBuildings->setDisabled(true);
    ui->pb_calculateSkyline->setDisabled(true);
    ui->pb_showSkyline->setDisabled(true);
    ui->te_buildings->clear();
    ui->te_skyline->clear();
    scene->clear();
    scene2->clear();
    scene2->setBackgroundBrush(QBrush());
}

void MainWindow::calculateBuildings()
{
    ui->te_buildings->clear();

    for(const auto& b : buildings)
    {
        ui->te_buildings->append(QString("%1 %2 %3").arg(b[0]).arg(b[1]).arg(b[2]));
    }
}

void MainWindow::calculateSkyline()
{
    ui->te_skyline->clear();

    for(const auto& s : skyline_buildings)
    {
        ui->te_skyline->append(QString("%1 %2").arg(s[0]).arg(s[1]));
    }
}

void MainWindow::showBuildings()
{
    scene->clear();

    QPen pen(Qt::black);
    pen.setWidth(0.2);

    for(const auto& b : buildings)
    {
        int left = b[0];
        int right = b[1];
        int height = b[2];

        int width = right - left;

        QColor color = QColor::fromRgb(
            QRandomGenerator::global()->bounded(256),
            QRandomGenerator::global()->bounded(256),
            QRandomGenerator::global()->bounded(256),
            140
        );

        scene->addRect(
            left,
            0,
            width,
            height,
            pen,
            color
        );

    }

    QRectF bounds = scene->itemsBoundingRect();

    const qreal marginX = bounds.width() * 0.1;
    const qreal marginY = bounds.height() * 0.1;
    bounds.adjust(-marginX, -marginY, marginX, marginY);
    scene->setSceneRect(bounds);

    drawRoad(scene, Qt::black);

    ui->graphicsView->fitInView(scene->sceneRect(), Qt::KeepAspectRatio);
}

void MainWindow::showSkyline()
{
    scene2->clear();

    int maxHeight = 0;
    int maxX = 0;

    QColor color = QColor::fromRgb(190, 190, 190);

    for(const auto& b : buildings)
    {
        int left = b[0];
        int right = b[1];
        int height = b[2];

        int width = right - left;

        scene2->addRect(
            left,
            0,
            width,
            height,
            Qt::NoPen,
            color
        );

        maxHeight = std::max(maxHeight, height);
        maxX = std::max(maxX, right);

    }

    QRectF bounds = scene2->itemsBoundingRect();

    const qreal marginX = bounds.width() * 0.1;
    const qreal marginY = bounds.height() * 0.1;
    bounds.adjust(-marginX, -marginY, marginX, marginY);
    scene2->setSceneRect(bounds);

    drawRoad(scene2, color);
    drawNightBackground();

    ui->graphicsView_2->fitInView(scene2->sceneRect(), Qt::KeepAspectRatio);
}

void MainWindow::drawSkyline()
{
    if (skyline_buildings.empty())
         return;

     QPainterPath path;

     path.moveTo(skyline_buildings[0][0], 0);
     path.lineTo(skyline_buildings[0][0], skyline_buildings[0][1]);

     int skyline_size = skyline_buildings.size();
     for (int i = 1; i < skyline_size; i++)
     {
         qreal x = skyline_buildings[i][0];
         qreal y = skyline_buildings[i][1];

         path.lineTo(x, path.currentPosition().y());
         path.lineTo(x, y);
     }

     QPen pen(QColor(255, 80, 80));
     pen.setWidth(3);
     pen.setCosmetic(true);

     QGraphicsPathItem* item = scene2->addPath(path, pen);
     item->setZValue(5);

}

void MainWindow::drawRoad(QGraphicsScene *s, const QColor &color)
{
    QPen roadPen(color);
    roadPen.setWidth(0.5);

    QGraphicsLineItem* road = s->addLine(
        s->sceneRect().left(), 0,
        s->sceneRect().right(), 0,
        roadPen
    );
    road->setZValue(-1);
}

void MainWindow::drawNightBackground()
{
    QRectF r = scene2->sceneRect();

    QLinearGradient gradient;
    gradient.setStart(0, r.top());
    gradient.setFinalStop(0, r.bottom());

    gradient.setColorAt(0.0, QColor(5,5,25));
    gradient.setColorAt(0.3, QColor(20,20,60));
    gradient.setColorAt(1.0, QColor(60,60,60));

    scene2->setBackgroundBrush(QBrush(gradient));
}
