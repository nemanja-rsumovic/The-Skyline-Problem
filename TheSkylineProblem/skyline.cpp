#include "skyline.h"

QVector<QVector<int>> Skyline::getSkyline(QVector<QVector<int>>& buildings)
{
    if (buildings.isEmpty())
        return {};

    return divide(buildings, 0, static_cast<int>(buildings.size()) - 1);
}

QVector<QVector<int>> Skyline::divide(QVector<QVector<int>>& buildings, int left, int right)
{
    if (left == right)
    {
        int xStart = buildings[left][0];
        int xEnd   = buildings[left][1];
        int height = buildings[left][2];

        return
        {
            {xStart, height},
            {xEnd, 0}
        };
    }

    int center = left + (right - left) / 2;

    auto skylineLeft  = divide(buildings, left, center);
    auto skylineRight = divide(buildings, center + 1, right);

    return merge(skylineLeft, skylineRight);
}

QVector<QVector<int>> Skyline::merge(const QVector<QVector<int>>& a, const QVector<QVector<int>>& b)
{
    int x;
    QVector<QVector<int>> result;

    size_t i = 0;
    size_t j = 0;

    size_t aSize = a.size();
    size_t bSize = b.size();

    int aHeight = 0;
    int bHeight = 0;
    int lastHeight = 0;

    while (i < aSize && j < bSize)
    {
        int xa = a[i][0];
        int xb = b[j][0];

        if (xa < xb)
        {
            x = xa;
            aHeight = a[i][1];

            i++;
        }
        else if (xa > xb)
        {
            x = xb;
            bHeight = b[j][1];

            j++;
        }
        else
        {
            x = xa;
            aHeight = a[i][1];
            bHeight = b[j][1];

            i++;
            j++;
        }

        int height = std::max(aHeight, bHeight);

        if (height != lastHeight)
        {
            result.append({x, height});
            lastHeight = height;
        }
    }

    while (i < aSize)
    {
        result.append(a[i]);
        i++;
    }
    while (j < bSize)
    {
        result.append(b[j]);
        j++;
    }

    return result;
}
