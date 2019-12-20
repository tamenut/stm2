/**
 * Project Untitled
 */


#include "GUID_t.h"
#include "TUtil.h"

namespace STM {
/**
* GUID_t implementation
*/

GUID_t::GUID_t()
{
	host_id = 0;
    app_id = 0;
    counter = 0;
}

GUID_t::GUID_t(int32_t _host_id, int16_t _app_id, int16_t _counter)
{
    host_id = _host_id;
    app_id = _app_id;
    counter = _counter;
}

void GUID_t::set_info(int32_t _host_id, int16_t _app_id, int16_t _counter)
{
    host_id = _host_id;
    app_id = _app_id;
    counter = _counter;
}

bool GUID_t::operator==(const GUID_t& _guid)
{
	return (host_id==_guid.host_id 
		&& app_id==_guid.app_id 
		&& counter==_guid.counter);
}

bool GUID_t::operator!=(const GUID_t& _guid)
{
	return (host_id!=_guid.host_id 
		|| app_id!=_guid.app_id 
		|| counter!=_guid.counter);
}

int GUID_t::serialize(char *payload)
{
	int offset = 0;
	
	offset += htonl_t((const char*)&host_id, payload + offset);
	offset += htons_t((const char*)&app_id, payload + offset);
	offset += htons_t((const char*)&counter, payload + offset);

	return offset;
}

int GUID_t::deserialize(char *payload)
{
	int offset = 0;
	
	offset += ntohl_t(payload + offset, (char*)&host_id);
	offset += ntohs_t(payload + offset, (char*)&app_id);
	offset += ntohs_t(payload + offset, (char*)&counter);

	return offset;
}

}
