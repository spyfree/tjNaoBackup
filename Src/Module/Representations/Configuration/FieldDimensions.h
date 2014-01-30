/**
* @file FieldDimensions.h
*
* Description of the dimensions of the field.
*
* @author Matthias Jüngel
* @author Thomas Röfer
*/

#ifndef __FieldDimensions_h_ 
#define __FieldDimensions_h_ 

#include "Module/Common/Geometry.h"
//#include "Platform/GTAssert.h"
#include "Module/Network/RoboCupGameControlData.h"
#include "Tools/Boundary.h"
//#include "Module/Vision/DebugDrawings.h"

/**
* Class containing definitions and functions
* regarding field dimensions.
*
* @author Max Risler
*/
class FieldDimensions : public Boundary<double>
{
public:
  int xPosOpponentFieldBorder;
  int xPosOpponentGoal;
  int xPosOpponentGoalpost;
  int xPosOpponentGroundline;
  int xPosOpponentSideCorner;
  int xPosOpponentPenaltyArea;
  int xPosHalfWayLine;
  int xPosOwnPenaltyArea;
  int xPosOwnSideCorner;
  int xPosOwnGroundline;
  int xPosOwnGoalpost;
  int xPosOwnGoal;
  int xPosOwnFieldBorder;

  int yPosLeftFieldBorder;
  int yPosLeftSideline;
  int yPosLeftGroundline;
  int yPosLeftPenaltyArea;
  int yPosLeftGoal;
  int yPosCenterGoal;
  int yPosRightGoal;
  int yPosRightPenaltyArea;
  int yPosRightGroundline;
  int yPosRightSideline;
  int yPosRightFieldBorder;

  //other dimensions
  int centerCircleRadius;
  int goalHeight;
  int ballRadius;
  int ballFriction; // in mm/s^2
  int fieldLinesWidth;
  int goalPostRadius;

  //throw-in points
  int xPosThrowInPointOpponentHalf;
  int xPosThrowInPointCenter;
  int xPosThrowInPointOwnHalf;

  /**
  * This is a collection of line- or boundary segments with start-Pose2D and length.
  */
  class LinesTable 
  {
  public:
    class Line 
    {
    public:
      Pose2D corner; /**< The field corners. */
      double length; /**< The lengths of the border segments starting at a corresponding corner. */
	  bool isPartOfCircle; /**< Whether the line is a part of a circle. */
    };
    std::vector<Line> lines;

    void push(const Pose2D& p, double l, bool isPartOfCircle = false);

    void push(const Vector2<double>& s, const Vector2<double>& e, bool isPartOfCircle = false);

    void pushCircle(const Vector2<double>& center, double radius, int numOfSegments);

    void pushQuarterCircle(const Vector2<double>& center, double radius, int numOfSegments, int angle);

    /**
    * Doubles the line segments creating double sided segments that have edges on both sides.
    * @param width The distance between both edges of each line segment.
    * @param single A lines table with single-sided lines.
    */
    void doubleSided(double width, const LinesTable& single);


    /*
    * Returns whether a given point is inside the polygon described by the line segments.
    * Only valid if the line segment table describes a closed polygon. 
    */

   bool isInside(const Vector2<double>& v) const;

    /**
    * The function clips a point to the polygon described by the line segments.
    * Only valid if the line segment table describes a closed polygon. 
    * @param v The point.
    * @return How far was the point moved?
    */
    double clip(Vector2<double>& v) const;
    
    /**
    * The function returns the point on a line of a certain type closest to given a point.
    * @param point The point on a line.
    * @param p The reference point and the rotation of the line.
    * @param numberOfRotations The number of discretizations of line rotations.
    * @param minLength The minimum length of the line segments that are considered.
    * @return whether there is a matching point in that direction
    */
    bool getClosestPoint(Vector2<double>& point, const Pose2D& p, int numberOfRotations, double minLength) const;

    /**
    * The function returns the distance between a point and the closest point on a line of a certain type in a certain direction.
    * @param pose The reference point and direction.
    * @return The distance. It is -1 if no line of that type exists in the certain direction.
    */
    double getDistance(const Pose2D& pose) const;

    /**
    * Draws a debug field drawing that displays the set of lines.
    */
    //void draw(const ColorRGBA& color, bool drawNormals = true) const;
  };

  /**
  * Tables of line segments
  */
  LinesTable fieldLines;

  /** 
  * Describes a polygon around the border of the field carpet.
  * All legal robot positions are inside this polygon.
  */
  LinesTable carpetBorder;

  /** 
  * Describes a polygon around the border of the playing field.
  * All legal ball positions are inside this polygon.
  */
  LinesTable fieldBorder;

  /**
  *Describes a polygon around the border of own field
  */
  LinesTable ownFieldBorder;
  /**
  * The class represents all corners of a certain type.
  * It only exists, because an array of vectors is not serializable.
  */
  class CornersTable : public std::vector<Vector2<int> >
  {
  public:
    /**
    * The method returns the position of the corner closest to a point.
    * The method is only defined if !empty().
    * @param p The point.
    * @return The position of the closest corner.
    */
    const Vector2<int>& getClosest(const Vector2<int>& p) const;

    /**
    * The method returns the position of the corner closest to a point.
    * The method is only defined if !empty().
    * @param p The point.
    * @return The position of the closest corner.
    */
    const Vector2<double> getClosest(const Vector2<double>& p) const;
  };

  /**
  * All different corner classes.
  */
  enum CornerClass
  {
    xCorner,
    tCorner0,
    tCorner90,
    tCorner180,
    tCorner270,
    lCorner0,
    lCorner90,
    lCorner180,
    lCorner270,
    numOfCornerClasses
  };
  
  /**
  * The method returns the name of a corner class as string.
  * @param cornerClass The corner class.
  * @return The name of the corner class.
  */
  static const char* getCornerClassName(CornerClass cornerClass);
  
  CornersTable corners[numOfCornerClasses]; /**< All corners on the field. */

public:
  /**
   * Default constructor.
   */
  FieldDimensions();

  /**
  * Read field dimensions from configuration file.
  */
  void load();

  /**
  * Returns true when p is inside the carpet.
  */
  bool isInsideCarpet(const Vector2<double> &p) const
  {
    return carpetBorder.isInside(p);
  }

  /**
  * The function clips a point to the carpet.
  * @param v The point.
  * @return How far was the point moved?
  */
  double clipToCarpet(Vector2<double>& v) const
  {
    return carpetBorder.clip(v);
  }

  /**
  * Returns true when p is inside the playing field.
  */
  bool isInsideField(const Vector2<double> &p) const
  {
    return fieldBorder.isInside(p);
  }

  /**
  * The function clips a point to the field.
  * @param v The point.
  * @return How far was the point moved?
  */
  double clipToField(Vector2<double>& v) const
  {
    return fieldBorder.clip(v);
  }

  bool isInsideOwnSide(const Vector2<double> &p) const
  {
	  return ownFieldBorder.isInside(p);
  }

  /**
  * The function returns a random pose inside the field.
  * @return The random pose.
  */
  Pose2D randomPoseOnField() const;

  /**
  * The function returns a random pose on the carpet.
  * @return The random pose.
  */
  Pose2D randomPoseOnCarpet() const;

  /**
  * The function returns  a random pose on the own side.
  * @return The random pose
  */
  Pose2D randomPoseOnOwnSide() const;

  void readDimensions();
  void readLines();
  void readCorners();

};

#endif //FieldDimensions
