#pragma once
#include <sstream>
#include "PolyhedronMesh.hpp"

using namespace std;

namespace PolyhedronLibrary {
	
	namespace ImportFile {
	
		// Importa il solido platonico di partenza.
		// polyhedron: una struct di tipo PolyhedronMesh.
		// InputFile: nome del file .csv in cui sono contenuti i dati del solido corrispondente.
		// risultato: true se il file è importatato correttamente, false altrimenti.
		bool ImportPolyhedronMesh(PolyhedronMesh& polyhedron, 
									const string& InputFile);


		// Importa la Base Dati delle Cell0Ds dal file Cell0Ds.csv.
		// polyhedron: una struct di tipo PolyhedronMesh.
		// InputFile: nome del file .csv in cui sono contenuti i dati del solido corrispondente.
		// risultato: true se il file è importatato correttamente, false altrimenti.
		bool ImportCell0Ds(PolyhedronMesh& polyhedron, 
							const string& InputFile);


		// Importa la Base Dati delle Cell1Ds dal file Cell1Ds.csv.
		// polyhedron: una struct di tipo PolyhedronMesh.
		// InputFile: nome del file .csv in cui sono contenuti i dati del solido corrispondente.
		// risultato: true se il file è importatato correttamente, false altrimenti.
		bool ImportCell1Ds(PolyhedronMesh& polyhedron, 
							const string& InputFile);


		// Importa la Base Dati delle Cell2Ds dal file Cell2Ds.csv.
		// polyhedron: una struct di tipo PolyhedronMesh.
		// InputFile: nome del file .csv in cui sono contenuti i dati del solido corrispondente.
		// risultato: true se il file è importatato correttamente, false altrimenti.
		bool ImportCell2Ds(PolyhedronMesh& polyhedron, 
							const string& InputFile);
	}
	
	
	namespace ExportFile {
		
		// Esporta la Base Dati delle Cell0Ds dal file Cell0Ds.txt.
		// polyhedron: una struct di tipo PolyhedronMesh.
		// OutputFile: nome del file .txt in cui sono stampati i dati del solido corrispondente.
		// risultato: true se il file è esportato correttamente, false altrimenti.
		bool ExportCell0Ds(PolyhedronMesh& polyhedron, 
								const string& OutputFile);
		
		
		// Esporta la Base Dati delle Cell1Ds dal file Cell1Ds.txt.
		// polyhedron: una struct di tipo PolyhedronMesh.
		// OutputFile: nome del file .txt in cui sono stampati i dati del solido corrispondente.
		// risultato: true se il file è esportato correttamente, false altrimenti.
		bool ExportCell1Ds(PolyhedronMesh& polyhedron, 
							const string& OutputFile);
	
	
		// Esporta la Base Dati delle Cell2Ds dal file Cell2Ds.txt.
		// polyhedron: una struct di tipo PolyhedronMesh.
		// OutputFile: nome del file .txt in cui sono stampati i dati del solido corrispondente.
		// risultato: true se il file è esportato correttamente, false altrimenti.
		bool ExportCell2Ds(PolyhedronMesh& polyhedron,
							const string& OutputFile, 
							const int& p);
			
			
		// Esporta la Base Dati delle Cell3Ds dal file Cell3Ds.txt.
		// polyhedron: una struct di tipo PolyhedronMesh.
		// OutputFile: nome del file .txt in cui sono stampati i dati del solido corrispondente.
		// risultato: true se il file è esportato correttamente, false altrimenti.
		bool ExportCell3Ds(PolyhedronMesh& polyhedron, 
							const string& OutputFile);
	}
	
	namespace GeneratePlatonic {
		
		// Generazione del poliedro platonico di classe 1.
		// Platonic: una struct del tipo PolyhedronMesh contiene la Base Dati del solido.
		// Geodetic: una struct del tipo PolyhedronMesh che riporta le proprietà del poliedro Geodedico.
		// n: coefficiente di triangolazione in base ai valori di b e c.
		bool Solid_Class1(const PolyhedronMesh& Platonic, 
							PolyhedronMesh& Geodetic, 
							const int& n);
				
			
		// Costruzione del poliedro duale a partire dal solido di partenza.
		bool Duale(const PolyhedronMesh& InitialPolyhedron, 
									PolyhedronMesh& DualPolyhedron);
	}
	
	
	// Controllo dei vertici duplicati all'interno della matrice delle cellODsCoordinates.
	// matrice: matrcie 3xN delle coordinate 3D dei vertici.
	// edge_check_id: id corrente che viene verificato.
	// duplicate_id: id aggiunto nel caso le coordinate siano duplicate.
	bool Check_Duplicates_Vertex(const MatrixXd& matrice, 
									const Vector3d& vector, 
									int& edge_check_id, 
									int& duplicate_id);
	
	
	// Controllo dei lati duplicati verificando gli estremi.
	// matrice: matrcie 2XN dei lati, i coefficienti sono gli estremi di ciascuno spigolo.
	// v1, v2: id del vertice corrente.
	// edge_check_id: id corrente che viene verificato.
	// duplicate_id: id aggiunto nel caso il lato sia duplicate.
	bool CheckDuplicates_Edge(const MatrixXi& matrice, 
								const int& v1, 
								const int& v2, 
								int& edge_check_id, 
								int& duplicate_id);
								
								
	// Proiezione dei punti sulla sfera di raggio 1 e centrata nell'origine degli assi.
	// vertices: cooridnate nello spazio 3D del solido da proiettare.
	bool ProjectonPointToSphere(MatrixXd& vertices);

	
	// Ordinazione delle facce intorno al vertice nella costruzione del solido.
	// Coordinates: coordinate necessarie per il calcolo del centroide di ciasuna faccia.
	// unordered_faces: vettore delle facce non ordinate.
	// risultato: vettore delle facce ordinate.
	vector<int> OrderFaceAroundVertex(const MatrixXd& dual_Coordinates, 
										const vector<int>& unordered_faces);
	
	// Generazione del grafo associato al solido e ricerca del cammino minimo.
	// Platonic: una struct di tipo PolyhedronMesh.
	// id_1, id_2: id dei vertici iniziale e finale candidati alla ricerca del cammino minimo.
	vector<int> Short_Path(const PolyhedronMesh& Platonic, 
							const int& id_1, 
							const int& id_2);

}