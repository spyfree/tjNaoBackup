#ifndef OutBehaviorData_h_DEFINED
#define OutBehaviorData_h_DEFINED

class OutBehaviorData
{
public:
    OutBehavior()
    {
        intData.reserve(NUM_INT_DATA);
        jointData.reserve(NUM_JOINT_DATA);
    }
    ~OutBehavior();

    enum
    {
        NUM_INT_DATA = 4,
        NUM_FLOAT_DATA = 22
    };
    std::vector<int> intData;
    std::vector<float> jointData;

};

#endif
