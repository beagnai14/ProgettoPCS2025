#pragma once

#include <iostream>
#include"Eigen/Eigen"

using namespace std;
using namespace Eigen;

namespace PolyhedronLibrary{
	struct PolyhedronMesh
	{
		// Cell0D
		int NumCell0Ds = 0;										// numero vertici
		vector<int> Cell0DsId = {};								// Id vertici
		MatrixXd Cell0DsCoordinates = {};						// coordinate vertici
		
		// Cell1D
		int NumCell1Ds = 0;										// numero lati
		vector<int> Cell1DsId = {};								// Id lati
		MatrixXi Cell1DsExtrema = {};							// vertici estremi (per ciascun lato)
		
		// Cell2D
		int NumCell2Ds = 0;										// numero facce
		vector<int> Cell2DsId = {};								// Id facce
		vector<int> Cell2DsNumVertices = {};					// numero vertici (per ciascuna faccia)
		vector<int> Cell2DsNumEdges = {};						// numero spigoli (per ciascuna faccia)
		vector<vector<int>>  Cell2DsVertices = {};				// id vertici (per ciascuna faccia)
		vector<vector<int>> Cell2DsEdges = {};					// id spigoli (per ciascuna faccia)
		
		//Cell3D
		int NumCell3Ds = 0;
		vector<int> Cell3DsId = {};					
		vector<int> Cell3DsNumVertices = {};		
		vector<int> Cell3DsNumEdges = {};	
		vector<int> Cell3DsNumFaces = {};
		vector<vector<int>> Cell3DsVertices = {};	
		vector<vector<int>> Cell3DsEdges = {};
		vector<vector<int>> Cell3DsFaces = {};

	};

}