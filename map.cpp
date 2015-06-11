#include "map.h"
#include <memory>
#include <fstream>
#include <sstream>

//using namespace planner;

bool Map::load(std::string filename){
    std::ifstream file(filename);
    if (file.is_open())
    { // open file
        std::string line;
        int lineNo = 0; int rowNo = 0;
        while (std::getline (file, line))
        { // load each line
            std::istringstream is(line);
            std::cout << line << "\n";
            if (line[0] != '#')
            {
                if (lineNo == 1)
                {
                    is >> numCols >> numRows >> rasterX >> rasterY;
                    sizeX = rasterX*numCols; sizeY = rasterY*numRows;
                    std::cout << "map params: " << numCols << " " << numRows << " " << sizeX << " " << sizeY << "\n";
                    map.clear();
                    map.resize(numRows);
                }
                else
                {
                    map[numRows-rowNo-1].resize(numCols);
                    for (int i=0; i < numCols; i++)
                        is >> map[numRows-rowNo-1][i];
                    rowNo++;
                }
            }
            lineNo++;
        }
        file.close();
        return true;
    }
    else
    {
        return false;
    }
}

/// Calculate normal vector (input: vertices of the triangle, output: normal vector)
Vec3 Map::calculateNormal(std::vector<Vec3>& vertices)
{
    Vec3 v1(vertices[0].x - vertices[1].x, vertices[0].y - vertices[1].y, vertices[0].z - vertices[1].z);
    Vec3 v2(vertices[1].x - vertices[2].x, vertices[1].y - vertices[2].y, vertices[1].z - vertices[2].z);

    Vec3 out(v1.y*v2.z - v1.z*v2.y, v1.z*v2.x - v1.x*v2.z, v1.x*v2.y - v1.y*v2.x);
    double module = sqrt(pow(out.x,2.0) + pow(out.y,2.0) + pow(out.z,2.0));
    out.x /= module; out.y /= module; out.z /= module;
}

/// export map to file
void Map::exportMap(const std::string filename) const
{
    std::ofstream file(filename);
    double div = 1.0;
    //file << "[X,Y]=meshgrid([" << -sizeX/2.0 << ":" << sizeX/(numCols) << ":" << sizeX/2.0 - sizeX/(numCols)<< "],[" << -sizeY/2.0 << ":" << sizeY/(numRows) << ":" << sizeY/2.0 - sizeY/(numRows) << "]);\n";
    file << "[X,Y]=meshgrid([" << -sizeX/2.0 << ":" << sizeX/(div*numCols) << ":" << sizeX/2.0 - sizeX/(div*numCols)<< "],[" << -sizeY/2.0 << ":" << sizeY/(div*numRows) << ":" << sizeY/2.0 - sizeY/(div*numRows) << "]);\n";
    file << "Z=[";
    /*for (auto itRow = map.begin(); itRow!=map.end(); itRow++){
        for (auto itCol = itRow->begin(); itCol!=itRow->end(); itCol++){
            file << *itCol << ", ";
        }
        file << ";\n";
    }*/
    for (double x=-sizeX/2.0; x<sizeX/2.0 - sizeX/(div*numCols); x=x+sizeX/(div*numCols))
    {
        for (double y=-sizeY/2.0; y<sizeX/2.0 - sizeX/(div*numCols); y=y+sizeY/(div*numRows))
        {
            file << get(y,x) << ", ";
        }
        file << ";\n";
    }

    file << "];\n surf(X,Y,Z); colormap('autumn');\n xlabel('x'); ylabel('y'); zlabel('z');\n";
    file.close();
}

/// compute spherical variance
//float_type Map::computeVariance(float_type x, float_type y, float_type range){

//}
