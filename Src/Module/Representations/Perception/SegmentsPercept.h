/**
* @file Representations/Perception/SegmentsPercept.h
*
* Representention of the seen segments
*
* @author <a href="mailto:rieskamp@informatik.uni-bremen.de">Andrik Rieskamp</a>
* @author <a href="mailto:felipe@informatik.uni-bremen.de">Felipe Lopez</a>
*/

#ifndef __SegmentsPercept_h_
#define __SegmentsPercept_h_

#include "Tools/Math/Pose2D.h"
#include <vector>

class SegmentsPercept
{
public:

  /** Constructor */
  SegmentsPercept() 
  {
    segmentsHorizontal.reserve(1000); // TODO: Check default segment count
    segmentsVertical.reserve(1000);   // TODO: Check default segment count
  }

  class Segment 
  {
  public:
    int x; /**< x coordinate of start point in image */
    int y; /**< y coordinate of start point in image */
    int length; /**< The length of the segment line in the image. */
    bool onHalfScanLine;
    unsigned char color; /**< The primary color of the segment. */
  };

  Pose2D origin; /**< All segments are specified relative to this origin. */
  std::vector<Segment> segmentsHorizontal; /**< The segments detected in horizontal scan */
  std::vector<Segment> segmentsVertical; /**< The segments detected in vertical scan  */

 // std::vector<Vector2<int> > fieldBorders;

};

#endif// __SegmentsPercept_h_
