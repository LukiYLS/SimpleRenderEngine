#include "HttpResponse.h"
#include "HttpDefine.h"
using namespace std;
namespace HTTP {

	HttpResponse::HttpResponse()
	{
		_respondCode = 0L;
		_cancelled = false;
	}

	HttpResponse::~HttpResponse()
	{

	}

	int HttpResponse::getStatusCode() const
	{
		return _respondCode;
	}

	const char*	HttpResponse::getContentType() const
	{
		return _contentType.c_str();
	}

	bool HttpResponse::isOk() const
	{
		return _respondCode == Http_Status_OK && !isCancelld();
	}

	bool HttpResponse::isCancelld() const
	{
		return _cancelled;
	}

	string* HttpResponse::getStringData()
	{
		return &_bufferData;
	}

	string HttpResponse::getHeader(const char* name)
	{
		//find name_ if exists
		if (name && strlen(name) > 0)
		{
			size_t header_pos = _headers.find(name);
			if (string::npos != header_pos)
			{
				size_t interval_len = strlen(name) + strlen(": ");
				size_t value_pos = header_pos + interval_len;
				size_t value_end_pos = value_pos;
				while (_headers[value_end_pos] != '\r' && _headers[value_end_pos + sizeof(char)] != '\n')
				{
					value_end_pos += sizeof(char);
				}

				return _headers.substr(value_pos, value_end_pos - value_pos);
			}
		}

		return "";
	}
}