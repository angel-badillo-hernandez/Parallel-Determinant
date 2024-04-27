#include <iostream>
#include <cstddef>
#include <vector>
using namespace std;

// Represents a 2-D vector of integers, i.e a 2-D dynamic array that can
// change in size.
using Matrix_t = vector<vector<float>>;
// Represents a row vector of integers, i.e a 1-D array that can change
// in size.
using Row_t = vector<float>;

const size_t matrix_size = 3;

void lu_decomposition(const Matrix_t &matrix, Matrix_t &lower_matrix, Matrix_t &upper_matrix);

float determinant_triangular(const Matrix_t &matrix);

void print_matrix(const Matrix_t &matrix);

int main()
{
    float determinant, l_det, u_det;

    Matrix_t matrix = {
        {2, -1, -2},
        {-4, 6, 3},
        {-4, -2, 8}};

    // Initializing output matrices
    Matrix_t lower(matrix_size, Row_t(matrix_size, 0));
    Matrix_t upper(matrix_size, Row_t(matrix_size, 0));

    lu_decomposition(matrix, lower, upper);

    cout << "Original Matrix" << '\n';
    print_matrix(matrix);

    cout << "Lower Triangular Matrix" << "\n\n";
    print_matrix(lower);

    cout << "Upper Triangular Matrix" << '\n';
    print_matrix(upper);

    l_det = determinant_triangular(lower);
    u_det = determinant_triangular(upper);
    determinant = l_det * u_det;

    cout << "det(L) = " << l_det << '\n';
    cout << "det(U) = " << u_det << '\n';
    cout << "det(A) = " << determinant << '\n';

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
            float sum = 0;
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
                float sum = 0;
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