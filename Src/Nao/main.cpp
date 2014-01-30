/**
 * \mainpage
 * \section Author
 * @author Jerome Monceaux
 *
 * \section Copyright
 * Aldebaran Robotics (c) 2007 All Rights Reserved. This is an example of use.\n
 * Version : $Id: main.cpp,v 1.7 2010/05/19 01:10:36 cvsadmin Exp $
 *
 * \section Description
 * Module list :  - STMOBJECT_NAME
 */

#ifndef _WIN32
# include <signal.h>
#else
# include <signal.h>
#endif

#include "altypes.h"
#include "alxplatform.h"
#include "main.h"
#include "albrokermanager.h"
#include "alptr.h"



using namespace std;
using namespace AL;

#ifdef NAO_IS_REMOTE_OFF

#ifdef _WIN32
# define ALCALL __declspec(dllexport)
#else
# define ALCALL
#endif

#ifdef __cplusplus
extern "C"
{
#endif


ALCALL int _createModule( ALPtr<ALBroker> pBroker )
{
  // init broker with the main broker instance
  // from the parent executable
  ALBrokerManager::setInstance(pBroker->fBrokerManager.lock());
  ALBrokerManager::getInstance()->addBroker(pBroker);


  // create modules instance. This will register automatically to the broker
  ALPtr<MainModule>mainptr = ALModule::createModule<MainModule>( pBroker ,  "MainModule");
  mainptr->mainstart();
//  mainptr->mainWaitOn();
//  mainptr->mainWaitOff();
  return 0;
}

ALCALL int _closeModule(  )
{
  // Delete module instance. Will unregister automatically

  return 0;
}

# ifdef __cplusplus
}
# endif

#else

void _terminationHandler( int signum )
{
  std::cout << "Exiting TJARKNAO." << std::endl;
  ALBrokerManager::getInstance()->killAllBroker();
  ::exit(0);
}


int usage( char* progName )
{
  std::cout << progName <<", a remote module of naoqi !" << std::endl
            << "Can find the age of the captain." << std::endl
            << "USAGE :" << std::endl
            << "-b\t<ip> : binding ip of the server. Default is 127.0.0.1" << std::endl
            << "-p\t<port> : binding port of the server. Default is 9559" << std::endl
            << "-pip\t<ip> : ip of the parent broker. Default is 127.0.0.1" << std::endl
            << "-pport\t<ip> : port of the parent broker. Default is 9559" << std::endl
            << "-h\t: Display this help\n" << std::endl;
  return 0;
}

int main( int argc, char *argv[] )
{


  std::cout << "..::: starting TJARKNAO :::.." << std::endl;

  int  i = 1;
  std::string brokerName = "TJArkNao";
  std::string brokerIP = "";
  int brokerPort = 0 ;
  // Default parent broker IP
  std::string parentBrokerIP = "192.168.1.62";
  // Default parent broker port
  int parentBrokerPort = kBrokerPort;

  // checking options
  while( i < argc ) {
    if ( argv[i][0] != '-' ) return usage( argv[0] );
    else if ( std::string( argv[i] ) == "-b" )        brokerIP          = std::string( argv[++i] );
    else if ( std::string( argv[i] ) == "-p" )        brokerPort        = atoi( argv[++i] );
    else if ( std::string( argv[i] ) == "-pip" )      parentBrokerIP    = std::string( argv[++i] );
    else if ( std::string( argv[i] ) == "-pport" )    parentBrokerPort  = atoi( argv[++i] );
    else if ( std::string( argv[i] ) == "-h" )        return usage( argv[0] );
    i++;
  }

  // If server port is not set
  /*if ( !brokerPort )
    brokerPort = FindFreePort( brokerIP );*/

  std::cout << "Try to connect to parent Broker at ip :" << parentBrokerIP
            << " and port : " << parentBrokerPort << std::endl;
  std::cout << "Start the server bind on this ip :  " << brokerIP
            << " and port : " << brokerPort << std::endl;

 //try
 //{
   AL::ALBroker::Ptr broker = AL::ALBroker::createBroker(brokerName, brokerIP, brokerPort, parentBrokerIP, parentBrokerPort);
ALPtr<MainModule>mainptr = ALModule::createModule<MainModule>( broker ,  "MainModule");
std::cout<<"main error"<<std::endl;
 //}
 //catch(ALError &e)
 //{
 //  std::cout << "-----------------------------------------------------" << std::endl;
 //  std::cout << "Creation of broker failed. Application will exit now." << std::endl;
 //  std::cout << "-----------------------------------------------------" << std::endl;
 //  std::cout << e.toString() << std::endl;
 //  exit(0);
 //}

# ifndef _WIN32
  struct sigaction new_action;
  /* Set up the structure to specify the new action. */
  new_action.sa_handler = _terminationHandler;
  sigemptyset( &new_action.sa_mask );
  new_action.sa_flags = 0;

  sigaction( SIGINT, &new_action, NULL );
# endif



  mainptr->mainstart();
  mainptr->mainWaitOn();
  mainptr->mainWaitOff();

  while( 1 )
  {
    SleepMs( 100 );
  }

# ifdef WIN32
  _terminationHandler( 0 );
# endif

  exit( 0 );

}
#endif

