#ifndef gw_http_request_impl_h__
#define gw_http_request_impl_h__

#include "gw_http.h"
#include "gw_http_post_field.h"

namespace gw_http
{

	class http_request_impl
		: public http_request
	{
	public:
		/* for command or download, url is a base url*/
		http_request_impl(const char* url_);
		
		/* for common post*/
		http_request_impl(const char* url_, char* buffer_data_, size_t data_size_);
		http_request_impl(const char* url_, const char* a_file_name_);

		/* for multipart/form_data post*/
		http_request_impl(const char* url_, const http_post_field::post_fields& fields_);

		~http_request_impl();

		virtual void set_method_(request_method method_);
		virtual request_method get_method() const;

		virtual void add_header(const char* name_, const char* value_);
		virtual const string get_header(const string& name_) const;

		//adds an http parameter to the request query string.
		virtual void add_query_parameter(const std::string& name_, const char* value_);
		virtual void add_query_parameter(const std::string& name_, const std::string& value_);
		virtual void add_query_parameter(const std::string& name_, int value_);
		virtual void add_query_parameter(const std::string& name_, double value_);
		virtual void add_query_parameter(const string& name_, unsigned long long value_);	// 64bit ÕûÐÍ
		virtual void add_query_parameter(const std::string& name_, unsigned int value_ );

		virtual std::string get_url() const;	/* only for query or post command*/

		virtual operation_type get_current_operation_type() const;
		virtual const parameters& get_parameters();
		virtual void set_http_version(const char* version_);

		//only for post.		
		virtual void add_upload_string_data(const char* field_name_, char* data_, size_t data_size_, http_post_field::data_type format_);
		virtual void add_upload_file_name(const char* field_name_, const char* file_name_, http_post_field::data_type format_);

		virtual const char* get_buffer_data() const;
		virtual size_t get_data_length() const;
		virtual const char* get_file_name() const;

		virtual const http_post_field::post_fields& get_fields() const;

		typedef std::map<std::string, std::string> headers;
		typedef std::map<std::string, std::string > query_params;		

	private:
		std::string						_version;
		request_method					_method;
		std::string						_base_url;
		operation_type			_operation;
		query_params					_query_params;

		headers							_headers;	/*save custom header*/

		char*							_buffer_data;		/* for post*/
		size_t							_data_length;

		std::string						_file_name;

		http_post_field::post_fields	_fields;			/* for post: multipart/form_data*/
	};
}

#endif // gw_http_request_impl_h__

