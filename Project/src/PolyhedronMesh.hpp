#pragma once

#include <iostream>
#include"Eigen/Eigen"

using namespace std;
using namespace Eigen;

struct PolyhedronMesh
{
	// Cell0D
	unsigned int NumCell0Ds = 0;							// numero vertici
	vector<unsigned int> Cell0DsId = {};					// Id vertici
	MatrixXd Cell0DsCoordinates = {};						// coordinate vertici
	
	// Cell1D
	unsigned int NumCell1Ds = 0;							// numero lati
	vector<unsigned int> Cell1DsId = {};					// Id lati
	MatrixXi Cell1DsExtrema = {};							// vertici estremi (per ciascun lato)
	
	// Cell2D
	unsigned int NumCell2Ds = 0;							// numero facce
	vector<unsigned int> Cell2DsId = {};					// Id facce
	vector<unsigned int> Cell2DsNumVertices = {};			// numero vertici (per ciascuna faccia)
	vector<unsigned int> Cell2DsNumEdges = {};				// numero spigoli (per ciascuna faccia)
	vector<vector<unsigned int>>  Cell2DsVertices= {};		// id vertici (per ciascuna faccia)
	vector<vector<unsigned int>> Cell2DsEdges = {};			// id spigoli (per ciascuna faccia)