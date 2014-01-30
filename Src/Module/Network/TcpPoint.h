#ifndef TcpPoint_H
#define TcpPoint_H

#include <exception>
#include <vector>
#include <Module/Common/CommonData.h>
#include "WirelessConfig.h"
//s#include <linux/in.h>
#include "networkComm/NetworkInclude.h"

class socket_error: public std::exception
{
public:
	socket_error(const socket_error& other) throw()
		: exception()
	{
		fname = other.fname;
		lineno = other.lineno;
		err = other.err;

		msg = (char *) malloc(strlen(other.msg));
		strcpy(msg, other.msg);
	}

	socket_error(const char *f, int l, int e) throw()
		: exception()
	{
		fname = f;
		lineno = l;
		err = e;

		build_message(strerror(e));
	}

	socket_error(const char *f, int l, const char *m) throw()
		: exception()
	{
		fname = f;
		lineno = l;
		err = -1;

		build_message(m);
	}
	~socket_error() throw() {
		free(msg);
	}

	void build_message(const char *m) {
		//format:
		// [file ..., line ...] ...
		// 123456   7890123   45     = 15 + 10 characters for line number
		int len = strlen(fname) + strlen(m) + 25;

		msg = (char*) malloc(len);
		sprintf(msg, "[file %s, line %d] %s", fname, lineno, m);
	}

	virtual const char* what() const throw() {
		return msg;
	}

	const char* file() {
		return fname;
	}

	int line() {
		return lineno;
	}

	int error() {
		return err;
	}

private:
	const char *fname;
	char *msg;
	int lineno;
	int err;
};


//
// DataSerializer class definition
//

class TcpPoint {
public:
	TcpPoint() ;
	~TcpPoint();

	void accept  (bool block=false);
	void bind    (int port) ;
	void close   () ;
	void closeAll() ;
	void setblocking(bool toBlock) ;
	void connect(const char *addr);
	bool bound();
	bool connected();
	int select();
	bool insert();
	byte read_byte () ;
	void write(const void *data, int len) ;
	void read (char *data, int len) ;
private:
	int bind_sockn;
	int sockn;
	fd_set fdread;
	bool blocking;
	char buf[9];
};


#endif /* TcpPoint_H */
