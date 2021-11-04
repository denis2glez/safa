#ifndef __ITKDICOMTAGS_H__
#define __ITKDICOMTAGS_H__

/* (0008,0021)	Series Date: Date the Series started. */
#define DCM_DATE_TAG                    "0008|0021"

#define DCM_ADQUISITION_NUMBER_TAG      "0020|0012"
#define DCM_COMPONENT_TAG               DCM_ADQUISITION_NUMBER_TAG

/* (0008,0032) Acquisition Time: The time the acquisition of data that
   resulted in this image started
 */
#define DCM_ACQUISITION_TIME_TAG        "0008|0032"
/*
  (0018,1060) Trigger Time: Time interval measured in msec from the
  start of the R-wave to the beginning of the data taking.
*/
#define DCM_TRIGGER_TIME_TAG            "0018|1060"
//#define DCM_TIMESTEP_TAG                DCM_TRIGGER_TIME_TAG
#define DCM_TIMESTEP_TAG                DCM_ACQUISITION_TIME_TAG

#endif

