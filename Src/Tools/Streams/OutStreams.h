/**
* @file OutStreams.h
*
* Declaration of out stream classes for different media and formats.
*
*/
#ifndef __OutStream_h_
#define __OutStream_h_

#include "InOut.h"
#include <iostream>
class OutTextCmd: public Out
{
public:
	OutTextCmd(){};
protected:

	/**
	* Virtual redirection for operator<<(const char& value).
	*/
	virtual void outChar(char d)
		{std::cout << d << ", ";}
	/**
	* Virtual redirection for operator<<(const unsigned char& value).
	*/
	virtual void outUChar(unsigned char d)
		{std::cout << d << ", ";}
	/**
	* Virtual redirection for operator<<(const short& value).
	*/
	virtual void outShort(short d)
		{std::cout << d << ", ";}
	/**
	* Virtual redirection for operator<<(const unsigned short& value).
	*/
	virtual void outUShort(unsigned short d)
		{std::cout << d << ", ";}
	/**
	* Virtual redirection for operator<<(const int& value).
	*/
	virtual void outInt(int d)
		{std::cout<<d << ", ";}
	/**
	* Virtual redirection for operator<<(const unsigned& value).
	*/
	virtual void outUInt(unsigned int d)
		{std::cout << d << ", ";}
	/**
	* Virtual redirection for operator<<(const long& value).
	*/
	virtual void outLong(long d) 
		{std::cout << d << ", ";}
	/**
	* Virtual redirection for operator<<(const unsigned long& value).
	*/
	virtual void outULong(unsigned long d)
		{std::cout << d << ", ";}
	/**
	* Virtual redirection for operator<<(const float& value).
	*/
	virtual void outFloat(float d)
		{std::cout << d << ", ";}
	/**
	* Virtual redirection for operator<<(const double& value).
	*/
	virtual void outDouble(double d)
		{std::cout << d << ", ";}
	/**
	* Virtual redirection for operator<<(const char* value).
	*/
	virtual void outString(const char * d)
		{std::cout<<d << ", ";}
	/**
	* Virtual redirection for operator<<(Out& (*f)(Out&)) that writes
	* the symbol "endl";
	*/
	virtual void outEndL()
		{std::cout << std::endl;}

	/**
	* Virtual redirection for operator>>(In& (*f)(In&)) that reads
	* the symbol "endl";
	*/
	virtual void inEndL()
		{std::cout << std::endl;}
public:
	/**
	* The function writes a number of bytes into a stream.
	* @param p The address the data is located at.
	* @param size The number of bytes to be written.
	*/
	virtual void write(const void* p,int size){}
};

#endif//__OutStream_h_