#include <sstream>
#include <fstream>

#include <iostream>
#include<vector>
#include<cmath>

#include "PolyhedronMesh.hpp"
#include "utils.hpp"

using namespace std;


int main(int n_arginput, char *argv[])   
{
	size_t n_arg = n_arginput - 1;
	
	int p; int q; int b; int c;
	int id_vertice1; int id_vertice2;
	
	cout << "The construction of a geodetic solid" << endl;
	cout << endl;
	
    if(n_arg < 4){
       cerr << "Insufficient number of arguments" << endl;
	   return 1;
	   
	} else if (n_arg >=4 && n_arg < 6){
		p = stoi(argv[1]); q = stoi(argv[2]); b = stoi(argv[3]); c = stoi(argv[4]);
		
	} else {
		p = stoi(argv[1]); q = stoi(argv[2]); b = stoi(argv[3]); c = stoi(argv[4]); id_vertice1 = stoi(argv[5]); id_vertice2 = stoi(argv[6]);
	}
	
	
	//controllo p e q
	if (p != 3 && p != 4 && p != 5) { 
		cerr << "Error: p must be in range 3,4,5" << endl; 
		return 1; }	
		
	if (q != 3 && q != 4 && q != 5) {
		cerr << "Error: q must be in range 3,4,5" << endl; 
		return 1; }	
		
	// Controllo su b e c -> Classe*
	bool class_1 = false;
	bool class_2 = false;
	
	if (b == 0 && c == 0) {
		cerr << "Error : b and c must be non zero integer" << endl;
		return 1;
	}

	if (b == c) {
		class_2 = true;
		cout << "Class II" << endl;
		
	} else if (b == 0 || c == 0) {
		class_1 = true;
		cout << "Class I" << endl;
		
	} else {
		cerr << "Error : b != c is not considered" << endl;
		return 1;
	}	
		
	
	//stampa 
	if (n_arg >=4 && n_arg < 6) {
		cout << "(p, q, b, c) = (" << p << ", " << q << ", " << b << ", " << c << ")" << endl;
	
	} else if (n_arg >=7) {
		cout << "(p, q, b, c, id_vertice1, id_vertice2) = (" << p << ", " << q << ", " << b << ", " << c << ", " << id_vertice1 << ", " << id_vertice2 << ")" << endl;
	}
		
	
	
	string InputFile = "../SolidiPlatonici";
	PolyhedronMesh polyhedron;
	if ( p == 3) { 
		switch(q) {
		case 3:
			// tetraedro
			cout << "The platonic polyhedron corresponding to {" << p << "," << q << "} is a tetrahedron" << endl;
			InputFile += "/tetrahedron/";
			break;
		case 4:
			// ottaedro
			cout << "The platonic polyhedron corresponding to {" << p << "," << q << "} is a octahedron" << endl;
			InputFile += "/octahedron/";
			break;
		case 5:
			// icosaedro
			cout << "The platonic polyhedron corresponding to {" << p << "," << q << "} is a icosahedron" << endl;
			InputFile += "/icosahedron/";
			break;
			}
	}	
	
	if ( q == 3) {
		switch(p) {
		case 4:
			//cubo 
			cout << "The platonic polyhedron corresponding to {" << p << "," << q << "} is a cube" << endl;
			break;
		case 5:
			//dodecaedro
			cout << "The platonic polyhedron corresponding to {" << p << "," << q << "} is a dodecahedron" << endl;
			break;
		}
	}
	
	
	if(!ImportPolyhedronMesh(polyhedron, InputFile)){
		cerr << "Something went wrong during the creation of the platonic polyhedron mesh" << endl;
		return 1;
	}
	
	
	//CREAZIONE FILE OUTPUT.TXT
	string outCell0Ds = "Cell0Ds.txt";
	ExportCell0Ds(polyhedron, outCell0Ds);
	
	string outCell1Ds = "Cell1Ds.txt";
	ExportCell1Ds(polyhedron, outCell1Ds);
	
	string outCell2Ds = "Cell2Ds.txt";
	ExportCell2Ds(polyhedron, outCell2Ds, p);
	
	string outCell3Ds = "Cell3Ds.txt";
	ExportCell3Ds(polyhedron, outCell3Ds);
	
	
	
	
	
	
	return 0;
}
