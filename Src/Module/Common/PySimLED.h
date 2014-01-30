#ifndef PyLED_H
#define PyLED_H
#include "Module/Common/LEDSimRequest.h"
#include <boost/python.hpp>
using namespace std;
using namespace boost::python;
using namespace boost;
class LEDSimModule
{
public:
	LEDSimModule();
	~LEDSimModule();
	void onFindOppGoal();
	void onFindOwnGoal();
	void offFindOppGoal();
	void offFindOwnGoal();

private:
	LEDSimRequest* ledRequest;
};

void set_led_reference(LEDSimRequest* _ledRequest);
void c_init_led();

#endif
