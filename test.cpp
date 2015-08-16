#include "EQEW.hpp"
#include <string>
#include <iostream>
#include <thread>
using namespace std;
int main()
{
	string key = "PBQ03l26fX3eAsgmvPdL9KEhS";
	string sec = "mqs1xwzNU08gvszwdZ8qPg4DKZOLlIRxNJOU3UnNL3YQmiuxpX";
	EQEW e(key,sec);
	cout<<e.beginObtainingAccessTokenAndSecret()<<endl;
	string pin;
	cin >> pin;
	e.completeObtainingAccessTokenAndSecret(pin);
	e.beginMonitoring();
	cout<<"CALLED!!"<<endl;
	std::this_thread::sleep_for(std::chrono::seconds(60));	// 60sec
	e.stopMonitoring();
	return 0;
}
