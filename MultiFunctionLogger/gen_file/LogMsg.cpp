/********************************************************************************
 * This file was automatically generated by SmartDDS generator.
 * Version 1.0.1
 *
 * Do not make changes to this file unless you know what you are doing--modify
 * the SmartDDS interface file instead.
 ********************************************************************************/

#include "LogMsg.h"

SDDS_BEGIN_TYPE_INITIALIZE_METHOD(LogMsg, LogMsg)
	SDDS_DEFINE_INITIALIZE(DDS_LongLong, log_time, 0)
	SDDS_DEFINE_INITIALIZE(DDS_Int, log_level, 0)
	SDDS_DEFINE_INITIALIZE(DDS_Int, log_definition, 0)
	SDDS_DEFINE_INITIALIZE_ARRAY(DDS_Char, log_str, 0, 256)
SDDS_END_TYPE_INITIALIZE_METHOD(LogMsg)

SDDS_BEGIN_TYPE_FINALIZE_METHOD(LogMsg, LogMsg)
	SDDS_DEFINE_FINALIZE(DDS_LongLong, log_time, 0)
	SDDS_DEFINE_FINALIZE(DDS_Int, log_level, 0)
	SDDS_DEFINE_FINALIZE(DDS_Int, log_definition, 0)
	SDDS_DEFINE_FINALIZE_ARRAY(DDS_Char, log_str, 0, 256)
SDDS_END_TYPE_FINALIZE_METHOD(LogMsg)

SDDS_BEGIN_TYPE_COPY_METHOD(LogMsg, LogMsg)
	SDDS_DEFINE_COPY(DDS_LongLong, log_time, 0)
	SDDS_DEFINE_COPY(DDS_Int, log_level, 0)
	SDDS_DEFINE_COPY(DDS_Int, log_definition, 0)
	SDDS_DEFINE_COPY_ARRAY(DDS_Char, log_str, 0, 256)
SDDS_END_TYPE_COPY_METHOD(LogMsg)

SDDS_BEGIN_TYPE_SERIALIZE_METHOD(LogMsg, LogMsg)
	SDDS_DEFINE_SERIALIZE(DDS_LongLong, log_time, 0)
	SDDS_DEFINE_SERIALIZE(DDS_Int, log_level, 0)
	SDDS_DEFINE_SERIALIZE(DDS_Int, log_definition, 0)
	SDDS_DEFINE_SERIALIZE_ARRAY(DDS_Char, log_str, 0, 256)
SDDS_END_TYPE_SERIALIZE_METHOD(LogMsg)

SDDS_BEGIN_TYPE_DESERIALIZE_METHOD(LogMsg, LogMsg)
	SDDS_DEFINE_DESERIALIZE(DDS_LongLong, log_time, 0)
	SDDS_DEFINE_DESERIALIZE(DDS_Int, log_level, 0)
	SDDS_DEFINE_DESERIALIZE(DDS_Int, log_definition, 0)
	SDDS_DEFINE_DESERIALIZE_ARRAY(DDS_Char, log_str, 0, 256)
SDDS_END_TYPE_DESERIALIZE_METHOD(LogMsg)

SDDS_BEGIN_TYPE_GETMAXSIZESERIALIZED_METHOD(LogMsg, LogMsg)
	SDDS_DEFINE_GETMAXSIZESERIALIZED(DDS_LongLong, log_time, 0)
	SDDS_DEFINE_GETMAXSIZESERIALIZED(DDS_Int, log_level, 0)
	SDDS_DEFINE_GETMAXSIZESERIALIZED(DDS_Int, log_definition, 0)
	SDDS_DEFINE_GETMAXSIZESERIALIZED_ARRAY(DDS_Char, log_str, 0, 256)
SDDS_END_TYPE_GETMAXSIZESERIALIZED_METHOD(LogMsg)



