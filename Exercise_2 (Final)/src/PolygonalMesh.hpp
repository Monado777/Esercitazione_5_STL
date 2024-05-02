#pragma once

#include <iostream>
#include "Eigen/Eigen"

using namespace std;
using namespace Eigen;


namespace PolygonalLibrary {

struct PolygonalMesh
{
    unsigned int NumberCell0D = 0;
    std::vector<unsigned int> Cell0DId = {};
    std::vector<Vector2d> Cell0DCoordinates = {};
    std::map<unsigned int, list<unsigned int>> Cell0DMarkers = {};

    unsigned int NumberCell1D = 0;
    std::vector<unsigned int> Cell1DId = {};
    std::vector<Vector2i> Cell1DVertices = {};
    std::map<unsigned int, list<unsigned int>> Cell1DMarkers = {};
    std::vector<unsigned int> Cell1DNullLines = {};


    unsigned int NumberCell2D = 0;
    std::vector<unsigned int> Cell2DId = {};
    std::vector<vector<unsigned int>> Cell2DVertices = {};
    std::vector<vector<unsigned int>> Cell2DEdges = {};
    std::vector<unsigned int> Cell2DNullEdges = {};
    std::vector<unsigned int> Cell2DNullVertices = {};
    std::vector<unsigned int> Cell2DNullAreas = {};

    std::map<unsigned int, list<unsigned int>> Cell2DMarkers = {};
    std::map<unsigned int, list<unsigned int>> Cell2DNumVertices = {};
    std::map<unsigned int, list<unsigned int>> Cell2DNumEdges = {};
};

}
