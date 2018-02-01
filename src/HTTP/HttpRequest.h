#pragma once
#include <memory>
#include <map>
#include <string>
#include "HttpPostField.h"
namespace HTTP {

	//«Î«Û
	class  HttpRequest
	{
	public:
		typedef std::shared_ptr<HttpRequest> ptr;
		

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
		HttpRequest(const char* url, char* bufferData, size_t dataSize);
		HttpRequest(const char* url, const char* fileName);

		/* for multipart/form_data post*/
		HttpRequest(const char* url, const HttpPostField::PostFields& fields);

		virtual ~HttpRequest() {};

		virtual void setMethod(RequestMethod method);
		virtual RequestMethod getMethod() const;

		virtual void addHeader(const char* name, const char* value);
		virtual const std::string getHeader(const std::string& name) const;

		//adds an http parameter to the request query string.
		virtual void addQueryParameter(const std::string& name, const char* valu);
		virtual void addQueryParameter(const std::string& name, const std::string& value);
		virtual void addQueryParameter(const std::string& name, int value);
		virtual void addQueryParameter(const std::string& name, unsigned int value);
		virtual void addQueryParameter(const std::string& name, unsigned long long value);	// 64bit ’˚–Õ
		virtual void addQueryParameter(const std::string& name, double value);

		virtual std::string getUrl() const;	/* only for query or post command*/

		virtual Operation getCurrentOperation() const;
		virtual const Parameters& getParameters();
		virtual void setHttpVersion(const char* version);

		//only for post.		
		virtual void addUploadStringData(const char* field_name_, char* data_, size_t data_size, HttpPostField::DataType format);
		virtual void addUploadFileName(const char* field_name_, const char* file_name, HttpPostField::DataType format);

		virtual const char* getBufferData() const;
		virtual size_t getDataLength() const;
		virtual const char* getFileName() const;

		virtual const HttpPostField::PostFields& getFields() const;

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

}