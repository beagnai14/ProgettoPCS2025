#pragma once

#include <iostream>
#include"Eigen/Eigen"

using namespace std;
using namespace Eigen;

struct PolyhedronMesh
{
	// Cell0D
	unsigned int NumCell0Ds = 0;				// numero di vertici
	vector<unsigned int> Cell0DsId = {};		// Id dei vertici
	MatrixXd Cell0DsCoordinates = {};			// coordinate nello spazio dei vertici
	
	// Cell1D
	unsigned int NumCell1Ds = 0;				// numero di lati
	vector<unsigned int> Cell1DsId = {};		// Id dei lati
	MatrixXi Cell1DsExtrema = {};
	
	// Cell2D
	unsigned int NumCell2Ds = 0;				// numero di facce
	vector<unsigned int> Cell2DsId = {};		// Id delle facce
	vector<unsigned int> Cell2DsNumVertices = {};	// numero di vertici di una faccia
	vector<unsigned int> Cell2DsNumEdges = {};		// numero di spigoli di una faccia
	vector<vector<unsigned int>>  Cell2DsVertices= {};	// id dei vertici che compongono la faccia
	vector<vector<unsigned int>> Cell2DsEdges = {};		// id degli spigoli che compongono la faccia		
};