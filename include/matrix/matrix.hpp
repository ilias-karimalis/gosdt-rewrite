// Ilias Karimalis - 2022


// This is a very quickly written, untested, and simple Matrix class
// that will hopefully be used in our code to replace the instances of
// vector<vector<float>> (Or any other such similar type)
// NOTE: Memory allocation is handled by the default cpp allocator, this could
// be changed later if it makes sense to do so.
// NOTE: Matrix uses the () operator for subscripting, I don't love this but
// without using C++23, [] must be a binary operator. WEIRD!


#ifndef MATRIX_HPP
#define MATRIX_HPP

#include <cstddef>

template<class T>
struct Matrix {


    [[maybe_unused]] size_t n_rows;
    size_t n_columns;

    // We want to make sure that we allocate this memory contiguously and to allow
    // for better cache coherence in row major order.
    T * data;

    Matrix() = default;
    ~Matrix() = default;
    Matrix(size_t n_rows, size_t n_columns, const T& initial_value);
    T operator()(std::size_t row_index, std::size_t column_index) const;
    T& operator()(std::size_t row_index, std::size_t column_index);

};

template<class T>
Matrix<T>::Matrix(size_t n_rows, size_t n_columns, const T& initial_value)
: n_rows(n_rows), n_columns(n_columns)
{
    assert(n_rows > 0);
    assert(n_columns > 0);
    data = new T[n_rows * n_columns];

    for (auto i = 0u; i < n_rows; i++) {
        for (auto j = 0u; j < n_columns; j++) {
            data[i * n_columns + j] = initial_value;
        }
    }
}

template<class T>
T Matrix<T>::operator()(std::size_t row_index, std::size_t column_index) const {
    return data[row_index * n_columns + column_index];
}

template<class T>
T& Matrix<T>::operator()(std::size_t row_index, std::size_t column_index) {
    return data[row_index * n_columns + column_index];
}

#endif
