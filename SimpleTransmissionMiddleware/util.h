#include <string>
#include "STMCommonPrivate.h"

namespace STM {

bool is_ack_pkt(char *payload, int pktLen);
bool is_user_data_pkt(char *payload, int pktLen);
bool is_discovery_pkt(char *payload, int pktLen);

}