#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "PrivateProfileManager.h"
#include "TUtil.h"

PrivateProfileManager::PrivateProfileManager(char *file_name)
{
	strcpy(_file_name, file_name);
	_section_cnt = 0;
	read_file();
}

PrivateProfileManager::~PrivateProfileManager()
{

}

unsigned short PrivateProfileManager::get_section_name(char *bp, char *section_name)
{
	unsigned short res = 0;
	//unsigned short idx=0;
	unsigned short last_idx = (unsigned short)strlen(bp);
	if (bp[0] == '[' && bp[last_idx-1] == ']')
	{
		memcpy(section_name, bp + 1, last_idx - 2);
		section_name[last_idx-2] = '\0';
		res = (unsigned short)strlen(section_name);
	}
	return res;
}

bool PrivateProfileManager::is_section(char *bp)
{
	bool res = false;
	//unsigned short idx=0;
	unsigned short last_idx = (unsigned short)strlen(bp);
	if (bp[0] == '[' && bp[last_idx] == ']')
	{
		res = true;
	}
	return res;
}

PrivateProfile PrivateProfileManager::create_private_profile(char *buf)
{
	PrivateProfile private_profile;
	char buf_tmp[MAX_VALUE_STR_LEN + MAX_NAME_LEN];
	char * del_prt;

	strcpy(buf_tmp, buf);
	del_prt = strrchr(buf_tmp, '=');
	if (del_prt != NULL)
	{
		del_prt++;
		ignore_blank(del_prt);
	
		strcpy(private_profile._value_str, del_prt);
		*(--del_prt) = '\0';
		ignore_blank(buf_tmp);
		strcpy(private_profile._entry_str, buf_tmp);
	}

	return private_profile;
}

bool PrivateProfileManager::get_entry_value(char * section_name, char * entry_name, int *entry_value, int default_value)
{
	char entry_value_str[MAX_VALUE_STR_LEN];
	bool res = get_entry_value(section_name, entry_name, entry_value_str);
	if (res)
	{
		*entry_value = atoi(entry_value_str);
	}
	else
	{
		*entry_value = default_value;
	}
	return res;
}

bool PrivateProfileManager::get_entry_value(char * section_name, char * entry_name, unsigned int *entry_value, unsigned int default_value)
{
	char entry_value_str[MAX_VALUE_STR_LEN];
	bool res = get_entry_value(section_name, entry_name, entry_value_str);
	if (res)
	{
		*entry_value = atoi(entry_value_str);
	}
	else
	{
		*entry_value = default_value;
	}
	
	return res;
}

bool PrivateProfileManager::get_entry_value(char * section_name, char * entry_name, float *entry_value, float default_value)
{
	char entry_value_str[MAX_VALUE_STR_LEN];
	bool res = get_entry_value(section_name, entry_name, entry_value_str);
	if (res)
	{
		*entry_value = (float)atof(entry_value_str);
	}
	else
	{
		*entry_value = default_value;
	}
	
	return res;
}

bool PrivateProfileManager::get_entry_value(char * section_name, char * entry_name, char * entry_value)
{
	bool res = false;
	entry_value[0] = '\0';

	if (strlen(section_name) >= MAX_NAME_LEN - 1)
	{
		printf("Error : section_name's length is too long(section_name:%s, length:%u) - In PrivateProfileManager::get_entry_value()\n", 
			section_name, strlen(section_name));
	}

	if (strlen(entry_name) >= MAX_NAME_LEN - 1)
	{
		printf("Error : entry_name's length is too long(entry_name:%s, length:%u) - In PrivateProfileManager::get_entry_value()\n",
			entry_name, strlen(entry_name));
	}

	int section_idx = get_section_idx(section_name);
	if (section_idx >= 0 && section_idx < MAX_SECTION_CNT)
	{
		int entry_idx = get_entry_idx(section_idx, entry_name);
		if (entry_idx >= 0 || entry_idx < MAX_ENTRY_CNT)
		{
			strcpy(entry_value, private_profile_table[section_idx]._private_profile[entry_idx]._value_str);
			res = true;
		}
		else
		{
			printf("Error : Can't get_entry_value(section_name:%s, entry_name:%s, entry_idx:%d)\n", section_name, entry_name, entry_idx);
		}
	}

	return res;
}

bool PrivateProfileManager::set_entry_value(char * section_name, char *entry_name, int * entry_value)
{
	char entry_value_str[MAX_VALUE_STR_LEN];
	sprintf(entry_value_str, "%d", *entry_value);
	return set_entry_value(section_name, entry_name, entry_value_str);
}

bool PrivateProfileManager::set_entry_value(char * section_name, char *entry_name, unsigned int * entry_value)
{
	char entry_value_str[MAX_VALUE_STR_LEN];
	sprintf(entry_value_str, "%d", *entry_value);
	return set_entry_value(section_name, entry_name, entry_value_str);
}

bool PrivateProfileManager::set_entry_value(char * section_name, char *entry_name, float * entry_value)
{
	char entry_value_str[MAX_VALUE_STR_LEN];
	sprintf(entry_value_str, "%f", *entry_value);
	return set_entry_value(section_name, entry_name, entry_value_str);
}

