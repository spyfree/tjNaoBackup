
#include "TcpPoint.h"
#include <fcntl.h>
TcpPoint::TcpPoint ()
: bind_sockn(-1), sockn(-1), blocking(true)
{

}

TcpPoint::~TcpPoint()
{
	closeAll();
}

void
TcpPoint::accept (bool block)
{
   cout<<"in   accept"<<endl;
	sockn = SOCKETNS::accept(bind_sockn, NULL, NULL);
	cout<<"accept"<<endl;

	if(block==true)
	{
		DWORD   ul=1;
    #ifdef WIN32
	SOCKETNS::ioctlsocket(sockn,FIONBIO,&ul);
	#else
	SOCKETNS::fcntl(sockn,FIONBIO,&ul);
	#endif
	}
	cout<<"accept  finish"<<endl;
	if (sockn == -1) {
		close();
		//throw SOCKET_ERROR(errno);
	}

	if (!blocking) {
		// for Aibo's, we must use non-blocking operations, as TOOLConnect must run
		// in-line in the same thread as Vision

	}

	// set socket send and receive buffer sizes
	/*
	if (::setsockopt(sockn, SOL_SOCKET, SO_SNDBUF, (void*)BUF_SIZE,
	sizeof(int)) == -1 ||
	::setsockopt(sockn, SOL_SOCKET, SO_RCVBUF, (void*)BUF_SIZE,
	sizeof(int)) == -1)
	error(errno);
	*/
}

void
TcpPoint::bind (int port)
{

	// create socket
	bind_sockn = SOCKETNS::socket(AF_INET, SOCK_STREAM, 0);
     cout<<"bind"<<endl;
	if (bind_sockn == -1) {
	    cout<<"bindclose"<<endl;
		close();
		//throw SOCKET_ERROR(errno);
	}
	// set bind address parameters

	struct sockaddr_in bind_addr;
	bind_addr.sin_family = AF_INET;
	bind_addr.sin_port = htons(port);
	bind_addr.sin_addr.s_addr = htonl(INADDR_ANY);


	// bind socket to address

	if (SOCKETNS::bind(bind_sockn, (const struct sockaddr*)&bind_addr,
		sizeof(bind_addr)) == -1 ||
		SOCKETNS::listen(bind_sockn, 10) == -1) {
            cout<<"listenclose"<<endl;
			closeAll();
			//throw SOCKET_ERROR(errno);
	}
}



void
TcpPoint::close ()
{
	// attempt to close if we haven't already
	#ifdef WIN32
	if (sockn != -1)
		SOCKETNS::closesocket(sockn);
    #else
    if (sockn != -1)
		SOCKETNS::close(sockn);
    #endif
	// invalidate file descriptor
	sockn = -1;
}

void
TcpPoint::closeAll ()
{
	close();
    #ifdef WIN32
	if (bind_sockn != -1)
		if (SOCKETNS::closesocket(bind_sockn) == -1)
			fprintf(stderr, "Error closing socket: #%i - %s\n", errno,
			strerror(errno));
    #else
    if (bind_sockn != -1)
		if (SOCKETNS::close(bind_sockn) == -1)
			cout<<"Error closing socket: #%i - %s\n";
    #endif
	bind_sockn = -1;



}

void
TcpPoint::setblocking (bool toBlock)
{
	if (toBlock ^ blocking) {


		fprintf(stderr, "Non-blocking sockets currently not supported on non-Aibo "
			"platforms\n");

	}
	blocking = toBlock;
}

bool
TcpPoint::bound ()
{
	return bind_sockn != -1;
}

bool
TcpPoint::connected ()
{
    cout<<"conected"<<(sockn != -1)<<endl;
	return sockn != -1;
}
int TcpPoint::select()
{

	int ret;
	FD_ZERO(&fdread);
	FD_SET(sockn,&fdread);
	ret=SOCKETNS::select(0,&fdread,NULL,NULL,NULL);
	return ret;
}
bool TcpPoint::insert()
{
	return FD_ISSET(sockn,&fdread);
}
void
TcpPoint::connect (const char *addr)
{
  //struct sockaddr_in raw_addr;
  //int result;

  //// convert 'addr' to in_addr struct
  //result = inet_aton(addr, &raw_addr.sin_addr);
  //if (result == 0) {
  //  // 'addr' is not a valid address, attempt to resolve hostname
  //  hostent *host_info = gethostbyname(addr);

  //  // resolving worked, convert to in_addr struct
  //  addr = *(host_info->h_addr_list);
  //  result = inet_aton(addr, &raw_addr.sin_addr);
  //  if (result == 0) {
  //    close();
  //    throw SOCKET_ERROR(errno);
  //  }
  //}

  //// create socket
  //sockn = SOCKETNS::socket(AF_INET, SOCK_STREAM, 0);
  //if (sockn == -1) {
  //  close();
  //  throw SOCKET_ERROR(errno);
  //}

  //// set socket send and receive buffer sizes
  //if (SOCKETNS::setsockopt(sockn, SOL_SOCKET, SO_SNDBUF, (void*)BUF_SIZE,
  //      sizeof(int)) != 0 ||
  //    SOCKETNS::setsockopt(sockn, SOL_SOCKET, SO_RCVBUF, (void*)BUF_SIZE,
  //      sizeof(int)) != 0) {
  //  close();
  //  throw SOCKET_ERROR(errno);
  //}

  //// connect socket to address
  //raw_addr.sin_family = AF_INET;
  //raw_addr.sin_port = 4001;
  //if (SOCKETNS::connect(sockn, (struct sockaddr *)&raw_addr, sizeof raw_addr) == -1)
  //  error(errno);
}

typedef   unsigned   char   byte;
void
TcpPoint::write (const void *data, int len)
{
	int wrote = 0, result;

	while (wrote < len) {
		result = SOCKETNS::send(sockn, (char*)((byte*)data + wrote) , len - wrote, 0);

		if (result == -1) {
			if (blocking )
				close();

		}else if (result == 0) {
			close();

		}

		wrote += result;
	}
}

void
TcpPoint::read (char *data, int len)
{
	int nread = 0, result;
	while (nread < len) {

		result = SOCKETNS::recv(sockn, /*(char*)*/(/*(byte*)*/data + nread), len - nread, 0);

		if (result == -1) {
			//close();
		return ;

		}else if (result == 0) {
			close();
			return ;

		}
		nread += result;
	}
}

byte
TcpPoint::read_byte ()
{
	read(&buf[0], 2 * SIZEOF_BYTE);

	if (buf[0] != TYPE_BYTE) {
		close();

	}

	return buf[SIZEOF_BYTE];
}





