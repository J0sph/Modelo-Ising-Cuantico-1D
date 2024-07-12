#include <iostream>
#include <vector>
#include <Eigen/Dense>

// Función para calcular el producto de Kronecker entre dos matrices representadas como vectores unidimensionales de reales.
double* kronecker(double* mat_a, int columnas_a, int filas_a, double* mat_b, int columnas_b, int filas_b) {
    double* resultado = new double[columnas_a * columnas_b * filas_a * filas_b];

    for (int i = 0; i < filas_a; i++) {
        for (int j = 0; j < columnas_a; j++) {
            for (int k = 0; k < filas_b; k++) {
                for (int l = 0; l < columnas_b; l++) {
                    resultado[l + j * columnas_b + (k + i * filas_b) * (columnas_a * columnas_b)] = mat_a[j + i * columnas_a] * mat_b[l + k * columnas_b];
                }
            }
        }
    }
    return resultado;
}

// Función para convertir un array unidimensional a Eigen::MatrixXd
Eigen::MatrixXd array_to_matrix(double* arr, int rows, int cols) {
    Eigen::MatrixXd mat(rows, cols);
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            mat(i, j) = arr[i * cols + j];
        }
    }
    return mat;
}

// Función para generar las matrices sigma_z^(i) y sigma_x^(i) para N espines
std::vector<Eigen::MatrixXd> S(int N, double* s, int s_size) {
    std::vector<Eigen::MatrixXd> S;
    int s_dim = sqrt(s_size);

    for (int i = 0; i < N; ++i) {
        int left_dim = 1 << i;
        int right_dim = 1 << (N - i - 1);

        Eigen::MatrixXd identidad_izquierda = Eigen::MatrixXd::Identity(left_dim, left_dim);
        Eigen::MatrixXd identidad_derecha = Eigen::MatrixXd::Identity(right_dim, right_dim);

        std::vector<double> identidad_izquierda_vec(identidad_izquierda.data(), identidad_izquierda.data() + left_dim * left_dim);
        std::vector<double> identidad_derecha_vec(identidad_derecha.data(), identidad_derecha.data() + right_dim * right_dim);

        double* temp_kron_left = kronecker(identidad_izquierda_vec.data(), left_dim, left_dim, s, s_dim, s_dim);
        double* temp_kron = kronecker(temp_kron_left, left_dim * s_dim, left_dim * s_dim, identidad_derecha_vec.data(), right_dim, right_dim);

        Eigen::MatrixXd s_i = array_to_matrix(temp_kron, left_dim * s_dim * right_dim, left_dim * s_dim * right_dim);

        S.push_back(s_i);

        delete[] temp_kron_left;
        delete[] temp_kron;
    }
    return S;
}

// Función para calcular el Hamiltoniano de un sistema de N espines
Eigen::MatrixXd hamiltonian(double J, double g, int N) {
    double sx[4] = {0, 1, 1, 0};
    double sz[4] = {1, 0, 0, -1};

    std::vector<Eigen::MatrixXd> Sz = S(N, sz, 4);
    std::vector<Eigen::MatrixXd> Sx = S(N, sx, 4);

    int dim = 1 << N;
    Eigen::MatrixXd suma_z = Eigen::MatrixXd::Zero(dim, dim);
    Eigen::MatrixXd suma_x = Eigen::MatrixXd::Zero(dim, dim);

    if (N > 2) {
        for (int i = 0; i < N; ++i) {
            suma_z += Sz[i] * Sz[(i + 1) % N];
            suma_x += Sx[i];
        }
    } else {
        for (int i = 0; i < N - 1; ++i) {
            suma_z += Sz[i] * Sz[i + 1];
        }
        for (int i = 0; i < N; ++i) {
            suma_x += Sx[i];
        }
    }

    Eigen::MatrixXd H = -J * suma_z - g * suma_x;
    return H;
}

int main(int argc, char* argv[]) {
    if (argc != 4) {
        std::cerr << "Uso: " << argv[0] << " <N> <J> <g>" << std::endl;
        return 1;
    }

    int N = std::stoi(argv[1]);
    double J = std::stod(argv[2]);
    double g = std::stod(argv[3]);

    Eigen::MatrixXd H = hamiltonian(J, g, N);
    std::cout << "Hamiltoniano:" << std::endl << H << std::endl;

    Eigen::SelfAdjointEigenSolver<Eigen::MatrixXd> eigensolver(H);
    if (eigensolver.info() != Eigen::Success) {
        std::cerr << "No se pudo calcular los autovalores." << std::endl;
        return 1;
    }

    std::cout << "Autovalores:" << std::endl << eigensolver.eigenvalues() << std::endl;

    return 0;
}
