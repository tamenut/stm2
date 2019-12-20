#pragma once

#ifdef WIN32
#ifndef SM_DLLFunction

#ifdef SYNCMEMORYSERVICE_EXPORTS
#define SM_DLLFunction  __declspec(dllexport)
#else
#define SM_DLLFunction  __declspec(dllimport)
#endif

#endif
#else
#define SM_DLLFunction  
#endif