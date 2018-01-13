#ifndef gw_http_response_impl_h__
#define gw_http_response_impl_h__

#include <vector>
#include <sstream>

#include "gw_http.h"

namespace gw_http
{
	//class multipart_form_data_parser;
	class http_response_impl
		:public http_response
	{
	public:
		http_response_impl(); 
		~http_response_impl();

		typedef gw_shared_ptr<http_response_impl> ptr;
				
		virtual bool is_ok() const;

		virtual bool is_cancelld() const;
				
		virtual int get_status_code() const;
		virtual std::string* get_string_data();
		/** Gets the master mime-type returned by the request */
		virtual const char*	get_content_type() const;

		virtual string get_header(const char* name_);

	private:		
		long			_respond_code;
		std::string		_content_type;
		bool			_cancelled;
		
		std::string		_headers;
		std::string		_buffer_data;		/* for get*/
		
		friend class http_service_impl;

	};

}

#endif // gw_http_response_impl_h__

