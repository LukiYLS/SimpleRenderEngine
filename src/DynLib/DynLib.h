#pragma once


#ifdef  __WIN32__
#    define DYNLIB_HANDLE hInstance
#    define DYNLIB_LOAD( a ) LoadLibraryEx( a, NULL, 0 ) // we can not use LOAD_WITH_ALTERED_SEARCH_PATH with relative paths
#    define DYNLIB_GETSYM( a, b ) GetProcAddress( a, b )
#    define DYNLIB_UNLOAD( a ) !FreeLibrary( a )
#endif
struct HINSTANCE__;
typedef struct HINSTANCE__* hInstance;


#include <string>
namespace SRE {

	class DynLib
	{
	protected:
		std::string _name;
		/// Gets the last loading error
		std::string dynlibError(void);
	public:
		/** Default constructor - used by DynLibManager.
		@warning
		Do not call directly
		*/
		DynLib(const std::string& name);

		/** Default destructor.
		*/
		~DynLib();

		/** Load the library
		*/
		void load();
		/** Unload the library
		*/
		void unload();
		/// Get the name of the library
		const std::string& getName(void) const { return _name; }

		/**
		Returns the address of the given symbol from the loaded library.
		@param
		strName The name of the symbol to search for
		@return
		If the function succeeds, the returned value is a handle to
		the symbol.
		@par
		If the function fails, the returned value is <b>NULL</b>.

		*/
		void* getSymbol(const std::string& strName) const throw();

	protected:

		/// Handle to the loaded library.
		DYNLIB_HANDLE mInst;
	};
}