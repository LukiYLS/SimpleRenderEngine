#include "HttpRequest.h"
#include <sstream>
#include "HttpDefine.h"
using namespace std;
namespace HTTP {


	HttpRequest::HttpRequest(const char* url)
	{
		if (url)
			_baseUrl = url;
		else
			_baseUrl = "";

		setMethod(RequestMethod::GET);
		_version = HTTP_PROTOCOL_VERSION_1_1;
		_operation = Operation::SimpleGet;
	}

	HttpRequest::HttpRequest(const char* url, char* bufferData, size_t dataSize)
	{
		_baseUrl = url;
		setMethod(RequestMethod::POST);

		_bufferData = bufferData;
		_dataLength = dataSize;

		_version = HTTP_PROTOCOL_VERSION_1_1;
		_operation = Operation::StringPost;
	}

	HttpRequest::HttpRequest(const char* url, const char* fileName)
	{
		_baseUrl = url;
		setMethod(RequestMethod::POST);

		_fileName = fileName;

		_version = HTTP_PROTOCOL_VERSION_1_1;
		_operation = Operation::FilePost;
	}

	HttpRequest::HttpRequest(const char* url, const HttpPostField::PostFields& fields)
	{
		_baseUrl = url;
		setMethod(RequestMethod::POST);
		addHeader(ContentTypeField, MultiFieldOrFromDataField);
		_fields = fields;
		_version = HTTP_PROTOCOL_VERSION_1_1;
		_operation = Operation::MultiFilesPost;
	}


	void HttpRequest::setMethod(RequestMethod method)
	{
		_method = method;
	}

	HttpRequest::RequestMethod HttpRequest::getMethod() const
	{
		return _method;
	}

	void HttpRequest::addQueryParameter(const string& name, const string& value)
	{
		_queryParams[name] = value;
	}

	void HttpRequest::addQueryParameter(const std::string& name, unsigned int value)
	{
		stringstream pri_buffer;
		pri_buffer << value;
		_queryParams[name] = pri_buffer.str();
	}

	void HttpRequest::addQueryParameter(const string& name, int value)
	{
		stringstream pri_buffer;
		pri_buffer << value;
		_queryParams[name] = pri_buffer.str();
	}

	void HttpRequest::addQueryParameter(const string& name, unsigned long long value)
	{
		stringstream pri_buffer;
		pri_buffer << value;
		_queryParams[name] = pri_buffer.str();;
	}

	void HttpRequest::addQueryParameter(const string& name, double value)
	{
		stringstream pri_buffer;
		pri_buffer << value;
		_queryParams[name] = pri_buffer.str();
	}

	void HttpRequest::addQueryParameter(const std::string& name, const char* value)
	{
		_queryParams[name] = value;
	}

	void HttpRequest::setHttpVersion(const char* version)
	{
		_version = version;
	}

	const HttpRequest::Parameters& HttpRequest::getParameters()
	{
		return _queryParams;
	}

	string HttpRequest::getUrl() const
	{
		if (0 == _queryParams.size())
			return _baseUrl;
		else
		{
			stringstream buf;
			buf << _baseUrl;
			Parameters::const_iterator i = _queryParams.begin();
			for (; i != _queryParams.end(); i++)
			{
				if (i == _queryParams.begin())
				{
					if (_baseUrl.find("?") == string::npos)
					{
						buf << "?" << i->first << "=" << i->second;
					}
					else
						buf << i->first << "=" << i->second;
				}
				else
					buf << "&" << i->first << "=" << i->second;

			}
			return buf.str();
		}
	}

	HttpRequest::Operation HttpRequest::getCurrentOperation() const
	{
		return _operation;
	}

	void HttpRequest::addHeader(const char* name, const char* value)
	{
		if (NULL == name)
			return;

		if (NULL != value)
			_headers[name] = value;
		else
			_headers[name] = "";
	}

	const string HttpRequest::getHeader(const string& name) const
	{
		Headers::const_iterator it = _headers.find(name);
		if (it != _headers.end())
		{
			return it->second;
		}
		else
			return "";
	}

	void HttpRequest::addUploadStringData(const char* field_name, char* data, size_t data_size, HttpPostField::DataType format)
	{
		if (NULL != field_name && NULL != data)
		{
			_fields.push_back(HttpPostField::ptr(new HttpPostField(field_name, data, data_size, format)));
		}
	}

	void HttpRequest::addUploadFileName(const char* field_name, const char* file_name, HttpPostField::DataType format)
	{
		if (NULL != file_name && NULL != file_name)
		{
			_fields.push_back(HttpPostField::ptr(new HttpPostField(field_name, file_name, format)));
		}
	}

	const char* HttpRequest::getBufferData() const
	{
		return _bufferData;
	}

	size_t HttpRequest::getDataLength() const
	{
		return _dataLength;
	}

	const char* HttpRequest::getFileName() const
	{
		return _fileName.c_str();
	}

	const HttpPostField::PostFields& HttpRequest::getFields() const
	{
		return _fields;
	}
}