#ifndef SKYLINE_H
#define SKYLINE_H

#include <QVector>

class Skyline
{
public:
    Skyline() = default;
    QVector<QVector<int>> getSkyline(QVector<QVector<int>>& buildings);

private:
    QVector<QVector<int>> divide(QVector<QVector<int>>& buildings, int left, int right);
    QVector<QVector<int>> merge(const QVector<QVector<int>>& a, const QVector<QVector<int>>& b);
};

#endif // SKYLINE_H
