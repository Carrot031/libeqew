#pragma once
#include <string>
namespace libeqew
{
	struct EarthquakeData
	{
		bool isDrill = false;
		time_t announcedTime = -1;
		std::string id;
		double nLatitude = -361;
		double eLongitude = -361;
		time_t occuredTime;
		std::string epicenterName;
		int depth;
		double magnitude;
		bool occuredAtSea = false;
		bool isEmergency = true;
	/*
		public:
		bool getIsDrill()const;
		void setIsDrill(bool value);
		time_t getAnnouncedTime();
		void setAnnouncedTime(time_t value);
		int getNLatitude();
		void setNLatitude(int value);
		int getELongitude();
		void setELongitude(int value);
		time_t getOccuredTime();
		void setOccuredTime(time_t value);
		std::string getEpicenterName();
		void setEpicenterName(int value);
		int getDepth();
		void setDepth(int value);
		int getMagnitude();
		void setMagnitude(int value);
		bool getOccuredAtSea();
		void setOccuredAtSea(bool value);
		bool getIsEmergency();
		void setIsEmergency(bool value);
	*/
	};
}
