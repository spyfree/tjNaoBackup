#ifndef __StreamHandler_h_
#define __StreamHandler_h_

#include "Tools/Streams/InOut.h"
#include <vector>
/**
* Registeres and streams a base class
* @param base A pointer to the base class.
*/
#define STREAM_BASE( base) \
	this-> base ::serialize( in, out);
/**
* Registration and streaming of a member.
* @param member The member to be streamed.
*/
#define STREAM( member) \
	if( in){ \
	*in >> member; \
	}else{ \
	*out << member; \
	}
/**
* Registeres and streams a member.
* @param object Object to be streamed.
*/
#define STREAM_EXT(stream, object) \
	streamObject( stream , object);

#define STREAM_ENUMASINT( s) \
  if( in){ \
  streamEnum( *in , s);\
  }else{ \
  streamEnum( *out , s);\
  }

#define STREAM_ENUM( s, numberOfEnumElements, getNameFunctionPtr) \
	if( in){ \
	streamEnum( *in , s);\
	}else{ \
	streamEnum( *out , s);\
	}

#define STREAM_ARRAY( s) \
	if(in) \
	streamStaticArray( *in, s, sizeof(s)); \
  else \
	streamStaticArray( *out, s, sizeof(s));

#define STREAM_ENUM_ARRAY( s, numberOfEnumElements, getNameFunctionPtr) \
	if( in){ \
	streamStaticEnumArray( *in, s, sizeof(s));\
	}else{ \
	streamStaticEnumArray( *out, s, sizeof(s));\
	}

#define STREAM_DYN_ARRAY( s, count) \
	if(in) \
	streamDynamicArray( *in, s, count); \
  else \
	streamDynamicArray( *out, s, count); 

#define STREAM_VECTOR( s) \
	if(in) \
	streamVector( *in, s); \
  else \
	streamVector( *out, s); 

template<class T>
In& streamComplexStaticArray(In& in, T inArray[], int size)
{
	int numberOfEntries = size / sizeof(T);
	for(int i = 0; i < numberOfEntries; ++i)
		in >> inArray[i];
	return in;
}

template<class T>
Out& streamComplexStaticArray(Out& out, T outArray[], int size)
{
	int numberOfEntries = size / sizeof(T);
	for(int i = 0; i < numberOfEntries; ++i)
		out << outArray[i];
	return out;
}

template<class T>
In& streamBasicStaticArray(In& in, T inArray[], int size)
{
	if(in.isBinary())
	{
		in.read(inArray, size);
		return in;
	}
	else
		return streamComplexStaticArray(in, inArray, size);
}

template<class T>
Out& streamBasicStaticArray(Out& out, T outArray[], int size)
{
	if(out.isBinary())
	{
		out.write(outArray, size);
		return out;
	}
	else
		return streamComplexStaticArray(out, outArray, size);
}

template<class T>
In& streamStaticEnumArray(In& in, T inArray[], int size)
{
	if(in.isBinary())
	{
		in.read(inArray, size);
		return in;
	}
	else
	{
		int numberOfEntries = size / sizeof(T);
		for(int i = 0; i < numberOfEntries; ++i)
		{
			int j;
			in >> j;
			inArray[i] = (T) j;
		}
		return in;
	}
}

template<class T>
Out& streamStaticEnumArray(Out& out, T outArray[], int size)
{
	if(out.isBinary())
	{
		out.write(outArray, size);
		return out;
	}
	else
	{
		int numberOfEntries = size / sizeof(T);
		for(int i = 0; i < numberOfEntries; ++i)
		{
			int j = (int) outArray[i];
			out << j;
		}
		return out;
	}
}

