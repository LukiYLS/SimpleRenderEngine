#include "HttpConnection.h"
#include <sstream>
using namespace std;
namespace HTTP {
	//Callback
	static size_t dataWriteCallback(void* ptr, size_t size, size_t nmemb, void* data)
	{
		size_t num_written = size*nmemb;
		string* data_ptr = (string*)data;
		if (data_ptr)
			data_ptr->append((const char*)ptr, num_written);

		return num_written;
	}

	/*for progress*/
	static int getProgressCallback(void *clientp, curl_off_t dltotal, curl_off_t dlnow,
		curl_off_t ultotal, curl_off_t ulnow)
	{
		ProgressCallBack* call_back = (ProgressCallBack*)clientp;
		bool cancelled = false;
		if (call_back)
		{
			cancelled = call_back->isCanceled() || call_back->reportProgress((double)dlnow, (double)dltotal);
		}
		return cancelled;
	}

	/* for libcurl older than 7.32.0 (CURLOPT_PROGRESSFUNCTION) */
	static int olderProgressCallback(void *clientp, double dltotal, double dlnow, double ultotal, double ulnow)
	{
		return getProgressCallback(clientp, (curl_off_t)dltotal, (curl_off_t)dlnow, (curl_off_t)ultotal, (curl_off_t)ulnow);
	}

	//==================================================================================================================

	HttpConnection::HttpConnection()
	{

	}

	HttpConnection::~HttpConnection()
	{
		std::list<CURL*>::iterator fiter = _freeConnections.begin();
		while (fiter != _freeConnections.end())
		{
			CURL* connectTemp = *fiter;
			if (connectTemp)
			{
				curl_easy_cleanup(connectTemp);
				connectTemp = NULL;
			}
			fiter++;
		}
		_freeConnections.clear();

		std::list<CURL*>::iterator biter = _busyConnections.begin();
		
		while (biter != _busyConnections.end())
		{
			CURL* connectTemp = *biter;
			if (connectTemp)
			{
				curl_easy_cleanup(connectTemp);
				connectTemp = NULL;
			}
			biter++;
		}
		_busyConnections.clear();
	}


	void HttpConnection::initializeConnection(CURL* handle)
	{
		const std::string& proxy_host = _proxySettings.hostName();
		if (!proxy_host.empty())
		{
			int res_code = CURLE_OK;
			stringstream buf;
			buf << proxy_host << ":" << _proxySettings.port();
			string proxy_addr = buf.str();
			res_code = curl_easy_setopt(handle, CURLOPT_PROXY, proxy_addr.c_str());
			if (CURLE_OK == res_code)
			{
				//_logger->info("using proxy: %s\n", proxy_addr.c_str());
			}

			if (!_proxySettings.userName().empty() && !_proxySettings.password().empty())
			{
				string proxy_auth = _proxySettings.userName() + ":" + _proxySettings.password();
				res_code = curl_easy_setopt(handle, CURLOPT_PROXYUSERPWD, proxy_auth.c_str());
				if (CURLE_OK == res_code)
				{
					//_logger->info("using proxy authentication:  %s\n", proxy_auth.c_str());
				}
			}
		}
		else
		{
			//cout << "the proxy is not set, removing proxy settings." << endl;
			curl_easy_setopt(handle, CURLOPT_PROXY, 0);
		}

		/*when is debugenable*/
		//cout << "http_service will output base debug info. " << endl;
		//curl_easy_setopt(*curl_handle_, CURLOPT_VERBOSE, 1L);	//output base debug info
		setUserAgent("Mozilla/5.0 (Windows NT 6.1; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/45.0.2454.101 Safari/537.36");  //for google earth
		if (_userAgent.length() > 0)
		{
			int res_code = CURLE_OK;
			res_code = curl_easy_setopt(handle, CURLOPT_USERAGENT, _userAgent.c_str());
			if (CURLE_OK == res_code )
			{
				//_logger->info("setting user_agent=: %s\n", _user_agent.c_str());
			}
		}

		curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, dataWriteCallback);

# if LIBCURL_VERSION_NUM >= 0X072000
		/* xferinfo was introduced in 7.32.0, no earlier libcurl versions will
		compile as they won't have the symbols around.

		If built with a newer libcurl, but running with an older libcurl:
		curl_easy_setopt() will fail in run-time trying to set the new
		callback, making the older callback get used.

		New libcurls will prefer the new callback and instead use that one even
		if both callbacks are set. */

