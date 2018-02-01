#pragma once
#include <memory>
#include <string>
namespace HTTP {

	//œÏ”¶
	class HttpResponse {


	public:
		typedef std::shared_ptr<HttpResponse> ptr;
		typedef const std::shared_ptr<HttpResponse> const_ptr;
		HttpResponse();
		virtual ~HttpResponse();

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

}