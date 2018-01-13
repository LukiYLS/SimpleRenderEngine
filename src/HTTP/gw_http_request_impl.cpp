#include <sstream>
#include "gw_http_request_impl.h"

using namespace std;

namespace gw_http
{
	http_request_impl::http_request_impl(const char* url_)
	{
		if (url_)
			_base_url = url_;
		else
			_base_url = "";
				
		set_method_(request_method::get);
		_version = HTTP_PROTOCOL_VERSION_1_1;		
		_operation = operation_type::simple_get;
	}

	http_request_impl::http_request_impl(const char* url_, char* buffer_data_, size_t data_size_)
	{
		_base_url = url_;
		set_method_(request_method::post);
		
		_buffer_data = buffer_data_;
		_data_length = data_size_;

		_version = HTTP_PROTOCOL_VERSION_1_1;
		_operation = operation_type::string_post;
	}

	http_request_impl::http_request_impl(const char* url_, const char* a_file_name_)
	{
		_base_url = url_;
		set_method_(request_method::post);

		_file_name = a_file_name_;

		_version = HTTP_PROTOCOL_VERSION_1_1;
		_operation = operation_type::file_post;
	}
	
	http_request_impl::http_request_impl(const char* url_, const http_post_field::post_fields& fields_)
	{
		_base_url = url_;
		set_method_(request_method::post);
		add_header(content_type_field, multi_field_form_data);		
		_fields = fields_;
		_version = HTTP_PROTOCOL_VERSION_1_1;
		_operation = operation_type::multi_files_post;
	}

	http_request_impl::~http_request_impl()
	{
		
	}

	void http_request_impl::set_method_(request_method method_)
	{
		_method = method_;
	}

	http_request::request_method http_request_impl::get_method() const
	{
		return _method;
	}

	void http_request_impl::add_query_parameter(const string& name_, const string& value_)
	{
		_query_params[name_] = value_;
	}

	void http_request_impl::add_query_parameter(const std::string& name_, unsigned int value_ )
	{
		stringstream pri_buffer;
		pri_buffer << value_;
		_query_params[name_] = pri_buffer.str();
	}

	void http_request_impl::add_query_parameter(const string& name_, int value_)
	{
		stringstream pri_buffer;
		pri_buffer << value_;
		_query_params[name_] = pri_buffer.str();
	}

	void http_request_impl::add_query_parameter(const string& name_, unsigned long long value_)
	{
		stringstream pri_buffer;
		pri_buffer << value_;
		_query_params[name_] = pri_buffer.str();;
	}

	void http_request_impl::add_query_parameter(const string& name_, double value_)
	{
		stringstream pri_buffer;
		pri_buffer << value_;
		_query_params[name_] = pri_buffer.str();
	}

	void http_request_impl::add_query_parameter(const std::string& name_, const char* value_)
	{
		_query_params[name_] = value_;
	}

	void http_request_impl::set_http_version(const char* version_)
	{
		_version = version_;
	}

	const http_request::parameters& http_request_impl::get_parameters()
	{
		return _query_params;
	}

	string http_request_impl::get_url() const
	{
		if (0 == _query_params.size())
			return _base_url;
		else
		{
			stringstream buf;
			buf << _base_url;
			parameters::const_iterator i = _query_params.begin();
			for (; i != _query_params.end(); i++)
			{
				if (i == _query_params.begin())
				{
					if (_base_url.find("?") == string::npos)
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

	http_request::operation_type http_request_impl::get_current_operation_type() const
	{
		return _operation;
	}

	void http_request_impl::add_header(const char* name_, const char* value_)
	{
		if (NULL == name_)
			return;

		if (NULL != value_)
			_headers[name_] = value_;
		else
			_headers[name_] = "";
	}

	const string http_request_impl::get_header(const string& name_) const
	{
		headers::const_iterator it = _headers.find(name_);
		if (it != _headers.end())
		{
			return it->second;
		}
		else
			return "";
	}

	void http_request_impl::add_upload_string_data(const char* field_name_, char* data_, size_t data_size_, http_post_field::data_type format_)
	{
		if (NULL != field_name_ && NULL != data_)
		{
			_fields.push_back(new http_post_field(field_name_, data_, data_size_, format_));
		}
	}

	void http_request_impl::add_upload_file_name(const char* field_name_, const char* file_name_, http_post_field::data_type format_)
	{
		if (NULL != field_name_ && NULL != file_name_)
		{
			_fields.push_back(new http_post_field(field_name_, file_name_, format_));
		}
	}

	const char* http_request_impl::get_buffer_data() const
	{
		return _buffer_data;
	}

	size_t http_request_impl::get_data_length() const
	{
		return _data_length;
	}

	const char* http_request_impl::get_file_name() const
	{
		return _file_name.c_str();
	}

	const http_post_field::post_fields& http_request_impl::get_fields() const
	{
		return _fields;
	}

}