		curl_easy_setopt(handle, CURLOPT_XFERINFOFUNCTION, &getProgressCallback);
# else
		/* for libcurl older than 7.32.0 (CURLOPT_PROGRESSFUNCTION) */
		curl_easy_setopt(*_curl_handle, CURLOPT_PROGRESSFUNCTION, &olderProgressCallback);
#endif
		curl_easy_setopt(handle, CURLOPT_NOPROGRESS, (void*)0);
		curl_easy_setopt(handle, CURLOPT_FOLLOWLOCATION, (void*)1);

		if (_timeout)
			curl_easy_setopt(handle, CURLOPT_TIMEOUT_MS, _timeout);
		else
			curl_easy_setopt(handle, CURLOPT_TIMEOUT_MS, 10000); //by default,10000ms

		if (_connectTimeout)
			curl_easy_setopt(handle, CURLOPT_CONNECTTIMEOUT, _connectTimeout);

		curl_easy_setopt(handle, CURLOPT_FORBID_REUSE, 1);
		curl_easy_setopt(handle, CURLOPT_NOSIGNAL, 1);
	}

	void HttpConnection::createConnections(int count)
	{
		CURL* handle = NULL;
		for (int i = 0; i<count; i++)
		{
			handle = curl_easy_init();
			if (handle == NULL)
			{
				//if (_logger->is_error_enabled())
				{
					//_logger->error("failed to create CURL connection\n");
				}
			}

			// initialize the handle
			initializeConnection(handle);
			_freeConnections.push_back(handle);
		}
	}

	CURL* HttpConnection::getConnection(unsigned long userID)
	{
		ConnectMap::iterator connPos = _idConnections.find(userID);
		if (connPos != _idConnections.end())
		{
			return (connPos->second);		 // take the connection
		}
		else
		{
			// add a mutex later!!!!!!!!
			//auto_lock(_connect_map_mutex);
			CURL* handleTmp;

			// if _sqlite_init_connections have no more connection, please create a new free connection and add it
			if (_freeConnections.size())
			{
				handleTmp = _freeConnections.front();
				_freeConnections.pop_front();
			}
			else
			{
				handleTmp = curl_easy_init();
				// initialize the handle
				initializeConnection(handleTmp);
			}

			_busyConnections.push_back(handleTmp);
			_idConnections[userID] = handleTmp;

			return handleTmp;
		}
	}
	void HttpConnection::freeConnection(unsigned long userID)
	{
		CURL*  connection = NULL;
		ConnectMap::iterator connPos = _idConnections.find(userID);
		if (connPos != _idConnections.end())
		{
			connection = connPos->second;
			_idConnections.erase(connPos);
		}
		if (connection)
		{
			list<CURL*>::iterator _iter = _busyConnections.begin();
			while (_iter != _busyConnections.end())
			{
				if ((*_iter) == connection)
				{
					_iter = _busyConnections.erase(_iter);
				}
			}

			_freeConnections.push_back(connection);
		}
	}

	void HttpConnection::closeConnection(unsigned long userID)
	{
		CURL*  connection = NULL;
		ConnectMap::iterator connPos = _idConnections.find(userID);
		if (connPos != _idConnections.end())
		{
			connection = connPos->second;
			if (connection)
			{
				curl_easy_cleanup(connection);
				connection = NULL;
			}
			_idConnections.erase(connPos);
		}
	}
}