#ifndef SKYLINE_H
#define SKYLINE_H

#include <iostream>
#include <vector>

class Skyline
{
public:
    Skyline();

    std::vector<std::vector<int>> getSkyline(std::vector<std::vector<int>>& buildings);

};

#endif // SKYLINE_H
