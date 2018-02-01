///////////////////////////////////////////////////////////////////////////
//
//  System:    Simplygon
//  File:      Example.h
//  Language:  C++
//  Date:      $Date: 2009-08-27 11:42:00 $
//  Version:   $Revision: 2.9 $
// 
//  Copyright (c) 2009 Donya Labs AB. All rights reserved.
//
///////////////////////////////////////////////////////////////////////////
//
//	#Description# Example.h & Example.cpp implements common example 
//	functions, such as init/deinit and error handling, which are used by 
//	all examples in the Simplygon SDK.
//
///////////////////////////////////////////////////////////////////////////

#ifndef __EXAMPLE_H__
#define __EXAMPLE_H__

// standard headers
#include "SimplygonSDKLoader.h"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string>
#include <vector>
#include <float.h>


#ifdef _WIN32
#include <windows.h>
#include <process.h>
#include<string>
#include<sstream>
#include <vector>
#include<iostream>
#elif defined(__linux__)
#include <pthread.h>
#include <unistd.h>
#endif

// use SimplygonSDK as a default namespace
using namespace SimplygonSDK;

// all examples expect a global variable sg that has
// a pointer to the simplygon interface
extern ISimplygonSDK *sg;

// main example init/deinit function
void InitExample();
void DeinitExample();
std::basic_string<TCHAR> GetExecutablePath();

/*
Copy Assets To Current Application Directory
*/
void MoveAssetToExecutablePath(LPCTSTR assetName, std::basic_string<TCHAR> assetDirPath=_T(""));
void MoveDirToExecutablePath(std::basic_string<TCHAR> dirPathSrc, std::basic_string<TCHAR> dirPathDest);
std::vector<std::basic_string<TCHAR>> stringSplit(LPCTSTR source, TCHAR splitOn);
bool contains(std::vector<std::basic_string<TCHAR>> strCollection, std::basic_string<TCHAR> val);

void exit_with_error( const char *errorstr );

//inline Matrix4x4 GetMatrix4x4FromIMatrix( spMatrix4x4 src )
//{
//
//	real srcElements[16];
//	src->GetElements(srcElements);
//	return Matrix4x4(srcElements);
//}

inline real GetRadFromDegrees( real angle)
{
	return (real)(3.141592/180)*angle;
}





#endif //__EXAMPLE_H__