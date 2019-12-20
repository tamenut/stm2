#include "TUtil.h"
#ifdef WIN32
#include <windows.h>
#include "psapi.h"
#else
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h>
#endif

enum FLAG_ENDIAN
{
	FLAG_BIG_ENDIAN  = false,//0,
	FLAG_LITTLE_ENDIAN  = true//1
};


//문자열의 앞, 뒤에 공백을 제거
void ignore_blank(char *bp)
{
	int src_i=0, dst_i=0;

	while((bp[src_i]==' ') || (bp[src_i]=='\t'))
	{
		if(bp[src_i] == 0)
		{
			break;
		}
		src_i++;
		
	}
	while(bp[src_i] != 0)
	{
		bp[dst_i++] = bp[src_i++];
	}
	//bp[dst_i]='\0';

	dst_i--;
	while(bp[dst_i]==' ' || bp[dst_i]=='\t'){
		dst_i--;
	}
	bp[++dst_i]='\0';
}

/*
인수로 받은 문자열의 맨앞에 공백을 없애는 함수
*/
/*
char *ignore_blank(char *bp)
{
	int i=0;

	while((bp[i]==' ') || (bp[i]=='\t'))
	{
		if(bp[i] == 0)
		{
			break;
		}

		i++;
	}
	return &bp[i];
}
*/
//#ifndef WIN32
#if 1

/*
파일에서 한줄을 읽어 return하는 함수
*/
#if 1
int read_line(FILE *fp, char *bp)
{
	int res = 0;

	bp[0] = '\0';

	if(fgets(bp, MAX_LINE_LENGTH, fp) == NULL)
	{
		res = -1;
	}
	else
	{
		if(bp[strlen(bp)-1] == '\n')
			bp[strlen(bp)-1] = '\0';
	}

	remove_comment(bp);
	ignore_blank(bp);
	
	//읽어들인 문자열도 없고 파일의 끝인경우는 -1 리턴
	//읽어들인 문자열의 사이즈를 리턴
	if(strlen(bp) > 0 && res == -1)
	{
		res = strlen(bp);
	}

	return res;
}

#else
int read_line(FILE *fp, char *bp)
{   
	char c = '\0';
	int i = 0;
	int res = 0;

/*
	do
	{ 
		c = getc(fp);
		if( c == EOF || c == '\n')
			break;

	}while(true);

	bp[i] = '\0';
*/
	/* Read one line from the source file */
	while( (c = getc(fp)) != '\n' )
	{   
		if( c == EOF )         /* return FALSE on unexpected EOF */
		{
			res = -1;
			break;
		}
		bp[i++] = c;
	}
	bp[i] = '\0';
	
	ignore_blank(bp);

	if(strlen(bp) > 0 && res == -1)
	{
		res = strlen(bp);
	}

	return res;
}

#endif
void getParam(char *buff, char *param, int &offset)
{
	int src_i=offset, dst_i=0;
#if 1
	while ((buff[src_i] != ' ') && (buff[src_i] != '\t') && buff[src_i] != 0)
	{
		param[dst_i++] = buff[src_i++];
		if (buff[src_i] == 0)
		{
			break;
		}
	}

	param[dst_i] = '\0';
	offset = ++src_i;
#else
	while((buff[src_i]==' ') || (buff[src_i]=='\t'))
	{
		if(buff[src_i] == 0)
		{
			break;
		}
		src_i++;
		
	}
	while(buff[src_i] != 0)
	{
		param[dst_i++] = buff[src_i++];
	}
	//bp[dst_i]='\0';


	param[++dst_i]='\0';
#endif
}

//인자로 받은 문자열이 Section인지를 확인
bool isSection(char *buff)
{
	bool res = false;
	int len = strlen(buff);

	if(buff[0] == '[' && buff[len-1] == ']')
		res = true;

	return res;
}

