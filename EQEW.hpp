#pragma once
#include <string>
#include <map>
#include <vector>
#include <sstream>
#include <thread>
#include <memory>
#include "EarthquakeData.hpp"
namespace libeqew
{
	class EQEW
	{

		static const std::string URI_TWITTERAPI_REQUEST_TOKEN;
		static const std::string URI_TWITTERAPI_ACCESS_TOKEN;
		static const std::string URI_TWITTERAPI_USERS_SHOW;
		static const std::string URI_TWITTERUSERSTREAM_USER;
		static const std::string URI_TWITTERSTREAM_FILTER;
		std::string consumerKey;
		std::string consumerSecret;
		std::string requestToken;
		std::string requestTokenSecret;
		std::string accessToken;
		std::string accessTokenSecret;
		std::multimap<void (*)(EarthquakeData& earthquake,void* userdata),void*> onEarthquakeOccuredFp;

		static size_t curl_writefunction(char* ptr,size_t size, size_t nmemb, void* userdata);
		static std::map<std::string,std::string> parseQueryString(const std::string& query);
		static std::vector<std::string> split(const std::string& s,char delim);

		bool stopMonitoringFlag = false;
		std::shared_ptr<std::thread> monitoringThread;
		void monitoringWorker();
		public:

		EQEW();
		EQEW(const std::string ck,const std::string cs);

		std::string getConsumerKey() const;
		void setConsumerKey(const std::string value);
		std::string getConsumerSecret() const;
		void setConsumerSecret(const std::string value);
		std::string getAccessToken() const;
		
		//Set the access token to 'value'.
		void setAccessToken(const std::string value);

		//Get current access token secret.
		std::string getAccessTokenSecret()const;

		//Set the access token secret to 'value'.
		void setAccessTokenSecret(const std::string value);

		std::string beginObtainingAccessTokenAndSecret();
		void completeObtainingAccessTokenAndSecret(const std::string& pin);
		
		void beginMonitoring();
		void stopMonitoring();

		void addOnEarthquakeOccured(void (*fp)(EarthquakeData& earthquake,void* u),void* userdata);
		void clearOnEarthquakeOccured();
	};

}
