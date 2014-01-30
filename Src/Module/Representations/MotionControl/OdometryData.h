/**
* @file OdometryData.h
* Contains the OdometryData class.
*
* @author Max Risler
*/

#ifndef __OdometryData_h_
#define __OdometryData_h_

#include "Tools/Math/Pose2D.h"

/**
* @class OdometryData
* OdometryData contains an approximation of overall movement the robot has done.
* @attention Only use differences of OdometryData at different times.
* Position in mm
*/
class OdometryData : public Pose2D
{
public:
  bool operator==(const OdometryData& other) const 
  {
    return (const Pose2D&) *this == other;
  }
  
  bool operator!=(const OdometryData& other) const
  {
    return !(*this == other);
  }
};

#endif //__OdometryData_h_
