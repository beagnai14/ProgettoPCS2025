#pragma once
#include <gtest/gtest.h>
#include <Eigen/Dense>
#include "Utils.hpp" 

using namespace std;
using namespace Eigen;
using namespace PolyhedronLibrary;

//TEST CHECK VERTICI 
// Test 1: Duplicato trovato
TEST(PolyhedronTest, CheckDuplicatesVertex_Found) {
    MatrixXd matrix(3, 2);
    matrix << 1, 2,
              3, 4,
              5, 6;

    Eigen::Vector3d newVertex(1, 3, 5);
    int vertexCheckId = 2;
    int duplicateId = -1;			

    bool isDuplicate = Check_Duplicates_Vertex(matrix, newVertex, vertexCheckId, duplicateId);

    ASSERT_TRUE(isDuplicate);
    ASSERT_EQ(duplicateId, 0);
}

// Test 2: Nessun duplicato
TEST(PolyhedronTest, CheckDuplicatesVertex_NotFound) {
    MatrixXd matrix(3, 2);
    matrix << 1, 2,
              3, 4,
              5, 6;

    Eigen::Vector3d newVertex(7, 8, 9);
    int vertexCheckId = 2;
    int duplicateId = -1;

    bool isDuplicate = Check_Duplicates_Vertex(matrix, newVertex, vertexCheckId, duplicateId);

    ASSERT_FALSE(isDuplicate);
    ASSERT_EQ(duplicateId, -1);
}


/**********************************************************************************************************/
//TEST CAMMINO MINIMO
// Test 1: grafo classico	(caso base)
TEST(ShortPathTest, BasicShortPath) {
    PolyhedronMesh polyhedron;

    // Base Dati
    polyhedron.NumCell0Ds = 4;
    polyhedron.Cell0DsId = {0, 1, 2, 3};
    polyhedron.Cell0DsCoordinates.resize(3, 4);
    polyhedron.Cell0DsCoordinates <<
        0, 1, 0, -1, 
        0, 0, 1, 0,
        0, 0, 0, 1;

    polyhedron.NumCell1Ds = 4;
    polyhedron.Cell1DsId = {0, 1, 2, 3};
    polyhedron.Cell1DsExtrema.resize(2, 4);
    polyhedron.Cell1DsExtrema <<
        0, 1, 2, 3, 
        1, 2, 3, 0;

   std::vector<int> path = Short_Path(polyhedron, 0, 2);

    ASSERT_EQ(path.size(), 3); 
    ASSERT_EQ(path[0], 0);
    ASSERT_EQ(path[1], 1);
    ASSERT_EQ(path[2], 2);
}

// Test 2: grafo con un solo nodo (caso limite)
TEST(ShortPathTest, SameVertexShortPath) {
    PolyhedronMesh polyhedron;

    polyhedron.NumCell0Ds = 1;
    polyhedron.Cell0DsId = {0};
    polyhedron.Cell0DsCoordinates.resize(3, 1);
    polyhedron.Cell0DsCoordinates <<
        0, 
        0,
        0;

    polyhedron.NumCell1Ds = 0;

    vector<int> path = Short_Path(polyhedron, 0, 0);

    ASSERT_EQ(path.size(), 1); 
    ASSERT_EQ(path[0], 0);
}

/***********************************************************************************************************/
//TEST VALIDAZIONE INPUT
// Test 1: valori input validi
TEST(InputValidationTest, ValidatePAndQ) {
    ASSERT_NO_THROW({
        int p = 3;
        int q = 4;
        if (p != 3 && p != 4 && p != 5) {
            throw invalid_argument("p must be in range 3, 4, 5");
        }
        if (q != 3 && q != 4 && q != 5) {
            throw invalid_argument("q must be in range 3, 4, 5");
        }
    });

//valori input non validi
    ASSERT_THROW({
        int p = 2;
        int q = 6;
        if (p != 3 && p != 4 && p != 5) {
            throw invalid_argument("p must be in range 3, 4, 5");
        }
        if (q != 3 && q != 4 && q != 5) {
            throw invalid_argument("q must be in range 3, 4, 5");
        }
    }, invalid_argument);
}

