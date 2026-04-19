#include "mainwindow.h"
#include "ui_mainwindow.h"

static const QColor buildingPalette[] = {
    QColor(88,  166, 255, 210),
    QColor(63,  185,  80, 210),
    QColor(247, 147,  58, 210),
    QColor(196,  98, 247, 210),
    QColor(58,  214, 247, 210),
    QColor(247,  58, 130, 210),
    QColor(247, 218,  58, 210),
    QColor(58,  247, 196, 210),
};
static const int paletteSize = 8;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    applyStylesheet();

    scene = new QGraphicsScene(this);
    ui->graphicsView->setScene(scene);
    ui->graphicsView->setRenderHint(QPainter::Antialiasing);
    ui->graphicsView->scale(1, -1);

    scene2 = new QGraphicsScene(this);
    ui->graphicsView_2->setScene(scene2);
    ui->graphicsView_2->setRenderHint(QPainter::Antialiasing);
    ui->graphicsView_2->scale(1, -1);

    connect(ui->pb_loadData,        &QPushButton::clicked, this, &MainWindow::on_loadData);
    connect(ui->pb_calculateSkyline,&QPushButton::clicked, this, &MainWindow::on_calculateSkyline);
    connect(ui->pb_showBuildings,   &QPushButton::clicked, this, &MainWindow::on_showBuildings);
    connect(ui->pb_showSkyline,     &QPushButton::clicked, this, &MainWindow::on_showSkyline);
    connect(ui->pb_reset,           &QPushButton::clicked, this, &MainWindow::on_reset);

    statusBar()->showMessage("Ready — load building data to begin");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::applyStylesheet()
{
    setStyleSheet(R"(
QMainWindow {
    background-color: #0d1117;
}
QWidget#centralwidget {
    background-color: #0d1117;
}
QFrame#sidePanel {
    background-color: #161b22;
    border-right: 2px solid #21262d;
}
QLabel#appTitle {
    color: #58a6ff;
    font-size: 22px;
    font-weight: 700;
}
QLabel#appSubtitle {
    color: #484f58;
    font-size: 10px;
}
QLabel#lbl_buildings, QLabel#lbl_skyline {
    color: #484f58;
    font-size: 9px;
    font-weight: bold;
    margin-top: 4px;
}
QLabel#lbl_buildingsView, QLabel#lbl_skylineView {
    color: #484f58;
    font-size: 9px;
    font-weight: bold;
}
QTextEdit {
    background-color: #0d1117;
    color: #c9d1d9;
    border: 1px solid #30363d;
    border-radius: 6px;
    font-family: "Courier New", monospace;
    font-size: 12px;
    padding: 4px 6px;
    selection-background-color: #1f6feb;
    selection-color: #ffffff;
}
QTextEdit:focus {
    border-color: #1f6feb;
}
QPushButton {
    background-color: #21262d;
    color: #c9d1d9;
    border: 1px solid #30363d;
    border-radius: 6px;
    padding: 7px 12px;
    font-size: 12px;
    font-weight: 500;
}
QPushButton:hover {
    background-color: #30363d;
    border-color: #58a6ff;
    color: #e6edf3;
}
QPushButton:pressed {
    background-color: #161b22;
    border-color: #58a6ff;
}
QPushButton:disabled {
    background-color: #161b22;
    color: #484f58;
    border-color: #21262d;
}
QPushButton#pb_loadData {
    background-color: #1f6feb;
    color: #ffffff;
    border: 1px solid #388bfd;
    font-weight: 600;
}
QPushButton#pb_loadData:hover {
    background-color: #388bfd;
    border-color: #58a6ff;
    color: #ffffff;
}
QPushButton#pb_loadData:pressed {
    background-color: #1158c7;
}
QPushButton#pb_reset {
    background-color: #160b0b;
    color: #f85149;
    border: 1px solid #6e1d28;
}
QPushButton#pb_reset:hover {
    background-color: #6e1d28;
    color: #ffffff;
    border-color: #f85149;
}
QPushButton#pb_reset:pressed {
    background-color: #490d18;
}
QFrame#sep1, QFrame#sep2 {
    border: none;
    border-top: 1px solid #21262d;
    max-height: 1px;
    margin: 4px 0px;
    background-color: transparent;
}
QGraphicsView {
    background-color: #1c2128;
    border: 1px solid #21262d;
    border-radius: 8px;
}
QScrollBar:vertical {
    background: transparent;
    width: 6px;
    margin: 0;
}
QScrollBar::handle:vertical {
    background: #30363d;
    border-radius: 3px;
    min-height: 20px;
}
QScrollBar::handle:vertical:hover { background: #484f58; }
QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical { height: 0px; }
QScrollBar:horizontal {
    background: transparent;
    height: 6px;
    margin: 0;
}
QScrollBar::handle:horizontal {
    background: #30363d;
    border-radius: 3px;
    min-width: 20px;
}
QScrollBar::handle:horizontal:hover { background: #484f58; }
QScrollBar::add-line:horizontal, QScrollBar::sub-line:horizontal { width: 0px; }
QStatusBar {
    background-color: #161b22;
    color: #484f58;
    border-top: 1px solid #21262d;
    font-size: 11px;
    padding: 2px 8px;
}
    )");
}

