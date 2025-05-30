#pragma once
#include <sstream>
#include "PolyhedronMesh.hpp"

using namespace std;


bool ImportPolyhedronMesh(PolyhedronMesh& polyhedron, 
						const string& InputFile);

bool ImportCell0Ds(PolyhedronMesh& polyhedron, 
					const string& InputFile);

bool ImportCell1Ds(PolyhedronMesh& polyhedron, 
					const string& InputFile);

bool ImportCell2Ds(PolyhedronMesh& polyhedron, 
					const string& InputFile);

bool ExportCell0Ds(PolyhedronMesh& polyhedron, const string& OutputFile);

bool ExportCell1Ds(PolyhedronMesh& polyhedron, const string& OutputFile);

bool ExportCell2Ds(PolyhedronMesh& polyhedron, const string& OutputFile, const int& p);

bool ExportCell3Ds(PolyhedronMesh& polyhedron, const string& OutputFile);

bool GenerateGeodedicSolid_Class1(const PolyhedronMesh& Platonic, PolyhedronMesh& Geodetic, const int& n);
	
bool Check_Duplicates_Vertex(const MatrixXd& matrice, const Vector3d& vector, int& edge_check_id, int& duplicate_id);

bool CheckDuplicates_Edge(const MatrixXi& matrice, const int& v1, const int& v2, int& edge_check_id, int& duplicate_id);

bool ProjectonPointToSphere(MatrixXd& vertices);

bool Duale(PolyhedronMesh& InitialPolyhedron, PolyhedronMesh& DualPolyhedron);

vector<int> OrderFaceAroundVertex(const MatrixXd& Coordinates, const vector<int>& unordered_faces);

vector<int> Short_Path( const PolyhedronMesh& Platonic, const int& id_1, const int& id_2);
