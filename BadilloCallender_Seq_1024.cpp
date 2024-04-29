/**
 * Team Members: Angel Badillo, Chad Callender
 * Final Project: Calculating the Determinant
 * 
 * Description:
 * Text here.
 * 
 * Compilation Instructions:
 * 
 * Execution Instructions:
 * 
 */
#include <iostream>
#include <cstddef>
#include <vector>
#include <random>
#include <limits>
#include <ctime>
#include <chrono>
using namespace std;

// Represents a 2-D vector of integers, i.e a 2-D dynamic array that can
// change in size.
using Matrix_t = vector<vector<double>>;
// Represents a row vector of integers, i.e a 1-D array that can change
// in size.
using Row_t = vector<double>;

// Size N for NxN square matrix.
const size_t matrix_size = 32;

/// @brief Performs LU Decomposition using the Doolittle Algorithm.
/// @param matrix Matrix of size NxN to decompose.
/// @param lower_matrix Lower triangular matrix.
/// @param upper_matrix Upper triangular matrix.
void lu_decomposition(const Matrix_t &matrix, Matrix_t &lower_matrix, Matrix_t &upper_matrix);

/// @brief Computes the determinant of a triangular matrix.
/// @param matrix Triangular matrix.
/// @return The determinant, a float.
float determinant_triangular(const Matrix_t &matrix);

/// @brief Creates an NxN identity matrix.
/// @param size N, size of identity matrix.
/// @return Matrix_t, the identity matrix.
Matrix_t identity_matrix(const size_t size);

/// @brief Creates an NxN matrix with random values.
/// @param size N, size of the matrix.
/// @param min_val Minimum value for the random number.
/// @param max_val Maximum value for the random number.
/// @return 
Matrix_t random_matrix(const size_t size, const double min_val, const double max_val);

/// @brief Prints out the contents of a matrix.
/// @param matrix 
void print_matrix(const Matrix_t &matrix);

int main()
{
    // Determinants for original matrix, lower triangular matrix, and upper triangular matrix
    float determinant, l_det, u_det;

    // Matrix to decompose
    Matrix_t matrix = random_matrix(matrix_size, -1.0, 1.0);

    // Lower and upper triangular matrices
    Matrix_t lower(matrix_size, Row_t(matrix_size, 0));
    Matrix_t upper(matrix_size, Row_t(matrix_size, 0));

    // Begin timing
    auto start_time = std::chrono::steady_clock::now();

    // Perform LU decomposition
    lu_decomposition(matrix, lower, upper);

    // Comput determinant
    l_det = determinant_triangular(lower);
    u_det = determinant_triangular(upper);
    determinant = l_det * u_det;

    // Stop timing
    auto end_time = std::chrono::steady_clock::now();

    // Calculate elapsed time
    auto elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(end_time - start_time).count();

    // Print results
    cout << "Elapsed time (nanoseconds): " << elapsed << '\n';
    cout << "det(L) = " << l_det << '\n';
    cout << "det(U) = " << u_det << '\n';
    cout << "det(A) = " << determinant << '\n';

    return 0;
}

Matrix_t random_matrix(const size_t size, const double min_val, const double max_val)
{
    // Setup up random number generation
    default_random_engine generator((long unsigned int)time(0));
    uniform_real_distribution<double> dist(min_val, max_val);

    Matrix_t matrix(size, Row_t(size));

    // Initialize matrix with random numbers
    for (auto &row : matrix)
    {
        for (auto &value : row)
        {
            value = dist(generator);
        }
    }

    return matrix;
}

Matrix_t identity_matrix(const size_t size)
{
    // Create square matrix with elements
    // set to 0
    Matrix_t matrix(size, Row_t(size, 0));

    for (size_t i = 0; i < size; i++)
    {
        matrix[i][i] = 1;
    }

    return matrix;
}

void lu_decomposition(const Matrix_t &matrix, Matrix_t &lower_matrix, Matrix_t &upper_matrix)
{
    size_t size = matrix.size();

    for (size_t i = 0; i < size; i++)
    {
        // Upper triangular
        for (size_t k = i; k < size; k++)
        {
            // Summation of L(i, j) * U(j, k)
            double sum = 0;
            for (size_t j = 0; j < i; j++)
            {
                sum += (lower_matrix[i][j] * upper_matrix[j][k]);
            }
            // Evaluating U(i, k)
            upper_matrix[i][k] = matrix[i][k] - sum;
        }

        // Lower triangular
        for (size_t k = i; k < size; k++)
        {
            // Diagonal as 1
            if (i == k)
            {
                lower_matrix[i][i] = 1;
            }
            else
            {
                // Summation of L(k, j) * U(j, i)
                double sum = 0;
                for (size_t j = 0; j < i; j++)
                {
                    sum += (lower_matrix[k][j] * upper_matrix[j][i]);
                }

                // Evaluating L(k, i)
                lower_matrix[k][i] = (matrix[k][i] - sum) / upper_matrix[i][i];
            }
        }
    }
}

float determinant_triangular(const Matrix_t &matrix)
{
    size_t size = matrix.size();

    // 0x0 matrix has determinant of 1
    float determinant = 1;

    // Compute determinant of triangular matrix
    for (size_t i = 0; i < size; i++)
    {
        determinant *= matrix[i][i];
    }

    return determinant;
}

void print_matrix(const Matrix_t &matrix)
{
    for (auto &&row : matrix)
    {
        for (auto &&value : row)
        {
            cout << value << ' ';
        }
        cout << '\n';
    }
}