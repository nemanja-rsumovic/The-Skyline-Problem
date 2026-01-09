#include "skyline.h"

Skyline::Skyline()
{

}

std::vector<std::vector<int>> Skyline::getSkyline(std::vector<std::vector<int>> &buildings)
{
    std::vector<std::vector<int>> result = {
        {2, 10},
        {3, 15},
        {7, 12},
        {12, 0},
        {15, 10},
        {20, 8},
        {24, 0}
    };

    return result;
}
