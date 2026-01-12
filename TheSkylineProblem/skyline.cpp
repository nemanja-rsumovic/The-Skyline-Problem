#include "skyline.h"

std::vector<std::vector<int>> Skyline::getSkyline(std::vector<std::vector<int>>& buildings)
{
    if (buildings.empty())
        return {};

    return divide(buildings, 0, static_cast<int>(buildings.size()) - 1);
}

std::vector<std::vector<int>> Skyline::divide(std::vector<std::vector<int>>& buildings, int left, int right)    
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

std::vector<std::vector<int>> Skyline::merge(const std::vector<std::vector<int>>& a, const std::vector<std::vector<int>>& b)
{
    int x = 0;
    int height = 0;
    std::vector<std::vector<int>> result;
    
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

        height = std::max(aHeight, bHeight);

        if (height != lastHeight)
        {
            result.push_back({x, height});
            lastHeight = height;
        }
    }

    while (i < aSize)
    {
        result.push_back(a[i]);
        i++;
    }
    while (j < bSize)
    {
        result.push_back(b[j]);
        j++;
    }

    return result;
}
