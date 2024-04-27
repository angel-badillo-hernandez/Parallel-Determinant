#include <iostream>
#include <cstddef>
#include <vector>
#include <thread>
#include <future>
#include <functional>
using namespace std;
using Matrix_t = vector<vector<float>>;
using Row_t = vector<float>;

const size_t matrix_size = 3;
const size_t num_threads = 3;

void lu_decomposition(const size_t threadId, const Matrix_t &matrix, Matrix_t &lower_matrix, Matrix_t &upper_matrix)
{
    size_t size = matrix.size();
    size_t start = threadId * size / num_threads;
    size_t end = (threadId + 1) * size / num_threads;

    for (size_t i = start; i < end; i++)
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

int main()
{
    float determinant, l_det, u_det;

    Matrix_t matrix = {
        {2, -1, -2},
        {-4, 6, 3},
        {-4, -2, 8}};

    Matrix_t lower(matrix_size, Row_t(matrix_size, 0));
    Matrix_t upper(matrix_size, Row_t(matrix_size, 0));

    vector<thread> threads;

    for (size_t i = 0; i < num_threads; i++)
    {
        threads.push_back(thread(lu_decomposition, i, cref(matrix), ref(lower), ref(upper)));
    }

    for (size_t i = 0; i < num_threads; i++)
    {
        threads[i].join();
    }

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