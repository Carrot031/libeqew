#include "EQEW.hpp" //include header.
#include <string>
#include <iostream>
#include <thread>
using namespace std;
using namespace libeqew;

//This function is called when Earthquake occurs.
void a(EarthquakeData& eq,void* userdata)
{
	cout<<"location="<<eq.epicenterName<<endl;
}
int main()
{
	//Twitter consumer key and consumer secret for your app.
	string key = "";
	string sec = "";

	if(key=="")
	{
		cout<<"Enter consumer key:"<<flush;
		cin>>key;
	}
	if(sec=="")
	{
		cout<<"Enter consumer secret:"<<flush;
		cin>>sec;
	}

	EQEW e(key,sec);
	cout<<e.beginObtainingAccessTokenAndSecret()<<endl; //Show URI to get PIN for access token.
	string pin;
	cin >> pin;
	e.completeObtainingAccessTokenAndSecret(pin); //Get access token.
	
	e.addOnEarthquakeOccured(&a,nullptr);


	e.beginMonitoring();
	std::this_thread::sleep_for(std::chrono::seconds(60));	// 60sec
	
	e.stopMonitoring();
	return 0;
}
