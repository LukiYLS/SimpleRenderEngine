#pragma once
#include "HttpPostField.h"
#include "CallBack.h"
#include "HttpResponse.h"
#include "HttpRequest.h"
#include "HttpDefine.h"
namespace HTTP {

	//代理请求
	class ProxySetting {

	public:
		ProxySetting() = default;
		ProxySetting(const char* host, int port) :_hostname(host), _port(port) {

		}

		virtual ~ProxySetting() { }

		typedef std::shared_ptr<ProxySetting> ptr;

		std::string& hostName() { return _hostname; }
		const std::string& hostName() const { return _hostname; }

		int& port() { return _port; }
		const int& port() const { return _port; }

		std::string& userName() { return _username; }
		const std::string& userName() const { return _username; }

		std::string& password() { return _password; }
		const std::string& password() const { return _password; }

	protected:
		std::string _hostname;
		int _port;
		std::string _username;
		std::string _password;

	};

	//////////////////////////////////////////////////////////////////////////




	

	//////////////////////////////////////////////////////////////////////////
	class HttpConnection;
	class __declspec(dllexport) HttpService {

	public:
		static HttpService* getSingleton();
		typedef std::vector<std::string> FileNameList;

		HttpService();
		~HttpService();

		HttpRequest::ptr createSimpleGet(const char* url);//请求数据
		HttpRequest::ptr createStringPost(const char* url, char* stringBuffer, size_t dataSize);//发送数据
		HttpRequest::ptr createFilePost(const char* url, const char* fileName);//发送文件
		HttpRequest::ptr createMultiFilesPost(const char* url, const FileNameList& files);//发送多个文件

		void setTimeOut(long timeout = 2000);
		void setProxySettings(const ProxySetting &proxy_settings);
		//是否设置回调
		void get(const char* url, HttpResponse::ptr response, ProgressCallBack::ptr callback);
		void get(const HttpRequest::ptr request, HttpResponse::ptr response, ProgressCallBack::ptr callback);

	protected:

		HttpResponse::ptr doGet(const char* url, ProgressCallBack::ptr callback);
		HttpResponse::ptr doGet(const HttpRequest::ptr request, ProgressCallBack::ptr callback);

		HttpResponse::ptr performRequestGet(const HttpRequest* request, void* handle);
		HttpResponse::ptr performRequestPost(const HttpRequest* request, void* handle);

	private:
		static HttpService* instance;
		HttpConnection* _connection;

	};
}