void MainWindow::on_loadData()
{
    ui->te_buildings->clear();
    ui->te_skyline->clear();
    scene->clear();
    scene2->clear();
    scene2->setBackgroundBrush(QBrush());
    scene->setBackgroundBrush(QBrush());

    QString fileName = QFileDialog::getOpenFileName(this, "Load buildings data",
        "../TheSkylineProblem/examples", "Text files (*.txt)");

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
    while (!in.atEnd())
    {
        int l, r, h;
        in >> l >> r >> h;
        if (in.status() != QTextStream::Ok)
            break;
        buildings.append({l, r, h});
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

    statusBar()->showMessage(QString("Loaded %1 buildings — press Show Buildings to visualize")
                             .arg(buildings.size()));
}

void MainWindow::on_showBuildings()
{
    ui->pb_showBuildings->setDisabled(true);
    if (!ui->te_skyline->toPlainText().isEmpty())
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

    statusBar()->showMessage(QString("Skyline computed — %1 keypoints — press Show Skyline to render")
                             .arg(skyline_buildings.size()));
}

void MainWindow::on_showSkyline()
{
    ui->pb_showSkyline->setDisabled(true);
    drawSkyline();
    statusBar()->showMessage("Skyline rendered");
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
    scene->setBackgroundBrush(QBrush());
    scene2->setBackgroundBrush(QBrush());
    buildings.clear();
    skyline_buildings.clear();
    statusBar()->showMessage("Ready — load building data to begin");
}

void MainWindow::calculateBuildings()
{
    ui->te_buildings->clear();
    for (const auto& b : buildings)
        ui->te_buildings->append(QString("%1  %2  %3").arg(b[0]).arg(b[1]).arg(b[2]));
}

void MainWindow::calculateSkyline()
{
    ui->te_skyline->clear();
    for (const auto& s : skyline_buildings)
        ui->te_skyline->append(QString("%1  %2").arg(s[0]).arg(s[1]));
}

void MainWindow::showBuildings()
{
    scene->clear();

    QPen pen(QColor(255, 255, 255, 25));
    pen.setCosmetic(true);
    pen.setWidth(1);

    for (int i = 0; i < buildings.size(); i++)
    {
        const auto& b = buildings[i];
        int left = b[0], right = b[1], height = b[2];
        QColor color = buildingPalette[i % paletteSize];
        auto* rect = scene->addRect(left, 0, right - left, height, pen, QBrush(color));
        rect->setZValue(1);
    }

    QRectF bounds = scene->itemsBoundingRect();
    const qreal marginX = bounds.width()  * 0.1;
    const qreal marginY = bounds.height() * 0.15;
    bounds.adjust(-marginX, -marginY, marginX, marginY);
    scene->setSceneRect(bounds);

    drawBuildingsBackground();
    drawRoad(scene);

    ui->graphicsView->fitInView(scene->sceneRect(), Qt::KeepAspectRatio);
}

void MainWindow::showSkyline()
{
    scene2->clear();

    QColor buildingColor(22, 33, 52);

    for (const auto& b : buildings)
    {
        int left = b[0], right = b[1], height = b[2];
        auto* rect = scene2->addRect(left, 0, right - left, height, Qt::NoPen, QBrush(buildingColor));
        rect->setZValue(1);
    }

    QRectF bounds = scene2->itemsBoundingRect();
    const qreal marginX = bounds.width()  * 0.1;
    const qreal marginY = bounds.height() * 0.15;
    bounds.adjust(-marginX, -marginY, marginX, marginY);
    scene2->setSceneRect(bounds);

    drawNightBackground();
    drawStars();
    drawMoon();
    drawRoad(scene2);

    ui->graphicsView_2->fitInView(scene2->sceneRect(), Qt::KeepAspectRatio);
}

void MainWindow::drawSkyline()
{
    if (skyline_buildings.empty())
        return;

    QPainterPath path;
    path.moveTo(skyline_buildings[0][0], 0);
    path.lineTo(skyline_buildings[0][0], skyline_buildings[0][1]);

    for (int i = 1; i < skyline_buildings.size(); i++)
    {
        qreal x = skyline_buildings[i][0];
        qreal y = skyline_buildings[i][1];
        path.lineTo(x, path.currentPosition().y());
        path.lineTo(x, y);
    }

    // Outer glow
    QPen outerGlow(QColor(255, 120, 60, 18));
    outerGlow.setWidth(14);
    outerGlow.setCosmetic(true);
    outerGlow.setJoinStyle(Qt::RoundJoin);
    scene2->addPath(path, outerGlow)->setZValue(1.7);

    // Mid glow
    QPen midGlow(QColor(255, 140, 80, 45));
    midGlow.setWidth(7);
    midGlow.setCosmetic(true);
    midGlow.setJoinStyle(Qt::RoundJoin);
    scene2->addPath(path, midGlow)->setZValue(1.8);

    // Inner glow
    QPen innerGlow(QColor(255, 170, 100, 90));
    innerGlow.setWidth(4);
    innerGlow.setCosmetic(true);
    innerGlow.setJoinStyle(Qt::RoundJoin);
    scene2->addPath(path, innerGlow)->setZValue(1.9);

    // Main line
    QPen pen(QColor(255, 200, 140));
    pen.setWidth(2);
    pen.setCosmetic(true);
    pen.setJoinStyle(Qt::RoundJoin);
    scene2->addPath(path, pen)->setZValue(2);
}

void MainWindow::drawRoad(QGraphicsScene *s)
{
    QPen roadPen(QColor(50, 65, 80));
    roadPen.setCosmetic(true);
    roadPen.setWidth(2);

    auto* road = s->addLine(s->sceneRect().left(), 0, s->sceneRect().right(), 0, roadPen);
    road->setZValue(1.5);
}

void MainWindow::drawBuildingsBackground()
{
    QRectF r = scene->sceneRect();

    QLinearGradient grad;
    grad.setStart(0, r.top());
    grad.setFinalStop(0, r.bottom());
    grad.setColorAt(0.0, QColor(13, 17, 23));
    grad.setColorAt(1.0, QColor(18, 28, 45));

    auto* bg = scene->addRect(r, Qt::NoPen, QBrush(grad));
    bg->setZValue(-1);
}

void MainWindow::drawNightBackground()
{
    QRectF r = scene2->sceneRect();

    QLinearGradient grad;
    grad.setStart(0, r.top());
    grad.setFinalStop(0, r.bottom());
    grad.setColorAt(0.0, QColor(15, 22, 40));
    grad.setColorAt(0.6, QColor(6, 10, 25));
    grad.setColorAt(1.0, QColor(3, 5, 15));

    auto* bg = scene2->addRect(r, Qt::NoPen, QBrush(grad));
    bg->setZValue(-1);
}

void MainWindow::drawStars()
{
    QRectF r = scene2->sceneRect();
    QRandomGenerator* rng = QRandomGenerator::global();

    const qreal inset = r.width() * 0.01;
    const qreal xMin  = r.left()   + inset;
    const qreal xMax  = r.right()  - inset;
    const qreal yMin  = r.bottom() * 0.05;
    const qreal yMax  = r.bottom() - inset;

    for (int i = 0; i < 180; i++)
    {
        qreal x = xMin + rng->generateDouble() * (xMax - xMin);
        qreal y = yMin + rng->generateDouble() * (yMax - yMin);

        bool bright = rng->bounded(10) < 2;
        qreal size  = r.height() * (bright ? 0.012 : 0.005);
        int alpha   = rng->bounded(80) + 140;

        QColor color = bright
            ? QColor(255, 250, 210, alpha)
            : QColor(200, 210, 255, alpha);

        auto* star = scene2->addEllipse(x - size/2, y - size/2, size, size,
                                        Qt::NoPen, QBrush(color));
        star->setZValue(0.5);
    }
}

void MainWindow::drawMoon()
{
    QRectF r = scene2->sceneRect();

    qreal moonX = r.left() + r.width()  * 0.82;
    qreal moonY = r.bottom() * 0.72;
    qreal moonR = r.height() * 0.07;

    // Soft glow halo
    QRadialGradient halo(moonX, moonY, moonR * 3.5);
    halo.setColorAt(0.0, QColor(255, 255, 180, 35));
    halo.setColorAt(1.0, QColor(255, 255, 180,  0));
    scene2->addEllipse(moonX - moonR*3.5, moonY - moonR*3.5,
                       moonR*7, moonR*7, Qt::NoPen, QBrush(halo))->setZValue(0.3);

    // Moon disc
    scene2->addEllipse(moonX - moonR, moonY - moonR,
                       moonR*2, moonR*2, Qt::NoPen,
                       QBrush(QColor(255, 253, 220)))->setZValue(0.4);

    // Crescent shadow
    scene2->addEllipse(moonX - moonR*0.45, moonY - moonR,
                       moonR*2, moonR*2, Qt::NoPen,
                       QBrush(QColor(6, 10, 25)))->setZValue(0.45);
}