bool PrivateProfileManager::set_entry_value(char * section_name, char *entry_name, char * entry_value)
{
	bool res = false;
	if (strlen(section_name) >= MAX_NAME_LEN - 1)
	{
		printf("Error : section_name's length is too long(section_name:%s, length:%u) - In PrivateProfileManager::set_entry_value()\n", 
			section_name, strlen(section_name));
	}

	if (strlen(entry_name) >= MAX_NAME_LEN - 1)
	{
		printf("Error : entry_name's length is too long(entry_name:%s, length:%u) - In PrivateProfileManager::set_entry_value()\n", 
			entry_name, strlen(entry_name));
	}

	int section_idx = get_section_idx(section_name);
	if (section_idx < 0 || section_idx >= MAX_SECTION_CNT)
	{
		if (add_section(section_name) == true)
		{
			section_idx = _section_cnt - 1;
		}
	}

	if (section_idx >= 0 && section_idx < MAX_SECTION_CNT)
	{
		int entry_idx = get_entry_idx(section_idx, entry_name);
		if (entry_idx < 0 || entry_idx >= MAX_ENTRY_CNT)
		{
			PrivateProfile private_profile;
			strcpy(private_profile._entry_str, entry_name);
			strcpy(private_profile._value_str, entry_value);
			res = add_entry_value(section_idx, private_profile);
		}
		else
		{
			strcpy(private_profile_table[section_idx]._private_profile[entry_idx]._value_str, entry_value);
			res = true;
		}
	}
	else
	{
		printf("Error : Can't set_entry_value(section_name:%s, entry_name:%s, section_idx:%d) \n", 
			section_name, entry_name, section_idx);
	}
	return res;
}

int PrivateProfileManager::get_entry_idx(unsigned int section_idx, char *entry_name)
{
	int res = -1;
	for (unsigned int i = 0; i < private_profile_table[section_idx]._private_profile_cnt; i++)
	{
		if (strcmp(private_profile_table[section_idx]._private_profile[i]._entry_str, entry_name) == 0)
		{
			res = i;
			break;
		}
	}
	return res;
}

int PrivateProfileManager::get_section_idx(char *section_name)
{
	int res = -1;
	for (unsigned int i = 0; i < _section_cnt; i++)
	{
		if (strcmp(private_profile_table[i]._section_name, section_name) == 0)
		{
			res = i;
			break;
		}
	}
	return res;
}

void PrivateProfileManager::update_entry_value(unsigned int section_idx, unsigned int entry_idx, char *entry_value)
{
	strcpy(private_profile_table[section_idx]._private_profile[entry_idx]._value_str, entry_value);
}

bool PrivateProfileManager::add_entry_value(unsigned int section_idx, PrivateProfile private_profile)
{
	bool res = false;
	private_profile_table[section_idx]._private_profile_cnt++;
	if (private_profile_table[section_idx]._private_profile_cnt <= MAX_ENTRY_CNT)
	{
		unsigned short entry_idx = private_profile_table[section_idx]._private_profile_cnt - 1;
		private_profile_table[section_idx]._private_profile[entry_idx] = private_profile;
		res = true;
	}
	else
	{
		printf("Error - In add_entry_value(), profile_cnt:%u, profile:%s=%s \n", 
			private_profile_table[section_idx]._private_profile_cnt, private_profile._entry_str, private_profile._value_str);
	}
	return res;
}

void PrivateProfileManager::add_entry_value(char *buf)
{
	unsigned short section_idx = _section_cnt - 1;
	PrivateProfile private_profile = create_private_profile(buf);
	add_entry_value(section_idx, private_profile);
}

bool PrivateProfileManager::add_section(char *buf)
{
	bool res = false;
	_section_cnt++;
	if (_section_cnt <= MAX_SECTION_CNT)
	{
		unsigned short section_idx = _section_cnt - 1;
		strcpy(private_profile_table[section_idx]._section_name, buf);
		private_profile_table[section_idx]._private_profile_cnt = 0;
		res = true;
	}
	else
	{
		printf("Fail - In add_section(), sectioon_cnt:%u, buf-%s \n", _section_cnt, buf);
	}

	return res;
}

void PrivateProfileManager::write_file(FILE *fp, unsigned int section_idx, unsigned int entry_idx)
{
	fprintf(fp, "%s = %s\n", 
		private_profile_table[section_idx]._private_profile[entry_idx]._entry_str,
		private_profile_table[section_idx]._private_profile[entry_idx]._value_str);
}

void PrivateProfileManager::write_file(FILE *fp, unsigned int section_idx)
{
	fprintf(fp, "[%s]\n",private_profile_table[section_idx]._section_name);

	for (unsigned int i = 0; i < private_profile_table[section_idx]._private_profile_cnt; i++)
	{
		write_file(fp, section_idx, i);
	}
	fprintf(fp, "\n");
}

void PrivateProfileManager::flush()
{
	write_file();
}

void PrivateProfileManager::write_file()
{
	FILE *fp = fopen(_file_name, "w");
	if (fp != NULL)
	{
		for (unsigned int i = 0; i < _section_cnt; i++)
		{
			write_file(fp, i);
		}
		fclose(fp);
	}
	else
	{
		printf("Can't write file:%s - In PrivateProfileManager::flush()!!\n", _file_name);
	}
}

void PrivateProfileManager::read_file()
{
	bool res = false;
	char buf[MAX_LINE_LENGTH];
	char section_name[MAX_NAME_LEN];
	memset(private_profile_table, 0, sizeof(private_profile_table));

	FILE *fp = fopen(_file_name, "r");
	if (fp != NULL)
	{
		while (read_line(fp, buf) != -1)
		{
			if (strlen(buf) > 0)
			{
				if (get_section_name(buf, section_name) > 0)
				{
					add_section(section_name);
				}
				else
				{
					add_entry_value(buf);
				}
			}
		}
		fclose(fp);	
	}
	else
	{
		printf("Can't read file:%s - In PrivateProfileManager::read_all_private_profile_value()!!\n", _file_name);
	}
		
}