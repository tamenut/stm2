/********************************************************************************
 * This file was automatically generated by SmartDDS generator.
 * Version 1.0.1
 *
 * Do not make changes to this file unless you know what you are doing--modify
 * the SmartDDS interface file instead.
 ********************************************************************************/

#include "ConfigMsg.h"

SDDS_BEGIN_TYPE_INITIALIZE_METHOD(ConfigMsg, ConfigMsg)
	SDDS_DEFINE_INITIALIZE(DDS_Int, dst_process_id, 0)
	SDDS_DEFINE_INITIALIZE_ARRAY(DDS_Char, dst_process_name, 0, 128)
	SDDS_DEFINE_INITIALIZE(DDS_Int, config_type, 0)
	SDDS_DEFINE_INITIALIZE(DDS_Int, value, 0)
SDDS_END_TYPE_INITIALIZE_METHOD(ConfigMsg)

SDDS_BEGIN_TYPE_FINALIZE_METHOD(ConfigMsg, ConfigMsg)
	SDDS_DEFINE_FINALIZE(DDS_Int, dst_process_id, 0)
	SDDS_DEFINE_FINALIZE_ARRAY(DDS_Char, dst_process_name, 0, 128)
	SDDS_DEFINE_FINALIZE(DDS_Int, config_type, 0)
	SDDS_DEFINE_FINALIZE(DDS_Int, value, 0)
SDDS_END_TYPE_FINALIZE_METHOD(ConfigMsg)

SDDS_BEGIN_TYPE_COPY_METHOD(ConfigMsg, ConfigMsg)
	SDDS_DEFINE_COPY(DDS_Int, dst_process_id, 0)
	SDDS_DEFINE_COPY_ARRAY(DDS_Char, dst_process_name, 0, 128)
	SDDS_DEFINE_COPY(DDS_Int, config_type, 0)
	SDDS_DEFINE_COPY(DDS_Int, value, 0)
SDDS_END_TYPE_COPY_METHOD(ConfigMsg)

SDDS_BEGIN_TYPE_SERIALIZE_METHOD(ConfigMsg, ConfigMsg)
	SDDS_DEFINE_SERIALIZE(DDS_Int, dst_process_id, 0)
	SDDS_DEFINE_SERIALIZE_ARRAY(DDS_Char, dst_process_name, 0, 128)
	SDDS_DEFINE_SERIALIZE(DDS_Int, config_type, 0)
	SDDS_DEFINE_SERIALIZE(DDS_Int, value, 0)
SDDS_END_TYPE_SERIALIZE_METHOD(ConfigMsg)

SDDS_BEGIN_TYPE_DESERIALIZE_METHOD(ConfigMsg, ConfigMsg)
	SDDS_DEFINE_DESERIALIZE(DDS_Int, dst_process_id, 0)
	SDDS_DEFINE_DESERIALIZE_ARRAY(DDS_Char, dst_process_name, 0, 128)
	SDDS_DEFINE_DESERIALIZE(DDS_Int, config_type, 0)
	SDDS_DEFINE_DESERIALIZE(DDS_Int, value, 0)
SDDS_END_TYPE_DESERIALIZE_METHOD(ConfigMsg)

SDDS_BEGIN_TYPE_GETMAXSIZESERIALIZED_METHOD(ConfigMsg, ConfigMsg)
	SDDS_DEFINE_GETMAXSIZESERIALIZED(DDS_Int, dst_process_id, 0)
	SDDS_DEFINE_GETMAXSIZESERIALIZED_ARRAY(DDS_Char, dst_process_name, 0, 128)
	SDDS_DEFINE_GETMAXSIZESERIALIZED(DDS_Int, config_type, 0)
	SDDS_DEFINE_GETMAXSIZESERIALIZED(DDS_Int, value, 0)
SDDS_END_TYPE_GETMAXSIZESERIALIZED_METHOD(ConfigMsg)


SDDS_BEGIN_TYPE_INITIALIZE_METHOD(AppInfoMsg, AppInfoMsg)
	SDDS_DEFINE_INITIALIZE(DDS_Int, process_id, 0)
	SDDS_DEFINE_INITIALIZE_ARRAY(DDS_Char, process_name, 0, 128)
	SDDS_DEFINE_INITIALIZE(DDS_LongLong, app_created_time, 0)
	SDDS_DEFINE_INITIALIZE(DDS_Int, redundancy_status, 0)
SDDS_END_TYPE_INITIALIZE_METHOD(AppInfoMsg)

SDDS_BEGIN_TYPE_FINALIZE_METHOD(AppInfoMsg, AppInfoMsg)
	SDDS_DEFINE_FINALIZE(DDS_Int, process_id, 0)
	SDDS_DEFINE_FINALIZE_ARRAY(DDS_Char, process_name, 0, 128)
	SDDS_DEFINE_FINALIZE(DDS_LongLong, app_created_time, 0)
	SDDS_DEFINE_FINALIZE(DDS_Int, redundancy_status, 0)
SDDS_END_TYPE_FINALIZE_METHOD(AppInfoMsg)

SDDS_BEGIN_TYPE_COPY_METHOD(AppInfoMsg, AppInfoMsg)
	SDDS_DEFINE_COPY(DDS_Int, process_id, 0)
	SDDS_DEFINE_COPY_ARRAY(DDS_Char, process_name, 0, 128)
	SDDS_DEFINE_COPY(DDS_LongLong, app_created_time, 0)
	SDDS_DEFINE_COPY(DDS_Int, redundancy_status, 0)
SDDS_END_TYPE_COPY_METHOD(AppInfoMsg)

SDDS_BEGIN_TYPE_SERIALIZE_METHOD(AppInfoMsg, AppInfoMsg)
	SDDS_DEFINE_SERIALIZE(DDS_Int, process_id, 0)
	SDDS_DEFINE_SERIALIZE_ARRAY(DDS_Char, process_name, 0, 128)
	SDDS_DEFINE_SERIALIZE(DDS_LongLong, app_created_time, 0)
	SDDS_DEFINE_SERIALIZE(DDS_Int, redundancy_status, 0)
SDDS_END_TYPE_SERIALIZE_METHOD(AppInfoMsg)

SDDS_BEGIN_TYPE_DESERIALIZE_METHOD(AppInfoMsg, AppInfoMsg)
	SDDS_DEFINE_DESERIALIZE(DDS_Int, process_id, 0)
	SDDS_DEFINE_DESERIALIZE_ARRAY(DDS_Char, process_name, 0, 128)
	SDDS_DEFINE_DESERIALIZE(DDS_LongLong, app_created_time, 0)
	SDDS_DEFINE_DESERIALIZE(DDS_Int, redundancy_status, 0)
SDDS_END_TYPE_DESERIALIZE_METHOD(AppInfoMsg)

SDDS_BEGIN_TYPE_GETMAXSIZESERIALIZED_METHOD(AppInfoMsg, AppInfoMsg)
	SDDS_DEFINE_GETMAXSIZESERIALIZED(DDS_Int, process_id, 0)
	SDDS_DEFINE_GETMAXSIZESERIALIZED_ARRAY(DDS_Char, process_name, 0, 128)
	SDDS_DEFINE_GETMAXSIZESERIALIZED(DDS_LongLong, app_created_time, 0)
	SDDS_DEFINE_GETMAXSIZESERIALIZED(DDS_Int, redundancy_status, 0)
SDDS_END_TYPE_GETMAXSIZESERIALIZED_METHOD(AppInfoMsg)



