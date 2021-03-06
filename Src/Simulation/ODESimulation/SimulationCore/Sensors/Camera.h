/**
 * @file Camera.h
 * 
 * Definition of class Camera
 *
 * @author <A href="mailto:timlaue@informatik.uni-bremen.de">Tim Laue</A>
 */ 

#ifndef CAMERA_H_
#define CAMERA_H_

#include "ImagingSensor.h"


/**
* @class Camera
*
* A class representing a camera.
*/
class Camera : public ImagingSensor
{
protected:
  /** Past positions and rotations (needed for motion blur) */
  Vector3d previous_positions[12];
  Matrix3d previous_rotations[12];
  int cycle_order_offset;
  const float* clearColor;

  // environment mapping information
  void renderCubeMap(VisualizationParameterSet& visParams);

  /** A pointer to the image*/
  unsigned char* image;
  /** The size of the image*/
  unsigned int imageSize;
  
  /** Executes the rendering of the image */
  void render();

  /** render according to the composition */
  void renderingInstructions(VisualizationParameterSet& visParams);

  /** Helper Function to set the opengl camera
  * @param visParams the VisualizationParameterSet with cube map information
  * @param generateCubeMap if > 0 the camera is set inside the environment mapping center
  */
  void setCamera(const VisualizationParameterSet& visParams, const int generateCubeMap = 0);

public:
  /** Constructor*/
  Camera();

  /** Constructor
  * @param attributes The attributes of the camera
  */
  Camera(const AttributeSet& attributes);

  /** Destructor */
  ~Camera();

  /** Returns a string describing the kind of object
  * @return The kind
  */
  virtual std::string getKind() const {return "camera";}
  
  /** Sets the resolution and initializes the image
  * @param x The new image width
  * @param y The new image height
  */
  void setResolution(int x, int y);

  /** Adds the object to some internal lists
  * @param sensorportList A list of all sensor ports in the scene
  * @param actuatorportList A list of all actuator ports in the scene
  * @param actuatorList A list of all actuators in the scene
  */
  virtual void addToLists(std::vector<Sensorport*>& sensorportList,
    std::vector<Actuatorport*>& actuatorportList,
    std::vector<Actuator*>& actuatorList);

  /** Adds descriptions of the object and its children to a list
  * @param objectDescriptionTree The list of descriptions
  * @param The depth of the object in the object tree
  */
  virtual void addToDescriptions(std::vector<ObjectDescription>& objectDescriptionTree,
    int depth);

  /** Clones the object and its child nodes
  * @return A pointer to a copy of the object
  */
  virtual SimObject* clone() const;

  /** Computes the image
  * @param value A pointer to be set to the image
  * @param portId The id of the sensorport
  */
  void computeValue(unsigned char*& value, int portId);

  /**
   * Writes back the results from the physical simulation.
   * Sets position and rotation from body/geometry to simulation object.
   * @param store_last_results If true the last 12 results will be stored
   */
  virtual void writeBackPhysicalResult(const bool& store_last_results);
};

#endif //CAMERA_H_
