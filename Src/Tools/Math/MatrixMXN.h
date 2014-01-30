/**
* @file MatrixMXN.h
* Contains template class MatrixMXN of type V and size mxn
* @author <a href="mailto:martin.kallnik@gmx.de">Martin Kallnik</a>
* @author Max Risler
* @author Colin Graf
*/

#ifndef MatrixMXN_H
#define MatrixMXN_H

#include "Vector.h"

/** This class represents a mxn-MatrixMXN */
template <int m = 2, int n = 2, class V = float> class MatrixMXN
{
public:
	/** The columns of the MatrixMXN */
	Vector<m, V> c[n];

	/** Default constructor. */
	MatrixMXN<m, n, V>() {}

	MatrixMXN<m, n, V>(V v)
	{
		ASSERT(m == n);
		const int mnm = n < m ? n : m;
		for(int i = 0; i < mnm; ++i)
			c[i][i] = v;
	}

	/** Constructor */
	MatrixMXN<m, n, V>(const Vector<m, V>& c0, const Vector<m, V>& c1)
	{
		ASSERT(n == 2);
		c[0] = c0;
		c[1] = c1;
	}

	/** Constructor */
	MatrixMXN<m, n, V>(const Vector<m, V>& c0, const Vector<m, V>& c1, const Vector<m, V>& c2)
	{
		ASSERT(n == 3);
		c[0] = c0;
		c[1] = c1;
		c[2] = c2;
	}

	/** Constructor */
	MatrixMXN<m, n, V>(const Vector<m, V>& c0, const Vector<m, V>& c1, const Vector<m, V>& c2, const Vector<m, V>& c3)
	{
		ASSERT(n == 4);
		c[0] = c0;
		c[1] = c1;
		c[2] = c2;
		c[3] = c3;
	}

	/** Constructor */
	/*
	MatrixMXN<m, n, V>(const Vector<m, V>& c0, const Vector<m, V>& c1, const Vector<m, V>& c2, const Vector<m, V>& c3, ...)
	{
	c[0] = c0;
	c[1] = c1;
	c[2] = c2;
	c[3] = c3;
	va_list vl;
	va_start(vl, c3);
	for(int i = 4; i < n; ++i)
	(*this)[i] = va_arg(vl, const Vector<m, V>&);
	va_end(vl);
	}
	*/

	/**
	* Assignment operator
	* @param other The other MatrixMXN that is assigned to this one
	* @return A reference to this object after the assignment.
	*/
	MatrixMXN<m, n, V>& operator=(const MatrixMXN<m, n, V>& other)
	{
		for(int i = 0; i < n; ++i)
			c[i] = other.c[i];
		return *this;
	}

	/**
	* Array-like member access.
	* @param i index
	* @return reference to column
	*/
	inline Vector<m, V>& operator[](int i) 
	{
		return c[i];
	}

	/**
	* const array-like member access.
	* @param i index
	* @return reference to column
	*/
	inline const Vector<m, V>& operator[](int i) const
	{
		return c[i];
	}

	/**
	* Multiplication of this MatrixMXN by vector.
	* @param vector The vector this one is multiplied by 
	* @return A reference to a new vector containing the result of the calculation.
	*/
	Vector<m, V> operator*(const Vector<n, V>& vector) const
	{
		Vector<m, V> result = c[0] * vector[0];
		for(int i = 1; i < n; ++i)
			result += c[i] * vector[i];
		return result;
	}

	/**
	* Multiplication of this MatrixMXN by another MatrixMXN.
	* @param other The other MatrixMXN this one is multiplied by 
	* @return An object containing the result of the calculation.
	*/
	template<int o> MatrixMXN<m, o, V> operator*(const MatrixMXN<n, o, V>& other) const
	{
		MatrixMXN<m, o, V> result;
		for(int i = 0; i < n; ++i)
			for(int j = 0; j < m; ++j)
				for(int k = 0; k < o; ++k)
					result.c[k][j] += c[i][j] * other.c[k][i];
		return result;
	}

	/**
	* Multiplication of this MatrixMXN by another MatrixMXN.
	* @param other The other MatrixMXN this one is multiplied by 
	* @return A reference this object after the calculation.
	*/ 
	MatrixMXN<n, n, V> operator*=(const MatrixMXN<n, n, V>& other)
	{
		return *this = *this * other;
	}

	/**
	* Multiplication of this MatrixMXN by a factor.
	* @param factor The factor this MatrixMXN is multiplied by 
	* @return A reference to this object after the calculation.
	*/
	MatrixMXN<m, n, V>& operator*=(const V& factor)
	{
		for(int i = 0; i < n; ++i)
			c[i] *= factor;
		return *this;
	}

	/**
	* Multiplication of this MatrixMXN by a factor.
	* @param factor The factor this MatrixMXN is multiplied by 
	* @return A new object that contains the result of the calculation.
	*/
	MatrixMXN<m, n, V> operator*(const V& factor) const
	{
		return MatrixMXN<m, n, V>(*this) *= factor;
	}

	/**
	* Division of this MatrixMXN by a factor.
	* @param factor The factor this MatrixMXN is divided by 
	* @return A reference to this object after the calculation.
	*/
	MatrixMXN<m, n, V>& operator/=(const V& factor)
	{
		for(int i = 0; i < n; ++i)
			c[i] /= factor;
		return *this;
	}

	/**
	* Division of this MatrixMXN by a factor.
	* @param factor The factor this MatrixMXN is divided by 
	* @return A new object that contains the result of the calculation.
	*/
	MatrixMXN<m, n, V> operator/(const V& factor) const
	{
		return MatrixMXN<m, n, V>(*this) /= factor;
	}

	/**
	* Adds another MatrixMXN. 
	* @param other The other MatrixMXN that is added to this one
	* @return A reference to this object after the calculation.
	*/
	MatrixMXN<m, n, V>& operator+=(const MatrixMXN<m, n, V>& other)
	{
		for(int i = 0; i < n; ++i)
			c[i] += other.c[i];
		return *this;
	}

	/**
	* Computes the sum of two matrices
	* @param other Another MatrixMXN
	* @return The sum
	*/
	MatrixMXN<m, n, V> operator+(const MatrixMXN<m, n, V>& other) const
	{
		return MatrixMXN<m, n, V>(*this) += other;
	}

	/**
	* Subtracts another MatrixMXN.
	* @param other The other MatrixMXN that is subtracted from this one
	* @return A reference to this object after the calculation.
	*/
	MatrixMXN<m, n, V>& operator-=(const MatrixMXN<m, n, V>& other)
	{
		for(int i = 0; i < n; ++i)
			c[i] -= other.c[i];
		return *this;
	}

	/**
	* Computes the difference of two matrices
	* @param other Another MatrixMXN
	* @return The difference
	*/
	MatrixMXN<m, n, V> operator-(const MatrixMXN<m, n, V>& other) const
	{
		return MatrixMXN<m, n, V>(*this) -= other;
	}

	/**
	* Comparison of another MatrixMXN with this one.
	* @param other The other MatrixMXN that will be compared to this one
	* @return Whether the two matrices are equal.
	*/
	bool operator==(const MatrixMXN<m, n, V>& other) const
	{
		for(int i = 0; i < n; ++i)
			if(c[i] != other.c[i])
				return false;
		return true;
	}

	/**
	* Comparison of another MatrixMXN with this one.
	* @param other The other MatrixMXN that will be compared to this one
	* @return Whether the two matrixs are unequal.
	*/
	bool operator!=(const MatrixMXN<m, n, V>& other) const
	{
		for(int i = 0; i < n; ++i)
			if(c[i] != other.c[i])
				return true;
		return false;
	}

	/**
	* Transpose the MatrixMXN
	* @return A new object containing transposed MatrixMXN
	*/
	MatrixMXN<n, m, V> transpose() const
	{
		MatrixMXN<n, m, V> result;
		for(int i = 0; i < n; ++i)
			for(int j = 0; j < m; ++j)
				result.c[j][i] = c[i][j];
		return result;
	}

	/**
	* Calculation of the determinant of this MatrixMXN.
	* @return The determinant.
	*/
	V det() const;

	/**
	* Calculate the adjoint of this MatrixMXN.
	* @return the adjoint MatrixMXN.
	*/
	MatrixMXN<m, n, V> adjoint() const;

	/**
	* Calculate the inverse of this MatrixMXN.
	* @return The inverse MatrixMXN
	*/
	MatrixMXN<m, n, V> invert() const;
};

