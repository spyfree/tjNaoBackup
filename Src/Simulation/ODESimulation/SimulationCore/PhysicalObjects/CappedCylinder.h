/**
 * @file CappedCylinder.h
 * 
 * Definition of class CappedCylinder
 *
 * @author <A href="mailto:kspiess@informatik.uni-bremen.de">Kai Spiess</A>
 * @author <A href="mailto:timlaue@informatik.uni-bremen.de">Tim Laue</A>
 */ 

#ifndef CAPPEDCYLINDER_H_
#define CAPPEDCYLINDER_H_

#include "Cylinder.h"

/**
 * @class CappedCylinder
 *
 * A class representing a capped cylinder in 3D space. It is like a normal cylinder with half spheres at its ends as caps.
 */
class CappedCylinder : public Cylinder  
{
protected:
  /** Calculates the corners of the box around the cylinder*/
  virtual void calculateCorners();

public:
  /** Empty standard Constructor */
  CappedCylinder() {}

  /** Constructor
  * @param attributes The attributes of the capped cylinder
  */
  CappedCylinder(const AttributeSet& attributes);

 /** 
  * Returns a string describing the kind of object
  * @return The kind
  */
  virtual std::string getKind() const {return "cappedcylinder";}

  /**
   * Clones the object and its child nodes
   * @return A pointer to a copy of the object
   */
  virtual SimObject* clone() const;

  /** Returns the maximum distance of any point in the subtree to a given point
  * @param base The point to compute the distance to
  * @return The distance
  */
  virtual double getMaxDistanceTo(const Vector3d& base) const;

  /** Intersects the cylinder and its children with a ray
  * @param pos The starting position of the ray
  * @param ray The ray
  * @param intersectPos The intersection point nearest to pos (if found)
  * @return true, if an intersection point has been found
  */
  virtual bool intersectWithRay(const Vector3d& pos, const Vector3d& ray, Vector3d& intersectPos);

  /** 
   * Creats the physical objects used by the OpenDynamicsEngine (ODE).
   * These are a geometry object for collision detection and a body,
   * if the simulation object is movable.
   */
  virtual void createPhysics();

  /** 
   *  Create the body for this object including mass properties
   */
  virtual void createBody();
  /** 
   *  Create the geometry for this object and insert it in the given space
   *  @param space The space to insert the geometry into
   */
  virtual void createGeometry(dSpaceID space);
  /** 
   *  Compute the mass properties of the object
   *  @param m The mass object where the computed properties are stored
   */
  virtual void computeMassProperties(dMass& m);

  /** Return the number of graphicsHandles
  * @return The number of primitives
  */
  virtual unsigned int getNumPrim() const
  { return 1; };

  /** Return the number of Vertices
  * @return The number of vertices
  */
  virtual unsigned int getNumVert() const
  { return 288; };
};

#endif // CAPPEDCYLINDER_H_
