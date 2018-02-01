///////////////////////////////////////////////////////////////////////////
//
//  System:    Simplygon
//  File:      Example.cpp
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

#include "Example.h"


ISimplygonSDK *sg = NULL;

// Error handling class
class error_handler : public SimplygonSDK::rerrorhandler
	{
	public:
		virtual void HandleError( 
						IObject *object , 
						const char *interfacename , 
						const char *methodname , 
						rid errortype , 
						const char *errortext 
						)
			{
			printf("An error occured!\n");
			printf("\tInterface: %s (%p)\n",interfacename,object);
			printf("\tMethod: %s\n",methodname);
			printf("\tError Type: %d\n",errortype);
			printf("\tError Description: %s\n",errortext);

			fprintf( stderr , "A SimplygonSDK error occured!\n");
			fprintf( stderr , "\tInterface: %s (%p)\n",interfacename,object);
			fprintf( stderr , "\tMethod: %s\n",methodname);
			fprintf( stderr , "\tError Type: %d\n",errortype);
			fprintf( stderr , "\tError Description: %s\n",errortext);

			exit(1);
			}
	} eh;

void exit_with_error( const char *errorstr )
	{
	fprintf( stderr , "Runtime error: %s\n", errorstr );
	exit(1);
	}

void InitExample()
	{
	// Initiate
	SimplygonSDK::AddSearchPath( "..\\Build\\");
	SimplygonSDK::AddSearchPath( "..\\..\\Build\\");
	SimplygonSDK::AddSearchPath( "..\\..\\..\\Build\\");
	int initval = SimplygonSDK::Initialize( &sg );
	if( initval != SimplygonSDK::SG_ERROR_NOERROR )
		{
		fprintf(stderr,"Failed to initialize");
		exit(1);
		}
	sg->SetErrorHandler(&eh);
	}

void DeinitExample()
	{
	// Deinitialize the SDK
	SimplygonSDK::Deinitialize();
	}

std::basic_string<TCHAR> GetExecutablePath()
	{
		TCHAR currentDir[MAX_PATH];
		TCHAR drive[MAX_PATH];
		TCHAR dir[MAX_PATH];
		TCHAR filename[MAX_PATH];
		TCHAR ext[MAX_PATH];

		if( GetModuleFileName( NULL , currentDir , MAX_PATH ) == NULL )
			{	
			return std::basic_string<TCHAR>(_T(""));
			}

		_tsplitpath_s( currentDir, drive, _MAX_DRIVE, dir, _MAX_DIR, filename, _MAX_FNAME, ext, _MAX_EXT );
		_tmakepath_s( currentDir, _MAX_PATH, drive, dir, _T(""), _T("") );
		
		return currentDir;
	}


void MoveAssetToExecutablePath(LPCTSTR assetName, std::basic_string<TCHAR> assetDirPath)
	{


	if(assetDirPath == _T(""))
		{
		std::basic_string<TCHAR> pathToAssetsFolder;
		std::basic_string<TCHAR> currentWorkingDirectory = GetExecutablePath();

		SetCurrentDirectory(currentWorkingDirectory.c_str());
		//if assetDirPath is empty use currentworking directory to move to assets dir

		std::vector<std::basic_string<TCHAR>> folders = stringSplit(currentWorkingDirectory.c_str(),_T('\\'));

		//look for 
		std::vector<std::basic_string<TCHAR>>::iterator it;
		for ( it=folders.begin() ; it < folders.end(); it++ )
			{
			pathToAssetsFolder+=*it;
			pathToAssetsFolder+=_T("\\");
			if(*it == _T("Build") )
				{
				break;
				}
			}


		pathToAssetsFolder+=_T("..\\Assets\\");
		pathToAssetsFolder+=assetName;

		currentWorkingDirectory+=assetName;
		
		CopyFile(pathToAssetsFolder.c_str(),currentWorkingDirectory.c_str(), FALSE);
		}
	

	}