//인자로 받은 Section이 정의 되어있는지 확인하여
//Section이 정의 되어 있으면 파일 포인터를 다음 라인으로 변경
bool isDefinedSection(FILE *fp, const char *section)
{
	bool res = true;
	char t_section[MAX_LINE_LENGTH];
	char buff[MAX_LINE_LENGTH];
	sprintf(t_section,"[%s]",section); 

	do
	{   
		if( read_line(fp,buff) == -1)
		{   
			res = false;
			break;
		}
	}while( strcmp(buff,t_section) );

	return res;
}

char *getEntryStr(FILE *fp, const char *entry, char *buff)
{
	char *res = NULL;
	int len = strlen(entry);

	while(read_line(fp,buff) != -1)
	{
		if(isSection(buff) == true)
		{
			break;
		}

		if(strncmp(buff,entry,len) == 0)
		{
			res = strrchr(buff,'=');    /* Parse out the equal sign */
			res++;
			ignore_blank(res);
			break;
		}
	}

	return res;
}

//#if 1
/************************************************************************
* Function:     get_private_profile_int()
* Arguments:    <char *> section - the name of the section to search for
*               <char *> entry - the name of the entry to find the value of
*               <int> def - the default value in the event of a failed read
*               <char *> file_name - the name of the .ini file to read from
* Returns:      the value located at entry
*************************************************************************/
#ifdef WIN32
int get_private_profile_int(char *section, char *entry, int def, char *file_name)
#else
int get_private_profile_int(const char *section, const char *entry, int def, const char *file_name)
#endif
{   
	FILE *fp = fopen(file_name,"r");
	char *ep;
	char buff[MAX_LINE_LENGTH];
	int res = def;

	if( fp != NULL ) 
	{
		if(isDefinedSection(fp, (const char*)section) == true)
		{
			ep = getEntryStr(fp, (const char*)entry, buff);

			if( ep != NULL && strlen(ep) > 0)
			{
				res = atoi(ep);
			}
		}
		fclose(fp);		
	}
	return res;

}

//#if 1
/************************************************************************
* Function:     get_private_profile_float()
* Arguments:    <char *> section - the name of the section to search for
*               <char *> entry - the name of the entry to find the value of
*               <int> def - the default value in the event of a failed read
*               <char *> file_name - the name of the .ini file to read from
* Returns:      the value located at entry
*************************************************************************/
#ifdef WIN32
float get_private_profile_float(char *section, char *entry, float def, char *file_name)
#else
float get_private_profile_float(const char *section, const char *entry, float def, const char *file_name)
#endif
{   
	FILE *fp = fopen(file_name,"r");
	char *ep;
	char buff[MAX_LINE_LENGTH];
	float res = def;

	if( fp != NULL ) 
	{
		if(isDefinedSection(fp, (const char*)section) == true)
		{
			ep = getEntryStr(fp, (const char*)entry, buff);

			if( ep != NULL && strlen(ep) > 0)
			{
				res = (float)atof(ep);
			}
		}
		fclose(fp);		
	}
	return res;

}


/**************************************************************************
* Function:     get_private_profile_string()
* Arguments:    <char *> section - the name of the section to search for
*               <char *> entry - the name of the entry to find the value of
*               <char *> def - default string in the event of a failed read
*               <char *> buffer - a pointer to the buffer to copy into
*               <int> buffer_len - the max number of characters to copy
*               <char *> file_name - the name of the .ini file to read from
* Returns:      the number of characters copied into the supplied buffer
***************************************************************************/
#ifdef WIN32
int get_private_profile_string(char *section, char *entry, char *def, char *buffer, int buffer_len, char *file_name)
#else
int get_private_profile_string(const char *section, const char *entry, const char *def, char *buffer, int buffer_len, const char *file_name)
#endif
{   
	FILE *fp = fopen(file_name,"r");
	char *ep;
	char buff[MAX_LINE_LENGTH];
	int res = 0;

	strncpy(buffer, def, buffer_len);
	res = strlen(def);

	if( fp != NULL) 
	{
		if(isDefinedSection(fp, section) == true)
		{
			ep = getEntryStr(fp, entry, buff);
			if(ep != NULL)
			{
				strncpy(buffer, ep, buffer_len-1);
				buffer[buffer_len-1] = '\0';
				res = strlen(buffer);
			}
		}
		fclose(fp);
	}

	return res;
}
#endif

