/** 
* @file RotationMatrix.cpp
* Implementation of some functions of the MatrixMXN class for various types.
* @author <a href="mailto:martin.kallnik@gmx.de">Martin Kallnik</a>
* @author Max Risler
* @author Colin Graf
*/

#include "Platform/GTAssert.h"
#include "MatrixMXN.h"

template <> MatrixMXN<1, 1, float> MatrixMXN<1, 1, float>::invert() const
{
	return MatrixMXN<1, 1, float>(1.0f / c[0][0]);
}

template <> float MatrixMXN<2, 2, float>::det() const
{
	return c[0].x * c[1].y - c[1].x * c[0].y;
}

template <> MatrixMXN<2, 2, float> MatrixMXN<2, 2, float>::invert() const
{
	float factor(1.0f / det());
	return MatrixMXN<2, 2, float>(Vector<2, float>(factor * c[1].y, -factor * c[0].y), Vector<2, float>(-factor * c[1].x, factor * c[0].x));
}

template <> float MatrixMXN<3, 3, float>::det() const 
{ 
	return 
		c[0].x * (c[1].y * c[2].z - c[1].z * c[2].y) +
		c[0].y * (c[1].z * c[2].x - c[1].x * c[2].z) +
		c[0].z * (c[1].x * c[2].y - c[1].y * c[2].x);
}

template <class V> static V det2(V a, V b, V c, V d)
{
	return a * d - b * c;
}

template <> MatrixMXN<3, 3, float> MatrixMXN<3, 3, float>::adjoint() const
{
	return MatrixMXN<3, 3, float>(
		Vector<3, float>(
		det2(c[1].y, c[2].y, c[1].z, c[2].z), 
		det2(c[2].x, c[1].x, c[2].z, c[1].z), 
		det2(c[1].x, c[2].x, c[1].y, c[2].y)
		),
		Vector<3, float>(
		det2(c[2].y, c[0].y, c[2].z, c[0].z), 
		det2(c[0].x, c[2].x, c[0].z, c[2].z), 
		det2(c[2].x, c[0].x, c[2].y, c[0].y)
		),
		Vector<3, float>(
		det2(c[0].y, c[1].y, c[0].z, c[1].z), 
		det2(c[1].x, c[0].x, c[1].z, c[0].z), 
		det2(c[0].x, c[1].x, c[0].y, c[1].y)      
		)
		);
}  

template <> MatrixMXN<3, 3, float> MatrixMXN<3, 3, float>::invert() const
{
	return adjoint().transpose() / det();
}


template <int m, int n, class V> V MatrixMXN<m, n, V>::det() const 
{
	ASSERT(false); // not implemented
	return V(0);
}


template <int m, int n, class V> MatrixMXN<m, n, V> MatrixMXN<m, n, V>::adjoint() const
{
	ASSERT(false); // not implemented
	return MatrixMXN<m, n, V>();
}

template <int m, int n, class V> MatrixMXN<m, n, V> MatrixMXN<m, n, V>::invert() const
{
	MatrixMXN<n, n, V> left(*this);
	MatrixMXN<n, n, V> right(V(1)); // I
	Vector<n, int> ranking;

	for(int i = 0; i < n; ++i)
		ranking[i] = i;

	int r, r2, maxrow;
	for (r = 0; r < n - 1; ++r)
	{
		// find highest value
		V maxval = left[ranking[r]][r];
		maxrow = r;
		if (maxval < V(0))
			maxval = -maxval;
		for (r2 = r+1; r2 < n; ++r2)
		{
			float val = left[ranking[r2]][r];
			if (val < V(0))
				val = -val;
			if (val > maxval)
			{
				maxval = val;
				maxrow = r2;
			}
		}
		// swap rows
		int temp = ranking[r];
		ranking[r] = ranking[maxrow];
		ranking[maxrow] = temp;
		/*
		if (MVTools::isNearZero(left[ranking[r]][r]))
		{
		if (MVTools::isNearNegZero(left[ranking[r]][r]))
		throw MVException(MVException::DivByNegZero);
		else
		throw MVException(MVException::DivByPosZero);
		}
		*/

		for (r2 = r+1; r2 < n; ++r2)
		{
			// calc factor for subtracting
			ASSERT(left[ranking[r]][r] != 0.0f);
			V factor = left[ranking[r2]][r] / left[ranking[r]][r];
			/*
			if (MVTools::isNearInf(factor))
			{
			if (MVTools::isNearPosInf(factor))
			throw MVException(MVException::PosInfValue);
			else
			throw MVException(MVException::NegInfValue);
			}
			*/

			// change left MatrixMXN
			left[ranking[r2]] -= left[ranking[r]] * factor;

			// change right MatrixMXN
			right[ranking[r2]] -= right[ranking[r]] * factor;
		}
	}
	// MatrixMXN has triangle form
	// bring to diagonal form
	for (r = n - 1; r > 0; --r)
	{
		/*
		if (MVTools::isNearZero(left[ranking[r]][r]))
		{
		if (MVTools::isNearNegZero(left[ranking[r]][r]))
		throw MVException(MVException::DivByNegZero);
		else
		throw MVException(MVException::DivByPosZero);
		}
		*/
		for (r2 = r-1; r2 >= 0; --r2)
		{
			ASSERT(left[ranking[r]][r] != 0.0f);
			V factor = left[ranking[r2]][r] / left[ranking[r]][r];
			/*
			if (MVTools::isNearInf(factor))
			{
			if (MVTools::isNearPosInf(factor))
			throw MVException(MVException::PosInfValue);
			else
			throw MVException(MVException::NegInfValue);
			}
			*/

			// change left MatrixMXN
			left[ranking[r2]] -= left[ranking[r]] * factor;

			// change right MatrixMXN
			right[ranking[r2]] -= right[ranking[r]] * factor;
		}
	}
	// MatrixMXN has diagonal form
	// set entries of left MatrixMXN to 1 and apply multiplication to right
	MatrixMXN<n, n, V> res;
	for (r = 0; r < n; ++r)
	{
		res[r] = right[ranking[r]];
		ASSERT(left[ranking[r]][r] != 0.f);
		/*
		if (MVTools::isNearZero(left[ranking[r]][r]))
		{
		if (MVTools::isNearNegZero(left[ranking[r]][r]))
		throw MVException(MVException::DivByNegZero);
		else
		throw MVException(MVException::DivByPosZero);
		}
		*/
		res[r] /= left[ranking[r]][r];
	}
	return res;
}


// explicit template instantiation
template class MatrixMXN<1, 1, float>;
template class MatrixMXN<2, 2, float>;
template class MatrixMXN<3, 3, float>;
template class MatrixMXN<4, 4, float>;