//src path should be relative to the assets folder
void MoveDirToExecutablePath(std::basic_string<TCHAR> dirPathSrc, std::basic_string<TCHAR> dirPathDest)
	{


		std::basic_string<TCHAR> pathToAssetsFolder;
		std::basic_string<TCHAR> currentWorkingDirectory = GetExecutablePath();
		std::basic_string<TCHAR> destinationPath;

		
		SetCurrentDirectory(currentWorkingDirectory.c_str());
		//if assetDirPath is empty use currentworking directory to move to assets dir

		std::vector<std::basic_string<TCHAR>> folders_src = stringSplit(currentWorkingDirectory.c_str(),_T('\\'));

		std::vector<std::basic_string<TCHAR>> folders_dest = stringSplit(currentWorkingDirectory.c_str(),_T('\\'));

		folders_src.push_back(_T(".."));
		folders_src.push_back(_T(".."));

		
			{
			std::vector<std::basic_string<TCHAR>>::reverse_iterator it;
			std::vector<std::string> reverse_path;
			int remove_next_counter = 0;

			for ( it = folders_src.rbegin() ; it < folders_src.rend(); it++ )
				{
				if(*it == _T("..") )
					{
					remove_next_counter++;
					}
				else if(remove_next_counter > 0 )
					{
					remove_next_counter--;
					}
				else
					{
					reverse_path.push_back(*it);
					//pathToAssetsFolder+=*it;
					//pathToAssetsFolder+=_T("\\");
					}
				}

			for(int i = (int)reverse_path.size() - 1 ; i >= 0 ; --i)
				{
				pathToAssetsFolder += reverse_path[i];
				pathToAssetsFolder += _T("\\");
				}
			}

			{
			std::vector<std::basic_string<TCHAR>>::reverse_iterator it;
			std::vector<std::string> reverse_path;
			int remove_next_counter = 0;

			for ( it = folders_dest.rbegin() ; it < folders_dest.rend(); it++ )
				{
				if(*it == _T("..") )
					{
					remove_next_counter++;
					}
				else if(remove_next_counter > 0 )
					{
					remove_next_counter--;
					}
				else
					{
					reverse_path.push_back(*it);
					//pathToAssetsFolder+=*it;
					//pathToAssetsFolder+=_T("\\");
					}
				}

			for(int i = (int)reverse_path.size() - 1 ; i >= 0 ; --i)
				{
				destinationPath += reverse_path[i];
				destinationPath += _T("\\");
				}
			}

			pathToAssetsFolder+=_T("Assets\\");
			pathToAssetsFolder+=dirPathSrc;

			destinationPath += dirPathDest;
		
			
		//MoveFileEx(pathToAssetsFolder.c_str(),currentWorkingDirectory.c_str(), MOVEFILE_COPY_ALLOWED);

	//	FILE *f = fopen("C:\\output.txt", "w");
		printf("assets folder: %s\n", pathToAssetsFolder.c_str());
		printf("output folder: %s \n", destinationPath.c_str());
	//	fclose(f);

		printf("Initialized \n");

		HWND handle = NULL;
		//IsWindow(handle);
		
		SHFILEOPSTRUCT  fileOpObj;
		fileOpObj.hwnd = handle;
		pathToAssetsFolder += '\0';
		destinationPath += '\0';

		LPCTSTR c01 = pathToAssetsFolder.c_str();
		LPCTSTR c1 = destinationPath.c_str();

		
		fileOpObj.pFrom = pathToAssetsFolder.c_str();
		fileOpObj.pTo = destinationPath.c_str();

		//fileOpObj.pFrom = "C:\\Dev\\Devanastasia\\branches\\QualityRenderer\\Assets\\quality_render_wd\\*.*";
		//fileOpObj.pTo = "C:\\Dev\\Devanastasia\\branches\\QualityRenderer\\Build\\QualityRendererExample\\quality_render_wd\\";
		

		fileOpObj.wFunc = FO_COPY;
		fileOpObj.fFlags = FOF_MULTIDESTFILES | FOF_SILENT;

		SHFileOperation(&fileOpObj);
	


	}


std::vector<std::basic_string<TCHAR>> stringSplit(LPCTSTR source, TCHAR delim)
	{
		std::basic_string<TCHAR> tempString(source);
		//std::basic_string<TCHAR> delimStr(delim);
		std::basic_string<TCHAR> buffer;
		std::stringstream stringStream(tempString);
		std::vector<std::basic_string<TCHAR>> stringCollection;

		
		while(std::getline(stringStream,buffer,delim))
			{
			if(buffer != _T(""))
				stringCollection.push_back(buffer);
			}

		return stringCollection;
		
	}

bool contains(std::vector<std::basic_string<TCHAR>> strCollection, std::basic_string<TCHAR> val)
	{
	 std::vector<std::basic_string<TCHAR>>::iterator it;
	for ( it=strCollection.begin() ; it < strCollection.end(); it++ )
		{
			if(*it == val )
				return true;
		}

	return false;
	}
