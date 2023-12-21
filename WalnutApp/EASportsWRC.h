#pragma once
#include <array>
#include <vector>

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
	float speed;
	float gear;
	float stear;
	float clutch;
	float brake;
	float throttle;
	float rpm;
	float max_rpm;
	double track_length;
	double lap_distance;
	float handbrake;
	float current_time;
	float current_minutes;
	float current_seconds;
	float game_total_time;
	float game_delta_time;
	float game_frame_count;
};


class EASportsWRC
{
	public:
		bool OnStage_b = false;
		std::array<uint8_t, 265> UDPReceiveArray{};
		EAtelemetry_data_t data; //TODO check if deprecated
		std::vector<EAtelemetry_data_t> dataVector;
	private:


	public:
		void StartStage();
		bool GetOnStage();
		void SetOnStage(bool value);
		float UnpackArray(EAoffset_t offset);
		double dUnpackArray(EAoffset_t offset);
		void HandleArray();
	private:
		void PrintArray();
		void convertSeconds2Time();
		void Vector2Array();

};

