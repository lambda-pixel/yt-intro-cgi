#pragma once

struct Pnt3D {
    float x, y, z, w;
};


struct BBox {
    Pnt3D min;
    Pnt3D max;
};
