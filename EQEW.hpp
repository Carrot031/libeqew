#include <Poco/Poco.h>
#include <string>
#include <map>
#include <vector>
#include <sstream>
class EQEW
{

	static const std::string URI_TWITTERAPI_REQUEST_TOKEN;
	static const std::string URI_TWITTERAPI_ACCESS_TOKEN;
	static const std::string TWITTERAPI_HOST;
	static const Poco::UInt16 TWITTERAPI_PORT;
	std::string consumerKey;
	std::string consumerSecret;
	std::string requestToken;
	std::string requestTokenSecret;
	std::string accessToken;
	std::string accessTokenSecret;

	static size_t curl_writefunction(char* ptr,size_t size, size_t nmemb, void* userdata);
	static std::map<std::string,std::string> parseQueryString(const std::string& query);
	static std::vector<std::string> split(const std::string& s,char delim);

	public:

	EQEW();
	EQEW(const std::string ck,const std::string cs);

	std::string getConsumerKey() const;
	void setConsumerKey(const std::string value);
	std::string getConsumerSecret() const;
	void setConsumerSecret(const std::string value);
	std::string getAccessToken() const;
	void setAccessToken(const std::string value);
	std::string getAccessTokenSecret()const;
	void setAccessTokenSecret(const std::string value);

	std::string beginObtainingAccessTokenAndSecret();
	void completeObtainingAccessTokenAndSecret(const std::string& pin);
	
	void beginMonitoring();
};