inline In& streamStaticArray(In& in, unsigned char inArray[], int size) {return streamBasicStaticArray(in, inArray, size);}
inline Out& streamStaticArray(Out& out, unsigned char outArray[], int size) {return streamBasicStaticArray(out, outArray, size);}
inline In& streamStaticArray(In& in, char inArray[], int size) {return streamBasicStaticArray(in, inArray, size);}
inline Out& streamStaticArray(Out& out, char outArray[], int size) {return streamBasicStaticArray(out, outArray, size);}
inline In& streamStaticArray(In& in, unsigned short inArray[], int size) {return streamBasicStaticArray(in, inArray, size);}
inline Out& streamStaticArray(Out& out, unsigned short outArray[], int size) {return streamBasicStaticArray(out, outArray, size);}
inline In& streamStaticArray(In& in, short inArray[], int size) {return streamBasicStaticArray(in, inArray, size);}
inline Out& streamStaticArray(Out& out, short outArray[], int size) {return streamBasicStaticArray(out, outArray, size);}
inline In& streamStaticArray(In& in, unsigned int inArray[], int size) {return streamBasicStaticArray(in, inArray, size);}
inline Out& streamStaticArray(Out& out, unsigned int outArray[], int size) {return streamBasicStaticArray(out, outArray, size);}
inline In& streamStaticArray(In& in, int inArray[], int size) {return streamBasicStaticArray(in, inArray, size);}
inline Out& streamStaticArray(Out& out, int outArray[], int size) {return streamBasicStaticArray(out, outArray, size);}
inline In& streamStaticArray(In& in, unsigned long inArray[], int size) {return streamBasicStaticArray(in, inArray, size);}
inline Out& streamStaticArray(Out& out, unsigned long outArray[], int size) {return streamBasicStaticArray(out, outArray, size);}
inline In& streamStaticArray(In& in, long inArray[], int size) {return streamBasicStaticArray(in, inArray, size);}
inline Out& streamStaticArray(Out& out, long outArray[], int size) {return streamBasicStaticArray(out, outArray, size);}
inline In& streamStaticArray(In& in, float inArray[], int size) {return streamBasicStaticArray(in, inArray, size);}
inline Out& streamStaticArray(Out& out, float outArray[], int size) {return streamBasicStaticArray(out, outArray, size);}
inline In& streamStaticArray(In& in, double inArray[], int size) {return streamBasicStaticArray(in, inArray, size);}
inline Out& streamStaticArray(Out& out, double outArray[], int size) {return streamBasicStaticArray(out, outArray, size);}
template<class T> 
In& streamStaticArray(In& in, T inArray[], int size) {return streamComplexStaticArray(in, inArray, size);}
template<class T>
Out& streamStaticArray(Out& out, T outArray[], int size) {return streamComplexStaticArray(out, outArray, size);}

template<class T>
In& streamComplexDynamicArray(In& in, T& inArray, int& numberOfEntries)
{
	in >> numberOfEntries;
	for(int i = 0; i < numberOfEntries; ++i)
		in >> inArray[i];
	return in;
}

template<class T>
Out& streamComplexDynamicArray(Out& out, const T& outArray, int numberOfEntries)
{
	out << numberOfEntries;
	for(int i = 0; i < numberOfEntries; ++i)
		out << outArray[i];
	return out;
}

template<class T>
In& streamBasicDynamicArray(In& in, T* inArray, int& numberOfEntries)
{
	if(in.isBinary())
	{
		in >> numberOfEntries;
		in.read(inArray, numberOfEntries * sizeof(T));
		return in;
	}
	else
		return streamComplexDynamicArray(in, inArray, numberOfEntries);
}

template<class T>
Out& streamBasicDynamicArray(Out& out, T* outArray, int numberOfEntries)
{
	if(out.isBinary())
	{
		out << numberOfEntries;
		out.write(outArray, numberOfEntries * sizeof(T));
		return out;
	}
	else
		return streamComplexDynamicArray(out, outArray, numberOfEntries);
}

