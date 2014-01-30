#include "InOut.h"

Out& endl(Out& stream)
{
	stream.outEndL(); 
	return stream;
}

In& endl(In& stream)
{
	stream.inEndL();
	return stream;
}
