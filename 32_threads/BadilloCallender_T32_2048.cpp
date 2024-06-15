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
#include <thread>
#include <utility>
#include <functional>
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
const size_t matrix_size = 2048;
// Number of threads
const size_t num_threads = 32;

/// @brief Performs a part of LU decomposition, for a given thread.
/// @param threadId Index of the thread
/// @param matrix Square matrix to decompose.
/// @param lower_matrix Lower triangular matrix.
/// @param upper_matrix Upper triangular matrix.
void lu_decomposition(const size_t threadId, const Matrix_t &matrix, Matrix_t &lower_matrix, Matrix_t &upper_matrix);

/// @brief Computes the determinant of a triangular matrix.
/// @param matrix Triangular matrix.
/// @return The determinant, a double.
double determinant_triangular(const Matrix_t &matrix);

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
    double determinant, l_det, u_det;

    // Matrix to decompose
    Matrix_t matrix = random_matrix(matrix_size, -1.0, 1.0);

    // Lower and upper triangular matrices
    Matrix_t lower(matrix_size, Row_t(matrix_size, 0));
    Matrix_t upper(matrix_size, Row_t(matrix_size, 0));

    // Vector to store threads
    vector<thread> threads;

    // Begin timing
    auto start_time = std::chrono::steady_clock::now();

    // Creating threads and storing them in the vector of threads
    for (size_t i = 0; i < num_threads; i++)
    {
        // Create thread
        thread temp_thread = thread(lu_decomposition, i, cref(matrix), ref(lower), ref(upper));
        // Move contents of temp_thread into threads vector,
        // instead of copying.
        threads.push_back(move(temp_thread));
    }

    // Join threads for synchronization
    for (size_t i = 0; i < num_threads; i++)
    {
        threads[i].join();
    }

    // Compute determinant
    l_det = determinant_triangular(lower);
    u_det = determinant_triangular(upper);
    
    // det(A) = det(U) since the main diagonal of L is all 1's
    // and therefore does not contribute to the product
    determinant = u_det;

    // Stop timing
    auto end_time = std::chrono::steady_clock::now();

    // Calculate elapsed time
    auto elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(end_time - start_time).count();

    // Print results
    cout << "Matrix Order: " << matrix_size << '\n';
    
    cout << "Elapsed time (nanoseconds): " << elapsed << '\n';
    cout << "ln|det(L)| = " << l_det << '\n';
    cout << "ln|det(U)| = " << u_det << '\n';
    cout << "ln|det(A)| = " << determinant << '\n';

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

    // Set elements across diagonal to 1
    for (size_t i = 0; i < size; i++)
    {
        matrix[i][i] = 1;
    }

    return matrix;
}

void lu_decomposition(const size_t threadId, const Matrix_t &matrix, Matrix_t &lower_matrix, Matrix_t &upper_matrix)
{
    // N from NxN sized matrix
    size_t size = matrix.size();

    // Starting index for the outer for loop
    size_t start = threadId * size / num_threads;
    // One over the last index for the outer for loop
    size_t end = (threadId + 1) * size / num_threads;

    for (size_t i = start; i < end; i++)
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

double determinant_triangular(const Matrix_t &matrix)
{
    size_t size = matrix.size();

    // 0x0 matrix has determinant of 1
    // or a ln(det) = 0
    double determinant = 0;

    // Compute determinant of triangular matrix
    // by multiplying across the diagonals

    // Here, we compute the natural log of the absolute
    // value of the determinant because of data type
    // range constraints
    for (size_t i = 0; i < size; i++)
    {
        determinant += log(abs(matrix[i][i]));
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