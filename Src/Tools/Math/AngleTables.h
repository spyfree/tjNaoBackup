#ifndef AngleTables_H
#define AngleTables_H

#include "Common.h"

/**
* @class Atan2Approximation
*
* Faster atan2 function
*/
class Atan2Approximation
{
public:
  /** Implementation of atan2 using approximation of atan
  * @param x
  * @param y
  * @return What do you expect?
  */
  static double atan2(int y, int x)
  {
    if(y >= 0)
    { 
      if(x > 0)
        return atanApproximation(double(y)/double(x));
      else if(x < 0)
        return pi - atanApproximation(-double(y)/double(x));
      else if(y)
        return pi_2;
      else
        return 0.0;
    }
    else return -Atan2Approximation::atan2(-y, x);
  }

private:
  /** Approximation of atan function
  * @param x
  * @return The approximated atan(x)
  */
  static double atanApproximation(double x)
  {
    if(fabs(x) <= 1)
      return  (x / (1 + 0.28*x*x));
    else
      return pi_2 - (x / (x*x + 0.28));
  }
};

#endif //__AngleTables_h_
