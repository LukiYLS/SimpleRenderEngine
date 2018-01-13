#include <iostream>
#include <sstream>
#include <map>

#include "curl/curl.h"
#include "gw_types.h"
#include "gw_utility/gw_utility_data_tool.h"
#include "gw_http_request_impl.h"
#include "gw_http_response_impl.h"
#include "gw_http_data_connection_impl.h"
#include "gw_http_service_impl.h"

using namespace std;
using namespace gw;

#define connection_count 4
#define USE_CHUNKED
#define DISABLE_EXPECT

namespace gw_http
{
	typedef struct DATA_ARG
	{
		const char*			_read_ptr;
		size_t			_left_size;
	} post_data_arg;

	/* for a file download*/
	static size_t file_stream_write_callback(void* ptr_, size_t size_, size_t nmemb_, void* data_)
	{
		if (!data_)
			return 0;

		size_t num_written = size_*nmemb_;
		ofstream* stream_ptr = (ofstream*)data_;
		stream_ptr->write((const char*)ptr_, num_written);

		return num_written;
	}

	/* for a file upload*/
	static size_t file_stream_read_callback(void *ptr, size_t size, size_t nmemb, void *userdata)
	{
		istream* data_ = (istream*)userdata;
		size_t num_read_;
		data_->read((char*)ptr, size*nmemb);
		num_read_ = (size_t)data_->gcount();

		return num_read_;
	}

	/* for a string data upload*/
	static size_t string_data_read_callback(void *ptr, size_t size, size_t nmemb, void *userdata)
	{
		post_data_arg* data_arg = (post_data_arg*)userdata;
		if (data_arg)
		{
			size_t can_be_upload_size = size*nmemb;
			if (can_be_upload_size < 1)
				return 0;

			if (data_arg->_left_size)
			{
				size_t actual_upload_size = min(can_be_upload_size, data_arg->_left_size);
				memcpy(ptr, data_arg->_read_ptr, actual_upload_size);

				data_arg->_read_ptr += actual_upload_size;
				data_arg->_left_size -= actual_upload_size;

				return actual_upload_size;
			}
		}

		return 0;
	}

	static string get_file_base_name(const char* file_name_)
	{		
		size_t file_name_length = strlen(file_name_);
		const char* last_char_ptr = file_name_ + file_name_length - 1;
		char* field_name = NULL;
		size_t base_name_len = 0;
		size_t single_char_size = sizeof(char);
		while (1)
		{
			last_char_ptr--;
			base_name_len++;
			if (strstr(last_char_ptr, "/"))
			{
				field_name = new char[base_name_len + 1];
				memcpy(field_name, last_char_ptr + single_char_size, base_name_len);
				field_name[base_name_len] = '\0';

				string file_base_name = field_name;
				safe_delete_array(field_name);

				return file_base_name;
			}
			if (last_char_ptr == file_name_)
				return file_name_;	/*当前目录下的文件，相对路径*/
		}
	}
		
	proxy_settings::proxy_settings()
	{

	}
	proxy_settings::proxy_settings(const char* host_, int port_)
		:_hostname(host_),
		_port(port_)
	{

	}
	
	http_service_impl::http_service_impl(runtime* runtime_)		
	{
		/* http_service在整个程序中只有一个实例，所以先调用curl_global_init
		然后再由调用者再各个线程中调用curl_easy_init来创建curl的handle*/
		curl_global_init(CURL_GLOBAL_DEFAULT);
		//_timeout = 120000; // ms

		_runtime = runtime_;
		_http_connection = new http_data_connection_impl(_runtime);		
		_http_connection->create_connections(connection_count);		

		log_service* log_service_ = (log_service*)(_runtime->get_service(log_service_pid)->get_service());
		_logger = log_service_->get_logger(http_service_pid);
	}

	http_service_impl::~http_service_impl()
	{
		curl_global_cleanup();
	}  	
	
	http_request::ptr http_service_impl::create_simple_get(const char* url_)
	{
		if (NULL == url_)
			return NULL;

		http_request* request = new http_request_impl(url_);
		return http_request::ptr(request);
	}

	http_request::ptr http_service_impl::create_string_post(const char* url_, char* string_data_, size_t data_size_)
	{
		if (NULL == url_)
			return NULL;

		return http_request::ptr(new http_request_impl(url_, string_data_, data_size_));
	}

	http_request::ptr http_service_impl::create_file_post(const char* url_, const char* file_name_)
	{
		if (NULL == url_)
			return NULL;
		
		return http_request::ptr(new http_request_impl(url_, file_name_));
	}

	http_request::ptr http_service_impl::create_multi_files_post(const char* url_, const file_name_list& files_)
	{
		if (NULL == url_)
			return NULL;

		http_post_field::post_fields file_fields;
		file_name_list::const_iterator it = files_.begin();
		for (; it != files_.end(); it++)
		{
			file_fields.push_back(new http_post_field(get_file_base_name(it->c_str()).c_str(), it->c_str()));
		}
		
		return http_request::ptr(new http_request_impl(url_, file_fields));
	}

