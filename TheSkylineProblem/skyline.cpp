#include "skyline.h"
#include <algorithm>

QVector<QVector<int>> Skyline::getSkyline(QVector<QVector<int>>& buildings)
{
    if (buildings.isEmpty())
        return {};

    return divide(buildings, 0, static_cast<int>(buildings.size()) - 1);
}

QVector<QVector<int>> Skyline::divide(QVector<QVector<int>>& buildings, int lo, int hi)
{
    if (lo == hi)
    {
        int xLeft  = buildings[lo][0];
        int xRight = buildings[lo][1];
        int h      = buildings[lo][2];
        return { {xLeft, h}, {xRight, 0} };
    }

    int split = (lo + hi) / 2;

    QVector<QVector<int>> leftHalf  = divide(buildings, lo, split);
    QVector<QVector<int>> rightHalf = divide(buildings, split + 1, hi);

    return merge(leftHalf, rightHalf);
}

QVector<QVector<int>> Skyline::merge(const QVector<QVector<int>>& left, const QVector<QVector<int>>& right)
{
    QVector<int> xs;
    for (const auto& pt : left)  xs.push_back(pt[0]);
    for (const auto& pt : right) xs.push_back(pt[0]);
    std::sort(xs.begin(), xs.end());

    QVector<QVector<int>> outline;
    int li = 0, ri = 0;
    int hL = 0, hR = 0;
    int prevH = -1;

    for (int x : xs)
    {
        if (li < left.size()  && left[li][0]  == x) hL = left[li++][1];
        if (ri < right.size() && right[ri][0] == x) hR = right[ri++][1];

        int peak = std::max(hL, hR);
        if (peak != prevH)
        {
            outline.push_back({x, peak});
            prevH = peak;
        }
    }

    return outline;
}