void remove_comment(char *buf)
{
	char *res = NULL;
	res = strstr(buf, "//");
	if(res != NULL)
	{
		*res = '\0';
	}
}

bool is_comment(char *buf)
{
	bool res = false;
	if(buf[0] == ';' || memcmp(buf, "//", 2) == 0)
		res =true;

	return res;
}

#ifdef WIN32
char* parse_file_name(char *file_path)
{
 
    char *file_name;
 
    while(*file_path)
    {
        if(*file_path == '\\' && (file_path +1) != NULL )
        {
            file_name = file_path+1;
        }
        else
        { }
            
        file_path++; //mv pointer
               
    } 
    return file_name;
}
#endif

void miilisec_sleep(unsigned int miliseconds)
{
#ifdef WIN32
	Sleep(miliseconds);
#else
	usleep(miliseconds*1000);
#endif
}
#ifdef LINUX
int get_process_name_by_pid(int pid, char *name)
{
	bool result = true;
	char file_name[256];
	//sprintf(file_name, "/proc/%d/cmdline", pid);
	sprintf(file_name, "/proc/%d/comm", pid);
	FILE *fp = fopen(file_name, "r");

	if(!fp)
	{
		result = false;
	}
	else
	{
		size_t read_len = fread(name, sizeof(char), MAX_PROCESS_NAME, fp);
		if(read_len > 0)
			name[read_len-1]='\0';
		else
			result = false;
		fclose(fp);
	}
	return result;
}
#endif

const char *get_current_process_name()
{
	static char *buf = NULL;
	if(buf == NULL)
	{
		buf = new char[128];
#ifdef WIN32
		DWORD pid = GetCurrentProcessId();
		HANDLE handle = 
			OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, pid);
#if 1
		GetModuleBaseName(handle, 0, buf, 512);
		char *tmp = (char*)strchr(buf, '.');
		if(tmp != NULL)
			*tmp = '\0';
#else
		GetModuleFileName(handle, 0, tmp, 512);
		strcpy(buf, parse_file_name(tmp));
#endif
		CloseHandle(handle);
#else
		get_process_name_by_pid(getpid(), buf);
#endif
	}
	
	return buf;
}

int get_process_id()
{
#ifdef WIN32
	int pid = GetCurrentProcessId();
#else
	int pid = getpid();
#endif
	return pid;
}


int ntohss_t(const char* src, char* dst)
{
	memcpy(dst, src, 1);
	return 1;
}

int htonss_t(const char* src, char* dst)
{
	memcpy(dst, src, 1);
	return 1;
}

int ntohs_t(const char* src, char* dst)
{
	int systemEndian = get_system_endian();
	if(systemEndian == FLAG_BIG_ENDIAN)
		memcpy(dst, src, 2);
	else
		swap2bytes(src, dst);

	return 2;	
}

int htons_t(const char* src, char* dst)
{
	int systemEndian = get_system_endian();
	if(systemEndian == FLAG_BIG_ENDIAN)
		memcpy(dst, src, 2);
	else
		swap2bytes(src, dst);

	return 2;	
}

int ntohl_t(const char* src, char* dst)
{
	int systemEndian = get_system_endian();
	if(systemEndian == FLAG_BIG_ENDIAN)
		memcpy(dst, src, 4);
	else
		swap4bytes(src, dst);

	return 4;	
}

