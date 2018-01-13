#include "gw_http_response_impl.h"

namespace gw_http
{
	http_response_impl::http_response_impl()		
	{
		_respond_code = 0L;
		_cancelled = false;
	}

	http_response_impl::~http_response_impl()
	{

	}

	int http_response_impl::get_status_code() const
	{
		return _respond_code;
	}

	const char*	http_response_impl::get_content_type() const
	{
		return _content_type.c_str();
	}

	bool http_response_impl::is_ok() const
	{
		return _respond_code == gw_http_status_ok && !is_cancelld();
	}

	bool http_response_impl::is_cancelld() const
	{
		return _cancelled;
	}

	string* http_response_impl::get_string_data()
	{
		return &_buffer_data;
	}

	string http_response_impl::get_header(const char* name_)
	{
		//find name_ if exists
		if (name_ && strlen(name_) > 0)
		{
			size_t header_pos = _headers.find(name_);
			if (string::npos != header_pos)
			{
				size_t interval_len = strlen(name_) + strlen(": ");
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
