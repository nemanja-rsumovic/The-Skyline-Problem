#ifndef SKYLINE_H
#define SKYLINE_H

#include <vector>

class Skyline
{
public:
    Skyline() = default;
    std::vector<std::vector<int>> getSkyline(std::vector<std::vector<int>>& buildings);

private:
    std::vector<std::vector<int>> divide(std::vector<std::vector<int>>& buildings, int left, int right);
    std::vector<std::vector<int>> merge(const std::vector<std::vector<int>>& a, const std::vector<std::vector<int>>& b);
};

#endif // SKYLINE_H