int htonl_t(const char* src, char* dst)
{
	int systemEndian = get_system_endian();
	if(systemEndian == FLAG_BIG_ENDIAN)
		memcpy(dst, src, 4);
	else
		swap4bytes(src, dst);

	return 4;	
}

int ntohll_t(const char* src, char* dst)
{
	int systemEndian = get_system_endian();
	if(systemEndian == FLAG_BIG_ENDIAN)
		memcpy(dst, src, 8);
	else
		swap8bytes(src, dst);

	return 8;	
}

int htonll_t(const char* src, char* dst)
{
	int systemEndian = get_system_endian();
	if(systemEndian == FLAG_BIG_ENDIAN)
		memcpy(dst, src, 8);
	else
		swap8bytes(src, dst);

	return 8;	
}

int ntohstr_t(const char* src, char* dst, unsigned int str_len)
{
	memcpy(dst, src, str_len);
	return str_len;
}

int htonstr_t(const char* src, char* dst, unsigned int str_len)
{
	memcpy(dst, src, str_len);
	return str_len;
}


void swap2bytes(const char* src, char* dst)
{
	(void)(											\
	*((char *)(dst) + 0) = *((char *)(src) + 1),	\
	*((char *)(dst) + 1) = *((char *)(src)    )		\
	);
}

void swap4bytes(const char* src, char* dst)
{
	(void)(											\
	*((char *)(dst) + 0) = *((char *)(src) + 3),	\
	*((char *)(dst) + 1) = *((char *)(src) + 2),	\
	*((char *)(dst) + 2) = *((char *)(src) + 1),	\
	*((char *)(dst) + 3) = *((char *)(src)    )		\
	); 
}

void swap8bytes(const char* src, char* dst)
{
	(void)(											\
	*((char *)(dst) + 0) = *((char *)(src) + 7),	\
	*((char *)(dst) + 1) = *((char *)(src) + 6),	\
	*((char *)(dst) + 2) = *((char *)(src) + 5),	\
	*((char *)(dst) + 3) = *((char *)(src) + 4),	\
	*((char *)(dst) + 4) = *((char *)(src) + 3),	\
	*((char *)(dst) + 5) = *((char *)(src) + 2),	\
	*((char *)(dst) + 6) = *((char *)(src) + 1),	\
	*((char *)(dst) + 7) = *((char *)(src)    )		\
	); 
}

bool get_system_endian() 
{ 
	static bool bEndian;
	static bool isProcess = false;

	if(isProcess == false)
	{
		isProcess = true;
		int i = 0x00000001; 
		if ( ((char *)&i)[0] ) 
			bEndian = true;//Little Endian 
		else
			bEndian = false;//Big Endian
	}
	return bEndian;
} 

void wsa_startup()
{
#if defined(WIN32)
	static bool is_process= false;
	if(is_process==false)
	{
		WSADATA wsaData;
		WSAStartup(MAKEWORD(2, 2), &wsaData);
		is_process = true;
	}
#endif
}

int ntoh_t(char &value, char *payload)
{
	return ntohss_t(payload, &value);
}

int ntoh_t(unsigned char &value, char *payload)
{
	return ntohss_t(payload, (char*)&value);
}

int ntoh_t(short &value, char *payload)
{
	return ntohs_t(payload, (char*)&value);
}

int ntoh_t(unsigned short &value, char *payload)
{
	return ntohs_t(payload, (char*)&value);
}

int ntoh_t(int &value, char *payload)
{
	return ntohl_t(payload, (char*)&value);
}

int ntoh_t(unsigned int &value, char *payload)
{
	return ntohl_t(payload, (char*)&value);
}

int ntoh_t(long &value, char *payload)
{
	return ntohl_t(payload, (char*)&value);
}

int ntoh_t(unsigned long &value, char *payload)
{
	return ntohl_t(payload, (char*)&value);
}

int ntoh_t(long long &value, char *payload)
{
	return ntohll_t(payload, (char*)&value);
}

