#include <sstream>
#include <iostream>

#include "gw_configuration.h"
#include "gw_http_data_connection_impl.h"

using namespace std;
namespace gw_http
{
	http_data_connection_impl::http_data_connection_impl(runtime* runtime_)
	{
		_runtime = runtime_;
		_timeout = 0L;
		_connect_timeout = 0L;

		log_service* log_service_ = (log_service*)(_runtime->get_service(log_service_pid)->get_service());
		_logger = log_service_->get_logger(http_service_pid);

		thread_service* _thread_service = (thread_service*)(_runtime->get_service(thread_service_pid)->get_service());
		_connect_map_mutex = _thread_service->create_mutex();
		
		//check_proxy_configuration();
	}

	http_data_connection_impl::~http_data_connection_impl()
	{
		list<curl_handle>::iterator _f_iter = _free_connections.begin();
		while (_f_iter != _free_connections.end())
		{
			curl_handle tmp_curl_connect = *_f_iter;
			if (tmp_curl_connect)
			{
				curl_easy_cleanup(tmp_curl_connect);
				tmp_curl_connect = NULL;
			}
			_f_iter++;
		}
		_free_connections.clear();

		list<curl_handle>::iterator _b_iter = _busy_connections.begin();
		while (_b_iter != _busy_connections.end())
		{
			curl_handle tmp_curl_connect = *_b_iter;
			if (tmp_curl_connect)
			{
				curl_easy_cleanup(tmp_curl_connect);
				tmp_curl_connect = NULL;
			}
			_b_iter++;
		}
		_busy_connections.clear();
	}

	string find_string_description( string description_, string src_ )
	{
		string strSubstring;
		string::npos;
		int nPos = src_.find(description_);
		if ( nPos == string::npos)
		{
			return strSubstring;
		}
		strSubstring = src_.substr(nPos+description_.size()+1);
		int nPos2 = strSubstring.find(";");
		if (nPos2 != string::npos)
		{
			strSubstring = strSubstring.substr(0, strSubstring.find(";"));
		}

		return strSubstring;
	}

	void http_data_connection_impl::check_proxy_configuration()
	{
		configuration_service* _cfg_service = (configuration_service*)_runtime->get_service(configuration_service_pid)->get_service();
		string _proxy_setting = _cfg_service->get_value("gw_internet_proxy_config", "setting");
		if (_proxy_setting.length())
		{
			proxy_settings _ps;
			_ps.host_name() = find_string_description("proxy_server_address", _proxy_setting);					
			_ps.port() = atoi(find_string_description("port", _proxy_setting).c_str());
			_ps.password() = find_string_description("password", _proxy_setting);
			_ps.user_name() = find_string_description("username", _proxy_setting);

			set_proxy_settings(_ps);

			if (_logger->is_info_enabled())
			{
				_logger->info("read proxy settings configuration successfully!\n");
			}
		}
	}

	static size_t data_write_callback(void* ptr_, size_t size_, size_t nmemb_, void* data_)
	{ 		
		size_t num_written = size_*nmemb_;
		string* data_ptr = (string*)data_;
		if (data_ptr)
			data_ptr->append((const char*)ptr_, num_written);

		return num_written;
	}

	/*for progress*/
	static int get_progress_callback(void *clientp, curl_off_t dltotal, curl_off_t dlnow,
		curl_off_t ultotal, curl_off_t ulnow)
	{
		progress_callback* call_back = (progress_callback*)clientp;
		bool cancelled = false;
		if (call_back)
		{
			cancelled = call_back->isCanceled() || call_back->report_progress((double)dlnow, (double)dltotal);
		}
		return cancelled;
	}

	/* for libcurl older than 7.32.0 (CURLOPT_PROGRESSFUNCTION) */
	static int older_progress_callback(void *clientp, double dltotal, double dlnow, double ultotal, double ulnow)
	{
		return get_progress_callback(clientp, (curl_off_t)dltotal, (curl_off_t)dlnow, (curl_off_t)ultotal, (curl_off_t)ulnow);
	}