inline In& streamDynamicArray(In& in, unsigned char inArray[], int& numberOfEntries) {return streamBasicDynamicArray(in, inArray, numberOfEntries);}
inline Out& streamDynamicArray(Out& out, unsigned char outArray[], int numberOfEntries) {return streamBasicDynamicArray(out, outArray, numberOfEntries);}
inline In& streamDynamicArray(In& in, char inArray[], int& numberOfEntries) {return streamBasicDynamicArray(in, inArray, numberOfEntries);}
inline Out& streamDynamicArray(Out& out, char outArray[], int numberOfEntries) {return streamBasicDynamicArray(out, outArray, numberOfEntries);}
inline In& streamDynamicArray(In& in, unsigned short inArray[], int& numberOfEntries) {return streamBasicDynamicArray(in, inArray, numberOfEntries);}
inline Out& streamDynamicArray(Out& out, unsigned short outArray[], int numberOfEntries) {return streamBasicDynamicArray(out, outArray, numberOfEntries);}
inline In& streamDynamicArray(In& in, short inArray[], int& numberOfEntries) {return streamBasicDynamicArray(in, inArray, numberOfEntries);}
inline Out& streamDynamicArray(Out& out, short outArray[], int numberOfEntries) {return streamBasicDynamicArray(out, outArray, numberOfEntries);}
inline In& streamDynamicArray(In& in, unsigned int inArray[], int& numberOfEntries) {return streamBasicDynamicArray(in, inArray, numberOfEntries);}
inline Out& streamDynamicArray(Out& out, unsigned int outArray[], int numberOfEntries) {return streamBasicDynamicArray(out, outArray, numberOfEntries);}
inline In& streamDynamicArray(In& in, int inArray[], int& numberOfEntries) {return streamBasicDynamicArray(in, inArray, numberOfEntries);}
inline Out& streamDynamicArray(Out& out, int outArray[], int numberOfEntries) {return streamBasicDynamicArray(out, outArray, numberOfEntries);}
inline In& streamDynamicArray(In& in, unsigned long inArray[], int& numberOfEntries) {return streamBasicDynamicArray(in, inArray, numberOfEntries);}
inline Out& streamDynamicArray(Out& out, unsigned long outArray[], int numberOfEntries) {return streamBasicDynamicArray(out, outArray, numberOfEntries);}
inline In& streamDynamicArray(In& in, long inArray[], int& numberOfEntries) {return streamBasicDynamicArray(in, inArray, numberOfEntries);}
inline Out& streamDynamicArray(Out& out, long outArray[], int numberOfEntries) {return streamBasicDynamicArray(out, outArray, numberOfEntries);}
inline In& streamDynamicArray(In& in, float inArray[], int& numberOfEntries) {return streamBasicDynamicArray(in, inArray, numberOfEntries);}
inline Out& streamDynamicArray(Out& out, float outArray[], int numberOfEntries) {return streamBasicDynamicArray(out, outArray, numberOfEntries);}
inline In& streamDynamicArray(In& in, double inArray[], int& numberOfEntries) {return streamBasicDynamicArray(in, inArray, numberOfEntries);}
inline Out& streamDynamicArray(Out& out, double outArray[], int numberOfEntries) {return streamBasicDynamicArray(out, outArray, numberOfEntries);}
template<class T>
In& streamDynamicArray(In& in, T& inArray, int& numberOfEntries) {return streamComplexDynamicArray(in, inArray, numberOfEntries);}
template<class T>
Out& streamDynamicArray(Out& out, const T& outArray, int numberOfEntries) {return streamComplexDynamicArray(out, outArray, numberOfEntries);}

template<class T>
In& streamComplexVector(In& in, std::vector<T>& inVector)
{
	unsigned numberOfEntries;
	in >> numberOfEntries;
	inVector.resize(numberOfEntries);
	for(unsigned i = 0; i < numberOfEntries; ++i)
		in >> inVector[i];
	return in;
}