int ntoh_t(float &value, char *payload)
{
	return ntohl_t(payload, (char*)&value);
}

int ntoh_t(double &value, char *payload)
{
	return ntohll_t(payload, (char*)&value);
}

int ntoh_t(char *str, unsigned int str_len, char *payload)
{
	return ntohstr_t(payload, str, str_len);
}

/*
int ntoh_t(SerializedPayload &str, char *payload)
{
	return ntohstr_t(payload, str.get_payload_ptr(), str.get_payload_len());
}
*/

int hton_t(char &value, char *payload)
{
	return htonss_t(&value, payload);
}

int hton_t(unsigned char &value, char *payload)
{
	return htonss_t((const char*)&value, payload);
}

int hton_t(short &value, char *payload)
{
	return htons_t((const char*)&value, payload);
}

int hton_t(unsigned short &value, char *payload)
{
	return htons_t((const char*)&value, payload);
}

int hton_t(int &value, char *payload)
{
	return htonl_t((const char*)&value, payload);
}

int hton_t(unsigned int &value, char *payload)
{
	return htonl_t((const char*)&value, payload);
}

int hton_t(long &value, char *payload)
{
	return htonl_t((const char*)&value, payload);
}

int hton_t(unsigned long &value, char *payload)
{
	return htonl_t((const char*)&value, payload);
}

int hton_t(long long &value, char *payload)
{
	return htonll_t((const char*)&value, payload);
}

int hton_t(float &value, char *payload)
{
	return htonl_t((const char*)&value, payload);
}

int hton_t(double &value, char *payload)
{
	return htonll_t((const char*)&value, payload);
}

int hton_t(char *str, unsigned int str_len, char *payload)
{
	return htonstr_t(str, payload, str_len);
}

char ntoh_t(char &value)
{
	return value;
}

unsigned char ntoh_t(unsigned char &value) 
{
	return value;
}

short ntoh_t(short &value)
{
	return ntohs(value);
}

unsigned short ntoh_t(unsigned short &value)
{
	return ntohs(value);
}

int ntoh_t(int &value)
{
	return ntohl(value);
}

unsigned int ntoh_t(unsigned int &value)
{
	return ntohl(value);
}

long ntoh_t(long  &value)
{
	return ntohl(value);
}

unsigned long ntoh_t(unsigned long &value)
{
	return ntohl(value);
}

long long ntoh_t(long long &value)
{
	long long new_value;
	ntohll_t((char*)&value, (char*)&new_value);
	return new_value;
}

float ntoh_t(float &value)
{
	float new_value;
	ntohl_t((char*)&value, (char*)&new_value);
	return new_value;
}

double ntoh_t(double &value)
{
	double new_value;
	ntohll_t((char*)&value, (char*)&new_value);
	return new_value;
}


char hton_t(char &value)
{
	return value;
}

unsigned char hton_t(unsigned char &value)
{
	return value;
}

short hton_t(short &value)
{
	return htons(value);
}

unsigned short hton_t(unsigned short &value)
{
	return htons(value);
}

int hton_t(int &value)
{
	return htonl(value);
}

unsigned int hton_t(unsigned int &value)
{
	return htonl(value);
}

long hton_t(long  &value)
{
	return htonl(value);
}

unsigned long hton_t(unsigned long &value)
{
	return htonl(value);
}

long long hton_t(long long &value)
{
	long long new_value;
	htonll_t((char*)&value, (char*)&new_value);
	return new_value;
}

float hton_t(float &value)
{
	float new_value;
	htonl_t((char*)&value, (char*)&new_value);
	return new_value;
}

double hton_t(double &value)
{
	double new_value;
	htonll_t((char*)&value, (char*)&new_value);
	return new_value;
}


/*
int hton_t(SerializedPayload &str, char *payload)
{
	return htonstr_t(str.get_payload_ptr(), payload, str.get_payload_len());
}
*/
