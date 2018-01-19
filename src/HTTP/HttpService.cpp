#include "HttpService.h"
#include "curl/curl.h"
#include <iostream>
#include <sstream>

using namespace std;
namespace HTTP {



	//////////////////////////////////////////////////////////////////////////

	typedef struct DATA_ARG
	{
		const char*			_readPtr;
		size_t			_leftSize;
	} PostDataDrag;

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
		PostDataDrag* data_arg = (post_data_arg*)userdata;
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

	std::string getFileNameFromPath(const char* path)
	{
		size_t length = strlen(path);
		const char* lastCharIndex = path + length - 1;
		char* field_name = NULL;
		size_t base_name_len = 0;
		size_t single_char_size = sizeof(char);
		while (1)
		{
			lastCharIndex--;
			base_name_len++;
			if (strstr(lastCharIndex, "/"))
			{
				field_name = new char[base_name_len + 1];
				memcpy(field_name, lastCharIndex + sizeof(char), base_name_len);
				field_name[base_name_len] = '\0';

				std::string file_base_name = field_name;
				//safe_delete_array(field_name);

				return file_base_name;
			}
			//if (last_char_ptr == file_name_)
			//	return file_name_;	/*当前目录下的文件，相对路径*/
		}
	}

	HttpRequest::ptr HttpService::createSimpleGet(const char* url)
	{
		if (NULL == url)
			return NULL;

		HttpRequest* request = new HttpRequest(url);
		return HttpRequest::ptr(request);
	}

	HttpRequest::ptr HttpService::createStringPost(const char* url, char* stringBuffer, size_t dataSize)
	{

		if (NULL == url)
			return NULL;

		return HttpRequest::ptr(new HttpRequest(url, stringBuffer, dataSize));

	
	}

	HttpRequest::ptr HttpService::createFilePost(const char* url, const char* fileName)
	{
		if (NULL == url)
			return NULL;

		return HttpRequest::ptr(new HttpRequest(url, fileName));
	}

	HttpRequest::ptr HttpService::createMultiFilesPost(const char* url, const FileNameList& files)
	{
		if (NULL == url)
			return NULL;

		HttpPostField::PostFields file_fields;
		FileNameList::const_iterator it = files.begin();
		for (; it != files.end(); it++)
		{
			file_fields.push_back(new HttpPostField(getFileNameFromPath(it->c_str()).c_str(), it->c_str()));
		}

		return HttpRequest::ptr(new HttpRequest(url, file_fields));
	}

	HttpReponse::ptr HttpService::performRequestGet(const HttpRequest* request, void* handle)
	{
		CURLcode res;
		long responseCode = 0L;

		HttpReponse* response = new HttpReponse();

		////char error_buf[CURL_ERROR_SIZE];
		////error_buf[0] = 0;
		//curl_easy_setopt(handle_, CURLOPT_ERRORBUFFER, (void*)error_buf);	
		//int n0 = curl_easy_setopt(handle_, CURLOPT_NOSIGNAL, 1);
		int n1 = curl_easy_setopt(handle, CURLOPT_HTTPGET, 1L);
		int n2 = curl_easy_setopt(handle, CURLOPT_WRITEDATA, (void*)&(response->_bufferData));
		int n3 = curl_easy_setopt(handle, CURLOPT_WRITEHEADER, (void*)&(response->_headers));

		res = curl_easy_perform(handle);

		if (CURLE_OK == res)
		{
			curl_easy_getinfo(handle, CURLINFO_RESPONSE_CODE, &responseCode);
			response->_respondCode = responseCode;

			char* contentType;
			curl_easy_getinfo(handle, CURLINFO_CONTENT_TYPE, &contentType);

			if (NULL != contentType)
				response->_contentType = contentType;
		}
		else
		{
			response->_cancelled = true;
		}

		return HttpReponse::ptr(response);
	}