template<class T>
Out& streamComplexVector(Out& out, std::vector<T>& outVector)
{
	out << outVector.size();
	for(unsigned i = 0; i < outVector.size(); ++i)
		out << outVector[i];
	return out;
}

template<class T> In& streamBasicVector(In& in, std::vector<T>& inVector)
{
	if(in.isBinary())
	{
		unsigned numberOfEntries;
		in >> numberOfEntries;
		inVector.resize(numberOfEntries);
		if(numberOfEntries)
			in.read(&inVector[0], numberOfEntries * sizeof(T));
		return in;
	}
	else
		return streamComplexVector(in, inVector);
}

template<class T> Out& streamBasicVector(Out& out, std::vector<T>& outVector)
{
	if(out.isBinary())
	{
		out << outVector.size();
		if(outVector.size())
			out.write(&outVector[0], outVector.size() * sizeof(T));
		return out;
	}
	else
		return streamComplexVector(out, outVector);
}

inline In& streamVector(In& in, std::vector<unsigned char>& inVector) {return streamBasicVector(in, inVector);}
inline Out& streamVector(Out& out, std::vector<unsigned char>& outVector) {return streamBasicVector(out, outVector);}
inline In& streamVector(In& in, std::vector<char>& inVector) {return streamBasicVector(in, inVector);}
inline Out& streamVector(Out& out, std::vector<char>& outVector) {return streamBasicVector(out, outVector);}
inline In& streamVector(In& in, std::vector<unsigned short>& inVector) {return streamBasicVector(in, inVector);}
inline Out& streamVector(Out& out, std::vector<unsigned short>& outVector) {return streamBasicVector(out, outVector);}
inline In& streamVector(In& in, std::vector<short>& inVector) {return streamBasicVector(in, inVector);}
inline Out& streamVector(Out& out, std::vector<short>& outVector) {return streamBasicVector(out, outVector);}
inline In& streamVector(In& in, std::vector<unsigned int>& inVector) {return streamBasicVector(in, inVector);}
inline Out& streamVector(Out& out, std::vector<unsigned int>& outVector) {return streamBasicVector(out, outVector);}
inline In& streamVector(In& in, std::vector<int>& inVector) {return streamBasicVector(in, inVector);}
inline Out& streamVector(Out& out, std::vector<int>& outVector) {return streamBasicVector(out, outVector);}
inline In& streamVector(In& in, std::vector<unsigned long>& inVector) {return streamBasicVector(in, inVector);}
inline Out& streamVector(Out& out, std::vector<unsigned long>& outVector) {return streamBasicVector(out, outVector);}
inline In& streamVector(In& in, std::vector<long>& inVector) {return streamBasicVector(in, inVector);}
inline Out& streamVector(Out& out, std::vector<long>& outVector) {return streamBasicVector(out, outVector);}
inline In& streamVector(In& in, std::vector<float>& inVector) {return streamBasicVector(in, inVector);}
inline Out& streamVector(Out& out, std::vector<float>& outVector) {return streamBasicVector(out, outVector);}
inline In& streamVector(In& in, std::vector<double>& inVector) {return streamBasicVector(in, inVector);}
inline Out& streamVector(Out& out, std::vector<double>& outVector) {return streamBasicVector(out, outVector);}
template<class T>
In& streamVector(In& in, std::vector<T>& inVector) {return streamComplexVector(in, inVector);}
template<class T>
Out& streamVector(Out& out, std::vector<T>& outVector) {return streamComplexVector(out, outVector);}

template<class T>
void streamObject(In& in, T& t)
{
	in >> t;
}

template<class T>
void streamObject(Out& out, T& t)
{
	out << t;
}

template<class T>
void streamEnum(In& in, T& t)
{
	int i;
	in >> i;
	t = (T) i;
}

template<class T>
void streamEnum(Out& out, T& t)
{
	int i = (int) t;
	out << i;
}
#endif//__StreamHandler_h_