#include <Poco/Poco.h>
#include <string>
class EQEW
{

	static const std::string URI_TWITTERAPI_REQUEST_TOKEN;
	static const std::string TWITTERAPI_HOST;
	static const Poco::UInt16 TWITTERAPI_PORT;
	std::string consumerKey;
	std::string consumerSecret;
	std::string accessToken;
	std::string accessTokenSecret;

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

	void obtainAccessTokenAndSecret();
};
