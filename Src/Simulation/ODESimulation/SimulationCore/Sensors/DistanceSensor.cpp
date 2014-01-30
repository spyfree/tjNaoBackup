/**
 * @file DistanceSensor.cpp
 * 
 * Implementation of class DistanceSensor
 *
 * @author <A href="mailto:timlaue@informatik.uni-bremen.de">Tim Laue</A>
 */

#include "DistanceSensor.h"
#include "../Platform/OffScreenRenderer.h"
#include "../OpenGL/GLHelper.h"
#include "../Simulation/Sensorport.h"
#include "../Simulation/Simulation.h"


DistanceSensor::DistanceSensor():ImagingSensor()
{
  sensorData = 0;
  visualizationColor = Vector3d(0.0,1.0,0.0);
}


DistanceSensor::DistanceSensor(const AttributeSet& attributes):ImagingSensor(attributes)
{
  name = ParserUtilities::getValueFor(attributes, "name");
  sensorData = 0;
  visualizationColor = Vector3d(0.0,1.0,0.0);
}


DistanceSensor::~DistanceSensor()
{
  if(sensorData)
    delete [] sensorData;
}

void DistanceSensor::addToLists(std::vector<Sensorport*>& sensorportList,
                        std::vector<Actuatorport*>& actuatorportList,
                        std::vector<Actuator*>& actuatorList) 
{
  std::vector<int> dimensions;
  dimensions.clear();
  dimensions.push_back(resolutionX);
  dimensions.push_back(resolutionY);
  dimensions.push_back(1);
  Sensorport* distanceSensorPort = new Sensorport
     ("distance-data", 0, doubleArraySensor, this, minRange, maxRange, dimensions);
  sensorportList.push_back(distanceSensorPort);
}

void DistanceSensor::addToDescriptions(std::vector<ObjectDescription>& objectDescriptionTree,
                               int depth) 
{
  SimSensor::addToDescriptions(objectDescriptionTree, depth);  
  //Add Sensorport:
  ObjectDescription sensorportDesc;
  sensorportDesc.name = "distance-data";
  sensorportDesc.fullName = fullName + ".distance-data";
  sensorportDesc.depth = depth + 1;
  sensorportDesc.type = OBJECT_TYPE_SENSORPORT;
  objectDescriptionTree.push_back(sensorportDesc);
}

void DistanceSensor::setResolution(int x, int y)
{
  if(sensorData)
  {
    delete[] sensorData;
    standardRenderingBuffer.reset();
  }
  resolutionX = x;
  resolutionY = y;
  standardRenderingBuffer.resolutionX = x;
  standardRenderingBuffer.resolutionY = y;
  sensorDataSize = x * y;
  sensorData = new double[sensorDataSize];
}

void DistanceSensor::renderingInstructions(ImagingBuffer<float>& buffer)
{
  // set viewport
  glViewport(0, 0, buffer.resolutionX, buffer.resolutionY);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(angleY, tan(angleX * M_PI / 360) / tan(angleY * M_PI / 360), minRange, maxRange);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  // clear the viewport
  glClearDepth(1.0);
  Surface* backgroundSurface = simulation->getBackgroundSurface();
  glClearColor(backgroundSurface->color[0], backgroundSurface->color[1],
               backgroundSurface->color[2], backgroundSurface->color[3]);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // set the visual parameters
  VisualizationParameterSet visParams;
  visParams.surfaceStyle = VisualizationParameterSet::FLAT_SHADING;
  visParams.drawForSensor = true;
  simulation->enableStateDL(visParams.surfaceStyle);

  // set the camera
  Vector3d forwardVec(1.0,0.0,0.0);
  Vector3d upVec(0.0,0.0,1.0);
  forwardVec.rotate(rotation);
  upVec.rotate(rotation);
  gluLookAt(position.v[0], position.v[1], position.v[2], 
            position.v[0] + forwardVec.v[0], position.v[1] + forwardVec.v[1], 
            position.v[2] + forwardVec.v[2],
            upVec.v[0], upVec.v[1], upVec.v[2]);

  glGetDoublev(GL_MODELVIEW_MATRIX, buffer.modelViewMatrix);
  glGetDoublev(GL_PROJECTION_MATRIX, buffer.projectionMatrix);
  glGetIntegerv(GL_VIEWPORT, buffer.viewPort);
  
  rootNode->draw(visParams);

  glFlush();

  simulation->disableStateDL(visParams.surfaceStyle);
}

void DistanceSensor::computeValue(double*& value, int portId)
{ 
  if(!osRenderer)
    osRenderer = simulation->getOffscreenRenderer();
  int simulationStep(simulation->getSimulationStep());
  if(simulationStep>lastComputationStep)
  {
    OffscreenRenderer::Content offContent = OffscreenRenderer::DEPTH;
    osRenderer->prepareRendering(standardRenderingBuffer.resolutionX, 
                                 standardRenderingBuffer.resolutionY);
    renderingInstructions(standardRenderingBuffer);
    osRenderer->finishRendering(standardRenderingBuffer.pixels,
                                standardRenderingBuffer.resolutionX, 
                                standardRenderingBuffer.resolutionY, 
                                offContent);
    transformDataToDistances(standardRenderingBuffer);
    lastComputationStep = simulationStep;
  }
  value = sensorData;
}

