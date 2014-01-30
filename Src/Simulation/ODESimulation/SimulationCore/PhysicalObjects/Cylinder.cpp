/**
 * @file Cylinder.cpp
 * 
 * Implementation of class Cylinder
 *
 * @author <A href="mailto:timlaue@informatik.uni-bremen.de">Tim Laue</A>
 * @author <A href="mailto:kspiess@informatik.uni-bremen.de">Kai Spiess</A>
 */ 

#include "Cylinder.h"
#include <Tools/Surface.h>
#include <Tools/SimGraphics.h>
#include <Tools/SimGeometry.h>
#include <Tools/ODETools.h>
#include <Simulation/Simulation.h>


Cylinder::Cylinder(const AttributeSet& attributes)
{
  for(unsigned int i=0; i<attributes.size(); i++)
  {
    std::string attributeName(attributes[i].attribute);
    if(attributeName == "name")
    {
      name = attributes[i].value;
    }
    else if(attributeName == "radius")
    {
      radius = ParserUtilities::toDouble(attributes[i].value);
    }
    else if(attributeName == "height")
    {
      height = ParserUtilities::toDouble(attributes[i].value);
    }
    else if(attributeName == "canCollide")
    {
      collideBit = ParserUtilities::toBool(attributes[i].value);
    }
  }
  calculateCorners();
  graphicsHandle = graphicsManager->createNewCylinder(radius, height);
}

void Cylinder::drawObject(const VisualizationParameterSet& visParams)
{
  if((invisible && !visParams.drawForSensor) || visParams.mb_skip == mbMethod)
    return;

  if(shaderProgram)
    graphicsManager->shader->enableLocalShader(this);
  else if(visParams.provideUniforms)
    graphicsManager->shader->handleUniforms(this);

  graphicsManager->setMBInfos(previous_positions, previous_rotations, cycle_order_offset);
  graphicsManager->drawPrimitive(graphicsHandle, position, rotation, *surface,
                                 inverted, visParams.drawForSensor, visParams);

  if(shaderProgram)
    graphicsManager->shader->disableLocalShader();
}

void Cylinder::quickDrawObject()
{
  if(!invisible)
  {
    graphicsManager->quickDrawPrimitive(graphicsHandle,position, rotation, *surface);
  }
}

SimObject* Cylinder::clone() const
{
  Cylinder* newCylinder = new Cylinder();
  newCylinder->copyStandardMembers(this);
  bool createsNewMovID = newCylinder->adjustMovableID();
  newCylinder->radius = radius;
  newCylinder->height = height;
  newCylinder->graphicsHandle = graphicsHandle;
  memcpy(&newCylinder->boxCorners,&boxCorners[0], 8* sizeof(Vector3d));

  std::list<SimObject*>::const_iterator pos;
  for(pos = childNodes.begin(); pos != childNodes.end(); ++pos)
  {
    SimObject* childNode = (*pos)->clone();
    newCylinder->addChildNode(childNode, false);
  }

  if(createsNewMovID)
    simulation->removeCurrentMovableID();

  SimObject* newObject = newCylinder;
  return newObject;
}

double Cylinder::getMaxDistanceTo(const Vector3d& base) const
{
  double dist(0);
  double vecLen((base - position).getLength());
  double radius(sqrt(height*height/4.0 + this->radius*this->radius));
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

bool Cylinder::intersectWithRay(const Vector3d& pos, const Vector3d& ray, Vector3d& intersectPos) 
{
  return SimGeometry::intersectRayAndCylinder(pos,ray,position,rotation,height,radius,false,intersectPos);
}

void Cylinder::createPhysics()
{
  //create physic engine stuff
  setPointerToPhysicalWorld(simulation->getPhysicalWorld());
  setPointerToCollisionSpace(simulation->getCurrentSpace());
  if(movableID)
  {
    createGeometry(*simulation->getMovableSpace());
  }
  else
  {
    createGeometry(*simulation->getStaticSpace());
  }
  //create and set body only if moving object
  if(movableID)
  {
    createBody();
    dGeomSetBody(geometry, body);
  }
}

void Cylinder::createBody()
{
  if(body == 0)
  {
    body = dBodyCreate(*simulation->getPhysicalWorld());
    dMass m;
    dMassSetCylinder(&m, 1, 3, dReal(height), dReal(height));
    dMassAdjust(&m, dReal(mass));
    dBodySetMass(body, &m);
    dBodySetPosition(body, dReal(position.v[0]), dReal(position.v[1]), dReal(position.v[2]));
    dMatrix3 rotationMatrix;
    ODETools::convertMatrix(rotation, rotationMatrix);
    dBodySetRotation(body, rotationMatrix);
  }
}

void Cylinder::createGeometry(dSpaceID space)
{
  if(geometry == 0)
  {
    setGeometry(dCreateCylinder(space, dReal(radius), dReal(height)));
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

void Cylinder::computeMassProperties(dMass& m)
{
  dMassSetCylinder(&m, 1, 3, dReal(height), dReal(height));
  dMassAdjust(&m, dReal(mass));
}

void Cylinder::calculateCorners()
{
  //Set corner points:
  double length_2(radius);
  double width_2(radius);
  double height_2(height * 0.5);
  boxCorners[0] = Vector3d(-length_2, -width_2, height_2);
  boxCorners[1] = Vector3d(length_2, -width_2, height_2);
  boxCorners[2] = Vector3d(length_2, width_2, height_2);
  boxCorners[3] = Vector3d(-length_2, width_2, height_2);
  boxCorners[4] = Vector3d(-length_2, width_2, -height_2);
  boxCorners[5] = Vector3d(length_2, width_2, -height_2);
  boxCorners[6] = Vector3d(length_2, -width_2, -height_2);
  boxCorners[7] = Vector3d(-length_2, -width_2, -height_2);
}

void Cylinder::updateLocalBoundingBox()
{
  boundingBox.maxi = position;
  boundingBox.mini = position;
  for(int i=0; i<8; i++)
  {
    Vector3d p(boxCorners[i]);
    p.rotate(rotation);
    p += position;
    boundingBox.expand(p);
  }
}

void Cylinder::determineRollingFrictionFlag()
{
  bool result = false;
  if(movableID > 0)
  {
    if(compoundPhysicalObject->isSimple())
    {
      result = true;
      SimObject* parent = getParentNode();
      while(parent != simulation->getObjectTreeRoot() && result == true)
      {
        if(parent->getKind() != "compound")
        {
          result = false;
          return;
        }
        parent  = parent->getParentNode();
      }
    }
  }
  rollingFrictionFlag = result;
}

void Cylinder::applySimpleRollingFriction(double rfc)
{
  const dReal* curAngVel = dBodyGetAngularVel(body);
  dBodyAddTorque(body, (curAngVel[0] * (dReal)(-rfc)), (curAngVel[1] * (dReal)(-rfc)), (curAngVel[2] * (dReal)(-rfc)));
}

void Cylinder::applyRollingFriction(double cosAlpha, double rfc)
{
  // Something seems to be missing here...
}
