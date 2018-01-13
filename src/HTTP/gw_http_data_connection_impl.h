#include <list>
#include <map>

#include "gw_runtime.h"
#include "gw_log.h"
#include "gw_http.h"
#include "curl/curl.h"
#include "gw_thread.h"

namespace gw_http
{
	class http_data_connection_impl
		: public http_data_connection
	{
	public:
		http_data_connection_impl(gw::runtime* runtime_);
		~http_data_connection_impl();
			
		typedef CURL*  curl_handle;

	public:
		virtual void create_connections(int connection_count_);
		virtual curl_handle get_connection(unsigned long user_id_);
		virtual void free_connection(unsigned long user_id_);
		virtual void close_connection(unsigned long user_id_);

		virtual void set_timeout(long timeout_);
		virtual long get_timeout();

		virtual void set_connect_timeout(long conn_timeout_);
		virtual long get_connect_timeout();

		virtual void set_user_agent(const char* user_agent_);
		virtual const char* get_user_agent();

		virtual void set_proxy_settings(const proxy_settings &proxy_settings_);	 
	
	private:	 		
		typedef std::map<unsigned long, curl_handle> threadID_connect_map;
	
		void initialize_connection(curl_handle handle_);
		void check_proxy_configuration();

	private:
		std::string							_server_url;
		long								_timeout;
		long								_connect_timeout;
		string								_user_agent;

		proxy_settings						_proxy_settings;
											
		std::list<curl_handle>				_free_connections;
		std::list<curl_handle>				_busy_connections;
		threadID_connect_map				_id_connection_map;
		gw_shared_ptr<gw::mutex>			_connect_map_mutex;

		gw_shared_ptr<logger>				_logger;

		gw::runtime*						_runtime;
	};
}
