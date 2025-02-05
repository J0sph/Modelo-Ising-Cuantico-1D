#include <iostream>
#include <Eigen/Sparse>   //Para las matrices dispersas
#include <Eigen/Dense>    //Para las matrices normales
#include <vector>         //Para manejar arreglos o vectores 
#include <omp.h>          //Para biblioteca OpenMP para paralelizar el código 

using namespace Eigen;   //Para usar todas las funciones de Eigen 
using namespace std;     //Para usar todas las funciones del std 

//Declaración de las matrices dispersas de Pauli con nombre y dimensiones 
SparseMatrix<double> sx(2, 2);
SparseMatrix<double> sz(2, 2);
SparseMatrix<double> iden(2, 2);

//Inicialización de las matrices
void initialize_pauli() {   //la función no tiene ningún argumento ni devuelve ningún resultado (void) 
    sx.insert(0, 1) = 1;     //Inserta un 1 en la posición (0,1) de la matriz 
    sx.insert(1, 0) = 1;     //Inserta un 1 en la posición (1,0) de la matriz 

    sz.insert(0, 0) = 1;   //Inserta un 1 en la posición (0,0)
    sz.insert(1, 1) = -1;   //Inserta un 1 en la posición (1,1) 

    iden.insert(0, 0) = 1;    //INserta un 1 en la posición (0,0)
    iden.insert(1, 1) = 1;    //Inserta un 1 en la posición (1,1) 
}

SparseMatrix<double> kroneckerProduct(const SparseMatrix<double>& A, const SparseMatrix<double>& B) {  //Tiene como parámetros las referencias (no copias) de las matrices dispersas A y B tomándolas como constantes que no pueden ser modificadas
    SparseMatrix<double> result(A.rows() * B.rows(), A.cols() * B.cols());   //Inicia la matriz resultado con el tamaño respectivo
    for (int k = 0; k < A.outerSize(); ++k)        //Iteración sobre k sigue hasta que k sea menor que el numero de columnas de A
        for (SparseMatrix<double>::InnerIterator itA(A, k); itA; ++itA)     //Se hace otro ciclo con un iterador interno itA que recorre la columna k de A    
            for (int l = 0; l < B.outerSize(); ++l)       //Inicia iteración sobre la matriz B, iterador l empieza en cero y llega hasta que siga siendo menor que columnas de B
                for (SparseMatrix<double>::InnerIterator itB(B, l); itB; ++itB)  //Ciclo iterador interno itB, recorre la columna l de la matriz B 
                    result.insert(itA.row() * B.rows() + itB.row(), itA.col() * B.cols() + itB.col()) = itA.value() * itB.value();   //Hace el calculo de las posiciones segun la iteracion e inserta el resultado en la celda correspondiente
    return result;
}

SparseMatrix<double> term_z_component(int i, int N, double J) {   //calcula el primer componente del hamiltoniano, recibe índice, número de spines y J 
    if (i < N - 1) {   //Cálculo de los s_i y s_i+1
        return -J * kroneckerProduct(
            kroneckerProduct(SparseMatrix<double>(MatrixXd::Identity(pow(2, i), pow(2, i)).sparseView()), sz),
            kroneckerProduct(sz, SparseMatrix<double>(MatrixXd::Identity(pow(2, N - i - 2), pow(2, N - i - 2)).sparseView()))
        );
    }
    return SparseMatrix<double>(pow(2, N), pow(2, N));  //sino retorna la matriz vacía 
}

SparseMatrix<double> term_x_component(int i, int N, double g) {  //calculo del segundo término de hamiltoniano, recibe índice, spines y la constante g
    return -g * kroneckerProduct(  
        kroneckerProduct(SparseMatrix<double>(MatrixXd::Identity(pow(2, i), pow(2, i)).sparseView()), sx),
        SparseMatrix<double>(MatrixXd::Identity(pow(2, N - i - 1), pow(2, N - i - 1)).sparseView())   //MatrixXd es matriz de tamaño dinamico con elementos double
    );  //Cálculo de la suma de los s_i de x  
}

//Construcción de la matriz con paralelización 
SparseMatrix<double> sparse_hamiltonian(int N, double J, double g) {  //recibe de parámetros la cantidad de spines y las constantes J y g 
    SparseMatrix<double> H(pow(2, N), pow(2, N));   //genera la matriz de tamaño 2^N

    #pragma omp parallel  //empieza un bloque paralelo para que se ejecute en vatios hilos al mismo tiempo 
    {
        SparseMatrix<double> H_private(pow(2, N), pow(2, N));   //hace una matriz privada para que no se modifique mientras los hilos se ejecutan al mismo tiempo 

        #pragma omp for nowait     //hace que se distribuyan las iteraciones del for en paralelo 
        for (int i = 0; i < N; ++i) {  //iterador i inicia en cero y sigue hasta ser menor a N 
            if (i < N - 1) {    //para la primera suma se ocupan términos hasta N-1 por s_n+1
                H_private += term_z_component(i, N, J);  //se le suma el componente z, primer término 
            }
            H_private += term_x_component(i, N, g);   //se le suma el componente de x, segundo término
        }

        #pragma omp critical  //reduce a que las cosas se ejecuten individualmente 
        H += H_private;  //añade los elementos calculados 
    }

    return H;
}

//Función main para que se ejecute el codigo 
int main() {
    initialize_pauli();  //que se ejecute la funcion que crea las matrices de pauli 
     //Definición de los parámetros 
    int N = 2;
    double J = 1.0;
    double g = 1.0;

    SparseMatrix<double> H_sparse = sparse_hamiltonian(N, J, g);   //ejecusión de la matriz que hace el hamiltoniano 

    // Conversión a una matriz densa para la diagonalización
    MatrixXd H_dense = MatrixXd(H_sparse);   //tamaño dinanmino con elementos del tipo double 
    cout << "Hamiltoniano:" << endl;
    cout << H_dense << endl;

    // Diagonalización usando Eigen
    SelfAdjointEigenSolver<MatrixXd> solver(H_dense);
    if (solver.info() != Success) {   //Si no se puede hacer la diagonalización devuelve -1
        return -1;
    }

    VectorXd eigenvalues = solver.eigenvalues();   //calcula los valores propios
    MatrixXd eigenvectors = solver.eigenvectors();  //calcula los vectores propios 
    
    MatrixXd diagonal_matrix = eigenvalues.asDiagonal();   //poner los autovalores en la diagonal como matriz densa para impresion

    // Impresion de los resultados
    cout << "La matriz diagonalizada es: " << endl << diagonal_matrix << endl;
    cout << "Los autovalores son: " << endl << eigenvalues.transpose() << endl;
    cout << "Los autovectores son: " << endl << eigenvectors << endl;

    return 0;
}

