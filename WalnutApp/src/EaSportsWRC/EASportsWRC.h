#pragma once
#include <array>
#include <vector>
#include <yaml-cpp/yaml.h>
#include <fstream>
#include <filesystem>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <string>
#include <thread>



/* * * * * Type Definitions * * * * */

typedef std::vector <float> EAtelemetryfloat_t;
typedef std::vector <double> EAtelemetrydouble_t;
typedef std::vector <int> EAtelemetrybyte_t;
typedef std::map<std::string, EAtelemetryfloat_t> EAtelemetryfloatMap_t;
typedef std::map<std::string, EAtelemetrydouble_t> EAtelemetrydoubleMap_t;
typedef std::map<std::string, EAtelemetrybyte_t> EAtelemetrybyteMap_t;

enum class EAoffset_t : uint16_t {
	FourCC = 0,
	packet_uid = 4,
	shiftlights_fraction = 12,
	shiftlights_rpm_start = 16,
	shiftlights_rpm_end = 20,
	shiftlights_rpm_valid = 24,
	vehicle_gear_index = 25,
	vehicle_gear_index_neutral = 26,
	vehicle_gear_index_reverse = 27,
	vehicle_gear_maximum = 28,
	vehicle_speed = 29,
	vehicle_transmission_speed = 33,
	vehicle_position_x = 37,
	vehicle_position_y = 41,
	vehicle_position_z = 45,
	vehicle_velocity_x = 49,
	vehicle_velocity_y = 53,
	vehicle_velocity_z = 57,
	vehicle_acceleration_x = 61,
	vehicle_acceleration_y = 65,
	vehicle_acceleration_z = 69,
	vehicle_left_direction_x = 73,
	vehicle_left_direction_y = 77,
	vehicle_left_direction_z = 81,
	vehicle_forward_direction_x = 85,
	vehicle_forward_direction_y = 89,
	vehicle_forward_direction_z = 93,
	vehicle_up_direction_x = 97,
	vehicle_up_direction_y = 101,
	vehicle_up_direction_z = 105,
	vehicle_hub_position_bl = 109,
	vehicle_hub_position_br = 113,
	vehicle_hub_position_fl = 117,
	vehicle_hub_position_fr = 121,
	vehicle_hub_velocity_bl = 125,
	vehicle_hub_velocity_br = 129,
	vehicle_hub_velocity_fl = 133,
	vehicle_hub_velocity_fr = 137,
	vehicle_cp_forward_speed_bl = 141,
	vehicle_cp_forward_speed_br = 145,
	vehicle_cp_forward_speed_fl = 149,
	vehicle_cp_forward_speed_fr = 153,
	vehicle_brake_temperature_bl = 157,
	vehicle_brake_temperature_br = 161,
	vehicle_brake_temperature_fl = 165,
	vehicle_brake_temperature_fr = 169,
	vehicle_engine_rpm_max = 173,
	vehicle_engine_rpm_idle = 177,
	vehicle_engine_rpm_current = 181,
	vehicle_throttle = 185,
	vehicle_brake = 189,
	vehicle_clutch = 193,
	vehicle_steering = 197,
	vehicle_handbrake = 201,
	game_total_time = 205,
	game_delta_time = 209,
	game_frame_count = 213,
	stage_current_time = 221,
	stage_current_distance = 225,
	stage_length = 233

};

struct EAtelemetry_data_t {
	
	int gear;
	
	float shiftlightFraction;
	float shiftlightStart;
	float shiftlightEnd;
	float VehSpeed;
	float VehTransSpeed;
	float VehPosX;
	float VehPosY;
	float VehPosZ;
	float VehSpdX;
	float VehSpdY;
	float VehSpdZ;
	float VehAccelX;
	float VehAccelY;
	float VehAccelZ;
	float VehLeftDirX;
	float VehLeftDirY;
	float VehLeftDirZ;
	float VehFwrdDirX;
	float VehFwrdDirY;
	float VehFwrdDirZ;
	float VehUpDirX;
	float VehUpDirY;
	float VehUpDirZ;
	float HubVertPosBL;
	float HubVertPosBR;
	float HubVertPosFL;
	float HubVertPosFR;
	float HubVertSpdBL;
	float HubVertSpdBR;
	float HubVertSpdFL;
	float HubVertSpdFR;
	float WheelSpeedBL;
	float WheelSpeedBR;
	float WheelSpeedFL;
	float WheelSpeedFR;
	float stear;
	float clutch;
	float brake;
	float throttle;
	float rpm;
	float rpm_idle;
	float max_rpm;
	float handbrake;
	float current_time;
	float current_minutes;
	float current_seconds;
	float game_total_time;
	float game_delta_time;
	float game_frame_count;
	float brake_temp_bl;
	float brake_temp_br;
	float brake_temp_fl;
	float brake_temp_fr;

	double track_length;
	double lap_distance;
};

struct EAtelemetry_data_Vector_t {
	EAtelemetrybyte_t gear;
	
