
#include <iostream>
#include <fstream>
#include <sstream>

#include "Eigen/Eigen"


#include"utils.hpp"

using namespace Eigen;
using namespace std;

bool ImportPolyhedronMesh(PolyhedronMesh& polyhedron, const string& InputFile){
	
	if(!ImportCell0Ds(polyhedron,InputFile+"Cell0Ds.csv")) { 
		cerr << "File Cell0Ds.csv not found" << endl;
		return false; }
	
	if(!ImportCell1Ds(polyhedron,InputFile+"Cell1Ds.csv")) {
		cerr << "File Cell1Ds.csv not found" << endl;
		return false; }
	
	if(!ImportCell2Ds(polyhedron,InputFile+"Cell2Ds.csv")) {
		cerr << "File Cell2Ds.csv not found" << endl;
		return false; }
	
	return true;
	
}

bool ImportCell0Ds(PolyhedronMesh& polyhedron, const string& InputFile)
{
	ifstream file(InputFile);
	if(file.fail()){
		cerr << "Unable to open Cell0Ds.csv file" << endl;
		return false;
	}
	
	// Importo in una lista tutte le righe del file 
	list<string> listLines;
	string line;
	while(getline(file, line))
		listLines.push_back(line);
	file.close();
	
	// Remove header
	listLines.pop_front();
	
	polyhedron.NumCell0Ds = listLines.size();
	if(polyhedron.NumCell0Ds == 0){
		cerr << "There are no Cells 0D" << endl;
		return false;
	}
	
	polyhedron.Cell0DsId.reserve(polyhedron.NumCell0Ds);
	polyhedron.Cell0DsCoordinates = MatrixXd::Zero(3,polyhedron.NumCell0Ds);
	
	for (const string& line : listLines){
		
		istringstream is_line(line);
		unsigned int id;
		char separatore; //';' 
		
		is_line >> id >> separatore >> polyhedron.Cell0DsCoordinates(0,id) >> separatore >> polyhedron.Cell0DsCoordinates(1,id) >> separatore >> polyhedron.Cell0DsCoordinates(2,id);
		
		polyhedron.Cell0DsId.push_back(id);
		
		}
		
	cout << "=== Vertici (Cell0Ds) ===" << endl;
	for (unsigned int i = 0; i < polyhedron.NumCell0Ds; ++i) {
		unsigned int id = polyhedron.Cell0DsId[i];
		double x = polyhedron.Cell0DsCoordinates(0, id);
		double y = polyhedron.Cell0DsCoordinates(1, id);
		double z = polyhedron.Cell0DsCoordinates(2, id);
		cout << "ID " << id << ": (" << x << ", " << y << ", " << z << ")" << endl;
	}
		
		
	return true;
		
	
}

/************************************/

bool ImportCell1Ds(PolyhedronMesh& polyhedron, const string& InputFile)
{
	ifstream file(InputFile);
	
	if(file.fail()){
		cerr << "unable to open Cell1Ds.csv file" << endl;
		return false;
	}
	
	// importo in una lista tutte le righe del file
	list<string> listLines;
	string line;
	while(getline(file,line))
		listLines.push_back(line);
	
	file.close();
	
	// Remove header
	listLines.pop_front();
	
	polyhedron.NumCell1Ds = listLines.size();
	
	if (polyhedron.NumCell1Ds == 0){
		cerr << "There is no cell 1D" << endl;
		return false;
	}
	
	// salvo le informazioni nelle righe in polyhedron	
	polyhedron.Cell1DsId.reserve(polyhedron.NumCell1Ds);
	polyhedron.Cell1DsExtrema = MatrixXi::Zero(2, polyhedron.NumCell1Ds);
	
	for (const string& line : listLines){	
	
		istringstream is_line(line);
		
		unsigned int id;
		unsigned int Origin;
		unsigned int End;
		
		is_line >> id >> Origin >> End;
		
		
		// TEST: verify edge zero-length
		if(Origin == End){
			cerr <<"TEST NOT PASSED: the edge "<< id <<" has length equal to zero";
			return false;
		}
		
		polyhedron.Cell1DsExtrema(0, id) = Origin;
		polyhedron.Cell1DsExtrema(1, id) = End;
		
		polyhedron.Cell1DsId.push_back(id);
	
	}
	return true;
}

/************************************/

