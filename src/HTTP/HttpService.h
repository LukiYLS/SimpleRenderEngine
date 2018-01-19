#pragma once
#include "HttpPostField.h"
#include "CallBack.h"
#define HTTP_PROTOCOL_VERSION_1_1	"HTTP/1.1"


// 2xx: success
#define 	Http_Status_OK             200 ///< most common code returned when operation was successful
#define 	Http_Status_Created         201 ///< the resource has successfully been created
#define 	Http_Status_Accepted        202 ///< the request has been accepted, but will be processed later by the server
#define 	Http_Status_No_Content       204 ///< the server didn't send any data in return
#define 	Http_Status_Reset_Content    205 ///< the server informs the client that it should clear the view (form) that caused the request to be sent
#define 	Http_Status_Fieldial_Content  206 ///< the server has sent a field of the resource, as a response to a fieldial get request

// 3xx: redirection
#define 	Http_Status_Multiple_Choices   300 ///< the requested page can be accessed from several locations
#define 	Http_Status_Movedper_Nanently  301 ///< the requested page has permanently moved to a new location
#define 	Http_Status_Movedtem_Porarily  302 ///< the requested page has temporarily moved to a new location
#define 	Http_Status_Not_Modified       304 ///< for conditionnal requests, means the requested page hasn't changed and doesn't need to be refreshed

// 4xx: client error
#define 	Http_Status_Bad_Request           400 ///< the server couldn't understand the request (syntax error)
#define 	Http_Status_Unauthorized         401 ///< the requested page needs an authentification to be accessed
#define 	Http_Status_Forbidden            403 ///< the requested page cannot be accessed at all, even with authentification
#define 	Http_Status_Not_Found             404 ///< the requested page doesn't exist
#define 	Http_Status_Range_Not_Satisfiable  407 ///< the server can't satisfy the fieldial get request (with a "range" header field)

// 5xx: server error
#define 	Http_Status_Internal_Server_Error  500 ///< the server encountered an unexpected error
#define 	Http_Status_Not_Implemented       501 ///< the server doesn't implement a requested feature
#define 	Http_Status_Bad_Gateway           502 ///< the gateway server has received an error from the source server
#define 	Http_Status_Service_Not_Available  503 ///< the server is temporarily unavailable (overloaded, in maintenance, ...)
#define 	Http_Status_Gateway_Timeout       504 ///< the gateway server couldn't receive a response from the source server
#define 	Http_Status_Version_Not_Supported  505 ///< the server doesn't support the requested http version

// 10xx: custom codes
#define 	Http_Status_Invalid_Response   1000 ///< response is not a valid http one
#define 	Http_Status_Connection_Failed  1001  ///< connection with server failed

//后续用作请求影像服务

namespace HTTP {

	//代理请求
	class ProxySetting {

	};

	//////////////////////////////////////////////////////////////////////////


	//响应
	class HttpReponse {


	public:
		typedef std::shared_ptr<HttpReponse> ptr;
		typedef const std::shared_ptr<HttpReponse> const_ptr;

		virtual ~HttpReponse() {}

	public:
		virtual int getStatusCode() const;

		virtual bool isOk() const;
		virtual bool isCancelld() const;

		virtual const char*	getContentType() const;
		virtual std::string* getStringData();

		virtual std::string getHeader(const char* name);

	protected:
		long			_respondCode;
		std::string		_contentType;
		bool			_cancelled;

		std::string		_headers;
		std::string		_bufferData;		/* for get*/

		friend class HttpService;
	};


	//////////////////////////////////////////////////////////////////////////

	//请求
	class  HttpRequest
	{
	public:
		typedef std::shared_ptr<HttpRequest> ptr;
		virtual ~HttpRequest() {}

		enum RequestMethod
		{
			GET,  ///< Request in get mode, standard method to retrieve a page
			POST, ///< Request in post mode, usually to send data to a page
			HEAD  ///< Request a page's header only
		};

		enum Operation
		{
			SimpleGet,
			StringPost,
			FilePost,
			MultiFilesPost
		};
		typedef std::map<std::string, std::string> Headers;
		typedef std::map<std::string, std::string > Parameters;

	public:
		/* for command or download, url is a base url*/
		HttpRequest(const char* url);

		/* for common post*/
		HttpRequest(const char* url, char* buffer_data, size_t data_size);
		HttpRequest(const char* url, const char* a_file_name);

		/* for multipart/form_data post*/
		HttpRequest(const char* url, const HttpPostField::PostFields& fields);

		~HttpRequest();

		virtual void setMethod_(RequestMethod method);
		virtual RequestMethod getMethod() const;

		virtual void addHeader(const char* name, const char* value);
		virtual const std::string getHeader(const std::string& name) const;

		//adds an http parameter to the request query string.
		virtual void addQueryParameter(const std::string& name, const char* valu);
		virtual void addQueryParameter(const std::string& name, const std::string& value);
		virtual void addQueryParameter(const std::string& name, int value);
		virtual void addQueryParameter(const std::string& name, unsigned int value);
		virtual void addQueryParameter(const std::string& name, unsigned long long value);	// 64bit 整型
		virtual void addQueryParameter(const std::string& name, double value);

		virtual std::string getUrl() const;	/* only for query or post command*/

		virtual Operation getCurrentOperation() const;
		virtual const Operation& getParameters();
		virtual void setHttpVersion(const char* version);

		//only for post.		
		virtual void addUploadStringData(const char* field_name_, char* data_, size_t data_size, HttpPostField::DataType format);
		virtual void addUploadFileName(const char* field_name_, const char* file_name, HttpPostField::DataType format);

		virtual const char* getBufferData() const;
		virtual size_t getDataLength() const;
		virtual const char* getFileName() const;

		virtual const HttpPostField::PostFields& get_fields() const;

	protected:

		std::string						_version;
		RequestMethod					_method;
		std::string						_baseUrl;
		Operation			            _operation;
		Parameters					    _queryParams;

		Headers							_headers;	/*save custom header*/

		char*							_bufferData;		/* for post*/
		size_t							_dataLength;

		std::string						_fileName;

		HttpPostField::PostFields	   _fields;			/* for post: multipart/form_data*/

		friend class HttpService;
	};

	//////////////////////////////////////////////////////////////////////////


	

	//////////////////////////////////////////////////////////////////////////

	class HttpService {

	public:

		typedef std::vector<std::string> FileNameList;

		HttpRequest::ptr createSimpleGet(const char* url);//请求数据
		HttpRequest::ptr createStringPost(const char* url, char* stringBuffer, size_t dataSize);//发送数据
		HttpRequest::ptr createFilePost(const char* url, const char* fileName);//发送文件
		HttpRequest::ptr createMultiFilesPost(const char* url, const FileNameList& files);//发送多个文件

		void setTimeOut(long timeout = 2000);
		void setProxySettings(const ProxySetting &proxy_settings);
		//是否设置回调
		void get(const char* url, HttpReponse::ptr response, ProgressCallBack::ptr callback);
		void get(const HttpRequest::ptr request, HttpReponse::ptr response, ProgressCallBack::ptr callback);

	protected:

		HttpReponse::ptr doGet(const char* url, ProgressCallBack::ptr callback);
		HttpReponse::ptr doGet(const HttpRequest::ptr request, ProgressCallBack::ptr callback);

		HttpReponse::ptr performRequestGet(const HttpRequest* request, void* handle);
		HttpReponse::ptr performRequestPost(const HttpRequest* request, void* handle);

	};
}