	http_response::ptr http_service_impl::perform_request_get(const http_request* request_, void* handle_)
	{
		CURLcode res;
		long response_code = 0L;
		
		http_response_impl::ptr response = new http_response_impl();

		////char error_buf[CURL_ERROR_SIZE];
		////error_buf[0] = 0;
		//curl_easy_setopt(handle_, CURLOPT_ERRORBUFFER, (void*)error_buf);	
		//int n0 = curl_easy_setopt(handle_, CURLOPT_NOSIGNAL, 1);
		int n1 = curl_easy_setopt(handle_, CURLOPT_HTTPGET, 1L);
		int n2 = curl_easy_setopt(handle_, CURLOPT_WRITEDATA, (void*)&(response->_buffer_data));
		int n3 = curl_easy_setopt(handle_, CURLOPT_WRITEHEADER, (void*)&(response->_headers));
		
		res = curl_easy_perform(handle_);
				
		if (CURLE_OK == res)
		{
			curl_easy_getinfo(handle_, CURLINFO_RESPONSE_CODE, &response_code);
			response->_respond_code = response_code;

			char* content_type_cp;
			curl_easy_getinfo(handle_, CURLINFO_CONTENT_TYPE, &content_type_cp);

			if (NULL != content_type_cp)
				response->_content_type = content_type_cp;
		}
		else
		{
			response->_cancelled = true;
		}
		 
		return http_response::ptr(response);
	}

	http_response::ptr http_service_impl::perform_request_post(const http_request* request_, void* handle_)
	{
		CURLcode res;
		long response_code = 0L;
		
		http_response_impl::ptr response = new http_response_impl();				
		curl_easy_setopt(handle_, CURLOPT_HEADERDATA, &(response->_headers));		
		curl_easy_setopt(handle_, CURLOPT_WRITEDATA, &(response->_buffer_data));
		switch (request_->get_current_operation_type())
		{
		case http_request::string_post:
		{
			curl_easy_setopt(handle_, CURLOPT_POST, 1L);		
			curl_easy_setopt(handle_, CURLOPT_READFUNCTION, string_data_read_callback);	
			
			post_data_arg _data;
			_data._read_ptr = request_->get_buffer_data();
			_data._left_size = request_->get_data_length();

			curl_easy_setopt(handle_, CURLOPT_READDATA, (void*)(&_data));

			struct curl_slist *custome_headers = NULL;				
#ifdef USE_CHUNKED
			custome_headers = curl_slist_append(custome_headers, "Transfer-Encoding: chunked");
#else
			/* Set the expected POST size. If you want to POST large amounts of data,
			consider CURLOPT_POSTFIELDSIZE_LARGE */
			curl_easy_setopt(handle_, CURLOPT_POSTFIELDSIZE, _data._data_length);
#endif

#ifdef DISABLE_EXPECT
			custome_headers = curl_slist_append(custome_headers, "Expect:");
#endif
			res = curl_easy_setopt(handle_, CURLOPT_HTTPHEADER, custome_headers);
			res = curl_easy_perform(handle_);

			curl_slist_free_all(custome_headers);
			break;
		}

		case http_request::file_post:
		{
			curl_easy_setopt(handle_, CURLOPT_POST, 1L);		
			curl_easy_setopt(handle_, CURLOPT_READFUNCTION, file_stream_read_callback);
			if (NULL != request_->get_file_name())
			{
				ifstream in_file;
				in_file.open(request_->get_file_name(), ios_base::binary | ios_base::in);
				if (in_file.good())
				{
					in_file.seekg(0, in_file.end);
					streamoff size = in_file.tellg();
					in_file.seekg(0, in_file.beg);
					curl_easy_setopt(handle_, CURLOPT_READDATA, &in_file);
					curl_easy_setopt(handle_, CURLOPT_INFILESIZE_LARGE, (curl_off_t)size);
					
					res = curl_easy_perform(handle_);		
					curl_easy_setopt(handle_, CURLOPT_POST, 0L);
				}
			}
			break;
		}

		case http_request::multi_files_post:
		{
			size_t field_count = request_->get_fields().size();
			//form_add, you must specify the Content-Type: "multifield/form-data"
			if (multi_field_form_data == request_->
				get_header(content_type_field) && field_count > 0)
			{
				CURLcode res;
				long response_code = 0L;

				//char error_buf[CURL_ERROR_SIZE];
				//error_buf[0] = 0;
				curl_slist* http_header_list = NULL;

				http_header_list = curl_slist_append(http_header_list, "Content-type: multipart/form-data");
				//curl_easy_setopt(handle_, CURLOPT_ERRORBUFFER, (void*)error_buf);
				curl_easy_setopt(handle_, CURLOPT_HTTPHEADER, http_header_list);

				curl_httppost* form_post = NULL;
				curl_httppost *last_ptr = NULL;
				curl_slist* field_header_list = NULL;

				unsigned int i = 0;
				for (; i < field_count; i++)
				{
					/* have you added some custom headers except content-type? */
					http_post_field* p_field = request_->get_fields()[i].get();
					const http_post_field::headers& field_headers_ = p_field->get_headers();
					http_post_field::headers::const_iterator it = field_headers_.begin();
					for (; it != field_headers_.end(); it++)
					{
						if (content_type_field != it->first && field_name_field != it->first)
						{
							ostringstream oss_;
							oss_ << it->first << ": " << it->second;
							field_header_list = curl_slist_append(field_header_list, oss_.str().c_str());
						}
					}

					const string& field_name = field_headers_.at(field_name_field);
					http_post_field::field_type type_ = p_field->get_field_type();
					switch (type_)
					{
					case http_post_field::string_type:
					{
						curl_formadd(&form_post, &last_ptr,
							CURLFORM_PTRNAME, field_name.c_str(),
							CURLFORM_NAMELENGTH, field_name.length(),
							CURLFORM_PTRCONTENTS, p_field->get_string_data(),
							CURLFORM_CONTENTSLENGTH, p_field->get_buffer_data_length(),
							CURLFORM_CONTENTTYPE, field_headers_.at(content_type_field),
							CURLFORM_CONTENTHEADER, field_header_list,
							CURLFORM_END);

						break;
					}

					case http_post_field::file_type:
					{
						curl_formadd(&form_post, &last_ptr,
							CURLFORM_PTRNAME, field_name.c_str(),
							CURLFORM_NAMELENGTH, field_name.length(),
							CURLFORM_FILE, p_field->get_file_name(),
							CURLFORM_FILENAME, field_name.c_str(),
							CURLFORM_CONTENTTYPE, field_headers_.at(content_type_field),
							CURLFORM_CONTENTHEADER, field_header_list,
							CURLFORM_END);

						break;
					}

					}
				}

				/*Tells libcurl you want a multifield/formdata HTTP POST to be made and you instruct what data to pass on to the server.*/
				curl_easy_setopt(handle_, CURLOPT_HTTPPOST, form_post);

				res = curl_easy_perform(handle_);
				curl_slist_free_all(field_header_list);
				curl_slist_free_all(http_header_list);									
			}
			else
			{
				return NULL;
			}
			break;
		}

		}

		if (CURLE_OK == res)
		{
			curl_easy_getinfo(handle_, CURLINFO_RESPONSE_CODE, &response_code);
			response->_respond_code = response_code;

			char* content_type_cp;
			curl_easy_getinfo(handle_, CURLINFO_CONTENT_TYPE, &content_type_cp);

			if (NULL != content_type_cp)
				response->_content_type = content_type_cp;
		}
		else
		{
			response->_cancelled = true;
		}

		return response;
	}

