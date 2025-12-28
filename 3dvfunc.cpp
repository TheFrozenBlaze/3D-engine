#include <iostream>
#include <vector>
#include <cmath>
#include <string>
#include <fstream>
#include <sstream>


struct Coord {
public:
    //endpoint 
    std::vector<float> xcoords;
    std::vector<float> ycoords;
    std::vector<float> zcoords;
    //startpoint
    std::vector<float> stpxcoords;
    std::vector<float> stpycoords;
    std::vector<float> stpzcoords;
    // length
    std::vector<float> xlen;
    std::vector<float> ylen;
    std::vector<float> zlen;
    // vertices
    std::vector<float> vxc;
    std::vector<float> vyc;
    std::vector<float> vzc;
};

class Vector {
public:
    Coord cs;
    float xl = 0.0f;
    float yl = 0.0f;
    float zl = 0.0f;
    float a = 0.0f;
    float b = 0.0f;
    float c = 0.0f;
    float m = 0.0f; // steepness
    float cc = 0.0f; // function crossing the given axis

    enum class crdsysdim {
        XY,
        XZ,
        ZY
    };

    // Calculate vector length
    float lengthVector(float x, float y, float z, float stpx, float stpy, float stpz) {
        xl = std::fabs(stpx - x);
        cs.xlen.push_back(xl);

        yl = std::fabs(stpy - y);
        cs.ylen.push_back(yl);

        zl = std::fabs(stpz - z);
        cs.zlen.push_back(zl);

        float xzd = std::sqrt(std::pow(xl, 2) + std::pow(zl, 2));
        float ydxz = std::sqrt(std::pow(xzd, 2) + std::pow(yl, 2));

        return ydxz;
    }

    // Convert vector line to function form
    void funcVector(int num, const std::string& level) {
        crdsysdim state;
        std::toupper(level[0]);
        std::toupper(level[1]);
        if (level == "XY") {state = crdsysdim::XY;}
        else if (level == "XZ") {state = crdsysdim::XZ;}
        else {state = crdsysdim::ZY;}

        float stpdom = 0.0f;
        float dx = 0.0f;
        float dy = 0.0f;

        switch (state) {
            case crdsysdim::XY:
                dx = cs.stpxcoords[num] - cs.xcoords[num];
                dy = cs.stpycoords[num] - cs.ycoords[num];
                stpdom = cs.stpycoords[num];
                break;
            case crdsysdim::XZ:
                dx = cs.stpxcoords[num] - cs.xcoords[num];
                dy = cs.stpzcoords[num] - cs.zcoords[num];
                stpdom = cs.stpzcoords[num];
                break;
            case crdsysdim::ZY:
                dx = cs.stpzcoords[num] - cs.zcoords[num];
                dy = cs.stpycoords[num] - cs.ycoords[num];
                stpdom = cs.stpycoords[num];
                break;
        }

        a = std::fabs(dy);
        b = std::fabs(dx);
        c = std::sqrt(std::pow(a, 2) + std::pow(b, 2));
        if (dx == 0.0f) {
            m = INFINITY;
        } else {
            m = dy / dx;
        }
        double degree = std::acos(b / c);
        cc = std::fabs(stpdom) - ((b / std::cos(degree)) * std::sin(degree));
    }

    // Convert vector with rotation
    std::vector<float> vecConverter(float len, float x, float y, float z, float ydeg, float zdeg) {
        float yRad = static_cast<float>(ydeg * M_PI / 180.0);
        float zRad = static_cast<float>(zdeg * M_PI / 180.0);

        float cx = x;
        float cy = y;
        float cz = z;

        float x0 = cx - len;
        float y0 = cy;
        float z0 = cz;

        float dx = x0 - cx;
        float dy = y0 - cy;
        float dz = z0 - cz;

        float dx1 = dx * std::cos(yRad) + dz * std::sin(yRad);
        float dy1 = dy;
        float dz1 = -dx * std::sin(yRad) + dz * std::cos(yRad);

        float dx2 = dx1 * std::cos(zRad) - dy1 * std::sin(zRad);
        float dy2 = dx1 * std::sin(zRad) + dy1 * std::cos(zRad);
        float dz2 = dz1;

        float newX = cx + dx2;
        float newY = cy + dy2;
        float newZ = cz + dz2;

        return { newX, newY, newZ };
    }
};

class objIdent {
public:
    Vector vec;
    Coord cs;
    std::vector<std::vector<float>> vertices;

    // Read .obj file
    void objReader(const std::string& filename) {
        std::ifstream file(filename);
        if (!file.is_open()) {
            std::cout << "Error reading file: " << filename << std::endl;
            return;
        }

        std::string line;
        while (std::getline(file, line)) {
            if (line.size() > 1 && line[0] == 'v' && line[1] == ' ') {
                std::istringstream iss(line);
                std::string v;
                float x, y, z;
                iss >> v >> x >> y >> z;
                vertices.push_back({x, y, z});
                cs.vxc.push_back(x);
                cs.vyc.push_back(y);
                cs.vzc.push_back(z);
            }
        }
    }

    // Check vector intersection
    bool vectorCheck(float stpdom, const std::string& level, int vecnum) {
        Vector::crdsysdim sst;
        if (level == "XY" || level == "xy") {sst = Vector::crdsysdim::XY;
        }
        else if (level == "XZ" || level == "xz") 
        {sst = Vector::crdsysdim::XZ;
        }
        else {sst = Vector::crdsysdim::ZY;}


        float check;
        int above = 0;
        int below = 0;

        vec.funcVector(vecnum, level);

        for (size_t i = 0; i < cs.vxc.size(); i++) {
            switch (sst) {
                case Vector::crdsysdim::XY:
                    check = (cs.vxc[i] * vec.m) + vec.cc;
                    if (check == cs.vyc[i]) return true;
                    else if (check > cs.vyc[i]) above++;
                    else below++;
                    break;

                case Vector::crdsysdim::XZ:
                    check = (cs.vxc[i] * vec.m) + vec.cc;
                    if (check == cs.vzc[i]) return true;
                    else if (check > cs.vzc[i]) above++;
                    else below++;
                    break;

                case Vector::crdsysdim::ZY:
                    check = (cs.vzc[i] * vec.m) + vec.cc;
                    if (check == cs.vyc[i]) return true;
                    else if (check > cs.vyc[i]) above++;
                    else below++;
                    break;
            }

            if (above > 0 && below > 0)
                return true;
        }
        return false;
    }
};