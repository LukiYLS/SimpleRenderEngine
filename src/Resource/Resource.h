#pragma once
#include <memory>
#include <string>
#include <set>
using namespace std;
namespace Core
{
	class ResourceManager;
	class Resource {
	public:
		//监听
		class Listener {
		public:
			Listener(){}
			virtual ~Listener() {}
		};
	public:
		typedef std::shared_ptr<Resource> ptr;
		enum LoadingState
		{
			/// Not loaded
			LOADSTATE_UNLOADED,
			/// Loading is in progress
			LOADSTATE_LOADING,
			/// Fully loaded
			LOADSTATE_LOADED,
			/// Currently unloading
			LOADSTATE_UNLOADING,
			/// Fully prepared
			LOADSTATE_PREPARED,
			/// Preparing is in progress
			LOADSTATE_PREPARING
		};
	public:
		Resource(){}
		virtual ~Resource(){}
	protected:
		//加载，卸载，准备前的一些操作，比如执行加载的的过程，先执行preload，然后执行load，再执行postload
		virtual void preLoadImpl() {}
		virtual void preUnloadImpl() {}
		virtual void prepareImpl() {}


		virtual void postLoadImpl() {}		
		virtual void postUnloadImpl() {}

		virtual void prepareImpl(){}
		virtual void unPrepareImpl() {}

		virtual void loadImpl() = 0;
		virtual void unLoadImpl() = 0;	
		

		
	public:
		//多线程加载
		virtual void prepare();
		virtual void load();
		virtual void unLoad();

		virtual size_t getSize() { return _size; }
		virtual string getName() { return _name; }



		virtual const char* getResourceType() = 0;
		virtual void setResourceType(const char* type) = 0;
		virtual bool getLoadState() = 0;
		virtual void setLoadState(bool state) = 0;
	protected:
		string			_name;
		string			_group_name;
		size_t			_size;
		bool			_is_manual;
		LoadingState    _load_state;
		typedef set<Listener*> ListenerList;
		ListenerList   _listener_list;

	};
}