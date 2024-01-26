#pragma once

#include <curl/curl/curl.h>
#include <string>
#include <cstdio>

namespace NightRP::Curl {
	class CurlWrapper {
		CURL* m_curlHandle;

		static std::size_t writeFunction(
			void* ptr, std::size_t size,
			std::size_t nmemb, std::string* data
		) noexcept;
	public:
		CurlWrapper() noexcept;
		~CurlWrapper() noexcept;

		void Get(const char* path) noexcept {
			curl_easy_setopt(m_curlHandle, CURLOPT_URL, path);
			curl_easy_setopt(m_curlHandle, CURLOPT_HTTPGET, 1L);

			std::string response_string;
			std::string header_string;

			curl_slist* hs = curl_slist_append(nullptr, "Content-Type: application/json");

			curl_easy_setopt(m_curlHandle, CURLOPT_HTTPHEADER, hs);

			curl_easy_setopt(m_curlHandle, CURLOPT_WRITEFUNCTION, &CurlWrapper::writeFunction);
			curl_easy_setopt(m_curlHandle, CURLOPT_WRITEDATA, &response_string);
			curl_easy_setopt(m_curlHandle, CURLOPT_HEADERDATA, &header_string);


			auto res = curl_easy_perform(m_curlHandle);
			if (res != CURLE_OK)
				printf("curl_easy_perform() failed: %s\n",
					curl_easy_strerror(res));
			
			printf("%s\n", header_string.c_str());
		}
	};
}