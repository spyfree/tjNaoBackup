/**
* @file Representations/Perception/RegionPercept.cpp
*
*/

#include "BlobPercept.h"

BlobPercept::Blob* BlobPercept::Blob::getRootBlob() const
{
	Blob* r = root;
	if(r == NULL)
		return NULL;
	while(r->root != NULL)
		r = r->root;
	return r;
}

Vector2<int> BlobPercept::Blob::getCenter() const
{
	if(childs.size() == 0)
		return getRootBlob()->getCenter();

	int childIdx;
	Vector2<int> center;
	Segment* segment;
	for(std::vector<Segment*>::const_iterator segmentIter = childs.begin();
		segmentIter != childs.end();
		segmentIter++)
	{
		childIdx = segmentIter - childs.begin();
		segment = *segmentIter;
		center.x += segment->x;
		center.y += segment->y + (segment->length >> 1);
	}
	center.x /= (int)childs.size();
	center.y /= (int)childs.size();

	return center;
}

int BlobPercept::Blob::calcMoment00() const
{
	int m00 = 0;
	for(std::vector<Segment*>::const_iterator seg = childs.begin(); seg != childs.end(); seg++)
		m00 += (*seg)->length;

	return m00;
}

int BlobPercept::Blob::calcMoment10() const
{
	int m10 = 0;

	for(std::vector<Segment*>::const_iterator seg = childs.begin(); seg != childs.end(); seg++)
		m10 += (*seg)->x * (*seg)->length;

	return m10;
}

int BlobPercept::Blob::calcMoment01() const
{
	int m01 = 0;

	for(std::vector<Segment*>::const_iterator seg = childs.begin(); seg != childs.end(); seg++)
	{
		const int ylo = (*seg)->y;
		const int yhi = (*seg)->y + (*seg)->length;

		m01 += (yhi * (yhi - 1) - ylo * (ylo - 1))/2;
	}

	return m01;
}

double BlobPercept::Blob::calcCMoment11(int swp_x, int swp_y) const
{
	double cm11 = 0;
	for(std::vector<Segment*>::const_iterator seg = childs.begin(); seg != childs.end(); seg++)
	{
		const double yStart = (*seg)->y, yEnd = (*seg)->y + (*seg)->length;
		const double ySum = GAUSS_SUM(yEnd) - GAUSS_SUM(yStart);
		cm11 += (*seg)->x * ySum - (*seg)->length * (*seg)->x * swp_y - 
			swp_x * ySum + (*seg)->length * swp_x * swp_y;
	}

	return cm11;
}

int BlobPercept::Blob::calcCMoment20(int swp_x) const
{
	int cm20 = 0;

	for(std::vector<Segment*>::const_iterator seg = childs.begin(); seg != childs.end(); seg++)
		cm20 += (*seg)->length * ((*seg)->x-swp_x) * ((*seg)->x-swp_x);

	return cm20;
}


double BlobPercept::Blob::calcCMoment02(int swp_y) const
{
	double cm02 = 0;

	for(std::vector<Segment*>::const_iterator seg = childs.begin(); seg != childs.end(); seg++)
	{
		// sum(x) = sum_(y = seg->y ; y< seg->y + seg->length)(x)
		// cm02 = sum((y-y_s)^2)
		//      = sum(y^2 - 2y*y_s + y_s^2)
		//      = sum(y^2) - 2*y_s*sum(y) + sum(y_s^2)
		//      = sum(y^2) - 2*y_s*sum(y) + length * y_s^2
		const double yStart = (*seg)->y, yEnd = (*seg)->y+(*seg)->length;
		cm02 += GAUSS_SUM2(yEnd) - GAUSS_SUM2(yStart) - 2 * swp_y * (GAUSS_SUM(yEnd) - GAUSS_SUM(yStart)) + (*seg)->length * swp_y*swp_y;
	}

	return cm02;
}

void BlobPercept::Blob::mergeWithBlob(Blob *other)
{
	//merge the childs of both regions (sorted)
	std::vector<BlobPercept::Segment*>::iterator regIter1 = childs.begin();
	std::vector<BlobPercept::Segment*>::iterator regIter2 = other->childs.begin();
	std::vector<BlobPercept::Segment*> mergedChilds;
	while(!(regIter1 == childs.end() && regIter2 == other->childs.end()))
	{
		if(regIter1 == childs.end())
		{
			mergedChilds.push_back(*regIter2);
			(*regIter2)->blob = this;
			regIter2++;
		}
		else if(regIter2 == other->childs.end())
		{
			mergedChilds.push_back(*regIter1);
			regIter1++;
		}
		else
		{
			if(*regIter1 < *regIter2)//按照x从小到大，y从小到到排序
			{
				mergedChilds.push_back(*regIter1);
				regIter1++;
			}
			else
			{
				mergedChilds.push_back(*regIter2);
				(*regIter2)->blob = this;
				regIter2++;
			}
		}
	}
	childs = mergedChilds;

	size += other->size;
	neighborBlobs.insert(neighborBlobs.end(), other->neighborBlobs.begin(), other->neighborBlobs.end());
	if(other->min_y < min_y)
		min_y = other->min_y;
	if(other->max_y > max_y)
		max_y = other->max_y;
}

bool BlobPercept::Segment::operator<(Segment *s2)
{
	if(this->x < s2->x)
		return true;
	else if(this->x > s2->x)
		return false;
	else if(this->y < s2->y)
		return true;
	else
		return false;
}