void DistanceSensor::transformDataToDistances(ImagingBuffer<float>& buffer)
{
  Vector3d objectPos;
  GLdouble unProjectMatrix[16];
  GLHelper::getGLH()->prepareMatrixForUnProject(buffer.modelViewMatrix, 
                                         buffer.projectionMatrix, 
                                         unProjectMatrix);
  if(projection == PERSPECTIVE_PROJECTION)
  {
    for(unsigned int i=0; i<sensorDataSize; i++)
    {
      double winX((double)(i%buffer.resolutionX));
      double winY((double)(i/buffer.resolutionX));
      double winZ(buffer.pixels[i]);
      GLHelper::getGLH()->unProject(winX, winY, winZ, unProjectMatrix, buffer.viewPort, &objectPos.v[0], &objectPos.v[1], &objectPos.v[2]);
      sensorData[i] = (objectPos-position).getLength();
    }
  }
  else //projection == SPHERICAL_PROJECTION
  {
    for(unsigned int i=0; i<sensorDataSize; i++)
    {
      double winX(pixelLUT[i].pixel - &pixelLUT[i].buffer->pixels[0]);
      double winY((double)(0));
      double winZ(*(pixelLUT[i].pixel));
      GLHelper::getGLH()->unProject(winX, winY, winZ, unProjectMatrix, buffer.viewPort, &objectPos.v[0], &objectPos.v[1], &objectPos.v[2]);
      sensorData[sensorDataSize-1-i] = (objectPos-position).getLength();
    }
  }
}

SimObject* DistanceSensor::clone() const
{
  DistanceSensor* newDistanceSensor = new DistanceSensor();
  newDistanceSensor->copyStandardMembers(this);
  newDistanceSensor->sensorDataSize = sensorDataSize;
  newDistanceSensor->sensorData = new double[sensorDataSize];
  newDistanceSensor->standardRenderingBuffer = standardRenderingBuffer;
  std::list<SimObject*>::const_iterator pos;
  for(pos = childNodes.begin(); pos != childNodes.end(); ++pos)
  {
    SimObject* childNode = (*pos)->clone();
    newDistanceSensor->addChildNode(childNode, false);
  }
  SimObject* newObject = newDistanceSensor;
  return newObject;
}

void DistanceSensor::drawObject(const VisualizationParameterSet& visParams)
{
  if(visParams.visualizeSensors && !visParams.drawForSensor)
  {
    glColor3d(visualizationColor.v[0],
      visualizationColor.v[1],
      visualizationColor.v[2]);
    Vector3d camPos(position);
    if(projection == SPHERICAL_PROJECTION)
    {
      glBegin(GL_LINES);
        for(unsigned int i=0; i< sensorDataSize; i++)
        {
          Vector3d distanceVec(sensorData[i],0,0);
          double maxAngle(Functions::toRad(angleX));
          double zRotation(maxAngle/(double)resolutionX);
          zRotation *= (double)i;
          zRotation -= maxAngle/2.0;
          distanceVec.rotateZ(zRotation);
          distanceVec.rotate(rotation);
          distanceVec+=position;
          glVertex3d (camPos.v[0], camPos.v[1], camPos.v[2]);
          glVertex3d (distanceVec.v[0], distanceVec.v[1], distanceVec.v[2]);
        }
      glEnd();
    }
    else
    {
      Vector3d distanceVec(sensorData[0],0,0);
      distanceVec.rotate(rotation);
      distanceVec+=position;
      //Draw distance vector
      glBegin(GL_LINES);
        glVertex3d (camPos.v[0], camPos.v[1], camPos.v[2]);
        glVertex3d (distanceVec.v[0], distanceVec.v[1], distanceVec.v[2]);
      glEnd();
    }
  }
}

void DistanceSensor::postProcessAfterParsing()
{
  if(projection == SPHERICAL_PROJECTION)
  {
    pixelLUT.resize(sensorDataSize);
    if(angleX <= 120) //All pixels will be in the front buffer
    {
      //Compute new resolution of rendering buffer
      double maxAngle(Functions::toRad(angleX/2.0));
      double minPixelWidth(tan(maxAngle/((double)resolutionX/2.0)));
      double totalWidth(tan(maxAngle));
      double newXRes(totalWidth/minPixelWidth);
      standardRenderingBuffer.resolutionX = (unsigned int)(ceil(newXRes))*2;
      standardRenderingBuffer.reset();
      standardRenderingBuffer.pixels = new float[standardRenderingBuffer.resolutionX];
      //Compute values for LUT (sensor data -> rendering buffer)
      double firstAngle(-maxAngle);
      double step(maxAngle/((double)resolutionX/2.0));
      double currentAngle(firstAngle);
      double gToPixelFactor(newXRes/tan(maxAngle));
      for(unsigned int i=0; i<(unsigned int)resolutionX; i++)
      {
        pixelLUT[i].buffer = &standardRenderingBuffer;
        double g(tan(currentAngle));
        g *= gToPixelFactor;
        int gPixel((int)g + (int)standardRenderingBuffer.resolutionX/2);
        pixelLUT[i].pixel = &(standardRenderingBuffer.pixels[gPixel]);
        currentAngle += step;
      }
    }
    else
    {
      assert(false); //Not implemented yet :-(
    }
  }
  else //projection == PERSPECTIVE_PROJECTION
  {
    standardRenderingBuffer.reset();
    standardRenderingBuffer.pixels = new float[sensorDataSize];
  }
}
