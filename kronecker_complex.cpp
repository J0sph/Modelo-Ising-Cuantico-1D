/// El método permite obtener el producto de Kronecker de dos vectores  unidimensionales
/// Una matriz se expresa en el vector unidimensional encadenando fila por fila
/// Ejemplo:
/// x11;x12
/// x21;x22
/// =x11;x12;x21;x22
///
///
#include <iostream>
#include <complex>

std::complex<double>*  kronecker(std::complex<double>* mat_a, int columnas_a, int filas_a, std::complex<double>*mat_b, int columnas_b, int filas_b){
	//*mat_a es pointer al primer elemento del vector de la matriz A
	//columnas_a y fila_a son el número de columnas y filas de la matriz A
	//*mat_b es pointer al primer elemento del vector de la matriz B
        //columnas_b y fila _b son el numero de columnas y filas de la matriz B

	// Definiendo el vector unidimensional de salida
	std::complex<double>* resultado= new std::complex<double>[(columnas_a * columnas_b * filas_a * filas_b)];

	for(int i=0; i<filas_a; i++){
		for(int j=0; j<columnas_a; j++){
			for(int k=0 ; k<filas_b; k++){
				for(int l=0; l<columnas_b; l++){
					resultado[l+j*columnas_b+(k+i*filas_b)*(columnas_a*columnas_b)] = mat_a[j+i*columnas_a]*mat_b[l+k*columnas_b];
				}
			}
		}
	}
	return resultado;
}

int main(){
	//definiendo las matrices
	std::complex<double> A[4] = {std::complex<double>(0,3), std::complex<double>(7,4),std::complex<double>(1,6),std::complex<double>(4,1)};
	std::complex<double> B[4] = {std::complex<double>(9,4),std::complex<double>(1,1),std::complex<double>(7,1),std::complex<double>(6,4)};

	//llamando el metodo
	std::complex<double>* producto =  kronecker(A, 2, 2, B, 2, 2);

	//imprimiendo en la pantalla
        for (int i =0; i<4; i++){
                //loop de las filas de B
                for(int j = 0; j<4; j++){
                    std::cout<<*(producto+j+i*4)<<" ";
                }
		std::cout << std::endl;
        }

    	delete[] producto;
	return 0;
}
