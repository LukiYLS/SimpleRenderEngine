#ifndef gw_http_post_field_h__
#define gw_http_post_field_h__

#include <string>
#include <vector>
#include <sstream>
#include <fstream>
#include <iostream>

#include "gw_shared_ptr.h"

using namespace std;
namespace gw_http
{
#define content_type_field			"Content-Type"
#define field_name_field			"name"

#define multi_field_form_data		"multifield/form-data"

#define field_string_type			"text/plain"
#define field_buffer_type			"application/octet-stream"

#define field_file_stream_type		"file_stream"

	class http_post_field
	{
	public:	
		enum field_type
		{
			string_type,
			file_type
		};

		enum data_type
		{
			textual,	//for in_memory data
			tga,		//以下这些格式，可以是文件的扩展名，也可以是已全部读入buffer中的文件的binary
			jpg,
			stream
			/*以后添加新的格式*/
		};
		
		typedef std::map<std::string, std::string> headers;

		http_post_field(const char* field_name_, char* string_buffer_, size_t data_size_, data_type data_format_ = textual)
		{
			_string_data = string_buffer_;
			data_size_ = data_size_;
			_field_type = string_type;
			if (field_name_)
				add_header(field_name_field, field_name_);

			set_content_type_header(data_format_);
		}

		http_post_field(const char* field_name_, const char* file_name_, data_type data_format_ = stream)
		{
			_field_type = file_type;
			if (field_name_)
				add_header(field_name_field, field_name_);
			//get_file_extention_name(field_name_);

			set_content_type_header(data_format_);
		}

		typedef gw_shared_ptr<http_post_field> ptr;		
		typedef std::vector<gw_shared_ptr<http_post_field>> post_fields;

	public:
		const field_type get_field_type()
		{
			return _field_type;
		}
		
		const headers& get_headers()
		{
			return _headers;
		}
		
		size_t get_buffer_data_length()
		{
			return _data_size;
		}
		
		const char* get_string_data()
		{
			if (field_type::string_type == _field_type)
			{
				return _string_data;
			}
			else
			{
				cout << "you can not get string data, the filed_type you set is not a string type." << endl;
				return NULL;
			}
		}

		const char* get_file_name()
		{
			if (field_type::file_type == _field_type)
			{
				return _file_name.c_str();
			}
			else
			{
				cout << "you can not get the upload file's name, the filed_type you set is not a file type." << endl;
				return NULL;
			}				
		}
		
	private:
		void add_header(const char* name_, const char* value_)
		{
			if (NULL == name_)
				return;

			if (NULL != value_)
				_headers[name_] = value_;
			else
				_headers[name_] = "";
		}
		
		void set_content_type_header(data_type data_format_)
		{
			switch (data_format_)
			{
			case http_post_field::textual:
				add_header(content_type_field, "text/plain");
				break;
			case http_post_field::tga:
				add_header(content_type_field, "image/tga");
				break;
			case http_post_field::jpg:
				add_header(content_type_field, "image/jpg");
				break;
			case http_post_field::stream:
				add_header(content_type_field, "application/octet-stream");
				break;
			default:
				break;
			}
		}

	protected:
		field_type		_field_type;

		headers			_headers;		/*multipart/form-data, when post, save your custom header*/		
		
		char*			_string_data;
		size_t			_data_size;

		std::string		_file_name;

	};

}

#endif // gw_http_post_field_h__
