#include <sstream>
#include <fstream>
#include <unordered_set>
#include <iostream>
#include<vector>
#include<cmath>

#include "PolyhedronMesh.hpp"
#include "UCDUtilities.hpp"
#include "Utils.hpp"

using namespace std;
using namespace Gedim;


int main(int n_arginput, char *argv[])   
{
    size_t n_arg = n_arginput - 1;

    int p; int q; int b; int c;
    int id_vertex1; int id_vertex2;

    cout << "The construction of a geodetic solid" << endl;
    cout << endl;


    //ricavo input da terminale
    if(n_arg < 4){
        cerr << "Insufficient number of arguments" << endl;
        return 1;

    } else if (n_arg >=4 && n_arg < 6){
        p = stoi(argv[1]); q = stoi(argv[2]); b = stoi(argv[3]); c = stoi(argv[4]);

    } else {
        p = stoi(argv[1]); q = stoi(argv[2]); b = stoi(argv[3]); c = stoi(argv[4]); id_vertex1 = stoi(argv[5]); id_vertex2 = stoi(argv[6]);
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
        cout << "(p, q, b, c, id_vertex11, id_vertex2) = (" << p << ", " << q << ", " << b << ", " << c << ", " << id_vertex1 << ", " << id_vertex2 << ")" << endl;
    }



    //n definisce il valore in input della classe 1 
    int n = b + c;  						//numero suddivisioni

    string InputFile = "./SolidiPlatonici";
    PolyhedronMesh Platonic;


    if (p == 3) {
        switch (q)
        {
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

        PolyhedronMesh Geodetic;

        if (!ImportPolyhedronMesh(Geodetic, InputFile)) {
            cerr << "Error: creation of the platonic polyhedron mesh not done" << endl;
            return 1;
        }


        if (class_1) {
            GenerateGeodedicSolid_Class1(Geodetic, Platonic, n);
            cout << "Geodetic polyhedron generated." << endl;
        } else {
            cout << "Class II construction not yet implemented." << endl;
        }

    } else if(q == 3) {
        switch (p)
        {
        case 4:
            cout << "The platonic polyhedron corresponding to {" << p << "," << q << "} is an octahedron" << endl;
            InputFile += "/octahedron/";
            break;
        case 5:
            cout << "The platonic polyhedron corresponding to {" << p << "," << q << "} is an icosahedron" << endl;
            InputFile += "/icosahedron/";
            break;
        }

        PolyhedronMesh Geodetic;
        PolyhedronMesh DualPolyhedron;

        if (!ImportPolyhedronMesh(DualPolyhedron, InputFile)) {
            cerr << "Error: creation of the octahedron mesh not done" << endl;
            return 1;
        }

        if (class_1) {
            GenerateGeodedicSolid_Class1(DualPolyhedron, Geodetic, n);
            cout << "Geodetic solid generated from cube (dual of octahedron)." << endl;
        } else {
            cout << "Class II construction not yet implemented." << endl;
        }

        Duale(Geodetic, Platonic);  // cubo
        cout << "Cube mesh generated via dual of octahedron." << endl;

    } else {
        cerr << "Combination {" << p << "," << q << "} not supported yet." << endl;
        return 1;
    }


	UCDUtilities utilities;

    //PROPRIETA VERTICI
    vector<UCDProperty<double>> cell0Ds_properties(1);
	vector<double> markers0D(Platonic.NumCell0Ds, 0.0);
    cell0Ds_properties[0].Label = "ShortPath";
    cell0Ds_properties[0].NumComponents = 1;
    cell0Ds_properties[0].Data = markers0D.data();
 	
	//PROPRIETA LATI
    vector<UCDProperty<double>> cell1Ds_properties(1);
    vector<double> markers1D(Platonic.NumCell1Ds, 0.0);
    cell1Ds_properties[0].Label = "ShortPath";
    cell1Ds_properties[0].NumComponents = 1;
    cell1Ds_properties[0].Data = markers1D.data();
    

    //CAMMINO MINIMO
    if ( n_arg >= 6) {
        if((id_vertex1 < 0 || id_vertex2 >= Platonic.NumCell0Ds) || (id_vertex2 < 0 || id_vertex2 >= Platonic.NumCell0Ds)) {
            cerr << "Check the values of id_vertex1 and id_vertex2" << endl;
            return 1;
        } else {
            vector<int> path = Short_Path(Platonic, id_vertex1, id_vertex2);
			
			//CAMMINO MINIMO SU PARAVIEW
            for(int id_vertex : path) markers0D[id_vertex] = 1.0;				//marchio i vertici del cammino minimo

            for(size_t i=0; i < path.size() - 1; i++){
                int origin_vertex = path[i];
                int end_vertex = path[i+1];
                unordered_set<int> edge_current = {origin_vertex, end_vertex};						//non ordinato

                for(int j=0; j < Platonic.Cell1DsExtrema.cols(); j++){
                    Vector2i edge = Platonic.Cell1DsExtrema.col(j);
                    int a = edge[0]; 
                    int b = edge[1];
					//se lo spigolo contiene entrambi i vertici allora è proprio l'arco del cammino minimo -> marchio
                    if(edge_current.count(a) + edge_current.count(b) == 2) markers1D[j] = 1.0;
                }
            }   
        }
    }


    //CREAZIONE FILE OUTPUT.TXT 
    string outCell0Ds = "Cell0Ds.txt";
    ExportCell0Ds(Platonic, outCell0Ds);

    string outCell1Ds = "Cell1Ds.txt";
    ExportCell1Ds(Platonic, outCell1Ds);

    string outCell2Ds = "Cell2Ds.txt";
    ExportCell2Ds(Platonic, outCell2Ds, p);

    string outCell3Ds = "Cell3Ds.txt";
    ExportCell3Ds(Platonic, outCell3Ds);

	//CREAZIONE FILE OUTPUT.INP 
	utilities.ExportPoints("./Cell0Ds.inp",
                               Platonic.Cell0DsCoordinates);
							   
	utilities.ExportSegments("./Cell1Ds.inp",
                                 Platonic.Cell0DsCoordinates,
                                 Platonic.Cell1DsExtrema);
							   
							   
	
	
	
	
	
	
	/*
    UCDUtilities utilities;

	//ESPORTAZIONE VERTICI PARAVIEW
	vector<double> markers0D(Platonic.NumCell0Ds, 0.0); ///////////////
	for(const auto &m : Platonic.MarkerCell0Ds)
             for(const unsigned int id: m.second)
                 cell0Ds_marker.at(id) = m.first;
			 
	vector<UCDProperty<double>> cell0Ds_properties(1);////
    cell0Ds_properties[0].Label = "Marker";
    cell0Ds_properties[0].UnitLabel = "-";
    cell0Ds_properties[0].NumComponents = 1;
    cell0Ds_properties[0].Data = markers0D.data();
	
	utilities.ExportPoints("./Cell0Ds.inp", Platonic.Cell0DsCoordinates, cell0Ds_properties);
	
	//ESPORTAZIONE LATI PARAVIEW
	vector<double> markers1D(Platonic.NumCell1Ds, 0.0); ////////////////
	for(const auto &m : Platonic.MarkerCell1Ds)
             for(const unsigned int id: m.second)
                 cell1Ds_marker.at(id) = m.first;
	
    vector<UCDProperty<double>> cell1Ds_properties(1);/////////
    cell1Ds_properties[0].Label = "Marker";
    cell1Ds_properties[0].UnitLabel = "-";
    cell1Ds_properties[0].NumComponents = 1;
    cell1Ds_properties[0].Data = markers1D.data();

    utilities.ExportSegments("./Cell1Ds.inp", Platonic.Cell0DsCoordinates, Platonic.Cell1DsExtrema, points_properties, cell1Ds_properties);
	*/












	

    



































	/*PROF
    UCDUtilities utilities;
    {
		//ESPORTAZIONE PUNTI 
         vector<UCDProperty<double>> cell0Ds_properties(1);

         cell0Ds_properties[0].Label = "Short_Path";
         cell0Ds_properties[0].UnitLabel = "-"; 		
         cell0Ds_properties[0].NumComponents = 1;		
		
         vector<double> cell0Ds_marker(Platonic.NumCell0Ds, 0.0);
         for(const auto &m : Platonic.MarkerCell0Ds)
             for(const unsigned int id: m.second)
                 cell0Ds_marker.at(id) = m.first;
		
         cell0Ds_properties[0].Data = cell0Ds_marker.data();

        utilities.ExportPoints("./Cell0Ds.inp",
                               Platonic.Cell0DsCoordinates);
							   
							   
    }

    {
		//ESPORTAZIONE LATI
         vector<UCDProperty<double>> points_properties(1);

        points_properties[0].Label = "Short_Path";
        points_properties[0].UnitLabel = "-";
        points_properties[0].NumComponents = 1;

        vector<double> cell0Ds_marker(Platonic.NumCell0Ds, 0.0);
        for(const auto &m : Platonic.MarkerCell0Ds)
             for(const unsigned int id: m.second)
                 cell0Ds_marker.at(id) = m.first;

         points_properties[0].Data = cell0Ds_marker.data();
		 
		 utilities.ExportPoints("./Cell0Ds.inp", 
								Platonic.Cell0DsCoordinates, 
								cell0Ds_properties);

		
         vector<UCDProperty<double>> cell1Ds_properties(1);

         cell1Ds_properties[0].Label = "Marker";
         cell1Ds_properties[0].UnitLabel = "-";
         cell1Ds_properties[0].NumComponents = 1;

         vector<double> cell1Ds_marker(Platonic.NumCell1Ds, 0.0);
         for(const auto &m : Platonic.MarkerCell1Ds)
             for(const unsigned int id: m.second)
                 cell1Ds_marker.at(id) = m.first;
		
         cell1Ds_properties[0].Data = cell1Ds_marker.data();

        utilities.ExportSegments("./Cell1Ds.inp",
                                 Platonic.Cell0DsCoordinates,
                                 Platonic.Cell1DsExtrema);
    }
	*/
	

    return 0;
}
