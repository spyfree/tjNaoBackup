/**
 * @file CappedCylinder.cpp
 * 
 * Implementation of class CappedCylinder
 *
 * @author <A href="mailto:kspiess@informatik.uni-bremen.de">Kai Spiess</A>
 * @author <A href="mailto:timlaue@informatik.uni-bremen.de">Tim Laue</A>
 */ 

#include "CappedCylinder.h"
#include <Tools/SimGraphics.h>
#include <Tools/SimGeometry.h>
#include <Tools/ODETools.h>
#include <Simulation/Simulation.h>


CappedCylinder::CappedCylinder(const AttributeSet& attributes):Cylinder(attributes)
{
  graphicsHandle = graphicsManager->createNewCylinder(radius, height, true);
}


SimObject* CappedCylinder::clone() const
{
  CappedCylinder* newCappedCylinder = new CappedCylinder();
  newCappedCylinder->copyStandardMembers(this);
  bool createsNewMovID = newCappedCylinder->adjustMovableID();
  newCappedCylinder->radius = radius;
  newCappedCylinder->height = height;
  newCappedCylinder->graphicsHandle = graphicsHandle;
  memcpy(&newCappedCylinder->boxCorners,&boxCorners[0], 8* sizeof(Vector3d));

  std::list<SimObject*>::const_iterator pos;
  for(pos = childNodes.begin(); pos != childNodes.end(); ++pos)
  {
    SimObject* childNode = (*pos)->clone();
    newCappedCylinder->addChildNode(childNode, false);
  }

  if(createsNewMovID)
    simulation->removeCurrentMovableID();

  SimObject* newObject = newCappedCylinder;
  return newObject;
}


bool CappedCylinder::intersectWithRay(const Vector3d& pos, const Vector3d& ray, Vector3d& intersectPos) 
{
  return SimGeometry::intersectRayAndCylinder(pos,ray,position,rotation,height,radius,true,intersectPos);
}


double CappedCylinder::getMaxDistanceTo(const Vector3d& base) const
{
  double dist(0);
  double vecLen((base - position).getLength());
  double realHeight(height/2.0 + radius);
  double radius(sqrt(realHeight*realHeight + this->radius*this->radius));
  if(vecLen < radius)
  {
    dist = vecLen + radius;
  }
  else
  {
    dist = vecLen + 2.0*radius;
  }
  std::list<SimObject*>::const_iterator pos;
  for(pos = childNodes.begin(); pos != childNodes.end(); ++pos)
  {
    double childDist = (*pos)->getMaxDistanceTo(base);
    if(dist < childDist)
    {
      dist = childDist;
    }
  }
  return dist;
}


void CappedCylinder::createPhysics()
{
  //create physic engine stuff
  this->setPointerToPhysicalWorld(simulation->getPhysicalWorld());
  this->setPointerToCollisionSpace(simulation->getCurrentSpace());

  if(movableID)
  {
    createGeometry(*simulation->getMovableSpace());
    createBody();
    dGeomSetBody(geometry, body); //connect body and geometry
  }
  else
  {
    createGeometry(*simulation->getStaticSpace());
  }
}


void CappedCylinder::createBody()
{
  if(body == 0)
  {
    body = dBodyCreate(*simulation->getPhysicalWorld());
    dMass m;
    dMassSetCapsule(&m, 1, 3, dReal(height), dReal(height));
    dMassAdjust(&m, dReal(mass));
    dBodySetMass(body, &m);
    dBodySetPosition(body, dReal(position.v[0]), dReal(position.v[1]), dReal(position.v[2]));
    dMatrix3 rotationMatrix;
    ODETools::convertMatrix(rotation, rotationMatrix);
    dBodySetRotation(body, rotationMatrix);
  }
}


void CappedCylinder::createGeometry(dSpaceID space)
{
  if(geometry == 0)
  {
    setGeometry(dCreateCCylinder(space, dReal(radius), dReal(height)));
    dGeomSetPosition(geometry, dReal(position.v[0]), dReal(position.v[1]), dReal(position.v[2]));
    dMatrix3 rotationMatrix;
    ODETools::convertMatrix(rotation, rotationMatrix);
    dGeomSetRotation(geometry, rotationMatrix);
    //set user data pointer of ODE geometry object to this physical object
    dGeomSetData(geometry, this);
    //set collide bitfield
    PhysicalObject::setCollideBitfield();
  }
}


void CappedCylinder::computeMassProperties(dMass& m)
{
  dMassSetCapsule(&m, 1, 3, dReal(height), dReal(height));
  dMassAdjust(&m, dReal(mass));
}


void CappedCylinder::calculateCorners()
{
  //Set corner points:
  double length_2(radius);
  double width_2(radius);
  double height_2(height * 0.5 + radius);
  boxCorners[0] = Vector3d(-length_2, -width_2, height_2);
  boxCorners[1] = Vector3d(length_2, -width_2, height_2);
  boxCorners[2] = Vector3d(length_2, width_2, height_2);
  boxCorners[3] = Vector3d(-length_2, width_2, height_2);
  boxCorners[4] = Vector3d(-length_2, width_2, -height_2);
  boxCorners[5] = Vector3d(length_2, width_2, -height_2);
  boxCorners[6] = Vector3d(length_2, -width_2, -height_2);
  boxCorners[7] = Vector3d(-length_2, -width_2, -height_2);
}
