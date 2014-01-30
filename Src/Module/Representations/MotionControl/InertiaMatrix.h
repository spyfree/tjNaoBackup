#ifndef InertiaMatrix_H
#define InertiaMatrix_H

#include "Tools/Math/Pose3D.h"

/**
* @class InertiaMatrix
* Matrix describing the transformation from ground to the acceleration sensors.
*/
class InertiaMatrix : public Pose3D
{
public:
	/** Default constructor. */
	InertiaMatrix() {}

	Pose3D inertiaOffset; /**< The estimated offset (including odometry) from last inertia matrix to this one. (This is something like the velocity * 0.02.) */
	bool isValid; /**< Matrix is only valid if robot is on ground. */
};

#endif //InertiaMatrix_H