	void http_service_impl::get(const char* url_, http_receive_listener::ptr receive_listener_, progress_callback::ptr callback_)
	{
		if (!url_)
		{
			if (_logger->is_error_enabled())
			{
				_logger->error("the request url can not be null\n");
			}
			return;
		}
		if (NULL != receive_listener_)
		{
			http_response::ptr response = do_get(url_, callback_);
			receive_listener_->operator()(response);
		}
		
	}

	void http_service_impl::get(const http_request::ptr http_request_, http_receive_listener::ptr receive_listener_, progress_callback::ptr callback_)
	{
		if (NULL != http_request_ && NULL != receive_listener_)
		{			
			http_response::ptr response = do_get(http_request_, callback_);			
			receive_listener_->operator()(response);
		}		
	}
	

	void http_service_impl::set_time_out( long timeout_ /*= 2000*/ )
	{
		_http_connection->set_timeout(timeout_);
	}

	void http_service_impl::set_proxy_settings( const proxy_settings &proxy_settings_ )
	{					 
		_http_connection->set_proxy_settings(proxy_settings_);
	}

	http_response::ptr http_service_impl::do_get(const char* url_, progress_callback::ptr callback_)
	{
		http_request::ptr request_ptr = create_simple_get(url_);
		return do_get(request_ptr, callback_);
	}

	http_response::ptr http_service_impl::do_get(const http_request::ptr request_, progress_callback::ptr callback_)
	{
		CURL* _curl_handle = _http_connection->get_connection(gw_utility::get_current_thread_id());		
		string& tmp_req_url = request_.get()->get_url();	
		//curl_easy_setopt(_curl_handle, CURLOPT_URL, "http://127.0.0.1:9004");
		curl_easy_setopt(_curl_handle, CURLOPT_URL, tmp_req_url.c_str());
		if (callback_)
		{
			curl_easy_setopt(_curl_handle, CURLOPT_PROGRESSDATA, callback_.get());
		}

		if (http_request::get == request_.get()->get_method())
		{
			return perform_request_get(request_.get(), _curl_handle);			
		}
		if (http_request::post == request_.get()->get_method())
		{
			return perform_request_post(request_.get(), _curl_handle);
		}
	
		return NULL;
	}

}

