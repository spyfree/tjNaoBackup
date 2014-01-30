#ifndef OutBehaviorData_h_DEFINED
#define OutBehaviorData_h_DEFINED
#include <vector>

class OutBehaviorData
{
public:
    OutBehaviorData()
    {
        intData.reserve(NUM_INT_DATA);
        jointData.reserve(NUM_JOINT_DATA);
    }
    //~OutBehaviorData();

    enum
    {
        NUM_INT_DATA = 4,
        NUM_JOINT_DATA = 22
    };
    std::vector<int> intData;
    std::vector<float> jointData;
	double leftx,lefty,rightx,righty;

};

#endif
