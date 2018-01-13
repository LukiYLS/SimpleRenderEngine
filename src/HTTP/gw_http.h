#ifndef __gw_http_h__
#define __gw_http_h__

#include <map>
#include <string>
#include <iostream>

#include "gw_shared_ptr.h"
#include "gw_progress.h"
#include "gw_http_post_field.h"

using namespace std;

namespace gw_http
{
#define HTTP_PROTOCOL_VERSION_1_1	"HTTP/1.1"

	// 2xx: success
#define 	gw_http_status_ok              200 ///< most common code returned when operation was successful
#define 	gw_http_status_created         201 ///< the resource has successfully been created
#define 	gw_http_status_accepted        202 ///< the request has been accepted, but will be processed later by the server
#define 	gw_http_status_nocontent       204 ///< the server didn't send any data in return
#define 	gw_http_status_reset_content    205 ///< the server informs the client that it should clear the view (form) that caused the request to be sent
#define 	gw_http_status_fieldial_content  206 ///< the server has sent a field of the resource, as a response to a fieldial get request

	// 3xx: redirection
#define 	gw_http_status_multiple_choices   300 ///< the requested page can be accessed from several locations
#define 	gw_http_status_movedper_manently  301 ///< the requested page has permanently moved to a new location
#define 	gw_http_status_movedtem_porarily  302 ///< the requested page has temporarily moved to a new location
#define 	gw_http_status_not_modified       304 ///< for conditionnal requests, means the requested page hasn't changed and doesn't need to be refreshed

	// 4xx: client error
#define 	gw_http_status_bad_request           400 ///< the server couldn't understand the request (syntax error)
#define 	gw_http_status_unauthorized         401 ///< the requested page needs an authentification to be accessed
#define 	gw_http_status_forbidden            403 ///< the requested page cannot be accessed at all, even with authentification
#define 	gw_http_status_not_found             404 ///< the requested page doesn't exist
#define 	gw_http_status_range_not_satisfiable  407 ///< the server can't satisfy the fieldial get request (with a "range" header field)

	// 5xx: server error
#define 	gw_http_status_internal_server_error  500 ///< the server encountered an unexpected error
#define 	gw_http_status_not_implemented       501 ///< the server doesn't implement a requested feature
#define 	gw_http_status_bad_gateway           502 ///< the gateway server has received an error from the source server
#define 	gw_http_status_service_not_available  503 ///< the server is temporarily unavailable (overloaded, in maintenance, ...)
#define 	gw_http_status_gateway_timeout       504 ///< the gateway server couldn't receive a response from the source server
#define 	gw_http_status_version_not_supported  505 ///< the server doesn't support the requested http version

	// 10xx: custom codes
#define 	gw_http_status_invalid_response   1000 ///< response is not a valid http one
#define 	gw_http_status_connection_failed  1001  ///< connection with server failed


	class http_response
	{
	public:
		typedef gw_shared_ptr<http_response> ptr;
		typedef const gw_shared_ptr<http_response> const_ptr;

		virtual ~http_response(){}

	public:
		virtual int get_status_code() const = 0;

		virtual bool is_ok() const = 0 ;
		virtual bool is_cancelld() const = 0;
		
		virtual const char*	get_content_type() const = 0;
		virtual std::string* get_string_data() = 0;

		virtual string get_header(const char* name_) = 0;
	};

	class proxy_settings
	{
	public:
		proxy_settings();
		proxy_settings(const char* host_, int port_);

		virtual ~proxy_settings() { }

		typedef gw::gw_shared_ptr<proxy_settings> ptr;

		std::string& host_name() { return _hostname; }
		const std::string& host_name() const { return _hostname; }

		int& port() { return _port; }
		const int& port() const { return _port; }

		std::string& user_name() { return _username; }
		const std::string& user_name() const { return _username; }

		std::string& password() { return _password; }
		const std::string& password() const { return _password; }

	protected:
		std::string _hostname;
		int _port;
		std::string _username;
		std::string _password;
	};

	class http_data_connection
	{
	public:
		~http_data_connection(){}

		typedef gw::gw_shared_ptr<http_data_connection> ptr;

