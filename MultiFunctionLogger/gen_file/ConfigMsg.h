//ConfigMsg.h
/********************************************************************************
 * This file was automatically generated by SmartDDS generator.
 * Version 1.0.1
 *
 * Do not make changes to this file unless you know what you are doing--modify
 * the SmartDDS interface file instead.
 ********************************************************************************/

#ifndef _ConfigMsg_H_
#define _ConfigMsg_H_

#include "SDDS_Include.h"

typedef struct ConfigMsg
{
	DDS_Int dst_process_id;
	DDS_Char dst_process_name[128];
	DDS_Int config_type;
	DDS_Int value;
} ConfigMsg;
SDDS_SEQUENCE_DATA(ConfigMsgSeq, ConfigMsg);

typedef struct AppInfoMsg
{
	DDS_Int process_id;
	DDS_Char process_name[128];
	DDS_LongLong app_created_time;
	DDS_Int redundancy_status;
} AppInfoMsg;
SDDS_SEQUENCE_DATA(AppInfoMsgSeq, AppInfoMsg);



SDDS_DECLARE_TYPE_METHOD(ConfigMsg, ConfigMsg)

SDDS_DECLARE_TYPE_METHOD(AppInfoMsg, AppInfoMsg)



SDDS_BEGIN_MEMBER_INFO(ConfigMsg)
	SDDS_DEFINE_MEMBER_INFO_PRIMITIVE(dst_process_id, DDS_TK_INT, DDS_INT_SIZE, 1, false, DDS_Int)
	SDDS_DEFINE_MEMBER_INFO_PRIMITIVE(dst_process_name, DDS_TK_CHAR, DDS_OCTET_SIZE, 128, false, DDS_Char)
	SDDS_DEFINE_MEMBER_INFO_PRIMITIVE(config_type, DDS_TK_INT, DDS_INT_SIZE, 1, false, DDS_Int)
	SDDS_DEFINE_MEMBER_INFO_PRIMITIVE(value, DDS_TK_INT, DDS_INT_SIZE, 1, false, DDS_Int)
SDDS_END_MEMBER_INFO(ConfigMsg)

SDDS_BEGIN_MEMBER_INFO(AppInfoMsg)
	SDDS_DEFINE_MEMBER_INFO_PRIMITIVE(process_id, DDS_TK_INT, DDS_INT_SIZE, 1, false, DDS_Int)
	SDDS_DEFINE_MEMBER_INFO_PRIMITIVE(process_name, DDS_TK_CHAR, DDS_OCTET_SIZE, 128, false, DDS_Char)
	SDDS_DEFINE_MEMBER_INFO_PRIMITIVE(app_created_time, DDS_TK_LONGLONG, DDS_LONG_LONG_SIZE, 1, false, DDS_LongLong)
	SDDS_DEFINE_MEMBER_INFO_PRIMITIVE(redundancy_status, DDS_TK_INT, DDS_INT_SIZE, 1, false, DDS_Int)
SDDS_END_MEMBER_INFO(AppInfoMsg)




#endif
