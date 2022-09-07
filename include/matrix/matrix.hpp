// GOSDT algorithm
//
//
// MIT License
//
// Copyright (c) 2022 Systopia Lab, Computer Science, University of British Columbia
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.


// This is a very quickly written, untested, and simple Matrix class
// that will hopefully be used in our code to replace the instances of
// vector<vector<float>> (Or any other such similar type)
// NOTE: Memory allocation is handled by the default cpp allocator, this could
// be changed later if it makes sense to do so.
// NOTE: Matrix uses the () operator for subscripting, I don't love this but
// without using C++23, [] must be a binary operator. WEIRD!
// TODO write some tests for this class.

#pragma once

#include <cstddef>
#include <cassert>

template<class T>
struct Matrix {
    size_t n_rows;
    size_t n_columns;

    // We want to make sure that we allocate this memory contiguously and to allow
    // for better cache coherence in row major order.
    T * data;

    Matrix();
    Matrix(size_t n_rows, size_t n_columns, const T& initial_value);
    Matrix(const Matrix& other);
    Matrix(Matrix&& other) noexcept;
    ~Matrix();

    Matrix& operator=(const Matrix& other);
    Matrix& operator=(Matrix&& other) noexcept;

    T operator()(std::size_t row_index, std::size_t column_index) const;
    T& operator()(std::size_t row_index, std::size_t column_index);

};

template<class T>
Matrix<T>::Matrix()
: n_rows(0), n_columns(0), data(nullptr)
{}

template<class T>
Matrix<T>::Matrix(size_t n_rows, size_t n_columns, const T& initial_value)
: n_rows(n_rows), n_columns(n_columns)
{
    assert(n_rows > 0 && n_columns > 0);
    data = new T[n_rows * n_columns];
    for (auto i = 0u; i < n_rows; i++) {
        for (auto j = 0u; j < n_columns; j++) {
            data[i * n_columns + j] = initial_value;
        }
    }
}

template<class T>
Matrix<T>::~Matrix()
{
    delete[] data;
}

template<class T>
Matrix<T>::Matrix(const Matrix& other)
: n_rows(other.n_rows), n_columns(other.n_columns), data(new T[n_rows * n_columns])
{
    for (std::size_t i = 0; i < n_rows * n_columns; i++)
    {
        data[i] = other.data[i];
    }
}

template<class T>
Matrix<T>::Matrix(Matrix&& other) noexcept
: data(nullptr), n_rows(0), n_columns(0)
{
    data = other.data;
    n_rows = other.n_rows;
    n_columns = other.n_columns;
    other.data = nullptr;
    other.n_rows = 0;
    other.n_columns = 0;
}

template<class T>
Matrix<T>&
Matrix<T>::operator=(const Matrix& other)
{
    if (this == &other) return *this;
    delete[] data;
    n_rows = other.n_rows;
    n_columns = other.n_columns;
    data = new T[n_rows * n_columns];
    for (std::size_t i = 0; i < n_rows; i++)
    {
        for (std::size_t j = 0; j < n_columns; j++)
        {
            data[i * n_columns + j] = other.data[i * n_columns + j];
        }
    }
    return *this;
}

template<class T>
Matrix<T>&
Matrix<T>::operator=(Matrix&& other) noexcept
{
    delete[] data;
    n_rows = other.n_rows;
    n_columns = other.n_columns;
    data = other.data;
    other.data = nullptr;
    other.n_rows = 0;
    other.n_columns = 0;
    return *this;
}


template<class T>
T
Matrix<T>::operator()(std::size_t row_index, std::size_t column_index) const
{
    assert(row_index < n_rows && column_index < n_columns);
    return data[row_index * n_columns + column_index];
}

template<class T>
T&
Matrix<T>::operator()(std::size_t row_index, std::size_t column_index)
{
    assert(row_index < n_rows && column_index < n_columns);
    return data[row_index * n_columns + column_index];
}