	EAtelemetryfloat_t shiftlightFraction;
	EAtelemetryfloat_t shiftlightStart;
	EAtelemetryfloat_t shiftlightEnd;
	EAtelemetryfloat_t VehSpeed;
	EAtelemetryfloat_t VehTransSpeed;
	EAtelemetryfloat_t VehPosX;
	EAtelemetryfloat_t VehPosY;
	EAtelemetryfloat_t VehPosZ;
	EAtelemetryfloat_t VehSpdX;
	EAtelemetryfloat_t VehSpdY;
	EAtelemetryfloat_t VehSpdZ;
	EAtelemetryfloat_t VehAccelX;
	EAtelemetryfloat_t VehAccelY;
	EAtelemetryfloat_t VehAccelZ;
	EAtelemetryfloat_t VehLeftDirX;
	EAtelemetryfloat_t VehLeftDirY;
	EAtelemetryfloat_t VehLeftDirZ;
	EAtelemetryfloat_t VehFwrdDirX;
	EAtelemetryfloat_t VehFwrdDirY;
	EAtelemetryfloat_t VehFwrdDirZ;
	EAtelemetryfloat_t VehUpDirX;
	EAtelemetryfloat_t VehUpDirY;
	EAtelemetryfloat_t VehUpDirZ;
	EAtelemetryfloat_t HubVertPosBL;
	EAtelemetryfloat_t HubVertPosBR;
	EAtelemetryfloat_t HubVertPosFL;
	EAtelemetryfloat_t HubVertPosFR;
	EAtelemetryfloat_t HubVertSpdBL;
	EAtelemetryfloat_t HubVertSpdBR;
	EAtelemetryfloat_t HubVertSpdFL;
	EAtelemetryfloat_t HubVertSpdFR;
	EAtelemetryfloat_t WheelSpeedBL;
	EAtelemetryfloat_t WheelSpeedBR;
	EAtelemetryfloat_t WheelSpeedFL;
	EAtelemetryfloat_t WheelSpeedFR;
	EAtelemetryfloat_t stear;
	EAtelemetryfloat_t clutch;
	EAtelemetryfloat_t brake;
	EAtelemetryfloat_t throttle;
	EAtelemetryfloat_t rpm;
	EAtelemetryfloat_t max_rpm;
	EAtelemetryfloat_t current_time;
	EAtelemetryfloat_t handbrake;
	EAtelemetryfloat_t current_minutes;
	EAtelemetryfloat_t current_seconds;
	EAtelemetryfloat_t game_total_time;
	EAtelemetryfloat_t game_delta_time;
	EAtelemetryfloat_t game_frame_count;
	EAtelemetryfloat_t brake_temp_bl;
	EAtelemetryfloat_t brake_temp_br;
	EAtelemetryfloat_t brake_temp_fl;
	EAtelemetryfloat_t brake_temp_fr;

	EAtelemetrydouble_t track_length;
	EAtelemetrydouble_t lap_distance;
};

struct EAtelemetryMap_t
{
	EAtelemetryfloatMap_t EAtelemetryfloatMap;
	EAtelemetrydoubleMap_t EAtelemetrydoubleMap;
	EAtelemetrybyteMap_t EAtelemetrybyteMap;
};

//Configurable circular buffer max size
static const size_t CircularBufferMaxSize = 1;

class EASportsWRC
{
	public:	
		std::array<uint8_t, 265> UDPReceiveArray{};
		EAtelemetry_data_t data; //keep, fastest way to get the latest data
		//std::vector<EAtelemetry_data_t> TelemetryData_v;
		EAtelemetry_data_Vector_t TelemetryData_v;

		std::string  SERVER_IP_s;
		int PORT_i;
		SOCKET serverSocket;
		std::thread m_NetworkThread;
		bool isRunning_b = false;
	private:
		struct EAcircularBuff_t {
			uint8_t currentIndex = 0;
			std::array<EAtelemetry_data_t, CircularBufferMaxSize> circularBuffer;
		}EAcircularBuff_s;
		bool OnStage_b = false;
		EAtelemetryMap_t m_EAtelemetryMap;


	public:
		EASportsWRC(std::string serverIP_l, int port_l) : SERVER_IP_s(serverIP_l), PORT_i(port_l) {}
		~EASportsWRC();

		void StartStage();
		bool GetOnStage();
		void SetOnStage(bool value);
		float UnpackArray(EAoffset_t offset);
		double dUnpackArray(EAoffset_t offset);
		uint8_t bUnpackArray(EAoffset_t offset);
		void StoreVector(const std::filesystem::path& filepath);
		bool isOnStage();
		void ClearArray();
		int startClient();
		void stopClient();
		void receiveData();
		bool GenerateMap();
		void GenerateMapFromYAML(const std::filesystem::path& filepath);

	private:
		void PrintArray();
		void convertSeconds2Time();
		void AddtoCircularBuf(EAtelemetry_data_t data_s);
		void HandleArray();

};

