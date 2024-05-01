#include "Utils.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
using namespace std;
namespace PolygonalLibrary {
bool ImportMesh(const string& filepath,
                PolygonalMesh& mesh)
{

    if(!ImportCell0Ds(filepath + "/Cell0Ds.csv",
                       mesh))
    {
        return false;
    }
    else
    {
        cout << "Cell0D:" << endl;
        for(auto it = mesh.Cell0DMarkers.begin(); it != mesh.Cell0DMarkers.end(); it++)
        {
            unsigned int marker = it->first;
            list<unsigned int> ids = it->second;

            for(const unsigned int id : ids)
                cout << "Marker: " << marker << "\t ID: " << id << endl;

            cout << endl;
        }
    }

    if(!ImportCell1Ds(filepath + "/Cell1Ds.csv",
                       mesh))
    {
        return false;
    }
    else
    {
        cout << "Cell1D marker:" << endl;
        for(auto it = mesh.Cell1DMarkers.begin(); it != mesh.Cell1DMarkers.end(); it++)
        {
            cout << "key:\t" << it -> first << "\t values:";
            for(const unsigned int id : it -> second)
                cout << "\t" << id;

            cout << endl;
        }
    }

    if(!ImportCell2Ds(filepath + "/Cell2Ds.csv",
                       mesh))
    {
        return false;
    }
    else
    {
        // Test:
        for(unsigned int c = 0; c < mesh.NumberCell2D; c++)
        {
            array<unsigned int, 3> edges = mesh.Cell2DEdges[c];

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
    }

    return true;

}
// --------------------------------------------------------------------------------
bool ImportCell0Ds(const string &filename,
                   PolygonalMesh& mesh)
{
    string TempLine;
    string TokenString;
    ifstream file;
    file.open(filename);

    if(file.fail())
        return false;

    list<string> listLines;
    string Line;
    getline(file, Line);
    Line = "";
    while (getline(file, Line))
    {
        stringstream TempS(Line);
        string Token;
        vector<double> TokenVec;
        char Delimiter = ';';

        while (getline(TempS, Token, Delimiter))
        {
            TokenVec.push_back(stod(Token));
        }
        for (unsigned int i = 0; i <= TokenVec.size(); i++)
        {
            TokenString = to_string(TokenVec[i]);
            // TempLine = TempLine + TokenString;
            if (i == 0)
            {
                TempLine = TokenString;
            }
            else
            {
                TempLine = TempLine + " " + TokenString;
            }
        }
        Line = TempLine;
        TempLine = "";
        listLines.push_back(Line);
    }

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

        unsigned int ID;
        unsigned int Marker;
        Vector2d Coord;

        converter >>  ID >> Marker >> Coord(0) >> Coord(1);

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
    string TempLine;
    string TokenString;
    ifstream file;
    file.open(filename);

    if(file.fail())
        return false;

    list<string> listLines;
    string Line;
    getline(file, Line);
    Line = "";
    while (getline(file, Line))
    {
        stringstream TempS(Line);
        string Token;
        vector<double> TokenVec;
        char Delimiter = ';';

        while (getline(TempS, Token, Delimiter))
        {
            TokenVec.push_back(stod(Token));
        }
        for (unsigned int i = 1; i <= TokenVec.size(); i++)
        {
            TokenString = to_string(TokenVec[i]);
            TempLine = TempLine + " " + TokenString;
        }
        Line = TempLine;
        TempLine = "";
        listLines.push_back(Line);
    }

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

        unsigned int ID;
        unsigned int Marker;
        Vector2i Vertices;

        converter >>  ID >> Marker >> Vertices(0) >> Vertices(1);

        mesh.Cell1DId.push_back(ID);
        mesh.Cell1DVertices.push_back(Vertices);
        mesh.Cell1DMarkers.insert({Marker, {ID}});
    }

    file.close();

    return true;
}
// --------------------------------------------------------------------------------
bool ImportCell2Ds(const string &filename,
                   PolygonalMesh& mesh)
{
    string TempLine;
    string TokenString;
    ifstream file;
    file.open(filename);

    if(file.fail())
        return false;

    list<string> listLines;
    string Line;
    getline(file, Line);
    Line = "";
    while (getline(file, Line))
    {
        stringstream TempS(Line);
        string Token;
        vector<double> TokenVec;
        char Delimiter = ';';

        while (getline(TempS, Token, Delimiter))
        {
            TokenVec.push_back(stod(Token));
        }
        for (unsigned int i = 1; i <= TokenVec.size(); i++)
        {
            TokenString = to_string(TokenVec[i]);
            TempLine = TempLine + " " + TokenString;
        }
        Line = TempLine;
        TempLine = "";
        listLines.push_back(Line);
    }

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

        unsigned int ID;
        unsigned int Marker;
        unsigned int NumVertices;
        array<unsigned int, 3> Vertices;
        unsigned int NumEdges;
        array<unsigned int, 3> Edges;

        converter >>  ID >> Marker >> NumVertices;
        for(unsigned int i = 0; i < 3; i++)
            converter >> Vertices[i];
        converter >> NumEdges;
        for(unsigned int i = 0; i < 3; i++)
            converter >> Edges[i];

        mesh.Cell2DId.push_back(ID);
        mesh.Cell2DVertices.push_back(Vertices);
        mesh.Cell2DEdges.push_back(Edges);
        mesh.Cell2DMarkers.insert({Marker, {ID}});
        mesh.Cell2DNumEdges.insert({Marker, {NumEdges}});
        mesh.Cell2DNumVertices.insert({Marker, {NumVertices}});
    }
    file.close();
    return true;
}

}
