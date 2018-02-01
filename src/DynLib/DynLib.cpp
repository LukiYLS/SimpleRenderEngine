#include "DynLib.h"


namespace SRE {

	DynLib::DynLib(const std::string& name)
	{
		_name = name;
		mInst = NULL;
	}

	DynLib::~DynLib()
	{
	}

	void DynLib::load()
	{
		// Log library load
		//LogManager::getSingleton().logMessage("Loading library " + mName);

		std::string name = _name;
#if OGRE_PLATFORM == OGRE_PLATFORM_EMSCRIPTEN
		if (name.find(".js") == String::npos)
			name += ".js";
#elif OGRE_PLATFORM == OGRE_PLATFORM_LINUX || OGRE_PLATFORM == OGRE_PLATFORM_NACL
		// dlopen() does not add .so to the filename, like windows does for .dll
		if (name.find(".so") == String::npos)
		{
			name += ".so.";
			name += StringConverter::toString(OGRE_VERSION_MAJOR) + ".";
			name += StringConverter::toString(OGRE_VERSION_MINOR) + ".";
			name += StringConverter::toString(OGRE_VERSION_PATCH);
		}
#elif OGRE_PLATFORM == OGRE_PLATFORM_APPLE
		// dlopen() does not add .dylib to the filename, like windows does for .dll
		if (name.substr(name.find_last_of(".") + 1) != "dylib")
			name += ".dylib";
#elif OGRE_PLATFORM == OGRE_PLATFORM_WIN32 || OGRE_PLATFORM == OGRE_PLATFORM_WINRT
		// Although LoadLibraryEx will add .dll itself when you only specify the library name,
		// if you include a relative path then it does not. So, add it to be sure.
		if (name.substr(name.find_last_of(".") + 1) != "dll")
			name += ".dll";
#endif
		mInst = (DYNLIB_HANDLE)DYNLIB_LOAD(name.c_str());
#if OGRE_PLATFORM == OGRE_PLATFORM_APPLE
		if (!mInst)
		{
			// Try again as a framework
			mInst = (DYNLIB_HANDLE)FRAMEWORK_LOAD(mName);
		}
#endif
		if (!mInst)
			OGRE_EXCEPT(
				Exception::ERR_INTERNAL_ERROR,
				"Could not load dynamic library " + mName +
				".  System Error: " + dynlibError(),
				"DynLib::load");
	}
}