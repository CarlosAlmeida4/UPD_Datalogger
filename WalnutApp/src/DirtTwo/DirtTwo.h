#pragma once

#include <array>

enum class offset_t : uint16_t {
	TIME_TOTAL = 0,
	TIME_LAP = 4,

	DISTANCE_LAP = 8, // meters
	DISTANCE_TOTAL = 12,

	POSITION_X = 16,
	POSITION_Y = 20,
	POSITION_Z = 24,

	SPEED = 28, // the magnitude of the vehicles linear velocity, in [m/s]

	VELOCITY_X = 32,
	VELOCITY_Y = 36,
	VELOCITY_Z = 40,

	ROLL_VECTOR_X = 44,
	ROLL_VECTOR_Y = 48,
	ROLL_VECTOR_Z = 52,

	PITCH_VECTOR_X = 56,
	PITCH_VECTOR_Y = 60,
	PITCH_VECTOR_Z = 64,

	// the vertical position of the wheel from rest(in car space), measured in meters
	SUSPENSION_POSITION_RL = 68,
	SUSPENSION_POSITION_RR = 72,
	SUSPENSION_POSITION_FL = 76,
	SUSPENSION_POSITION_FR = 80,

	// the vertical acceleration of the wheel from rest(in car space), measured in m/s^2
	SUSPENSION_VELOCITY_RL = 84,
	SUSPENSION_VELOCITY_RR = 88,
	SUSPENSION_VELOCITY_FL = 92,
	SUSPENSION_VELOCITY_FR = 96,

	// the speed of the contact patch of the wheel, in m/s
	WHEEL_PATCH_SPEED_RL = 100,
	WHEEL_PATCH_SPEED_RR = 104,
	WHEEL_PATCH_SPEED_FL = 108,
	WHEEL_PATCH_SPEED_FR = 112,

	INPUT_THROTTLE = 116,
	INPUT_STEER = 120,
	INPUT_BRAKE = 124,
	INPUT_CLUTCH = 128,
	GEAR = 132, // 0: neutral, 1, 2, ... 10 = Reverse
	GFORCE_LATERAL = 136,
	GFORCE_LONGITUDINAL = 140,
	CURRENT_LAP = 144, // RX only
	ENGINE_RATE = 148, // [rpm/10]

	NATIVE_SLI_SUPPORT = 152,
	RACE_POSITION = 156,
	KERS_LEVEL = 160,
	KERS_LEVEL_MAX = 164,
	DRS = 168,
	TRACTION_CONTROL = 172,
	ABS = 176,
	FUEL_IN_TANK = 180,
	FUEL_CAPACITY = 184,
	IN_PITS = 188,
	RACE_SECTOR = 192, // the number of splits in the lap
	SECTION_TIME_1 = 196,
	SECTION_TIME_2 = 200,

	TEMPERATURE_BRAKE_RL = 204,
	TEMPERATURE_BRAKE_RR = 208,
	TEMPERATURE_BRAKE_FL = 212,
	TEMPERATURE_BRAKE_FR = 216,

	TYRE_PRESSURE_RL = 220,
	TYRE_PRESSURE_RR = 224,
	TYRE_PRESSURE_FL = 228,
	TYRE_PRESSURE_FR = 232,

	LAPS_COMPLETED = 236,

	TOTAL_LAPS = 240, // RX only, rally = 1
	TRACK_LENGTH = 244,

	LAST_LAP_TIME = 248,

	MAX_RPM = 252, // maximum rpm / 10

	IDLE_RPM = 256,
	MAX_GEARS = 260 // data length: 264
};

struct telemetry_data_t {
	float speed;
	float gear;
	float stear;
	float clutch;
	float brake;
	float throttle;
	float rpm;
	float max_rpm;
	float track_length;
	float lap_distance;
	float handbrake;
};


class DirtTwo
{
public:
	bool OnStage_b = false;
	std::array<uint8_t, 265> UDPReceiveArray{};
	telemetry_data_t data;
private:


public:
	void StartStage();
	bool GetOnStage();
	void SetOnStage(bool value);
	float UnpackArray(offset_t offset);
	void HandleArray();
private:
	void PrintArray();

};