	void http_data_connection_impl::initialize_connection(curl_handle handle_)
	{
		const string& proxy_host = _proxy_settings.host_name();
		if (!proxy_host.empty())
		{
			int res_code = CURLE_OK;
			stringstream buf;
			buf << proxy_host << ":" << _proxy_settings.port();
			string proxy_addr = buf.str(); 			
			res_code = curl_easy_setopt(handle_, CURLOPT_PROXY, proxy_addr.c_str());
			if (CURLE_OK == res_code && _logger->is_info_enabled())
			{
				_logger->info("using proxy: %s\n", proxy_addr.c_str());
			}

			if (!_proxy_settings.user_name().empty() && !_proxy_settings.password().empty())
			{
				string proxy_auth = _proxy_settings.user_name() + ":" + _proxy_settings.password();
				res_code = curl_easy_setopt(handle_, CURLOPT_PROXYUSERPWD, proxy_auth.c_str());
				if (CURLE_OK == res_code && _logger->is_info_enabled())
				{
					_logger->info("using proxy authentication:  %s\n", proxy_auth.c_str());
				}
			}
		}
		else
		{
			//cout << "the proxy is not set, removing proxy settings." << endl;
			curl_easy_setopt(handle_, CURLOPT_PROXY, 0);
		}

		/*when is debugenable*/
		//cout << "http_service will output base debug info. " << endl;
		//curl_easy_setopt(*curl_handle_, CURLOPT_VERBOSE, 1L);	//output base debug info
		set_user_agent("Mozilla/5.0 (Windows NT 6.1; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/45.0.2454.101 Safari/537.36");  //for google earth
		if (_user_agent.length() > 0)
		{
			int res_code = CURLE_OK; 			
			res_code = curl_easy_setopt(handle_, CURLOPT_USERAGENT, _user_agent.c_str());
			if (CURLE_OK == res_code && _logger->is_info_enabled())
			{
				_logger->info("setting user_agent=: %s\n", _user_agent.c_str());
			}
		}
		
		curl_easy_setopt(handle_, CURLOPT_WRITEFUNCTION, data_write_callback);

# if LIBCURL_VERSION_NUM >= 0X072000
		/* xferinfo was introduced in 7.32.0, no earlier libcurl versions will
		compile as they won't have the symbols around.

		If built with a newer libcurl, but running with an older libcurl:
		curl_easy_setopt() will fail in run-time trying to set the new
		callback, making the older callback get used.

		New libcurls will prefer the new callback and instead use that one even
		if both callbacks are set. */

		curl_easy_setopt(handle_, CURLOPT_XFERINFOFUNCTION, &get_progress_callback);
# else
		/* for libcurl older than 7.32.0 (CURLOPT_PROGRESSFUNCTION) */
		curl_easy_setopt(*_curl_handle, CURLOPT_PROGRESSFUNCTION, &older_progress_callback);
#endif
		curl_easy_setopt(handle_, CURLOPT_NOPROGRESS, (void*)0);
		curl_easy_setopt(handle_, CURLOPT_FOLLOWLOCATION, (void*)1);

		if (_timeout)
			curl_easy_setopt(handle_, CURLOPT_TIMEOUT_MS, _timeout);
		else
		   curl_easy_setopt(handle_, CURLOPT_TIMEOUT_MS, 10000); //by default,10000ms

		if (_connect_timeout)
			curl_easy_setopt(handle_, CURLOPT_CONNECTTIMEOUT, _connect_timeout);

		curl_easy_setopt(handle_, CURLOPT_FORBID_REUSE, 1);
		curl_easy_setopt(handle_, CURLOPT_NOSIGNAL, 1 );  
	}

	void http_data_connection_impl::create_connections( int connection_count_ )
	{
		CURL* _handle = NULL;
		for (int i=0; i<connection_count_; i++)
		{
			_handle = curl_easy_init();
			if (_handle == NULL)
			{
				if (_logger->is_error_enabled())
				{
					_logger->error("failed to create CURL connection\n");
				}
			}
			
			// initialize the handle
			initialize_connection(_handle);
			_free_connections.push_back(_handle); 
		}
	}

	http_data_connection_impl::curl_handle http_data_connection_impl::get_connection( unsigned long user_id_ )
	{
		threadID_connect_map::iterator _conn_pos = _id_connection_map.find(user_id_);
		if (_conn_pos != _id_connection_map.end())
		{	 			
			return (_conn_pos->second);		 // take the connection
		}
		else
		{
			// add a mutex later!!!!!!!!
			auto_lock(_connect_map_mutex);
			curl_handle _tmp_handle;

			// if _sqlite_init_connections have no more connection, please create a new free connection and add it
			if (_free_connections.size())
			{
				_tmp_handle = _free_connections.front();
				_free_connections.pop_front();
			}
			else
			{		
				_tmp_handle = curl_easy_init();
				// initialize the handle
				initialize_connection(_tmp_handle);
			}

			_busy_connections.push_back(_tmp_handle);  
			_id_connection_map[user_id_] = _tmp_handle;	  			

			return _tmp_handle;
		}
	}

	void http_data_connection_impl::free_connection( unsigned long user_id_ )
	{
		curl_handle  _http_connection = NULL;
		threadID_connect_map::iterator _conn_pos = _id_connection_map.find(user_id_);
		if (_conn_pos != _id_connection_map.end())
		{	 	
			_http_connection = _conn_pos->second;	
			_id_connection_map.erase(_conn_pos);
		}
		if (_http_connection)
		{
			list<curl_handle>::iterator _iter = _busy_connections.begin();
			while (_iter != _busy_connections.end())
			{
				if ((*_iter) == _http_connection)
				{
					_iter = _busy_connections.erase(_iter);
				}
			}

			_free_connections.push_back(_http_connection);
		}
	}


	void http_data_connection_impl::close_connection( unsigned long user_id_ )
	{
		curl_handle  _http_connection = NULL;
		threadID_connect_map::iterator _conn_pos = _id_connection_map.find(user_id_);
		if (_conn_pos != _id_connection_map.end())
		{	 	
			_http_connection = _conn_pos->second;	
			if (_http_connection)
			{					 
				curl_easy_cleanup(_http_connection);
				_http_connection = NULL;
			}
			_id_connection_map.erase(_conn_pos);
		}
	}

	void http_data_connection_impl::set_timeout(long timeout_)
	{
		_timeout = timeout_;
	}

	void http_data_connection_impl::set_connect_timeout(long conn_timeout_)
	{
		_connect_timeout = conn_timeout_;
	}

	long http_data_connection_impl::get_timeout()
	{
		return _timeout;
	}

	long http_data_connection_impl::get_connect_timeout()
	{
		return _connect_timeout;
	}

	void http_data_connection_impl::set_user_agent(const char* user_agent_)
	{
		if (!user_agent_)
			_user_agent = "";
		else
			_user_agent = user_agent_;
	}

	const char* http_data_connection_impl::get_user_agent()
	{
		return _user_agent.c_str();
	}

	void http_data_connection_impl::set_proxy_settings(const proxy_settings &proxy_settings_)
	{
		_proxy_settings = proxy_settings_;
	}
}