// Test 2: validazione di b e c
TEST(InputValidationTest, ValidateBandC) {
    ASSERT_NO_THROW({
        int b = 1;
        int c = 0;
        if (b <= 0 || c < 0) {
            throw invalid_argument("b,c must be non negative integers.");
        }
    });

    // Testez des valeurs invalides pour b et c
    ASSERT_THROW({
        int b = -1;
        int c = -1;
        if (b <= 0 || c < 0) {
            throw invalid_argument("b,c must be non negative integers.");
        }
    }, invalid_argument);
}

//Test 3: validazione vertici del cammino minimo
TEST(InputValidationTest, ValidateVertexIDs) {
    PolyhedronMesh polyhedron;
    polyhedron.NumCell0Ds = 4; 

    ASSERT_NO_THROW({
        int id_vertex1 = 0;
        int id_vertex2 = 3;
        if (id_vertex1 < 0 || id_vertex1 >= polyhedron.NumCell0Ds || id_vertex2 < 0 || id_vertex2 >= polyhedron.NumCell0Ds) {
            throw invalid_argument("Check the values of id_vertex1 and id_vertex2");
        }
    });

    ASSERT_THROW({
        int id_vertex1 = -1;
        int id_vertex2 = 5;
        if (id_vertex1 < 0 || id_vertex1 >= polyhedron.NumCell0Ds || id_vertex2 < 0 || id_vertex2 >= polyhedron.NumCell0Ds) {
            throw invalid_argument("Check the values of id_vertex1 and id_vertex2");
        }
    }, invalid_argument);
}


/*******************************************************************************************************************************************************/
//TEST DUALE CLASSE 1
TEST(DualClassITest, GenerateDualClassI) {

    PolyhedronMesh originalPolyhedron;

    //Creazione Basi Dati
    originalPolyhedron.NumCell0Ds = 4;
    originalPolyhedron.Cell0DsId = {0, 1, 2, 3};
    originalPolyhedron.Cell0DsCoordinates.resize(3, 4);
    originalPolyhedron.Cell0DsCoordinates <<
        1, -1, -1,  1, 
       -1,  1, -1, 1,
       -1, -1,  1, 1;

    originalPolyhedron.NumCell1Ds = 6;
    originalPolyhedron.Cell1DsId = {0, 1, 2, 3, 4, 5};
    originalPolyhedron.Cell1DsExtrema.resize(2, 6);
    originalPolyhedron.Cell1DsExtrema <<
        0, 1, 1, 2, 2, 0, 
        3, 2, 0, 3, 1, 3;

    originalPolyhedron.NumCell2Ds = 4;
    originalPolyhedron.Cell2DsId = {0, 1, 2, 3};
    originalPolyhedron.Cell2DsVertices = {{0, 1, 2}, {0, 2, 3}, {0, 1, 3}, {1, 2, 3}};
    originalPolyhedron.Cell2DsEdges = {{0, 1, 2}, {1, 3, 4}, {2, 4, 5}, {0, 3, 5}};

    //creazione del poliedro duale
    PolyhedronMesh dualPolyhedron;
    ASSERT_TRUE(GeneratePlatonic::Duale(originalPolyhedron, dualPolyhedron));		//i vertici sono proiettati sulla sfera

    //verifica che la faccia del poliedro originale diventa un vertice del poliedro duale
    ASSERT_EQ(dualPolyhedron.NumCell0Ds, originalPolyhedron.NumCell2Ds);

    //verifica che ogni vertice del poliedro originale diventa una faccia del poliedro duale
    ASSERT_EQ(dualPolyhedron.NumCell2Ds, originalPolyhedron.NumCell0Ds);

    //verifica che i vertici del duale siano i baricentri delle facce del poliedro originale
    for (int i = 0; i < dualPolyhedron.NumCell0Ds; i++) {
        Vector3d centroid = Vector3d::Zero();
        for (int j = 0; j < 3; j++) {
            int vertexId = originalPolyhedron.Cell2DsVertices[i][j];
            centroid += originalPolyhedron.Cell0DsCoordinates.col(vertexId);
        }
        centroid /= 3.0;
		centroid.normalize();
		
        ASSERT_TRUE(dualPolyhedron.Cell0DsCoordinates.col(i).isApprox(centroid, 1e-6));
    }
}

