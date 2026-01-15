#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QPalette darkPalette;

    darkPalette.setColor(QPalette::Window, QColor(45, 45, 45));
    darkPalette.setColor(QPalette::WindowText, Qt::white);
    darkPalette.setColor(QPalette::Base, QColor(30, 30, 30));
    darkPalette.setColor(QPalette::AlternateBase, QColor(45, 45, 45));
    darkPalette.setColor(QPalette::ToolTipBase, Qt::white);
    darkPalette.setColor(QPalette::ToolTipText, Qt::white);
    darkPalette.setColor(QPalette::Text, Qt::white);
    darkPalette.setColor(QPalette::Button, QColor(45, 45, 45));
    darkPalette.setColor(QPalette::ButtonText, Qt::white);
    darkPalette.setColor(QPalette::BrightText, Qt::red);
    darkPalette.setColor(QPalette::Highlight, QColor(90, 90, 90));
    darkPalette.setColor(QPalette::HighlightedText, Qt::white);

    darkPalette.setColor(QPalette::Disabled, QPalette::ButtonText, QColor(120, 120, 120));
    darkPalette.setColor(QPalette::Disabled, QPalette::Text, QColor(120, 120, 120));
    darkPalette.setColor(QPalette::Disabled, QPalette::WindowText, QColor(120, 120, 120));
    darkPalette.setColor(QPalette::Disabled, QPalette::Button, QColor(80, 80, 80));

    a.setPalette(darkPalette);

    MainWindow w;
    w.show();
    return a.exec();
}
