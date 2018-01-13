#ifndef gw_http_service_impl_h__
#define gw_http_service_impl_h__

#include <vector>
#include <map>

#include "gw_runtime.h"
#include "gw_log.h"
#include "gw_http.h"

namespace gw_http
{
	class http_service_impl
		: public http_service
	{
	public:
		http_service_impl(gw::runtime* runtime_);
		~http_service_impl();

		virtual http_request::ptr create_simple_get(const char* url_);
		
		virtual http_request::ptr create_string_post(const char* url_, char* string_buffer_, size_t data_size_);
		virtual http_request::ptr create_file_post(const char* url_, const char* file_name_);
		virtual http_request::ptr create_multi_files_post(const char* url_, const file_name_list& files_);

		virtual void set_time_out(long timeout_ = 2000);
		virtual void set_proxy_settings(const proxy_settings &proxy_settings_);

		virtual void get(const char* url_, http_receive_listener::ptr receive_listener_, progress_callback::ptr callback_);
		virtual void get(const http_request::ptr http_request_, http_receive_listener::ptr receive_listener_, progress_callback::ptr callback_);
		
	private:
		http_response::ptr do_get(const char* url_, progress_callback::ptr callback_);
		http_response::ptr do_get(const http_request::ptr request_, progress_callback::ptr callback_);

		http_response::ptr perform_request_get(const http_request* request_, void* handle_);
		http_response::ptr perform_request_post(const http_request* request_, void* handle_); 		

	private:		
		http_data_connection::ptr	_http_connection;
		gw_shared_ptr<logger>		_logger;

	public:
		gw::runtime*		_runtime;

	};

}

#endif // gw_http_service_impl_h__
