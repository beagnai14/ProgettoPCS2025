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
    //cout << "number of arg passed: " << (n_arginput - 1) << endl; 
	size_t n_arg = n_arginput - 1;
	
	int p_value;
	int q_value;
	int b;
	int c;
	int id_vertice1;
	int id_vertice2;
	
	cout << "The construction of a geodetic solid" << endl;
	cout << endl;
	
    if(n_arg < 4){
       cerr << "Insufficient number of arguments" << endl;
	   return 1;
	} else if (n_arg >=4 && n_arg < 6){
		
		p_value = stoi(argv[1]);
		q_value = stoi(argv[2]);
		b = stoi(argv[3]);		
		c = stoi(argv[4]);
		
		if (p_value != 3 && p_value != 4 && p_value != 5) { 
			cerr << "Error: p must be in range 3,4,5" << endl;
		}	
		
		if (q_value != 3 && q_value != 4 && q_value != 5) {
			cerr << "Error: q must be in range 3,4,5" << endl; 
		}		
		
		cout << "(p, q, b, c) = (" << p_value << ", " << q_value << ", " << b << ", " << c << ")" << endl;
			
	} else {
		
		p_value = stoi(argv[1]);
		q_value = stoi(argv[2]);
		b = stoi(argv[3]);		//convertire da stringa input a int
		c = stoi(argv[4]);
		id_vertice1 = stoi(argv[5]);
		id_vertice2 = stoi(argv[6]); 
		
		if (p_value != 3 && p_value != 4 && p_value != 5) { 
			cerr << "Error: p must be in range 3,4,5" << endl;
		return 1; 
		}	
		
		if (q_value != 3 && q_value != 4 && q_value != 5) {
			cerr << "Error: q must be in range 3,4,5" << endl;
		return 1; 
		}	
				
		cout << "(p, q, b, c, id_vertice1, id_vertice2) = (" << p_value << ", " << q_value << ", " << b << ", " << c << ", " << id_vertice1 << ", " << id_vertice2 << ")" << endl;
		}
	

	// Controllo su b e c -> Classe*
	bool class_1 = false;
	bool class_2 = false;
	if (b == 0 && c == 0) {
		cerr << "Erreur : b et c ne peuvent pas être tous les deux nuls." << endl;
		return -1;
	}

	if (b == c) {
		class_2 = true;
		cout << "Class II" << endl;
	} else if (b == 0 || c == 0) {
		class_1 = true;
		cout << "Class I" << endl;
	} else {
		cerr << "Erreur : combinaison b ≠ c non prise en charge.\n";
		return -1;
	}


	// chiamata alla costruzione del solido platonico di partenza
	string InputFile = "../SolidiPlatonici";
	PolyhedronMesh PlatonicPolyhedron;
	switch(q_value) {
		case 3:
			// tetraedro
			cout << "The platonic polyhedron corresponding to the Schlafli symbol {3, " << q_value << "} is a tetrahedron" << endl;
			InputFile += "/tetrahedron/";
			break;
		case 4:
			// ottaedro
			cout << "The platonic polyhedron corresponding to the Schlafli symbol {3, " << q_value << "} is a octahedron" << endl;
			InputFile += "/octahedron/";
			break;
		case 5:
			// icosaedro
			cout << "The platonic polyhedron corresponding to the Schlafli symbol {3, " << q_value << "} is a icosahedron" << endl;
			InputFile += "/icosahedron/";
			break;
	}
	
	if(!ImportPolyhedronMesh(PlatonicPolyhedron, InputFile)){
		cerr << "Something went wrong during the creation of the platonic polyhedron mesh" << endl;
		return 1;
	}
	
	PolyhedronMesh GeodeticSolid;
	
	return 0;
}
