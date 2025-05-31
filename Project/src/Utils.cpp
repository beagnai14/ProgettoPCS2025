#include <iostream>
#include <fstream>
#include <sstream>
#include <queue>

#include <Eigen/Dense>
#include "Utils.hpp"

using namespace Eigen;
using namespace std;

namespace PolyhedronLibrary{
	
	namespace ImportFile {
		
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
				int id;
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
				
				int id;
				int Origin;
				int End;
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
				int id; 
				char separatore;
				
				is_line >> id >> separatore;
				
				polyhedron.Cell2DsId.push_back(id);
				
				// memorizzo i vertici e gli spigoli
				vector<int> vertices(3);
				for(int i = 0; i < 3; i++)
					is_line >> vertices[i] >> separatore;
				polyhedron.Cell2DsVertices.push_back(vertices);
				
				vector<int> edges(3);
				for(int i = 0; i < 3; i++)
					is_line >> edges[i] >> separatore;
				polyhedron.Cell2DsEdges.push_back(edges);
				
			}	
			return true;
		}
	}

		/********************************************************************************************************/
	namespace ExportFile {	
	
		bool ExportCell0Ds(PolyhedronMesh& polyhedron, const string& OutputFile)
		{
			ofstream outCell0Ds(OutputFile);
			if (outCell0Ds.fail()) {
				cerr << "Unable to open output file: " << OutputFile << endl;
				return false;
			}
			
			outCell0Ds << "Id;X;Y;Z" << endl;
			
			for (int i = 0; i < polyhedron.NumCell0Ds; ++i) {
				int id = polyhedron.Cell0DsId[i];
				outCell0Ds << id << ";"
					<< polyhedron.Cell0DsCoordinates(0, id) << ";"
					<< polyhedron.Cell0DsCoordinates(1, id) << ";"
					<< polyhedron.Cell0DsCoordinates(2, id) << endl;
			}

			outCell0Ds.close();
			return true;
		}

		/***************************************************************************************************************/


		bool ExportCell1Ds(PolyhedronMesh& polyhedron, const string& OutputFile)
		{
			ofstream outCell1Ds(OutputFile);
			if (outCell1Ds.fail()) {
				cerr << "Unable to open output file: " << OutputFile << endl;
				return false;
			}
			
			outCell1Ds << "Id;Origin;End" << endl;
			
			for (int i = 0; i < polyhedron.NumCell1Ds; ++i) {
				int id = polyhedron.Cell1DsId[i];
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
			
			for (int i = 0; i < polyhedron.NumCell2Ds; ++i) {
				
				int id = polyhedron.Cell2DsId[i];
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
			for (int i = 0; i < polyhedron.NumCell0Ds; ++i) {
				outCell3Ds << i << ";" ;
			}
				
			 outCell3Ds << polyhedron.NumCell1Ds << ";" ;
			 for (int i = 0; i < polyhedron.NumCell1Ds; ++i) {
				outCell3Ds << i <<  ";" ;
			 }
			 
			 outCell3Ds << polyhedron.NumCell2Ds << ";";
			 for (int i = 0; i < polyhedron.NumCell2Ds; ++i) {
				outCell3Ds << i <<  ";" ;
			}
			
			return true;
		}
	}



	/*********************************************************************************************************************/

	namespace GenerateGeodedic {
	
		bool Solid_Class1(const PolyhedronMesh& Platonic, PolyhedronMesh& Geodetic, const int& n)
		{

			if(n == 1)
			{
				Geodetic = Platonic;
				return true;
			}

			int T = n * n;

			int E = 0; int F = 0;
			if (Platonic.Cell2DsId.size() == 4)
			{
				E = 6 * T;
				F = 4 * T;
			}
			else if (Platonic.Cell2DsId.size() == 8) {
				E = 12 * T;
				F = 8 * T;
			}
			else if (Platonic.Cell2DsId.size() == 20) {
				E = 30 * T;
				F = 20 * T;
			}
			else
			{
				return false;
			}



			int point_id = 0;
			int duplicate_id = 0;
			int edge_id = 0;
			int face_id = 0;
			
			int Tot_points = (Platonic.NumCell2Ds) * ((n + 1) * (n+2)/2); //punti per faccia * numero di facce (inclusi duplicati)
					
			//allocazione memoria per un totale massimo di punti che si può avere (devo eliminare poi quella non usata per i duplicati)
			Geodetic.Cell0DsId.reserve(Tot_points);							//vector id vertici
			Geodetic.Cell0DsCoordinates = MatrixXd::Zero(3, Tot_points);	//matrice 3d coordinate
			
			Geodetic.Cell1DsId.reserve(E);
			Geodetic.Cell1DsExtrema = MatrixXi::Zero(2, E); 
			
			Geodetic.Cell2DsId.reserve(F);
			Geodetic.Cell2DsEdges.resize(F);
			Geodetic.Cell2DsNumEdges.resize(F);
			Geodetic.Cell2DsVertices.resize(F);
			Geodetic.Cell2DsNumVertices.resize(F);
			
			map<array<int, 4>, int> point_informations;						//chiave: coordinate baricentriche e valore id del nuovo punto
			
			
			//triangolazione per ogni faccia
			for (const auto& id : Platonic.Cell2DsId) {
				
				Vector3d V1 = Platonic.Cell0DsCoordinates.col(Platonic.Cell2DsVertices[id][0]);
				Vector3d V2 = Platonic.Cell0DsCoordinates.col(Platonic.Cell2DsVertices[id][1]);
				Vector3d V3 = Platonic.Cell0DsCoordinates.col(Platonic.Cell2DsVertices[id][2]);
				
					
				//VERTICI
				for (int i = 0; i <= n; i++) {
					for (int j = 0; j <= i; j++) {
									
						//coefficienti della triangolazione
						int a = n - i;
						int b = i - j;
						int c = j;
										
						//punti su ciascun segmento
						Vector3d PointCoordinates =  (double(a) / n) * V1 + (double(b) / n) * V2 + (double(c) / n) * V3;
										
						//aggiorno la chiave
						array<int, 4> coefficients = {a, b, c, id};            
						
						//verifico la stampa dei dati 
						//NOTA->eliminare alla fine *** !!!
						cout << "Coefficiente baricentrico (a, b, c) sulla faccia ID " << coefficients[3] << ":\n";
						cout << "  a = " << coefficients[0] << ", b = " << coefficients[1] << ", c = " << coefficients[2] << endl;
						cout << "Coordinate punto: (" 
						<< PointCoordinates(0) << ", " 
						<< PointCoordinates(1) << ", " 
						<< PointCoordinates(2) << ")" << endl;
						cout << endl;
						
											
						//controllo duplicati: se il punto è nuovo viene aggiunto
						if (!Check_Duplicates_Vertex(Geodetic.Cell0DsCoordinates, PointCoordinates, point_id, duplicate_id)){
							
							point_informations[coefficients] = point_id;						//valore id nuovo punto associato alla chiave
							Geodetic.Cell0DsId.push_back(point_id);								//salvo id nelle CelleOd
							Geodetic.Cell0DsCoordinates.col(point_id) = PointCoordinates;		//aggiorno la matrice delle coordinate: x, y, z
												

							//verifico la stampa dei dati 
							//NOTA->eliminare alla fine *** !!!
							cout << "[Nuovo Punto] ID: " << point_id << " sulla faccia ID " << id << endl;
							cout << "  Coefficienti (a, b, c): " << a << ", " << b << ", " << c << endl;
							cout << "  Coordinate: (" << PointCoordinates.transpose() << ")" << endl << endl;

							
							point_id++;
							Geodetic.NumCell0Ds++;
							
							} else {
								point_informations[coefficients] = duplicate_id;	
								
								//verifico la stampa dei dati 
								//NOTA->eliminare alla fine *** !!!
								cout << "[Duplicato] Punto già esistente con ID: " << duplicate_id << " sulla faccia ID " << coefficients[3] << endl;
								cout << "  Coefficienti (a, b, c): "
								<< coefficients[0] << ", " << coefficients[1] << ", " << coefficients[2] << endl;
								cout << "  Coordinate: (" << PointCoordinates.transpose() << ")" << endl;
								cout<<endl;
							}	
						}	
					}
			}
			
			//ripulisco la matrice togliendo le colonne nulle a causa dei puplicati
			Geodetic.Cell0DsCoordinates.conservativeResize(3, Geodetic.NumCell0Ds);
			// ProjectonPointToSphere(Geodetic.Cell0DsCoordinates);
			
			
			//LATI E FACCIA 
			for  (const auto& id : Platonic.Cell2DsId){					
				for (int i = 0; i < n; i++){
					for (int j = 0; j < n - i; j++){
						
						int V1 = point_informations[{i,   n-i-j,     j,   id}];		// punto a sinistra
						int V2 = point_informations[{i,   n-i-(j+1), j+1, id}];		// punto a destra
						int V3 = point_informations[{i+1, n-(i+1)-j, j,   id}];		// punto in alto
			
						//primo tipo di tringolo: estremità in alto
						//Salvo uno nuova faccia triangolare
						Geodetic.NumCell2Ds++;
						Geodetic.Cell2DsId.push_back(face_id);						//aggiungo id 
						Geodetic.Cell2DsNumVertices[face_id] = 3;   				//id=* ha sempre 3 vertici perchè si ottiene un traingolo
						Geodetic.Cell2DsNumEdges[face_id] = 3;						//id=* ha sempre 3 lati perchè si ottiene un traingolo		
						vector<int> VerticesVector = {V1, V2, V3};					//vertici della nuova mini-faccia
						Geodetic.Cell2DsVertices[face_id] = VerticesVector;
						Geodetic.Cell2DsEdges[face_id].resize(3);						//spazio per i lati della faccia id=*
						
						
						
						cout << "Faccia 🔺 " << face_id << " (punta in ALTO) ha come vertici: ";
						for (int v : Geodetic.Cell2DsVertices[face_id]) {
						cout << v << " ";
						}
						cout << endl;
						
						
						
						//LATI triangolo punta in sù
						for (int k = 0; k < 3; k++) {
							int Origin_Vertex = Geodetic.Cell2DsVertices[face_id][k];	//{k, V2, V3} della faccia id
							int End_Vertex;	
							if ( k == 2 )
								End_Vertex = Geodetic.Cell2DsVertices[face_id][0];		//chiudo il triangolo collegando con il vertice di partenza
							else
								End_Vertex = Geodetic.Cell2DsVertices[face_id][k+1];	//vertice successivo
							
							if (edge_id > E) {
							std::cerr << "[ERRORE] edge_id fuori limite: " << edge_id << " >= " << E << std::endl;
							return false;
							}
							
							//se l'arco è nuovo
							if(!CheckDuplicates_Edge(Geodetic.Cell1DsExtrema, Origin_Vertex, End_Vertex, edge_id, duplicate_id)){
								Geodetic.NumCell1Ds++;
								Geodetic.Cell1DsId.push_back(edge_id);
								Geodetic.Cell1DsExtrema(0, edge_id) = Origin_Vertex;		//aggiorno estremo (origine) del lato
								Geodetic.Cell1DsExtrema(1, edge_id) = End_Vertex;			//aggiorno estremo (fine) del lato
								Geodetic.Cell2DsEdges[face_id][k] = edge_id;
								edge_id++;
							} else {
								Geodetic.Cell2DsEdges[face_id][k] = Geodetic.Cell1DsId[duplicate_id];
							}
						}
						face_id++;															//aggiorno la faccia per il prossimo ciclo
						
						
						//secondo tipo di trinagolo: punta in basso
						//il triangolo con la punta in basso viene eseguito dalla seconda iterazione del ciclo for esterno
						//perchè partendo dal vertice V1 in alto si tratta di un triangolo con la punta in sù
						if(i > 0){
							
							int V4 = point_informations[{i-1, n-(i-1)-(j+1), (j+1), id}];			
							
							Geodetic.NumCell2Ds++;
							Geodetic.Cell2DsId.push_back(face_id);
							Geodetic.Cell2DsNumVertices[face_id] = 3;
							Geodetic.Cell2DsNumEdges[face_id] = 3;
							VerticesVector[2] = V4; 										//sostituisco V3
							Geodetic.Cell2DsVertices[face_id] = VerticesVector;
							Geodetic.Cell2DsEdges[face_id].resize(3);
							
							cout << "Faccia 🔻 " << face_id << " (punta in BASSO) ha come vertici: ";
							for (int v : Geodetic.Cell2DsVertices[face_id]) {
							cout << v << " ";
							}
							cout << endl;
							
												
							for (int k = 0; k < 3; k++) {
								int Origin_Vertex = Geodetic.Cell2DsVertices[face_id][k];
								int End_Vertex;
								if ( k == 2 )
									End_Vertex = Geodetic.Cell2DsVertices[face_id][0];
								else
									End_Vertex = Geodetic.Cell2DsVertices[face_id][k+1];
								
								if(!CheckDuplicates_Edge(Geodetic.Cell1DsExtrema, Origin_Vertex, End_Vertex, edge_id, duplicate_id)){
									Geodetic.NumCell1Ds++;
									Geodetic.Cell1DsId.push_back(edge_id);
									Geodetic.Cell1DsExtrema(0, edge_id) = Origin_Vertex;
									Geodetic.Cell1DsExtrema(1, edge_id) = End_Vertex;
									Geodetic.Cell2DsEdges[face_id][k] = edge_id;
									edge_id++;
								} else {
									Geodetic.Cell2DsEdges[face_id][k] = Geodetic.Cell1DsId[duplicate_id];                  
								}
							}
							face_id++;
						}
					}
				}
			}
			Geodetic.Cell1DsExtrema.conservativeResize(2, Geodetic.NumCell1Ds);
			
			Geodetic.Cell2DsNumVertices.resize(Geodetic.NumCell2Ds);
			Geodetic.Cell2DsNumEdges.resize(Geodetic.NumCell2Ds);
			Geodetic.Cell2DsVertices.resize(Geodetic.NumCell2Ds);
			Geodetic.Cell2DsEdges.resize(Geodetic.NumCell2Ds); 
			
			Geodetic.NumCell3Ds++;
			Geodetic.Cell3DsId.push_back(0);
			Geodetic.Cell3DsNumVertices.push_back(Geodetic.NumCell0Ds);
			Geodetic.Cell3DsNumEdges.push_back(Geodetic.NumCell1Ds);
			Geodetic.Cell3DsNumFaces.push_back(Geodetic.NumCell2Ds);
			Geodetic.Cell3DsVertices.push_back(Geodetic.Cell0DsId);
			Geodetic.Cell3DsEdges.push_back(Geodetic.Cell1DsId);
			Geodetic.Cell3DsFaces.push_back(Geodetic.Cell2DsId);
			
			return true;
		}



		/***************************************************************************************************************************************/


		bool Duale(PolyhedronMesh& InitialPolyhedron, PolyhedronMesh& DualPolyhedron) {
			
			int centroid_id = 0;
			int edge_id = 0;
			int face_id = 0;
			int duplicate_id = 0;
			
			//Il duale di un poliedro P è il nuovo poliedro Q, ottenuto dal precedente scambiando il ruolo di vertici e facce. (-traccia)
			
			//Cell0DS
			DualPolyhedron.NumCell0Ds = InitialPolyhedron.NumCell2Ds;								//vertici <-> facce originali
			DualPolyhedron.Cell0DsId.resize(DualPolyhedron.NumCell0Ds); 							//creare nuova dimensione delle celle
			DualPolyhedron.Cell0DsCoordinates = MatrixXd::Zero(3, DualPolyhedron.NumCell0Ds);		//creo una matrice per le coordinate del duale
			
			//Cell1Ds
			//lati del duale che sono gli stessi per la proprietà dei poliedri di Golberg
			DualPolyhedron.NumCell1Ds = InitialPolyhedron.NumCell1Ds;								//stesso numero di lati
			DualPolyhedron.Cell1DsId.resize(DualPolyhedron.NumCell1Ds); 							//creare nuova dimensione delle celle
			DualPolyhedron.Cell1DsExtrema = MatrixXi::Zero(2, DualPolyhedron.NumCell1Ds);
			
			//Cell2Ds
			DualPolyhedron.Cell2DsId.reserve(DualPolyhedron.NumCell2Ds);
			DualPolyhedron.NumCell2Ds = InitialPolyhedron.NumCell0Ds;								//facce <-> vertici originali
			DualPolyhedron.Cell2DsEdges.resize(DualPolyhedron.NumCell2Ds);
			DualPolyhedron.Cell2DsNumEdges.resize(DualPolyhedron.NumCell2Ds);
			DualPolyhedron.Cell2DsVertices.resize(DualPolyhedron.NumCell2Ds);
			DualPolyhedron.Cell2DsNumVertices.resize(DualPolyhedron.NumCell2Ds);

			map<int, int> Face_Centroid_Id;															//chiave:id faccia -> id centroide
			
			
			//CREAZIONE VERTICI DUALE -> baricentri delle facce
			for (int i = 0; i < InitialPolyhedron.NumCell2Ds; i++) {
				
				//baricentro della faccia
				/*La faccia i è costituita dai vertici A=0,B=1,C=2. 
				Allora ricavo le coordinate di ciascun vertice, che si trovano nelle colonne della matrice, per calcolare il baricentro */
				Vector3d V1 = InitialPolyhedron.Cell0DsCoordinates.col(InitialPolyhedron.Cell2DsVertices[i][0]); 
				Vector3d V2 = InitialPolyhedron.Cell0DsCoordinates.col(InitialPolyhedron.Cell2DsVertices[i][1]); 
				Vector3d V3 = InitialPolyhedron.Cell0DsCoordinates.col(InitialPolyhedron.Cell2DsVertices[i][2]); 
				Vector3d centroid = (V1 + V2 + V3) / 3.0;
				
				DualPolyhedron.Cell0DsId.push_back(centroid_id);
				
				
				//CONTROLLO OUTPUT -> eliminare alla fine
				std::cout << "Faccia DUALE " << i << ":\n";
				std::cout << "  Vertice 1: (" << V1(0) << ", " << V1(1) << ", " << V1(2) << ")\n";
				std::cout << "  Vertice 2: (" << V2(0) << ", " << V2(1) << ", " << V2(2) << ")\n";
				std::cout << "  Vertice 3: (" << V3(0) << ", " << V3(1) << ", " << V3(2) << ")\n";
				std::cout << "  Baricentro: (" << centroid(0) << ", " << centroid(1) << ", " << centroid(2) << ")\n\n";
				
				
				/*aggiorno le coordinate: il centroide diventa il vertice del duale
				associo centroid(x,y,z) alle cell0ds.coordinate*/
				DualPolyhedron.Cell0DsCoordinates.col(i) = centroid;

				Face_Centroid_Id[i] = centroid_id;						//aggiorno la mappa
				centroid_id++;
			
			}
			
			
			vector<vector<int>> adjacent_new_faces;
			
			//CREAZIONE FACCE DEL DUALE
			//per ogni vertice originale cerco le facce che lo contengono
			//creo un vettore per ciasun vertice_id contenente id delle facce per cui è contenuto
			for (int v_id=0; v_id < InitialPolyhedron.NumCell0Ds; v_id++) {
				
				vector<int> adjacent_faces;
				
				for (int f_id = 0; f_id < InitialPolyhedron.NumCell2Ds; f_id++) {
					
					for (int j = 0; j < 3; j++) {
						if (InitialPolyhedron.Cell2DsVertices[f_id][j] == v_id) {
							adjacent_faces.push_back(f_id);
							break;  //passa alla prossima faccia dopo aver trovato
						}
					}
				}
			
			
				//CONTROLLO -> eliminare alla fine
				//controllo stampa vettore non ordinato
				cout << "VETTORE NON ORDINATO DELLE FACCE" << endl;
				cout << "Vertice " << v_id << " è condiviso dalle facce: ";
				for (int id : adjacent_faces) {
					cout << id << " ";
				}
				cout << endl;
				
				
				//a questo punto si sono ottenute tutte le facce che condividono un vertice nel poliedro originale
				//sono però in ordine casuale quindi devono essere ordinate in modo da creare un poligono che si chiudi.
				
				vector<int> ordered = OrderFaceAroundVertex(DualPolyhedron.Cell0DsCoordinates, adjacent_faces);
				adjacent_new_faces.push_back(ordered);
			
				//controllo stampa vettore ordinato -> eliminare
				cout << "adjacent_new_faces: " << v_id << ": ";
				for (int val : ordered) cout << val << " ";
				cout << endl;
			
				//Creazione dei lati
				int valence = ordered.size();   //numero di facce adiacenti al vertice: non è sempre 3
				
				vector<int> Dual_vertices;	
				for (size_t i = 0; i < ordered.size(); i++) {
					Dual_vertices.push_back(Face_Centroid_Id[ordered[i]]);
				}
				
				
				// DEBUG: stampa dei vertici della faccia duale  -> eliminare
				cout << "Faccia duale con ID " << face_id << " ha " << valence << " vertici: ";
				for (const auto& v : Dual_vertices) cout << v << " ";
				cout << endl;
				//
				
				
				//aggiorno struttura dati
				DualPolyhedron.Cell2DsId.push_back(face_id);
				DualPolyhedron.Cell2DsVertices[face_id] = Dual_vertices;
				DualPolyhedron.Cell2DsEdges[face_id].resize(valence);
						
				DualPolyhedron.Cell2DsNumVertices[face_id] = valence;
				DualPolyhedron.Cell2DsNumEdges[face_id] = valence; 

				
				for (int k = 0; k < valence; k++) {
					int Origin_Vertex = DualPolyhedron.Cell2DsVertices[face_id][k];
					int End_Vertex;
					if ( k == valence - 1 )
						End_Vertex = DualPolyhedron.Cell2DsVertices[face_id][0];
					else
						End_Vertex = DualPolyhedron.Cell2DsVertices[face_id][k+1];
					
					// DEBUG: stampa info lato
					cout << "  Lato " << k << ": da vertice " << Origin_Vertex << " a vertice " << End_Vertex;

					
					if(!CheckDuplicates_Edge(DualPolyhedron.Cell1DsExtrema, Origin_Vertex, End_Vertex, edge_id, duplicate_id)){
						DualPolyhedron.NumCell1Ds++;
						DualPolyhedron.Cell1DsId.push_back(edge_id);
						DualPolyhedron.Cell1DsExtrema(0, edge_id) = Origin_Vertex;
						DualPolyhedron.Cell1DsExtrema(1, edge_id) = End_Vertex;
						DualPolyhedron.Cell2DsEdges[face_id][k] = edge_id;
						
						//STAMPA PER CONTROLLO
						cout << " → Nuovo lato con ID " << edge_id << endl;
						
						
						edge_id++;
						} else {
							DualPolyhedron.Cell2DsEdges[face_id][k] = DualPolyhedron.Cell1DsId[duplicate_id];    

							//STAMPA PER CONTROLLO
							cout << " → Lato già esistente con ID " << DualPolyhedron.Cell1DsId[duplicate_id] << endl;
						}
				}
				face_id++;
			}
			
			ProjectonPointToSphere(DualPolyhedron.Cell0DsCoordinates);
			
			cout << "PROIEZIONE SULLA SFERA DEL DUALE" << endl;
			for (int i = 0; i < DualPolyhedron.Cell0DsCoordinates.cols(); ++i) {
			Vector3d pt = DualPolyhedron.Cell0DsCoordinates.col(i);
			std::cout << "Vertice " << i << ": (" 
			<< pt(0) << ", " << pt(1) << ", " << pt(2) << ")\n";  }
			
			
			
			
			DualPolyhedron.Cell1DsExtrema.conservativeResize(2, DualPolyhedron.NumCell1Ds);
			DualPolyhedron.Cell2DsNumVertices.resize(DualPolyhedron.NumCell2Ds);
			DualPolyhedron.Cell2DsNumEdges.resize(DualPolyhedron.NumCell2Ds);
			DualPolyhedron.Cell2DsVertices.resize(DualPolyhedron.NumCell2Ds);
			DualPolyhedron.Cell2DsEdges.resize(DualPolyhedron.NumCell2Ds);
			
			DualPolyhedron.NumCell3Ds++;
			DualPolyhedron.Cell3DsId.push_back(0);
			DualPolyhedron.Cell3DsNumVertices.push_back(DualPolyhedron.NumCell0Ds);
			DualPolyhedron.Cell3DsNumEdges.push_back(DualPolyhedron.NumCell1Ds);
			DualPolyhedron.Cell3DsNumFaces.push_back(DualPolyhedron.NumCell2Ds);
			DualPolyhedron.Cell3DsVertices.push_back(DualPolyhedron.Cell0DsId);
			DualPolyhedron.Cell3DsEdges.push_back(DualPolyhedron.Cell1DsId);
			DualPolyhedron.Cell3DsFaces.push_back(DualPolyhedron.Cell2DsId);
			
			
			return true;
		}
	}

	/*******************************************************************************************************/

	/*per ogni i=0 fino al numero id nuovo (vertex_index_id=point_id) scorro le colonne della matrice 
	che contengono le informazioni sulle coordinate (x,y,z) e confronto con il nuovo punto (vector). 
	se sono lo stesso (vicini nel senso della tolleranza) allora è una posizione duplicata*/

	bool Check_Duplicates_Vertex(const MatrixXd& matrice, const Vector3d& vector, int& vertex_check_id, int& duplicate_id)
	{
		for(int i = 0; i < vertex_check_id; i++){
			
			if( (matrice.col(i) - vector).norm() < 1e-16 ){
				duplicate_id = i;
				return true;
			}
		}
		return false;
	}

	/******************************************************************************************************/


	bool CheckDuplicates_Edge(const MatrixXi& matrice, const int& v1, const int& v2, int& edge_check_id, int& duplicate_id)
	{
		for (int i = 0; i < edge_check_id; i++){
			int u1 = matrice(0, i);
			int u2 = matrice(1, i);
			
			if( (v1 == u1 && v2 == u2) || (v1 == u2 && v2 == u1) ) {
				duplicate_id = i;
				return true;
			}
		}
		return false;
	}


	/********************************************************************************************************/

	bool ProjectonPointToSphere(MatrixXd& vertices)
	{
		for (int i = 0; i < vertices.cols() ; i++) {
			
			Vector3d Coordinate_xyz = vertices.col(i);			//coordinate della triangolazione
			double norm = Coordinate_xyz.norm();				//norma euclidea
			
			cout << "Punto " << i << " originale: " << Coordinate_xyz.transpose() << ", norma = " << norm << endl;
			
			if (norm > 1e-16) {									//se è dievrsa da zero
				vertices.col(i) = Coordinate_xyz / norm;		//normalizzo -> proietto sulla sfera
			}
			
			cout << "Punto " << i << " proiettato: " << vertices.col(i).transpose() << endl;
		}
		return true;
	}


	/********************************************************************************************************************************/


	vector<int> OrderFaceAroundVertex(const MatrixXd& dual_Coordinates, const vector<int>& unordered_faces) {
		
		vector<int> ordered_faces;
		
		if (unordered_faces.empty()) {
			return ordered_faces; 
		}
		
		//baricentro della faccia
		Vector3d baricenter = Vector3d::Zero();
		
		
		for (int face_id : unordered_faces) {
			
			//ricavo le coordinate dei vertici
			baricenter += dual_Coordinates.col(face_id); 	
		}
		baricenter /= unordered_faces.size();
		
		//punti nello spazio
		Vector3d V0 = dual_Coordinates.col(unordered_faces[0]);
		Vector3d V1 = dual_Coordinates.col(unordered_faces[1]);
		Vector3d V2 = dual_Coordinates.col(unordered_faces[2]);
		

		//calcolo della normale attraverso i vettori
		Vector3d n = (V1 - V0).cross(V2 - V0).normalized();
		
		//direzione di riferimento scelta: baricentro-V0
		Vector3d dir = (V0 - baricenter).normalized();
		
		//creazione sistema riferimento locale sulla faccia
		Vector3d asse_x = dir;
		Vector3d asse_y = n.cross(asse_x).normalized();
		
		//angolo polare in radianti (atan) sul piano dato tra il baricentro e il punto 
		vector<pair<double, unsigned int>> angle_id;
		for (auto id : unordered_faces) {
			Vector3d vector = dual_Coordinates.col(id) - baricenter;
			double x = vector.dot(asse_x);
			double y = vector.dot(asse_y);
			double angle = atan2(y, x);   //-> angolo tra (-pi, pi)
		
			//angolo positivo (0, 2pi)
			if (angle < 0) {
				angle = angle + 2 * M_PI; 
			}
		
			angle_id.emplace_back(angle, id);
		
		}
		
		//metto in ordine in base all'angolo (primo elemento della coppia) in senso antiorario
		sort(angle_id.begin(), angle_id.end());
		
		//aggiungo id corrispondente -> Id ordinati in senso antiorario
		for (const auto& val : angle_id) {
			ordered_faces.push_back(val.second);  //elemento secondo 
		}
		

		return ordered_faces;
	}


	/************************************************************************************************************/


	vector<int> Short_Path ( const PolyhedronMesh& Platonic, const int& id_1, const int& id_2) {
		
		vector<int> path;

		//creazione della lista di adiacenza per rappresentare il grafo
		
		vector<vector<int>> adjacent_list;
		adjacent_list.reserve(Platonic.NumCell0Ds);
																												
		for (int id = 0; id < Platonic.NumCell0Ds; id++) {
			
			vector<int> adjacent_vertex;
			for (int j = 0; j < Platonic.Cell1DsExtrema.cols(); j++) {
				Vector2i edge = Platonic.Cell1DsExtrema.col(j);
				int origin = edge[0];
				int end = edge[1];
				if(id == origin) {
					adjacent_vertex.push_back(end);
				}
				if(id == end) {
					adjacent_vertex.push_back(origin);
				}
			}
			adjacent_list.push_back(adjacent_vertex);
		}

		//CONTROLLO STAMPA -> eliminare alla fine
		for (size_t i = 0; i < adjacent_list.size(); ++i) {
		std::cout << "Vertice " << i << " è adiacente a: ";
		for (size_t j = 0; j < adjacent_list[i].size(); ++j) {
			std::cout << adjacent_list[i][j];
			if (j < adjacent_list[i].size() - 1)
				std::cout << ", ";
		}
		std::cout << std::endl;
		}
		//
		
		
		//gestisico il caso banale di BFS
		if(id_1 == id_2){
			path.reserve(1);
			path.push_back(id_1);
			cout << "Id_1=id_2" << endl; 
			return path;
		}
		
		
		//altri casi di BFS
		const int N = Platonic.NumCell0Ds;
		vector<bool> reached(N, false);
		vector<int> parent(N,N);
		queue<int> Q;
		
		Q.push(id_1);				//coda
		reached[id_1] = true; 		//nodo di partenza visitato
		parent[id_1] = N;
		
		bool end_path = false;
		while (!Q.empty() && !end_path) {
			int current = Q.front();
			Q.pop();
			
			for(int neighbour : adjacent_list[current]){
				if(!reached[neighbour]){
					reached[neighbour] = true;
					parent[neighbour] = current;
					Q.push(neighbour);
					
					if(neighbour == id_2) {
						end_path = true;
						break;
					}
				}
			}
		}
		
		//Ricostruzione path
		int final_node = id_2;
		while(final_node != N){
			path.push_back(final_node);
			final_node = parent[final_node];
		}
		
		reverse(path.begin(), path.end());
		

		//STAMPA NECESSARIA! -> NON ELIMINARE
		cout<<"Path: ";
		for(size_t i=0; i<path.size(); i++){
			cout<<path[i]<<' ';
		}
		cout<<endl;
		
		//STAMPA NECESSARIA numero di lati del cammino minimo
		cout<<"The shortest path between " << id_1 <<" and " << id_2 <<" has "<<(path.size()-1) << " number of edges "<<endl;
		
		double length = 0.0;
		for(size_t i=0; i < path.size() - 1; i++){
			int previous_id = path[i];
			int next_id = path[i+1];
			//estraggo le coordinate dei relativi vertici
			Vector3d X = Platonic.Cell0DsCoordinates.col(previous_id);
			Vector3d Y = Platonic.Cell0DsCoordinates.col(next_id);
			length += (X-Y).norm();

		}
		cout<<"The shortest path between "<< id_1 <<" and " << id_2 <<" is "<< length <<" long" <<endl;
		
		return path;

	}	


}

	/**************************************************************************************************************************************/
