#include "RobotPose.h"
#include "Module/Common/Geometry.h"

void RobotPose::draw()
{
	Vector2<double> bodyPoints[4] = {Vector2<double>(55,90),
		Vector2<double>(-55,90),
		Vector2<double>(-55,-90),
		Vector2<double>(55,-90)};
	for(int i=0; i<4; i++)
	{
		bodyPoints[i] = Geometry::rotate(bodyPoints[i], rotation);
		bodyPoints[i] += translation;
	}
	Vector2<double> dirVec(300,0);
	dirVec.rotate(rotation);
	dirVec += translation;
	LINE(translation.x, translation.y, dirVec.x, dirVec.y, 
		20, LocDrawings::ps_solid, ColorClasses::white);
	POLYGON( 4, bodyPoints, 20, LocDrawings::ps_solid, 
		ownTeamColorForDrawing, LocDrawings::bs_solid, ColorClasses::white);
	CIRCLE( translation.x, translation.y, 42, 1, 
		LocDrawings::ps_solid, ownTeamColorForDrawing, LocDrawings::bs_solid, ownTeamColorForDrawing);
}
void RobotPose::draw(ColorRGBA colour)
{
	Vector2<double> bodyPoints[4] = {Vector2<double>(55,90),
		Vector2<double>(-55,90),
		Vector2<double>(-55,-90),
		Vector2<double>(55,-90)};
	for(int i=0; i<4; i++)
	{
		bodyPoints[i] = Geometry::rotate(bodyPoints[i], rotation);
		bodyPoints[i] += translation;
	}
	Vector2<double> dirVec(300,0);
	dirVec.rotate(rotation);
	dirVec += translation;
	LINE(translation.x, translation.y, dirVec.x, dirVec.y, 
		50, LocDrawings::ps_solid, ColorClasses::white);
	POLYGON( 4, bodyPoints, 20, LocDrawings::ps_solid, 
		colour, LocDrawings::bs_solid, ColorClasses::white);
	CIRCLE( translation.x, translation.y, 42, 1, 
		LocDrawings::ps_solid, colour, LocDrawings::bs_solid, colour);
}

