#include <NRP/CurlWrapper.hpp>

namespace NightRP::Curl {
	CurlWrapper::CurlWrapper() noexcept {
		curl_global_init(CURL_GLOBAL_DEFAULT);
		m_curlHandle = curl_easy_init();
	}

	CurlWrapper::~CurlWrapper() noexcept {
		curl_easy_cleanup(m_curlHandle);
		curl_global_cleanup();
	}

	std::size_t CurlWrapper::writeFunction(
		void* ptr, std::size_t size,
		std::size_t nmemb, std::string* data
	) noexcept {
		data->append((char*) ptr, size * nmemb);
		return size * nmemb;
	}
}