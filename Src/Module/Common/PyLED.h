#ifndef PyLED_H
#define PyLED_H
#include "Module/Common/LEDRequest.h"
#include <boost/python.hpp>
using namespace std;
using namespace boost::python;
using namespace boost;
class LEDModule
{
public:
	LEDModule();
	~LEDModule();
	void onFindOppGoal();
	void onFindOwnGoal();
	void offFindOppGoal();
	void offFindOwnGoal();

private:
	LEDRequest* ledRequest;
};

void set_led_reference(LEDRequest* _ledRequest);
void c_init_led();

#endif
