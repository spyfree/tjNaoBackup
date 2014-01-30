#ifndef __SignalUnconflict_H_
#define __SignalUnconflict_H_ 

#if defined(signals) && defined(QOBJECTDEFS_H) && \
	!defined(QT_MOC_CPP)
#  undef signals
#  define signals signals
#endif

#include <boost/signal.hpp>
namespace boost
{
	namespace signalslib = signals;
}

#if defined(signals) && defined(QOBJECTDEFS_H) && \
	!defined(QT_MOC_CPP)
#  undef signals
// Restore the macro definition of "signals", as it was
// defined by Qt's <qobjectdefs.h>.
#  define signals protected
#endif

#endif//__SignalUnconflict_H_