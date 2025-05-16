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

/********************************************************************************************************/

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
	
		
	return true;	
}

/********************************************************************************************************/


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
		char separatore;
		
		is_line >> id >> separatore >> Origin >> separatore >> End;
		
		
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

/********************************************************************************************************/

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
		char separatore;
		
		is_line >> id >> separatore;
		
		polyhedron.Cell2DsId.push_back(id);
		
		// memorizzo i vertici e gli spigoli
		vector<unsigned int> vertices(3);
		for(unsigned int i = 0; i < 3; i++)
			is_line >> vertices[i] >> separatore;
		polyhedron.Cell2DsVertices.push_back(vertices);
		
		vector<unsigned int> edges(3);
		for(unsigned int i = 0; i < 3; i++)
			is_line >> edges[i] >> separatore;
		polyhedron.Cell2DsEdges.push_back(edges);
		
	}
	
	/*
	for (size_t i = 0; i < polyhedron.NumCell2Ds; ++i) {
		cout << "ID: " << polyhedron.Cell2DsId[i] << " | Vertici: ";
		for (unsigned int v : polyhedron.Cell2DsVertices[i])
			cout << v << " ";
		cout << "| Spigoli: ";
		for (unsigned int e : polyhedron.Cell2DsEdges[i])
			cout << e << " ";
		cout << endl;
	}
	*/
	
	return true;
}


/********************************************************************************************************/

bool ExportCell0Ds(PolyhedronMesh& polyhedron, const string& OutputFile)
{
	ofstream outCell0Ds(OutputFile);
    if (outCell0Ds.fail()) {
        cerr << "Unable to open output file: " << OutputFile << endl;
        return false;
    }
	
	outCell0Ds << "Id;X;Y;Z" << endl;
	
	for (size_t i = 0; i < polyhedron.NumCell0Ds; ++i) {
        unsigned int id = polyhedron.Cell0DsId[i];
        outCell0Ds << id << ";"
            << polyhedron.Cell0DsCoordinates(0, id) << ";"
            << polyhedron.Cell0DsCoordinates(1, id) << ";"
            << polyhedron.Cell0DsCoordinates(2, id) << endl;
    }

    outCell0Ds.close();
    return true;
}

bool ExportCell1Ds(PolyhedronMesh& polyhedron, const string& OutputFile)
{
	ofstream outCell1Ds(OutputFile);
    if (outCell1Ds.fail()) {
        cerr << "Unable to open output file: " << OutputFile << endl;
        return false;
    }
	
	outCell1Ds << "Id;Origin;End" << endl;
	
	for (size_t i = 0; i < polyhedron.NumCell1Ds; ++i) {
        unsigned int id = polyhedron.Cell1DsId[i];
        outCell1Ds << id << ";"
			<< polyhedron.Cell1DsExtrema(0, id) << ";"
			<< polyhedron.Cell1DsExtrema(1, id) << endl;
    }

    outCell1Ds.close();
    return true;
}

/********************************************************************************************************/

bool ExportCell2Ds(PolyhedronMesh& polyhedron, const string& OutputFile, const int& p)
{
	ofstream outCell2Ds(OutputFile);
    if (outCell2Ds.fail()) {
        cerr << "Unable to open output file: " << OutputFile << endl;
        return false;
    }	
	
	outCell2Ds << "Id;NumVertex;Vertex;NumEdges;Edge" << endl;
	
	for (size_t i = 0; i < polyhedron.NumCell2Ds; ++i) {
        
		unsigned int id = polyhedron.Cell2DsId[i];
		outCell2Ds << id << ";" << p << ";";
		
		for (unsigned int j = 0; j < polyhedron.Cell2DsVertices[i].size(); j++) {
			outCell2Ds << polyhedron.Cell2DsVertices[i][j] << ";" ;
		}
		
		outCell2Ds << p << ";" ;
		for (unsigned int j = 0; j < polyhedron.Cell2DsEdges[i].size(); j++) {
			outCell2Ds << polyhedron.Cell2DsEdges[i][j] << ";" ;
		}
	outCell2Ds << endl;	
	}	
		
	outCell2Ds.close();
	return true;
}


/********************************************************************************************************/

bool ExportCell3Ds(PolyhedronMesh& polyhedron, const string& OutputFile)
{
	ofstream outCell3Ds(OutputFile);
    if (outCell3Ds.fail()) {
        cerr << "Unable to open output file: " << OutputFile << endl;
        return false;
    }	
	
	outCell3Ds << "Id;NumVertex;NumEdges;NumFaces;Vertex;Edge;Faces" << endl;
	
	outCell3Ds << 0 << ";" << polyhedron.NumCell0Ds << ";" ;
	for (unsigned int i = 0; i < polyhedron.NumCell0Ds; ++i) {
		outCell3Ds << i << ";" ;
	}
		
	 outCell3Ds << polyhedron.NumCell1Ds << ";" ;
	 for (unsigned int i = 0; i < polyhedron.NumCell1Ds; ++i) {
		outCell3Ds << i <<  ";" ;
	 }
	 
	 outCell3Ds << polyhedron.NumCell2Ds << ";";
	 for (unsigned int i = 0; i < polyhedron.NumCell2Ds; ++i) {
		outCell3Ds << i <<  ";" ;
	}
	
	return true;
}

/***********************************************************************************************************/

