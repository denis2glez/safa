#ifndef ITK_READEREVENTS_H
#define ITK_READEREVENTS_H

#include "itkEventObject.h"

namespace itk {

itkEventMacro( StartScanUIDEvent, EventObject );
itkEventMacro( EndScanUIDEvent,   EventObject );
itkEventMacro( ProgressUIDEvent,  EventObject );
itkEventMacro( StartReadEvent,    EventObject );
itkEventMacro( EndReadEvent,      EventObject );
itkEventMacro( ProgressReadEvent, EventObject );
 
};

#endif
