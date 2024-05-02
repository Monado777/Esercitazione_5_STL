#include "Utils.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <cmath>
using namespace std;
namespace PolygonalLibrary {
bool ImportMesh(const string& filepath,
                PolygonalMesh& mesh)
{
    ofstream outFile("Output.txt");

    if(!ImportCell0Ds(filepath + "/Cell0Ds.csv",
                       mesh))
    {
        cerr << "Cannot find Cell0Ds.csv." << endl;
        return false;
    }
    else
    {
        cout << "\tCell0D:" << endl;
        outFile << "\tCell0D:" << endl;
        for(auto it = mesh.Cell0DMarkers.begin(); it != mesh.Cell0DMarkers.end(); it++)
        {
            unsigned int marker = it->first;
            list<unsigned int> ids = it->second;

            for(const unsigned int id : ids)
            {
                cout << "Marker: " << marker << "\t ID: " << id << endl;
                outFile << "Marker: " << marker << "\t ID: " << id << endl;
            }

            cout << endl;
            outFile << endl;
        }
    }

    if(!ImportCell1Ds(filepath + "/Cell1Ds.csv",
                       mesh))
    {
        cerr << "Cannot find Cell1Ds.csv." << endl;
        return false;
    }
    else
    {
        cout << "\n" << "-------------------------" << endl;
        outFile << "\n" << "-------------------------" << endl;
        cout << "\tCell1D:" << endl;
        outFile << "\tCell1D:" << endl;
        for(auto it = mesh.Cell1DMarkers.begin(); it != mesh.Cell1DMarkers.end(); it++)
        {
            unsigned int marker = it->first;
            list<unsigned int> ids = it->second;

            for(const unsigned int id : ids)
            {
                cout << "Marker: " << marker << "\t ID:" << id << endl;
                outFile << "Marker: " << marker << "\t ID:" << id << endl;
            }

            cout << endl;
            outFile << endl;
        }
        cout << "\n" << endl;
        outFile << "\n" << endl;
        for (unsigned int i = 0; i < mesh.Cell1DNullLines.size(); i++)
        {
            cout << "ID: " << mesh.Cell1DNullLines[i] << " 's segment's origin coincides with the end point." << endl;
            outFile << "ID: " << mesh.Cell1DNullLines[i] << " 's segment's origin coincides with the end point." << endl;
        }
    }

    if(!ImportCell2Ds(filepath + "/Cell2Ds.csv",
                       mesh))
    {
        cerr << "Cannot find Cell2Ds.csv." << endl;
        return false;
    }
    else
    {
        for(unsigned int c = 0; c < mesh.NumberCell2D; c++)
        {
            vector<unsigned int> edges = mesh.Cell2DEdges[c];

            for(unsigned int e = 0; e < 3; e++)
            {
                const unsigned int origin = mesh.Cell1DVertices[edges[e]][0];
                const unsigned int end = mesh.Cell1DVertices[edges[e]][1];

                auto findOrigin = find(mesh.Cell2DVertices[c].begin(), mesh.Cell2DVertices[c].end(), origin);
                if(findOrigin == mesh.Cell2DVertices[c].end())
                {
                    cerr << "Wrong mesh" << endl;
                    return 2;
                }

                auto findEnd = find(mesh.Cell2DVertices[c].begin(), mesh.Cell2DVertices[c].end(), end);
                if(findEnd == mesh.Cell2DVertices[c].end())
                {
                    cerr << "Wrong mesh" << endl;
                    return 3;
                }

            }
        }
        cout << "\n" << "-------------------------" << endl;
        outFile << "\n" << "-------------------------" << endl;
        cout << "\tCell2D:" << endl;
        outFile << "\tCell2D:" << endl;
        for(auto it = mesh.Cell2DMarkers.begin(); it != mesh.Cell2DMarkers.end(); it++)
        {
            unsigned int marker = it->first;
            list<unsigned int> ids = it->second;

            for(const unsigned int id : ids)
            {
                cout << "Marker: " << marker << "\t ID:" << id << endl;
                outFile << "Marker: " << marker << "\t ID:" << id << endl;
            }

            cout << endl;
            outFile << endl;
        }
        cout << "\n" << endl;
        outFile << "\n" << endl;
        for(auto it = mesh.Cell2DNumEdges.begin(); it != mesh.Cell2DNumEdges.end(); it++)
        {
            unsigned int marker = it->first;
            list<unsigned int> ids = it->second;

            for(const unsigned int id : ids)
            {
                cout << "Marker: " << marker << "\t Edges:" << id << endl;
                outFile << "Marker: " << marker << "\t Edges:" << id << endl;
            }

            cout << endl;
            outFile << endl;
        }
        cout << "\n" << endl;
        outFile << "\n" << endl;
        for(auto it = mesh.Cell2DNumVertices.begin(); it != mesh.Cell2DNumVertices.end(); it++)
        {
            unsigned int marker = it->first;
            list<unsigned int> ids = it->second;

            for(const unsigned int id : ids)
            {
                cout << "Marker: " << marker << "\t Vertices:" << id << endl;
                outFile << "Marker: " << marker << "\t Vertices:" << id << endl;
            }

            cout << endl;
            outFile << endl;
        }

        cout << "\n" << endl;
        outFile << "\n" << endl;
        if (mesh.Cell2DNullEdges.size() != 0)
        {
            for (unsigned int i = 0; i < mesh.Cell2DNullEdges.size(); i++)
            {
                cout << "ID: " << mesh.Cell2DNullEdges[i] << "has no edges." << endl;
                outFile << "ID: " << mesh.Cell2DNullEdges[i] << "has no edges." << endl;
            }
        }
        cout << "\n" << endl;
        outFile << "\n" << endl;
        if (mesh.Cell2DNullVertices.size() != 0)
        {
            for (unsigned int i = 0; i < mesh.Cell2DNullVertices.size(); i++)
            {
                cout << "ID: " << mesh.Cell2DNullVertices[i] << "has no vertices." << endl;
                outFile << "ID: " << mesh.Cell2DNullVertices[i] << "has no vertices." << endl;
            }
        }
        cout << "\n" << endl;
        outFile << "\n" << endl;
        if (mesh.Cell2DNullAreas.size() != 0)
        {
            for (unsigned int i = 0; i < mesh.Cell2DNullAreas.size(); i++)
            {
                cout << "ID: " << mesh.Cell2DNullAreas[i] << " is a degenerate triangle (area equal to 0)." << endl;
                outFile << "ID: " << mesh.Cell2DNullAreas[i] << " is a degenerate triangle (area equal to 0)." << endl;
            }
        }

    return true;

    }
    outFile.close();


}
// --------------------------------------------------------------------------------
bool ImportCell0Ds(const string &filename,
                   PolygonalMesh& mesh)
{
    ifstream file;
    file.open(filename);

    if(file.fail())
    {
        return false;
    }

    list<string> listLines;
    string Line;
    while (getline(file, Line))
    {
        listLines.push_back(Line);
    }

    listLines.pop_front();

    file.close();

    mesh.NumberCell0D = listLines.size();

    if (mesh.NumberCell0D == 0)
    {
        cerr << "There is no cell 0D" << endl;
        return false;
    }

    mesh.Cell0DId.reserve(mesh.NumberCell0D);
    mesh.Cell0DCoordinates.reserve(mesh.NumberCell0D);

    for (const string& Line : listLines)
    {
        istringstream converter(Line);
        char tmp;
        unsigned int ID;
        unsigned int Marker;
        Vector2d Coord;

        converter >>  ID >> tmp >> Marker >> tmp >> Coord(0) >> tmp >>Coord(1);

        mesh.Cell0DId.push_back(ID);
        mesh.Cell0DCoordinates.push_back(Coord);
        if (mesh.Cell0DMarkers.count(Marker) > 0) {
            mesh.Cell0DMarkers[Marker].push_back(ID);
        } else {
            mesh.Cell0DMarkers[Marker] = {ID};
        }
    }
    file.close();
    return true;
}
// --------------------------------------------------------------------------------
bool ImportCell1Ds(const string &filename,
                   PolygonalMesh& mesh)
{
    ifstream file;
    file.open(filename);

    if(file.fail())
    {
        return false;
    }

    list<string> listLines;
    string Line;
    while (getline(file, Line))
    {
        listLines.push_back(Line);
    }

    listLines.pop_front();

    file.close();

    mesh.NumberCell1D = listLines.size();

    if (mesh.NumberCell1D == 0)
    {
        cerr << "There is no cell 1D" << endl;
        return false;
    }

    mesh.Cell1DId.reserve(mesh.NumberCell1D);
    mesh.Cell1DVertices.reserve(mesh.NumberCell1D);

    for (const string& Line : listLines)
    {
        istringstream converter(Line);
        char tmp;
        unsigned int ID;
        unsigned int Marker;
        Vector2i Vertices;

        converter >>  ID >> tmp >> Marker >> tmp >> Vertices(0) >> tmp >> Vertices(1);
        if (Vertices(0) == Vertices(1))
        {
            mesh.Cell1DNullLines.push_back(ID);
        }
        mesh.Cell1DId.push_back(ID);
        mesh.Cell1DVertices.push_back(Vertices);
        if (mesh.Cell1DMarkers.count(Marker) > 0) {
            mesh.Cell1DMarkers[Marker].push_back(ID);
        } else {
            mesh.Cell1DMarkers[Marker] = {ID};
        }
    }


    return true;
}
// --------------------------------------------------------------------------------
bool ImportCell2Ds(const string &filename,
                   PolygonalMesh& mesh)
{
    ifstream file;
    file.open(filename);

    if(file.fail())
    {
        return false;
    }

    list<string> listLines;
    string Line;
    while (getline(file, Line))
    {
        listLines.push_back(Line);
    }

    listLines.pop_front();

    file.close();

    mesh.NumberCell2D = listLines.size();

    if (mesh.NumberCell2D == 0)
    {
        cerr << "There is no cell 2D" << endl;
        return false;
    }

    mesh.Cell2DId.reserve(mesh.NumberCell2D);
    mesh.Cell2DVertices.reserve(mesh.NumberCell2D);
    mesh.Cell2DEdges.reserve(mesh.NumberCell2D);

    for (const string& Line : listLines)
    {
        istringstream converter(Line);

        char tmp;
        unsigned int ID;
        unsigned int Marker;
        unsigned int NumVertices;
        vector<unsigned int> Vertices;
        unsigned int NumEdges;
        vector<unsigned int> Edges;

        converter >>  ID >> tmp >> Marker >> tmp >> NumVertices >> tmp;
        if (NumVertices > 0)
        {
            Vertices.resize(NumVertices);
            for(unsigned int i = 0; i < NumVertices; i++)
            {
                converter >> Vertices[i] >> tmp;
            }
        }
        else
        {
            mesh.Cell2DNullVertices.push_back(ID);
        }

        double a;
        double b;
        double c;
        double S;
        double TempArea;
        converter >> NumEdges >> tmp;
        if (NumEdges > 0)
        {
            Edges.resize(NumEdges);
            for(unsigned int i = 0; i < NumEdges - 1; i++)
            {
                converter >> Edges[i] >> tmp;
                if (NumEdges == 3 && NumVertices == 3)
                {
                    if (i == 0)
                    {
                        a = Edges[i];
                    }
                    if (i == 1)
                    {
                        b = Edges[i];
                    }
                }
            }
            converter >> Edges[NumEdges-1];
            if (NumEdges == 3 && NumVertices == 3)
            {
                c = Edges[NumEdges-1];
                S = (a+b+c)/2;
                TempArea = sqrt(S*(S-a)*(S-b)*(S-c)); // Heron theorem
            }
            if (TempArea == 0 && NumEdges ==3 && NumVertices == 3)
            {
                mesh.Cell2DNullAreas.push_back(ID);
            }
        }
        else
        {
            mesh.Cell2DNullEdges.push_back(ID);
        }


        mesh.Cell2DId.push_back(ID);
        mesh.Cell2DVertices.push_back(Vertices);
        mesh.Cell2DEdges.push_back(Edges);

        if (mesh.Cell2DMarkers.count(Marker) > 0) {
            mesh.Cell2DMarkers[Marker].push_back(ID);
        } else {
            mesh.Cell2DMarkers[Marker] = {ID};
        }

        if (mesh.Cell2DNumEdges.count(Marker) > 0) {
            mesh.Cell2DNumEdges[Marker].push_back(NumEdges);
        } else {
            mesh.Cell2DNumEdges[Marker] = {NumEdges};
        }

        if (mesh.Cell2DNumVertices.count(Marker) > 0) {
            mesh.Cell2DNumVertices[Marker].push_back(NumVertices);
        } else {
            mesh.Cell2DNumVertices[Marker] = {NumVertices};
        }
    }
    file.close();
    return true;
}

}
