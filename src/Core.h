#pragma once
#ifndef _CORE_H
#define _CORE_H

#ifdef WIN32
#ifdef ENGINE_EXPORT
#define ENGINE_API __declspec(dllexport)
#else 
#define ENGINE_API __declspec(dllimport)
#endif
#else 
#define ENGINE_API 
#endif

#endif