/**
* Streaming operator that reads a MatrixMXN<m, n, V> from a stream.
* @param stream The stream from which is read.
* @param MatrixMXN The MatrixMXN<m, n, V> object.
* @return The stream.
*/ 
//template <int m, int n, class V> In& operator>>(In& stream, MatrixMXN<m, n, V>& MatrixMXN)
//{
//	STREAM_REGISTER_BEGIN_EXT(MatrixMXN);
//	STREAM_ARRAY_EXT(stream, MatrixMXN.c);
//	STREAM_REGISTER_FINISH();
//	return stream;
//}
//
///**
//* Streaming operator that writes a MatrixMXN<m, n, V> to a stream.
//* @param stream The stream to write on.
//* @param MatrixMXN The MatrixMXN<m, n, V> object.
//* @return The stream.
//*/
//template <int m, int n, class V> Out& operator<<(Out& stream, const MatrixMXN<m, n, V>& MatrixMXN)
//{
//	STREAM_REGISTER_BEGIN_EXT(MatrixMXN);
//	STREAM_ARRAY_EXT(stream, MatrixMXN.c);
//	STREAM_REGISTER_FINISH();
//	return stream;
//}

//
typedef MatrixMXN<2, 2, float> Matrix2x2f;
typedef MatrixMXN<3, 3, float> Matrix3x3f;
typedef MatrixMXN<4, 4, float> Matrix4x4f;
typedef MatrixMXN<4, 2, float> Matrix4x2f;
typedef MatrixMXN<2, 4, float> Matrix2x4f;

#endif // Matrix_H