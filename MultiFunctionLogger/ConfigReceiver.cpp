#include "ConfigReceiver.h"
#include "Configuration.h"
#include "TUdpSocket.h"
#include "LogUtil.h"
#include "LogModuleImpl.h"

namespace MF_LOG
{
ConfigReceiver::ConfigReceiver(Configuration *conf)
{
	conf_instance = conf;

	receive_sock = new TUdpSocket();
	receive_sock->receiver_init(conf_instance->get_config_info().app_id + CONFIG_MSG_PORT_BASE);
}

ConfigReceiver::~ConfigReceiver(void)
{
}


void ConfigReceiver::run()
{
	char payload[MAX_LOG_PAYLOAD_SIZE];
	int pkt_len;

	ConfigMsg config_msg;
	//5000msec���� DiscoveryMsg�� �۽�
	//

	while (is_runnable() == true)
	{
		//read() ȣ��� Memory�Ҵ�� payload�� parameter�� �Ѿ��
		pkt_len = receive_sock->read(payload, MAX_LOG_PAYLOAD_SIZE);

		config_msg.deserialize(payload);
		pocess_config_msg(&config_msg);
	}
}

void ConfigReceiver::pocess_config_msg(ConfigMsg *config_msg)
{
	//������ config_msg�� �ڽſ��� �°����� Ȯ��
	if(config_msg->config_type == CONFIG_LOG_LEVEL)
	{
		conf_instance->set_log_level((LOG_LEVEL)config_msg->value);
	}
	else if(config_msg->config_type == CONFIG_REFRESH_TIME)
	{
		conf_instance->set_refresh_time(config_msg->value);
	}
	else if(config_msg->config_type == CONFIG_START_SEND_LOG)
	{
		LogModuleImpl *log_module = dynamic_cast<LogModuleImpl*>(LogModule::get_instance());
		log_module->set_send_log_flag(config_msg->server_info, true);
	}
	else if(config_msg->config_type == CONFIG_STOP_SEND_LOG)
	{
		LogModuleImpl *log_module = dynamic_cast<LogModuleImpl*>(LogModule::get_instance());
		log_module->set_send_log_flag(config_msg->server_info, false);
	}
	else if(config_msg->config_type == CONFIG_RESET_REMOTE_CONTROL)
	{
		conf_instance->reset_remote_control();
	}

}

ConfigCommInfo ConfigReceiver::get_config_comm_info()
{
	ConfigCommInfo ConfigCommInfo;
	ConfigCommInfo.ip_addr = inet_addr(LOG_CONFIG_INSTANCE->get_ip().c_str());
	ConfigCommInfo.port = receive_sock->get_port();

	return ConfigCommInfo;
}
}