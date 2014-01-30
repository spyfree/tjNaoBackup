#ifndef __SmartPtr_H_
#define __SmartPtr_H_
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <boost/enable_shared_from_this.hpp> 

template <class T> class SmartPtr: public boost::shared_ptr<T>
{
public:
	template<class Y> explicit SmartPtr(Y* p): boost::shared_ptr<T>(p) {}

	template <class Y> SmartPtr(const boost::shared_ptr<Y>& p):
	boost::shared_ptr<T>(p) {}

	template<class Y, class D> SmartPtr(Y* p, D d): boost::shared_ptr<T>(p, d){}
	//template<class Y, class D> shared_ptr(Y * p, D d)
	SmartPtr() : boost::shared_ptr<T>(){}
};
#endif //__SmartPtr_H_