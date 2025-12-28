#ifndef 3DVFUNC_H
#define 3DVFUNC_H

#include <iostream>
#include <vector>
#include <string>

class Coord {
public:
    std::vector<float> xcoords, ycoords, zcoords;
    std::vector<float> stpxcoords, stpycoords, stpzcoords;
    std::vector<float> xlen, ylen, zlen;
    std::vector<float> vxc, vyc, vzc;
};

class Vector {
public:
    Coord cs;
    float xl, yl, zl, a, b, c, m, cc;

    enum class crdsysdim { XY, XZ, ZY };

    float lengthVector(float x, float y, float z, float stpx, float stpy, float stpz);
    void funcVector(int num, const std::string& level);
    std::vector<float> vecConverter(float len, float x, float y, float z, float ydeg, float zdeg);
};

class objIdent {
public:
    Vector vec;
    Coord cs;
    std::vector<std::vector<float>> vertices;

    void objReader(const std::string& filename);
    bool vectorCheck(float stpdom, const std::string& level, int vecnum);
};

#endif

