#pragma once
#include "HttpService.h"
#include <curl/curl.h>
#include <list>
namespace HTTP {

	/*
	管理HTTP连接
	*/
	class HttpConnection {

	public:
		HttpConnection();
		~HttpConnection();

		void createConnections(int count);
		CURL* getConnection(unsigned long userID);

		void freeConnection(unsigned long userID);
		void closeConnection(unsigned long userID);

		void setTimeOut(long timeout) { _timeout = timeout; }
		long getTimeOut()const { return _timeout; }

		void setUserAgent(const char* agent) { _userAgent = agent; }
		const char* getUserAgent()const { return _userAgent.c_str(); }

		void setPorxySetting(const ProxySetting& proxySetting) { _proxySettings = proxySetting; }

	protected:

		typedef std::map<unsigned long, CURL*> ConnectMap;

		void initializeConnection(CURL* handle);
		//void checkProxyConfiguration();

	private:

		std::string							_serverUrl;
		long								_timeout;
		long								_connectTimeout;
		std::string								_userAgent;

		ProxySetting						_proxySettings;

		std::list<CURL*>				_freeConnections;
		std::list<CURL*>				_busyConnections;
		ConnectMap				_idConnections;
		//gw_shared_ptr<gw::mutex>			_connect_map_mutex;
		
	};
}