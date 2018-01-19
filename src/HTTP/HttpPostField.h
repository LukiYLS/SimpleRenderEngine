#pragma once
#include <string>
#include <map>
#include <vector>
#include <memory>
namespace HTTP {
#define ContentTypeField			"Content-Type"
#define FieldNameField			"name"

#define MultiFieldOrFromDataField		"multifield/form-data"

#define StringField			"text/plain"
#define StreamBufferField			"application/octet-stream"

#define FileStreamField		"file_stream"

	class HttpPostField {

	public:
		enum FieldType
		{
			STRING,
			FILE
		};

		enum DataType
		{
			TEXTURAL,	//for in_memory data
			TGA,		//以下这些格式，可以是文件的扩展名，也可以是已全部读入buffer中的文件的binary
			JPG,
			STREAM
			/*以后添加新的格式*/
		};

		typedef std::map<std::string, std::string> headers;

		HttpPostField(const char* field_name, char* string_buffer, size_t data_size, DataType data_format = TEXTURAL)
		{
			_stringData = string_buffer;
			_dataSize = data_size;
			_fieldType = STRING;
			if (field_name)
				add_header(FieldNameField, field_name);

			set_content_type_header(data_format);
		}

		HttpPostField(const char* field_name, const char* file_name, DataType data_format = STREAM)
		{
			_fieldType = FILE;
			if (field_name)
				add_header(FieldNameField, field_name);
			//get_file_extention_name(field_name_);

			set_content_type_header(data_format);
		}

		typedef std::shared_ptr<HttpPostField> ptr;
		typedef std::vector<std::shared_ptr<HttpPostField>> PostFields;

	public:
		const FieldType get_field_type()
		{
			return _fieldType;
		}

		const headers& get_headers()
		{
			return _headers;
		}

		size_t get_buffer_data_length()
		{
			return _dataSize;
		}

		const char* get_string_data()
		{
			if (FieldType::STRING == _fieldType)
			{
				return _stringData;
			}
			else
			{
				//cout << "you can not get string data, the filed_type you set is not a string type." << endl;
				return NULL;
			}
		}

		const char* get_file_name()
		{
			if (FieldType::FILE == _fieldType)
			{
				return _fileName.c_str();
			}
			else
			{
				//cout << "you can not get the upload file's name, the filed_type you set is not a file type." << endl;
				return NULL;
			}
		}

	private:
		void add_header(const char* name, const char* value)
		{
			if (NULL == name)
				return;

			if (NULL != value)
				_headers[name] = value;
			else
				_headers[name] = "";
		}

		void set_content_type_header(DataType data_format)
		{
			switch (data_format)
			{
			case HttpPostField::TEXTURAL:
				add_header(ContentTypeField, "text/plain");
				break;
			case HttpPostField::TGA:
				add_header(ContentTypeField, "image/tga");
				break;
			case HttpPostField::JPG:
				add_header(ContentTypeField, "image/jpg");
				break;
			case HttpPostField::STREAM:
				add_header(ContentTypeField, "application/octet-stream");
				break;
			default:
				break;
			}
		}

	protected:
		FieldType		_fieldType;

		headers			_headers;		/*multipart/form-data, when post, save your custom header*/

		char*			_stringData;
		size_t			_dataSize;

		std::string		_fileName;
	};
}