	HttpReponse::ptr HttpService::performRequestPost(const HttpRequest* request, void* handle)
	{
		CURLcode res;
		long response_code = 0L;

		HttpReponse* response = new HttpReponse();
		curl_easy_setopt(handle, CURLOPT_HEADERDATA, &(response->_headers));
		curl_easy_setopt(handle, CURLOPT_WRITEDATA, &(response->_bufferData));
		switch (request->getCurrentOperation())
		{
		case HttpRequest::StringPost:
		{
			curl_easy_setopt(handle, CURLOPT_POST, 1L);
			curl_easy_setopt(handle, CURLOPT_READFUNCTION, string_data_read_callback);

			PostDataDrag data;
			data._readPtr = request->_bufferData;
			data._leftSize = request->_dataLength;

			curl_easy_setopt(handle, CURLOPT_READDATA, (void*)(&data));

			struct curl_slist *custome_headers = NULL;
#ifdef USE_CHUNKED
			custome_headers = curl_slist_append(custome_headers, "Transfer-Encoding: chunked");
#else
			/* Set the expected POST size. If you want to POST large amounts of data,
			consider CURLOPT_POSTFIELDSIZE_LARGE */
			//curl_easy_setopt(handle, CURLOPT_POSTFIELDSIZE, data._data_length);
#endif

#ifdef DISABLE_EXPECT
			custome_headers = curl_slist_append(custome_headers, "Expect:");
#endif
			res = curl_easy_setopt(handle, CURLOPT_HTTPHEADER, custome_headers);
			res = curl_easy_perform(handle);

			curl_slist_free_all(custome_headers);
			break;
		}

		case HttpRequest::FilePost:
		{
			curl_easy_setopt(handle, CURLOPT_POST, 1L);
			curl_easy_setopt(handle, CURLOPT_READFUNCTION, file_stream_read_callback);
			if (NULL != request->getFileName())
			{
				std::ifstream in_file;
				in_file.open(request->get_file_name(), ios_base::binary | ios_base::in);
				if (in_file.good())
				{
					in_file.seekg(0, in_file.end);
					std::streamoff size = in_file.tellg();
					in_file.seekg(0, in_file.beg);
					curl_easy_setopt(handle, CURLOPT_READDATA, &in_file);
					curl_easy_setopt(handle, CURLOPT_INFILESIZE_LARGE, (curl_off_t)size);

					res = curl_easy_perform(handle);
					curl_easy_setopt(handle, CURLOPT_POST, 0L);
				}
			}
			break;
		}

		case HttpRequest::MultiFilesPost:
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

		return HttpReponse::ptr(response);
	}


	void HttpService::get(const char* url, HttpReponse::ptr response, ProgressCallBack::ptr callback)
	{
		if (!url)
		{
			/*if (_logger->is_error_enabled())
			{
				_logger->error("the request url can not be null\n");
			}*/
			return;
		}
		if (NULL != response)
		{
			response = doGet(url, callback);
			//receive_listener_->operator()(response);
		}

	}

	void HttpService::get(const HttpRequest::ptr request, HttpReponse::ptr response, ProgressCallBack::ptr callback)
	{
		if (NULL != request && NULL != response)
		{
			response = doGet(request, callback);			
		}
	}

	void HttpService::setTimeOut(long timeout /*= 2000*/)
	{
		//_http_connection->set_timeout(timeout_);
	}

	void HttpService::setProxySettings(const ProxySetting &proxy_settings)
	{
		_http_connection->set_proxy_settings(proxy_settings_);
	}

	HttpReponse::ptr HttpService::doGet(const char* url, ProgressCallBack::ptr callback)
	{
		HttpRequest::ptr request_ptr = createSimpleGet(url);
		return doGet(request_ptr, callback);
	}

	HttpReponse::ptr HttpService::doGet(const HttpRequest::ptr request, ProgressCallBack::ptr callback)
	{
		CURL* _curl_handle = _http_connection->get_connection(gw_utility::get_current_thread_id());
		string& tmp_req_url = request.get()->getUrl();
		//curl_easy_setopt(_curl_handle, CURLOPT_URL, "http://127.0.0.1:9004");
		curl_easy_setopt(_curl_handle, CURLOPT_URL, tmp_req_url.c_str());
		if (callback)
		{
			curl_easy_setopt(_curl_handle, CURLOPT_PROGRESSDATA, callback.get());
		}

		if (HttpRequest::GET == request.get()->getMethod())
		{
			return performRequestGet(request.get(), _curl_handle);
		}
		if (HttpRequest::POST == request.get()->getMethod())
		{
			return performRequestPost(request.get(), _curl_handle);
		}

		return NULL;
	}
}