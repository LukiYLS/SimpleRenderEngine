#pragma once
#include <string>
#include <map>
#include <vector>
#include <memory>
namespace HTTP {
#define content_type_field			"Content-Type"
#define field_name_field			"name"

#define multi_field_form_data		"multifield/form-data"

#define field_string_type			"text/plain"
#define field_buffer_type			"application/octet-stream"

#define field_file_stream_type		"file_stream"

	class HttpPostField {

	public:
		enum FieldType
		{
			STRING,
			FILE
		};

		enum DataType
		{
			textual,	//for in_memory data
			tga,		//以下这些格式，可以是文件的扩展名，也可以是已全部读入buffer中的文件的binary
			jpg,
			stream
			/*以后添加新的格式*/
		};

		typedef std::map<std::string, std::string> headers;

		HttpPostField(const char* field_name, char* string_buffer, size_t data_size, DataType data_format = textual)
		{
			_string_data = string_buffer;
			_data_size = data_size;
			_field_type = STRING;
			if (field_name)
				add_header(field_name_field, field_name);

			set_content_type_header(data_format);
		}

		HttpPostField(const char* field_name_, const char* file_name_, DataType data_format_ = stream)
		{
			_field_type = FILE;
			if (field_name_)
				add_header(field_name_field, field_name_);
			//get_file_extention_name(field_name_);

			set_content_type_header(data_format_);
		}

		typedef std::shared_ptr<HttpPostField> ptr;
		typedef std::vector<std::shared_ptr<HttpPostField>> PostFields;

	public:
		const FieldType get_field_type()
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
			if (FieldType::STRING == _field_type)
			{
				return _string_data;
			}
			else
			{
				//cout << "you can not get string data, the filed_type you set is not a string type." << endl;
				return NULL;
			}
		}

		const char* get_file_name()
		{
			if (FieldType::FILE == _field_type)
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

		void set_content_type_header(DataType data_format_)
		{
			switch (data_format_)
			{
			case HttpPostField::textual:
				add_header(content_type_field, "text/plain");
				break;
			case HttpPostField::tga:
				add_header(content_type_field, "image/tga");
				break;
			case HttpPostField::jpg:
				add_header(content_type_field, "image/jpg");
				break;
			case HttpPostField::stream:
				add_header(content_type_field, "application/octet-stream");
				break;
			default:
				break;
			}
		}

	protected:
		FieldType		_field_type;

		headers			_headers;		/*multipart/form-data, when post, save your custom header*/

		char*			_string_data;
		size_t			_data_size;

		std::string		_file_name;
	};
}