bool ImportCell2Ds(PolyhedronMesh& polyhedron, const string& InputFile) 
{
	ifstream file(InputFile);
	
	if(file.fail()){
		cerr << "unable to open Cell2Ds.csv file" << endl;
		return false;
	}
	
	// importo in una lista tutte le righe del file
	list<string> listLines;
	string line;
	while (getline(file, line))
		listLines.push_back(line);
	
	file.close();
	
	// remove header
	listLines.pop_front();
	
	polyhedron.NumCell2Ds = listLines.size();
	
	if(polyhedron.NumCell2Ds == 0){
		cerr << "there is no cell 2D" << endl;
		return false;
	}
	
	// salvo le informazioni nelle righe in polyhedron
	polyhedron.Cell2DsId.reserve(polyhedron.NumCell2Ds);
	polyhedron.Cell2DsVertices.reserve(polyhedron.NumCell2Ds);
	polyhedron.Cell2DsEdges.reserve(polyhedron.NumCell2Ds);
	
	for (const string& line : listLines){
		
		istringstream is_line(line);
		unsigned int id;
		
		is_line >> id;
		
		polyhedron.Cell2DsId.push_back(id);
		
		// memorizzo i vertici e gli spigoli
		vector<unsigned int> vertices(3);
		for(unsigned int i = 0; i < 3; i++)
			is_line >> vertices[i];
		polyhedron.Cell2DsVertices.push_back(vertices);
		
		vector<unsigned int> edges(3);
		for(unsigned int i = 0; i < 3; i++)
			is_line >> edges[i];
		polyhedron.Cell2DsEdges.push_back(edges);
		
	}
	
	return true;
}

/*	

bool check_area_3d(PolyhedronMesh& polyhedron) {
    
    for (unsigned int i = 0; i < polyhedron.NumCell2Ds; i++) {
        double area = 0.0;
        unsigned int n = mesh.Cell2DsVertices[i].size();
        if (n < 3) {
            cout << "Error: Polygon with ID " << i << " has less than 3 vertices and cannot form a polygon." << endl;
            return false;
        }

        for (unsigned int j = 0; j < n; j++) {
            // Get the IDs of the points which form the i-th polygon.
            unsigned int& P1_id = polyhedron.Cell2DsVertices[i][j];
            unsigned int& P2_id = polyhedron.Cell2DsVertices[i][(j + 1) % n]; // Wrap around to the first vertex
            unsigned int& P3_id = polyhedron.Cell2DsVertices[i][(j + 2) % n]; // Need a third point for triangle

            // Get the 3D coordinates of the vertices.
            double X_P1 = polyhedron.Cell0DsCoordinates(0, P1_id);
            double Y_P1 = polyhedron.Cell0DsCoordinates(1, P1_id);
            double Z_P1 = polyhedron.Cell0DsCoordinates(2, P1_id);

            double X_P2 = polyhedron.Cell0DsCoordinates(0, P2_id);
            double Y_P2 = polyhedron.Cell0DsCoordinates(1, P2_id);
            double Z_P2 = polyhedron.Cell0DsCoordinates(2, P2_id);

            double X_P3 = polyhedron.Cell0DsCoordinates(0, P3_id);
            double Y_P3 = polyhedron.Cell0DsCoordinates(1, P3_id);
            double Z_P3 = polyhedron.Cell0DsCoordinates(2, P3_id);

            // Calculate vectors for two sides of the triangle
            double U_x = X_P2 - X_P1;
            double U_y = Y_P2 - Y_P1;
            double U_z = Z_P2 - Z_P1;

            double V_x = X_P3 - X_P1;
            double V_y = Y_P3 - Y_P1;
            double V_z = Z_P3 - Z_P1;

            // Calculate the cross product
            double cross_x = U_y * V_z - U_z * V_y;
            double cross_y = U_z * V_x - U_x * V_z;
            double cross_z = U_x * V_y - U_y * V_x;

            // Calculate the magnitude of the cross product (area of the parallelogram)
            double triangle_area = 0.5 * sqrt(cross_x * cross_x + cross_y * cross_y + cross_z * cross_z);
            area += triangle_area;
        }

        // Tolerance check for the area of the polygon 
        if (area < 1e-16) {
            cout << "There is an error at polygon with ID " << i << ": it has zero area" << endl;
            return false;
        }
    }
    cout << "There are no zero area polygons" << endl;
    return true;
}
	
*/


/***************************************/

