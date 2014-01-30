/**
 * @file StandardMotor.h
 * 
 * Definition of class StandardMotor
 *
 * @author <A href="mailto:timlaue@informatik.uni-bremen.de">Tim Laue</A>
 * @author <A href="mailto:kspiess@informatik.uni-bremen.de">Kai Spiess</A>
 */ 

#ifndef STANDARDMOTOR_H_
#define STANDARDMOTOR_H_

#include "Motor.h"


/**
* @class StandardMotor
*
* A class representing a standard motor
*/
class StandardMotor: public Motor
{
public:
  /** Empty standard Constructor*/
  StandardMotor() {};

  /** Constructor*/
  StandardMotor(const AttributeSet& attributes);

  /** Clones the motor
   *  @return A pointer to a copy of the motor
   */
  virtual Motor* clone() const;

  /** Returns a string describing the kind of object
  * @return The kind
  */
  virtual std::string getKind() const {return "standardmotor";}

  /** Sets the value of one actuatorport
  * @param value The value
  * @param port The port
  */
  virtual void setValue(double value, int port);
};

#endif //STANDARDMOTOR_H_
