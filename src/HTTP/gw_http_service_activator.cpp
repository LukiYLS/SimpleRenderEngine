#include "gw_module.h"

#include "gw_http.h"
#include "gw_http_service_impl.h"

using namespace gw;
using namespace gw_http;

http_service* get_http_service(runtime* runtime_)
{
	static http_service_impl http_service_impl_(runtime_);
	return &http_service_impl_;
}

class http_service_activator
	: public module_activator
{
public:
	void load(runtime* runtime_)
	{
		_runtime = runtime_;
		_runtime->register_service(http_service_pid, get_http_service(runtime_));
	}

	void unload(runtime* runtime_)
	{
		runtime_->unregister_service(http_service_pid);
	}

private:
	runtime*	_runtime;
};

gw_register_module(http_service_pid, http_service_activator);

extern "C" gw_api void* load_http_service()
{
	gw_register_module(http_service_pid, http_service_activator);
	return NULL;
}