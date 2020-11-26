#pragma once

namespace HG { namespace Math {
    struct HGPos {
        HGPos() : X(0), Y(0) {}
        HGPos(int x, int y) : X(x), Y(y) {}
        int X, Y;
    };

    struct HGRect {
        HGRect() : H(0), W(0) {}
        HGRect(int w, int h) : H(h), W(w) {}
        int H, W;
    };
} }
