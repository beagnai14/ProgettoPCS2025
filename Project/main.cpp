#include <sstream>
#include <fstream>

#include <iostream>
#include<vector>
#include<cmath>

#include "PolyhedronMesh.hpp"
#include "Utils.hpp"

using namespace std;


int main(int n_arginput, char *argv[])   
{
	size_t n_arg = n_arginput - 1;
	
	int p; int q; int b; int c;
	int id_vertice1; int id_vertice2;
	
	cout << "The construction of a geodetic solid" << endl;
	cout << endl;
	
	
	//ricavo input da terminale
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
	
	if (b <= 0 || c < 0) {
		cerr << "Error: b must be strictly positive and c must be non-negative." << endl;
		return 1;
	}

	if (b == c) {
		class_2 = true;
		cout << "Class II" << endl;
	} else if (b == 0 || c == 0) {
		class_1 = true;
		cout << "Class I" << endl;
	} else {
		cerr << "Error: check value for b and c" << endl;
		return 1;
	}	
		
	
	//stampa 
	if (n_arg >=4 && n_arg < 6) {
		cout << "(p, q, b, c) = (" << p << ", " << q << ", " << b << ", " << c << ")" << endl;
	
	} else if (n_arg >=7) {
		cout << "(p, q, b, c, id_vertice1, id_vertice2) = (" << p << ", " << q << ", " << b << ", " << c << ", " << id_vertice1 << ", " << id_vertice2 << ")" << endl;
	}
	
		
	//in seguito le formule necessarie per individuare il numero di vertici, lati e facce 
	//in base ai valori di b e c passati in input
	//TRIANGOLAZIONE
	int n = b + c;  						//numero suddivisioni

	int T = b * b + b * c + c * c;

	//int V = 0; 
	int E = 0; int F = 0;
	if (q == 3) {
		//V = 2 * T + 2;
		E = 6 * T;
		F = 4 * T;
	} else if (q == 4) {
		//V = 4 * T + 2;
		E = 12 * T;
		F = 8 * T;
	} else if (q == 5) {
		//V = 10 * T + 2;
		E = 30 * T;
		F = 20 * T;
	} else { 
		return false;    
	}
	
	
	string InputFile = "../SolidiPlatonici";
	PolyhedronMesh Platonic;
	
	
	if (p == 3) {
		switch (q) {
			case 3:
				cout << "The platonic polyhedron corresponding to {" << p << "," << q << "} is a tetrahedron" << endl;
				InputFile += "/tetrahedron/";
				break;
			case 4:
				cout << "The platonic polyhedron corresponding to {" << p << "," << q << "} is an octahedron" << endl;
				InputFile += "/octahedron/";
				break;
			case 5:
				cout << "The platonic polyhedron corresponding to {" << p << "," << q << "} is an icosahedron" << endl;
				InputFile += "/icosahedron/";
				break;
		}

		if (!ImportPolyhedronMesh(Platonic, InputFile)) {
			cerr << "Error: creation of the platonic polyhedron mesh not done" << endl;
			return 1;
		}

		PolyhedronMesh Geodetic;
		
		if (class_1) {
			GenerateGeodedicSolid_Class1(Platonic, Geodetic, n, E, F);
			cout << "Geodetic polyhedron generated." << endl;
		} else {
			cout << "Class II construction not yet implemented." << endl;
		}

	} else if (p == 4 && q == 3) {
		//nel caso q=3 devo restituire il poliedro duale: {4,3} corrisponde a {3,4}
		cout << "The platonic polyhedron corresponding to {" << p << "," << q << "} is a cube" << endl;
		InputFile += "/octahedron/";

		if (!ImportPolyhedronMesh(Platonic, InputFile)) {
			cerr << "Error: creation of the octahedron mesh not done" << endl;
			return 1;
		}
		
		PolyhedronMesh Geodetic;
		PolyhedronMesh DualPolyhedron;
		Duale(Platonic, DualPolyhedron);  // cubo
		cout << "Cube mesh generated via dual of octahedron." << endl;

		if (class_1) {
			//GenerateGeodedicSolid_Class1(DualPolyhedron, Geodetic, n, E, F);
			cout << "Geodetic solid generated from cube (dual of octahedron)." << endl;
		} else {
			cout << "Class II construction not yet implemented." << endl;
		}
	
	} else if (p == 5 && q == 3) {
		// Dodecaedro ottenuto come duale dell'icosaedro
		cout << "The platonic polyhedron {" << p << "," << q << "} (dodecahedron) will be constructed as dual of {3,5}" << endl;
		InputFile += "/icosahedron/";

		if (!ImportPolyhedronMesh(Platonic, InputFile)) {
			cerr << "Error: creation of the icosahedron mesh not done" << endl;
			return 1;
		}

		PolyhedronMesh Geodetic;
		PolyhedronMesh DualPolyhedron;
		Duale(Platonic, DualPolyhedron);  // dodecaedro
		cout << "Dodecahedron mesh generated via dual of icosahedron." << endl;

		if (class_1) {
			//GenerateGeodedicSolid_Class1(DualPolyhedron, Geodetic, n, E, F);
			cout << "Geodetic solid generated from dodecahedron (dual of icosahedron)." << endl;
		} else {
			cout << "Class II construction not yet implemented." << endl;
		} 
	
	} else {
		cerr << "Combination {" << p << "," << q << "} not supported yet." << endl;
		return 1;
	}







	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	/*
	
	if (p == 3) {
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
	
	
	if(!ImportPolyhedronMesh(Platonic, InputFile)){
		cerr << "Error: creation of the platonic polyhedron mesh not done" << endl;
		return 1;
	}
	
	
	if (q == 3) {
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
	
	PolyhedronMesh Geodetic;
	PolyhedronMesh DualPolyhedron;

	if (class_1 == true) {	
		GenerateGeodedicSolid_Class1(Platonic, Geodetic, n, E, F);
		cout << "Geodetic polyhedron generated." << endl;
		
		if (q == 3) {
			Duale(Geodetic, DualPolyhedron);
			cout << "Goldberg polyhedron (dual of geodetic solid) also generated." << endl;
		}
		
	} else if (class_2 == true) {
		cout << "costruzione classe 2" << endl;
	}
	
	*/
	
	
	
	
	//CAMMINO MINIMO 
	if ( n_arg >= 6) {
		cout << "faccio il cammino minimo" << endl;
	
		if((id_vertice1 < 0 || id_vertice2 >= Platonic.NumCell0Ds) || (id_vertice2 < 0 || id_vertice2 >= Platonic.NumCell0Ds)) {
			cerr << "Check the values of id_vertice1 and id_vertice2" << endl;
			return 1;
		} else {
			Short_Path(Platonic, id_vertice1, id_vertice2);
		//AGGIUNGERE IL CAMMINO MINIMO SU PARAVIEW
		}
	}
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	

	return 0;
}
