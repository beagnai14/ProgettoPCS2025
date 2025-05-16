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