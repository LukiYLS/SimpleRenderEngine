#pragma once

namespace Core {



	template <typename T> 
	class Singleton {

	private:
		Singleton(const Singleton<T>&);
		Singleton& operator = (const Single<T>&);
	protected:

		static T* _instance;
	public:

	};
}