	public:
		virtual void create_connections(int connection_count_) = 0;
		virtual void* get_connection(unsigned long user_id_) = 0;
		virtual void free_connection(unsigned long user_id_) = 0;

		virtual void set_timeout(long timeout_) = 0;
		virtual long get_timeout() = 0;

		virtual void set_connect_timeout(long conn_timeout_) = 0;
		virtual long get_connect_timeout() = 0;

		virtual void set_user_agent(const char* user_agent_) = 0;
		virtual const char* get_user_agent() = 0;

		virtual void set_proxy_settings(const proxy_settings &proxy_settings_) = 0;
	};

	class  http_request
	{
	public :
		typedef gw_shared_ptr<http_request> ptr;
		virtual ~http_request(){}

		enum request_method
		{
			get,  ///< Request in get mode, standard method to retrieve a page
			post, ///< Request in post mode, usually to send data to a page
			head  ///< Request a page's header only
		};

		enum operation_type
		{
			simple_get,			
			string_post,		
			file_post,
			multi_files_post
		};

		typedef std::map<std::string, std::string > parameters;		

	public:
		virtual void set_method_(request_method method_) = 0;
		virtual request_method get_method() const = 0;

		virtual void add_header(const char* name_, const char* value_) = 0;
		virtual const string get_header(const string& name_) const = 0;

		//adds an http parameter to the request query string.
		virtual void add_query_parameter(const std::string& name_, const char* value_) = 0;
		virtual void add_query_parameter(const std::string& name_, const std::string& value_) = 0;
		virtual void add_query_parameter(const std::string& name_, int value_ ) = 0;
		virtual void add_query_parameter(const std::string& name_, unsigned int value_ ) = 0;
		virtual void add_query_parameter(const string& name_, unsigned long long value_) = 0;	// 64bit ÕûÐÍ
		virtual void add_query_parameter(const std::string& name_, double value_ ) = 0;	
				
		virtual std::string get_url() const = 0;	/* only for query or post command*/
		
		virtual operation_type get_current_operation_type() const = 0;
		virtual const parameters& get_parameters() = 0;
		virtual void set_http_version(const char* version_) = 0;
		
		//only for post.		
		virtual void add_upload_string_data(const char* field_name_, char* data_, size_t data_size_, http_post_field::data_type format_) = 0;
		virtual void add_upload_file_name(const char* field_name_, const char* file_name_, http_post_field::data_type format_) = 0;

		virtual const char* get_buffer_data() const = 0;
		virtual size_t get_data_length() const = 0 ;
		virtual const char* get_file_name() const = 0;

		virtual const http_post_field::post_fields& get_fields() const = 0;
	};

	class http_receive_listener
	{
	public:
		typedef gw_shared_ptr<http_receive_listener> ptr;

		virtual ~http_receive_listener(){}

		virtual void operator()(http_response::ptr response_) = 0;
		//virtual void operator()(http_response* http_response_, void *buffer, size_t size) = 0;
	};

	class http_service
	{
	public:
		virtual ~http_service(){}

		typedef gw_shared_ptr<http_service>		ptr;
		typedef std::vector<std::string>		file_name_list;
		
	public:		
		virtual http_request::ptr create_simple_get(const char* url_) = 0;
		
		virtual http_request::ptr create_string_post(const char* url_, char* string_buffer_, size_t data_size_) = 0;
		virtual http_request::ptr create_file_post(const char* url_, const char* file_name_) = 0;
		
		virtual http_request::ptr create_multi_files_post(const char* url_, const file_name_list& files_) = 0;
		
		virtual void set_time_out(long timeout_ = 2000) = 0;
		virtual void set_proxy_settings(const proxy_settings &proxy_settings_) = 0;

		virtual void get(const char* url_, http_receive_listener::ptr receive_listener_, progress_callback::ptr callback_) = 0;
		virtual void get(const http_request::ptr http_request_, http_receive_listener::ptr receive_listener_, progress_callback::ptr callback_) = 0;
	};	
	
#define http_service_pid "gw.http.service"
	extern "C" gw_api void* load_http_service();
}

#endif
