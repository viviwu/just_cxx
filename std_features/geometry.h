/**
 ******************************************************************************
 * @file           : geometry.h
 * @author         : Administrator //Administrator
 * @brief          : None
 * @attention      : None
 * @date           : 2023/12/9/009
 ******************************************************************************
 */
//

#ifndef STD_CXX_GEOMETRY_H
#define STD_CXX_GEOMETRY_H

struct XPoint {
    int x;
    int y;

    XPoint translate(int dx, int dy) {
        XPoint translatedPoint;
        translatedPoint.x = x + dx;
        translatedPoint.y = y + dy;
        return translatedPoint;
    }

    double distanceTo(XPoint otherPoint) {
        int dx = otherPoint.x - x;
        int dy = otherPoint.y - y;
        return sqrt(dx*dx + dy*dy);
    }
};

struct XSize {
    int width;
    int height;

    bool operator<(XSize otherSize) {
        return (width < otherSize.width) && (height < otherSize.height);
    }
};

struct XRect {
    XPoint origin;
    XSize size;

    bool intersects(XRect otherRect) {
        int thisRight = origin.x + size.width;
        int thisBottom = origin.y + size.height;
        int otherRight = otherRect.origin.x + otherRect.size.width;
        int otherBottom = otherRect.origin.y + otherRect.size.height;

        return (origin.x < otherRight) && (thisRight > otherRect.origin.x) &&
               (origin.y < otherBottom) && (thisBottom > otherRect.origin.y);
    }

    bool contains(XPoint point) {
        int thisRight = origin.x + size.width;
        int thisBottom = origin.y + size.height;

        return (point.x >= origin.x) && (point.x <= thisRight) &&
               (point.y >= origin.y) && (point.y <= thisBottom);
    }

    bool containsRect(XRect otherRect) {
        int thisRight = origin.x + size.width;
        int thisBottom = origin.y + size.height;
        int otherRight = otherRect.origin.x + otherRect.size.width;
        int otherBottom = otherRect.origin.y + otherRect.size.height;

        return (otherRect.origin.x >= origin.x) && (otherRight <= thisRight) &&
               (otherRect.origin.y >= origin.y) && (otherBottom <= thisBottom);
    }
};


#endif  // STD_CXX_GEOMETRY_H
