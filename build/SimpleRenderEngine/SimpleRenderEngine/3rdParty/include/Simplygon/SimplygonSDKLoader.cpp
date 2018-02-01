#include <stdio.h>
#include <stdlib.h> 
#include <string>
#include <vector>
#include <tchar.h>

#include "SimplygonSDKLoader.h"

static std::vector<std::basic_string<TCHAR>> AdditionalSearchPaths;

#ifdef _WIN32 
#include <windows.h>
#include <shlobj.h>
#include <process.h>
#include <io.h>



typedef int (CALLBACK* LPINITIALIZESIMPLYGONSDK)( LPCTSTR license_data , SimplygonSDK::ISimplygonSDK **pInterfacePtr );
typedef void (CALLBACK* LPDEINITIALIZESIMPLYGONSDK)();
typedef void (CALLBACK* LPGETINTERFACEVERSIONSIMPLYGONSDK)( char *deststring );
typedef int (CALLBACK* LPPOLLLOGSIMPLYGONSDK)( char *destbuffer , int max_length );

#define PUBLICBUILD

namespace SimplygonSDK
	{

	static LPINITIALIZESIMPLYGONSDK InitializeSimplygonSDKPtr = NULL; 
	static LPDEINITIALIZESIMPLYGONSDK DeinitializeSimplygonSDKPtr = NULL;
	static LPGETINTERFACEVERSIONSIMPLYGONSDK GetInterfaceVersionSimplygonSDKPtr = NULL;
	static LPPOLLLOGSIMPLYGONSDK PollLogSimplygonSDKPtr = NULL;
	static HINSTANCE hDLL = NULL; // Handle to SimplygonSDK DLL
	static int LoadError = 0; // if the load failed, this contains the error

	// critical sections, process-local mutexes
	class rcriticalsection
		{
		private:
			CRITICAL_SECTION cs;

		public:
			rcriticalsection() { ::InitializeCriticalSection(&cs); }
			~rcriticalsection() { ::DeleteCriticalSection(&cs); }

			void Enter() { EnterCriticalSection(&cs); }
			void Leave() { LeaveCriticalSection(&cs); }
		};

	static int GetStringFromRegistry( LPCTSTR keyid , LPCTSTR valueid , LPTSTR dest )
		{
		HKEY reg_key;
		if( RegOpenKey( HKEY_LOCAL_MACHINE , keyid , &reg_key ) != ERROR_SUCCESS )
			return SimplygonSDK::SG_ERROR_NOLICENSE; 

		// read the value from the key
		DWORD path_size = MAX_PATH;
		if( RegQueryValueEx( reg_key , valueid , NULL ,	NULL , (unsigned char *)dest , &path_size ) != ERROR_SUCCESS )
			return SimplygonSDK::SG_ERROR_NOLICENSE;
		dest[path_size] = 0;

		// close the key
		RegCloseKey( reg_key );
		return SimplygonSDK::SG_ERROR_NOERROR;
		}

	inline LPTSTR ConstCharPtrToLPTSTR(const char * stringToConvert) 
		{
		size_t newsize = strlen(stringToConvert)+1;
		LPTSTR returnString = (LPTSTR)malloc(newsize * sizeof(TCHAR));
#ifndef _UNICODE
		memcpy( returnString , stringToConvert , newsize );	
#else
		size_t convertedChars = 0;
		mbstowcs_s(&convertedChars, returnString, newsize, stringToConvert, _TRUNCATE);
#endif // _UNICODE
		return returnString;
		}

	static bool FileExists( LPCTSTR path )
		{
		DWORD v = ::GetFileAttributes( path );
		if( v == INVALID_FILE_ATTRIBUTES )
			{
			return false;
			}
		return true;
		}


	static std::basic_string<TCHAR> GetInstallationPath()
		{
		TCHAR InstallationPath[MAX_PATH];

		// get the installation path string from the registry
		if( GetStringFromRegistry( _T("Software\\DonyaLabs\\SimplygonSDK") , _T("InstallationPath") , InstallationPath ) != 0 )
			{
			return std::basic_string<TCHAR>(_T(""));
			}

		size_t t = _tcslen(InstallationPath);
		if( InstallationPath[t] != _T('\\') )
			{
			InstallationPath[t] = _T('\\');
			InstallationPath[t+1] = _T('\0');
			}

		// append a backslash
		return InstallationPath;
		}

	static std::basic_string<TCHAR> GetLicensePath()
		{
		TCHAR AppDataPath[MAX_PATH];

		// Get the common appdata path
		if( SHGetFolderPath( NULL, CSIDL_COMMON_APPDATA, NULL, 0, AppDataPath ) != 0 )
			{
			return std::basic_string<TCHAR>(_T(""));
			}

		// append the path to Simplygon SDK
		return std::basic_string<TCHAR>(AppDataPath) + _T("\\DonyaLabs\\SimplygonSDK\\");
		}

	static std::basic_string<TCHAR> GetApplicationPath()
		{
		TCHAR Path[MAX_PATH];
		TCHAR drive[_MAX_DRIVE];
		TCHAR dir[_MAX_DIR];
		TCHAR fname[_MAX_FNAME];
		TCHAR ext[_MAX_EXT];

		if( GetModuleFileName( NULL , Path , MAX_PATH ) == NULL )
			{	
			return std::basic_string<TCHAR>(_T(""));
			}

		_tsplitpath_s( Path, drive, _MAX_DRIVE, dir, _MAX_DIR, fname, _MAX_FNAME, ext, _MAX_EXT );
		_tmakepath_s( Path, _MAX_PATH, drive, dir, _T(""), _T("") );

		return Path;
		}

	static std::basic_string<TCHAR> GetWorkingDirectory()
		{
		TCHAR dir[MAX_PATH];
		GetCurrentDirectory(MAX_PATH, dir);
		return dir;
		}

	static std::basic_string<TCHAR> GetLibraryDirectory( std::basic_string<TCHAR> path )
		{
		TCHAR Directory[_MAX_PATH];
		TCHAR drive[_MAX_DRIVE];
		TCHAR dir[_MAX_DIR];
		TCHAR fname[_MAX_FNAME];
		TCHAR ext[_MAX_EXT];

		_tsplitpath_s( path.c_str(), drive, _MAX_DRIVE, dir, _MAX_DIR, fname, _MAX_FNAME, ext, _MAX_EXT );
		_tmakepath_s( Directory, _MAX_PATH, drive, dir, _T(""), _T("") );

		return Directory;
		}

	static bool DynamicLibraryIsLoaded()
		{
		if(hDLL == NULL)
			{
			return false;
			}	
		return true;
		}

	static void UnloadDynamicLibrary()
		{
		if( !DynamicLibraryIsLoaded() )
			return;

		FreeLibrary(hDLL);
		hDLL = NULL;	

		InitializeSimplygonSDKPtr = NULL;
		DeinitializeSimplygonSDKPtr = NULL;
		GetInterfaceVersionSimplygonSDKPtr = NULL;
		PollLogSimplygonSDKPtr = NULL;
		}

	static bool LoadOSLibrary( std::basic_string<TCHAR> path )
		{
		// if no file exists, just return
		if( !FileExists( path.c_str() ) )
			{
			return false;
			}

		// get the lib directory, it should be initialized in its directory
		std::basic_string<TCHAR> lib_dir = GetLibraryDirectory( path );
		std::basic_string<TCHAR> curr_dir = GetWorkingDirectory();

		// move to the dir where the library is 
		SetCurrentDirectory( lib_dir.c_str() );

		// try loading the library
		hDLL = LoadLibrary( path.c_str() );

		// move back to the previous dir
		SetCurrentDirectory( curr_dir.c_str() );

		// now, check if the library was loaded
		if( !DynamicLibraryIsLoaded() )
			{
			// load failed, probably a missing dependency
			TCHAR buffer[400];
			DWORD err = GetLastError();
			_stprintf_s(buffer,MAX_PATH,_T("Loading the Simplygon DLL failed, which is in most cases because of a missing dependency, are all dependencies installed? Please try reinstalling Simplygon. GetLastError returns %d\n"), err);
			OutputDebugString(buffer);

			// set the error
			LoadError = SimplygonSDK::SG_ERROR_LOADFAILED;
			return false;
			}

		// setup the pointers
		InitializeSimplygonSDKPtr = (LPINITIALIZESIMPLYGONSDK)GetProcAddress(hDLL,"InitializeSimplygonSDK");
		DeinitializeSimplygonSDKPtr = (LPDEINITIALIZESIMPLYGONSDK)GetProcAddress(hDLL,"DeinitializeSimplygonSDK");
		GetInterfaceVersionSimplygonSDKPtr = (LPGETINTERFACEVERSIONSIMPLYGONSDK)GetProcAddress(hDLL,"GetInterfaceVersionSimplygonSDK");
		PollLogSimplygonSDKPtr = (LPPOLLLOGSIMPLYGONSDK)GetProcAddress(hDLL,"PollLogSimplygonSDK");
		if( InitializeSimplygonSDKPtr==NULL || 
			DeinitializeSimplygonSDKPtr==NULL ||
			GetInterfaceVersionSimplygonSDKPtr==NULL )
			{
			// load failed, invalid version
			TCHAR buffer[400];
			_stprintf_s(buffer,MAX_PATH,_T("The Simplygon DLL loaded, but the .dll functions could not be retrieved, this is probably not a Simplygon dll file, or it is corrupted. Please reinstall Simplygon\n"));
			OutputDebugString(buffer);

			// set error
			LoadError = SimplygonSDK::SG_ERROR_WRONGVERSION;
			UnloadDynamicLibrary();
			return false;
			}

		// check the version string
		char versionstring[200];
		GetInterfaceVersionSimplygonSDKPtr( versionstring );
		const SimplygonSDK::rchar *header_version = SimplygonSDK::GetInterfaceVersionHash();
		if( strcmp (versionstring,header_version) != 0 )
			{
			// load failed, invalid version
			TCHAR buffer[400];
			LPTSTR pheader_version = ConstCharPtrToLPTSTR(header_version);
			LPTSTR pversionstring = ConstCharPtrToLPTSTR(versionstring);
			_stprintf_s(buffer,MAX_PATH,_T("The Simplygon DLL loaded, but the interface version of the header is not the same as the library.\n\tHeader: %s\n\tLibrary: %s\n"),pheader_version,pversionstring);
			OutputDebugString(buffer);
			free(pheader_version);
			free(pversionstring);

			// set error
			LoadError = SimplygonSDK::SG_ERROR_WRONGVERSION;
			UnloadDynamicLibrary();
			return false;
			}

		// loaded successfully
		TCHAR buffer[400];
		LPTSTR pversionstring = ConstCharPtrToLPTSTR(versionstring);
		_stprintf_s(buffer,MAX_PATH,_T("The Simplygon DLL was found and loaded successfully!\n\tInterface hash: %s \n"),versionstring);
		OutputDebugString(buffer);
		free(pversionstring);
		LoadError = SimplygonSDK::SG_ERROR_NOERROR;
		return true;
		}

	static bool FindNamedDynamicLibrary( std::basic_string<TCHAR> DLLName )
		{
		std::basic_string<TCHAR> InstallationPath = GetInstallationPath();
		std::basic_string<TCHAR> ApplicationPath = GetApplicationPath();
		std::basic_string<TCHAR> WorkingPath = GetWorkingDirectory() + _T("/");

		// try additional paths first, so they are able to override
		for( size_t i=0; i<AdditionalSearchPaths.size(); ++i )
			{
			if( LoadOSLibrary( AdditionalSearchPaths[i] + DLLName ) )
				{
				return true;
				}
			}

		// try local application path first
		if( LoadOSLibrary( ApplicationPath + DLLName ) )
			{
			return true;
			}

		// next try installation path
		if( !InstallationPath.empty() )
			{
			if( LoadOSLibrary( InstallationPath + DLLName ) )
				{
				return true;
				}
			}

		// try working directory as well
		if( !WorkingPath.empty() )
			{
			if( LoadOSLibrary( WorkingPath + DLLName ) )
				{
				return true;
				}
			}

		return false;
		}

	static bool FindDynamicLibrary( LPCTSTR SDKPath )
		{
		std::basic_string<TCHAR> DLLName;

		if( SDKPath != NULL )
			{
			// load from explicit place, fail if not found
			if( LoadOSLibrary( std::basic_string<TCHAR>(SDKPath) ) )
				{
				return true;
				}
			return false;
			}

		// if debug version, try debug build first
#ifdef _DEBUG

#ifdef _WIN64
		DLLName = _T("SimplygonSDKRuntimeDebugx64.dll");
#else
		DLLName = _T("SimplygonSDKRuntimeDebugWin32.dll");
#endif

		if( FindNamedDynamicLibrary( DLLName ) )
			{
			return true;
			}

#endif//_DEBUG

		// if debugrelease version, try that
#ifdef REDEBUG
#ifdef _WIN64
		DLLName = _T("SimplygonSDKRuntimeDebugReleasex64.dll");
#else
		DLLName = _T("SimplygonSDKRuntimeDebugReleaseWin32.dll");
#endif
		if( FindNamedDynamicLibrary( DLLName ) )
			{
			return true;
			}
#endif//REDEBUG

		// next (or if release) try release builds
#ifdef _WIN64
		DLLName = _T("SimplygonSDKRuntimeReleasex64.dll");
#else
		DLLName = _T("SimplygonSDKRuntimeReleaseWin32.dll");
#endif

		if( FindNamedDynamicLibrary( DLLName ) )
			{
			return true;
			}

		LoadError = SimplygonSDK::SG_ERROR_FILENOTFOUND;

		return false;
		}

	static bool LoadDynamicLibrary( LPCTSTR SDKPath )
		{
		LoadError = SimplygonSDK::SG_ERROR_NOERROR;

		if( !FindDynamicLibrary(SDKPath) )
			{
			return false;
			}

		return true;
		}

	static std::basic_string<TCHAR> FindNamedProcess( std::basic_string<TCHAR> EXEName )
		{
		std::basic_string<TCHAR> InstallationPath = GetInstallationPath();
		std::basic_string<TCHAR> ApplicationPath = GetApplicationPath();
		std::basic_string<TCHAR> WorkingPath = GetWorkingDirectory();

		// order of running batch process
		std::basic_string<TCHAR> *dirs[] = {
			&ApplicationPath,
			&InstallationPath,
			&WorkingPath,
			NULL
			};

		// try the different paths
		int i=0;
		while( dirs[i] != NULL )
			{
			if( !dirs[i]->empty() )
				{
				std::basic_string<TCHAR> path = (*dirs[i]) + EXEName;
				if( FileExists( path.c_str() ) )
					{
					return path;
					}
				}
			++i;
			}

		return _T("");
		}


	// executes the batch process. redirects the stdout to the handle specified
	// places the handle of the process into the variable that phProcess points at
	static bool ExecuteProcess( const TCHAR *exepath , const TCHAR *ini_file , HANDLE *phProcess )
		{
		PROCESS_INFORMATION piProcInfo; 
		STARTUPINFO siStartInfo;
		BOOL bFuncRetn = FALSE; 

		// Set up members of the PROCESS_INFORMATION structure. 
		ZeroMemory( &piProcInfo, sizeof(PROCESS_INFORMATION) );

		// Set up members of the STARTUPINFO structure. 
		ZeroMemory( &siStartInfo, sizeof(STARTUPINFO) );
		siStartInfo.cb = sizeof(STARTUPINFO); 

		// create a command line string
		TCHAR *tmp_cmdline = new TCHAR[MAX_PATH*2];
		if( ini_file != NULL )
			_stprintf_s(tmp_cmdline,MAX_PATH*2,_T("\"%s\" %s"),exepath,ini_file);
		else
			_stprintf_s(tmp_cmdline,MAX_PATH*2,_T("\"%s\" "),exepath);

		// Create the child process. 
		bFuncRetn = CreateProcess( 
			NULL,			// exe file path
			tmp_cmdline,    // command line 
			NULL,          	// process security attributes 
			NULL,          	// primary thread security attributes 
			TRUE,          	// handles are inherited 
			0,             	// creation flags 
			NULL,          	// use parent's environment 
			NULL,          	// use parent's current directory 
			&siStartInfo,  	// STARTUPINFO pointer 
			&piProcInfo		// receives PROCESS_INFORMATION 
			);  

		delete [] tmp_cmdline;

		if(bFuncRetn == 0) 
			{
			return false;
			}

		// function succeeded, return handle to process, release handles we will not use anymore
		*phProcess = piProcInfo.hProcess;
		CloseHandle(piProcInfo.hThread);
		return true;
		}

	// waits for process to end, and returns exit value
	// the process handle is also released by the function
	static DWORD WaitForProcess( HANDLE &hProcess )
		{
		DWORD exitcode;

		for(;;)
			{
			// check if process has ended
			GetExitCodeProcess( hProcess , &exitcode );
			if( exitcode != STILL_ACTIVE )
				{
				break;
				}

			// wait for it to signal. 
			WaitForSingleObject( hProcess , 1000 );
			}

		CloseHandle(hProcess);
		hProcess = INVALID_HANDLE_VALUE;
		return exitcode;
		}

#ifdef PUBLICBUILD

	static bool ReadLicense( std::basic_string<TCHAR> LicensePath , LPTSTR *dest )
		{
		FILE *fp = NULL;

		if( _tfopen_s(&fp,LicensePath.c_str(),_T("rb")) != 0 )
			{
			return false;
			}

		int len = _filelength(_fileno(fp));
		*dest = (LPTSTR)malloc(len+1);
		if( fread(*dest,1,len,fp) != len )
			{
			fclose(fp);
			return false;
			}

		(*dest)[len] = '\0';
		fclose(fp);
		return true;	
		}

	static bool LoadLicense( LPTSTR *dest )
		{
		// try the local folder first
		if( ReadLicense( GetApplicationPath() + _T("License.dat") , dest ) )
			{
			return true;
			}

		// next, try the common application folder
		if( ReadLicense( GetLicensePath() + _T("License.dat") , dest ) )
			{
			return true;
			}

		return false;
		}

#endif//PUBLICBUILD

#endif//_WIN32

#ifdef __linux__

#include<dlfcn.h>
#include <pthread.h>

	typedef int (*LPINITIALIZESIMPLYGONSDK)( LPCTSTR license_data , SimplygonSDK::ISimplygonSDK **pInterfacePtr );
	typedef void (*LPDEINITIALIZESIMPLYGONSDK)();
	typedef void (*LPGETINTERFACEVERSIONSIMPLYGONSDK)( char *deststring );

	static LPINITIALIZESIMPLYGONSDK InitializeSimplygonSDKPtr; 
	static LPDEINITIALIZESIMPLYGONSDK DeinitializeSimplygonSDKPtr;
	static LPGETINTERFACEVERSIONSIMPLYGONSDK GetInterfaceVersionSimplygonSDKPtr;
	static void *hSO = NULL; // Handle to SimplygonSDK SO

	// critical sections, process-local mutexes
	class rcriticalsection
		{
		private:
			pthread_mutex_t mutex;

		public:
			rcriticalsection() 
				{ 
				::pthread_mutexattr_t mutexattr;
				::pthread_mutexattr_settype(&mutexattr, PTHREAD_MUTEX_RECURSIVE_NP);
				::pthread_mutex_init(&mutex, &mutexattr);
				::pthread_mutexattr_destroy(&mutexattr);
				}
			~rcriticalsection() 
				{ 
				::pthread_mutex_destroy(&mutex); 
				}

			void Enter() { pthread_mutex_lock(&mutex); }
			void Leave() { pthread_mutex_unlock(&mutex); }
		};

	static bool dlErrorHandler()
		{
		char *dlError = dlerror();
		if(dlError) 
			{
			fprintf(stderr,"dlErrorHandler(): Error with dl function: %s\n", dlError);
			return false;
			}
		return true; // all ok
		}

	static bool LoadDynamicLibrary( LPCTSTR SDKPath )
		{
		if( SDKPath != NULL )
			{
			hSO = dlopen(SDKPath, RTLD_NOW);
			}
		else
			{
			hSO = dlopen("./libSimplygonSDK.so", RTLD_NOW);
			}

		if( !dlErrorHandler() )
			{
			return false;
			}

		// setup the function pointers
		InitializeSimplygonSDKPtr = (LPINITIALIZESIMPLYGONSDK)dlsym(hSO,"InitializeSimplygonSDK");
		DeinitializeSimplygonSDKPtr = (LPDEINITIALIZESIMPLYGONSDK)dlsym(hSO,"DeinitializeSimplygonSDK");
		GetInterfaceVersionSimplygonSDKPtr = (LPGETINTERFACEVERSIONSIMPLYGONSDK)dlsym(hSO,"GetInterfaceVersionSimplygonSDK");
		if( InitializeSimplygonSDKPtr==NULL || 
			DeinitializeSimplygonSDKPtr==NULL ||
			GetInterfaceVersionSimplygonSDKPtr==NULL )
			{
			dlErrorHandler();
			fprintf(stderr,"LoadDynamicLibrary(): Failed to retrieve all pointers.\n");
			return false;
			}
		return true;
		}

	static bool DynamicLibraryIsLoaded()
		{
		if(hSO == NULL)
			{
			return false;
			}	
		return true;
		}

	static void UnloadDynamicLibrary()
		{
		dlclose(hSO);
		hSO = NULL;	
		}

#endif //__linux__


	int InitializeSimplygonSDK( SimplygonSDK::ISimplygonSDK **pInterfacePtr , LPCTSTR SDKPath )
		{
		LPTSTR LicenseData = NULL;

		// load the library
		if( !LoadDynamicLibrary(SDKPath) )
			{
			return LoadError;
			}	

		// read the license file from the installation path
#if defined(_WIN32) && defined(PUBLICBUILD)
		bool license_load_succeeded = LoadLicense(&LicenseData);
#endif

		// call the initalization function
		int retval = InitializeSimplygonSDKPtr(LicenseData,pInterfacePtr);

		// clean up the allocated data
		if( LicenseData )
			{
			free(LicenseData);
			}

		if( retval == 0 )
			{
			TCHAR buffer[400];
			LPTSTR versionstring = ConstCharPtrToLPTSTR((*pInterfacePtr)->GetVersion());
			_stprintf_s(buffer,MAX_PATH,_T("Simplygon initialized and running.\n\tVersion: %s\n"),versionstring);
			OutputDebugString(buffer);
			free(versionstring);
			}

		// return retval
		return retval;
		}

	void DeinitializeSimplygonSDK()
		{
		if( !DynamicLibraryIsLoaded() )
			{
			return;
			}
		DeinitializeSimplygonSDKPtr();
		UnloadDynamicLibrary();
		}

	////////////////////////////////////////////////////////////////////////////////////


	static rcriticalsection init_guard; // only one thread is allowed to run init/deinit at any time
	static int init_count = 0; // a reference count of the number of Init/Deinits
	static ISimplygonSDK *InterfacePtr; // the pointer to the only interface

	void AddSearchPath( LPCTSTR search_path )
		{
		TCHAR full[MAX_PATH];
		if( _tfullpath( full, search_path, MAX_PATH ) != NULL )
			{
			AdditionalSearchPaths.push_back( full );
			}

		}

	void ClearAdditionalSearchPaths()
		{
		AdditionalSearchPaths.clear();
#if _HAS_CPP0X
		// make it release its memory, so this does not give a false positive in memory leak detection
		AdditionalSearchPaths.shrink_to_fit();
#endif
		}

	int Initialize( ISimplygonSDK **pInterfacePtr , LPCTSTR SDKPath )
		{
		int retval = SG_ERROR_NOERROR;

		// if the interface does not exist, init it
		if( InterfacePtr == NULL )
			{
			init_count = 0;
			retval = InitializeSimplygonSDK( &InterfacePtr , SDKPath );
			}

		// if everything is ok, add a reference
		if( retval == SG_ERROR_NOERROR )
			{
			++init_count;
			*pInterfacePtr = InterfacePtr;
			}

		return retval;
		}

	void Deinitialize()
		{
		// release a reference
		--init_count;

		// if the reference is less or equal to 0, release the interface 
		if( init_count <= 0 )
			{
			// only release if one exists
			if( InterfacePtr != NULL )
				{
				DeinitializeSimplygonSDK();
				InterfacePtr = NULL;
				}

			// make sure the init_count is 0
			init_count = 0;
			}
		}

	LPCTSTR GetError( int error_code )
		{
		switch( error_code ) 
			{
			case SG_ERROR_NOERROR:
				return _T("No error");
			case SG_ERROR_NOLICENSE:
				return _T("No license was found, or the license is not valid. Please install a valid license.");
			case SG_ERROR_NOTINITIALIZED:
				return _T("The SDK is not initialized, or no process object is loaded.");
			case SG_ERROR_ALREADYINITIALIZED:
				return _T("The SDK is already initialized. Please call Deinitialize() before calling Initialize() again.");
			case SG_ERROR_FILENOTFOUND:
				return _T("The specified file was not found.");
			case SG_ERROR_INVALIDPARAM:
				return _T("An invalid parameter was passed to the method");
			case SG_ERROR_WRONGVERSION:
				return _T("The Simplygon DLL and header file interface versions do not match");
			case SG_ERROR_LOADFAILED:
				return _T("the Simplygon DLL failed loading, probably because of a missing dependency");
			default:
				return _T("Licensing error, check log for specifics.");
			}
		}

	int PollLog( LPTSTR dest , int max_len_dest )
		{
		if( dest == nullptr || max_len_dest == 0 || PollLogSimplygonSDKPtr == nullptr )
			{
			return 0;
			}

		int sz;

#ifdef UNICODE
		char *tmp = new char[max_len_dest];
		PollLogSimplygonSDKPtr(tmp,max_len_dest);
		size_t cnv_sz;
		mbstowcs_s( 
			&cnv_sz,
			dest,
			max_len_dest,
			tmp,
			_TRUNCATE
			);
		delete [] tmp;
		sz = (int)cnv_sz;
#else
		sz = PollLogSimplygonSDKPtr(dest,max_len_dest);
#endif//UNICODE	

		return sz;
		}

	int RunLicenseWizard( LPCTSTR batch_file )
		{
		// find process
		std::basic_string<TCHAR> path = FindNamedProcess( _T("LicenseApplication.exe") );
		if( path.empty() )
			{
			return SG_ERROR_FILENOTFOUND;
			}

		// run process
		HANDLE hProcess;
		bool succ = ExecuteProcess(path.c_str(),batch_file,&hProcess);
		if( !succ )
			{
			return SG_ERROR_LOADFAILED;
			}

		// wait for it to end
		return WaitForProcess(hProcess);
		}

	// end of namespace
	};

