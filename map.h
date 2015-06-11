/** @file map.h
 *
 * Mapping class
 *
 */

#ifndef _MAP_H_
#define _MAP_H_

#include <iostream>
#include <vector>
#include <math.h>
//using namespace planner;

class Vec3
{
public:
    Vec3(double x, double y, double z)
    {
        this->x = x;
        this->y = y;
        this->z = z;
    }

    double x, y, z;
};

/// 2.5D elevation map
class Map
{
public:
    /// Default constructor
    Map(size_t numRows, size_t numCols, double sizeX, double sizeY) :
        numRows(numRows),
        numCols(numCols),
        sizeX(sizeX),
        sizeY(sizeY),
        rasterX(sizeX/numRows),
        rasterY(sizeY/numCols),
        map(numRows, std::vector<double>(numCols, 0.0))
    {
    }

    /// Load elevation map from file
    bool load(std::string filename);

    /// Calculate normal vector (input: vertices of the triangle, output: normal vector)
    Vec3 calculateNormal(std::vector<Vec3>& vertices);

    /// export map to file
    void exportMap(const std::string filename) const;

    /// compute spherical variance
    //float_type computeVariance(float_type x, float_type y, float_type range);
    /// set of maps
    typedef std::vector<Map> Seq;

    inline double get(int row, int col) const
    {
        return ((row<numRows) && (col<numCols) && (row>-1) && (col>-1)) ? map[col][row] : -1;
    }

    inline double get(double x, double y) const
    {
        int row, col;
        toRaster(x, y, row, col);
        return get(row, col);
    }

    inline void toRaster(double x, double y, int& row, int& col) const
    {
        row = (x / (sizeX / numRows)) + (numRows / 2) + 1;
        col = (y / (sizeY / numRows)) + (numCols / 2) + 1;
    }

    double getMax(double x, double y, unsigned int area) const
    {
        int row, col;
        toRaster(x, y, row, col);
        double max = -10.0;
        for (int i = -int(area); i <= int(area); i++){
            for (int j = -int(area); j <= int(area); j++) {
                if (get(row + i, col+j) > max)
                    max = get(row+i, col+j);
            }
        }
        return max;
    }

    inline bool set(int row, int col, double height)
    {
        if ((row > numRows) && (col > numCols) && (row < 0) && (col < 0)) return false;
        else { map[row][col] = height; return true;}
    }

    bool set(double x, double y, double height)
    {
        int row, col;
        toRaster(x, y, row, col);
        if ((row < numRows) && (col < numCols) && (row <0 ) && (col < 0)) return false;
        else { map[row][col] = height; return true;}
    }

    /// compute variance
    double computeVariance(double x, double y, double range) const
    {
        double defaultHeight = get(x,y);
        double variance = 0;
        int rastersNoX = range/rasterX; int rastersNoY = range/rasterY;
        for(int i = -rastersNoX; i <= rastersNoX; i++)
        {
            for(int j=-rastersNoY;j<=rastersNoY;j++)
            {
                variance+= pow(get(x+i*rasterX, y+j*rasterY)-defaultHeight,2.0);
            }
        }
        return variance/((rastersNoX*2)+(rastersNoY*2)+2);
    }

//    /// compute variance
//    double computeVariance(Mat34 pose, double rangeX, double rangeY) const
//    {
//        double defaultHeight = get(pose(0,3),pose(1,3));
//        double variance = 0;
//        int rastersNoX = rangeX/rasterX; int rastersNoY = rangeY/rasterY;
//        for(int i=-rastersNoX;i<=rastersNoX;i++)
//        {
//            for(int j=-rastersNoY;j<=rastersNoY;j++)
//            {
//                //std::cout << i << " " << j << "\n";
//                Mat34 motion(Mat34::Identity());
//                motion(0,3) = i*rasterX; motion(1,3) = j*rasterY;
//                Mat34 currentPose = pose*motion;
//                //std::cout << "get: " << currentPose(0,3) << ", " <<  currentPose(1,3) << " : " << get(currentPose(0,3), currentPose(1,3)) << std::endl;
//                variance+= pow(get(currentPose(0,3), currentPose(1,3))-defaultHeight,2.0);
//            }
//        }
//        //std::cout << "var: " << variance <<"\n";
//        return variance/((rastersNoX*2)+(rastersNoY*2)+2);
//    }

private:
    /// 2.5D map
    std::vector< std::vector<double> > map;
    /// map size
    double sizeX, sizeY;

    /// raster size
    double rasterX, rasterY;

    /// map size
    size_t numRows, numCols;
};


#endif // _MAP_H_
