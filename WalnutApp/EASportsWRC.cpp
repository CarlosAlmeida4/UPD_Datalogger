#include "EASportsWRC.h"
#include <iostream>

/***Utilities***/
template <typename E>
constexpr auto to_underlying(E e) noexcept {
	return static_cast<std::underlying_type_t<E>>(e);
}

/***

Array Operations

***/

void EASportsWRC::HandleArray()
{
	EAtelemetry_data_t data_s;
	data_s.speed = UnpackArray(EAoffset_t::vehicle_speed);
	data_s.gear = UnpackArray(EAoffset_t::vehicle_gear_index);
	data_s.stear = UnpackArray(EAoffset_t::vehicle_steering);
	data_s.clutch = UnpackArray(EAoffset_t::vehicle_clutch);
	data_s.brake = UnpackArray(EAoffset_t::vehicle_brake);
	data_s.throttle = UnpackArray(EAoffset_t::vehicle_throttle);
	data_s.rpm = UnpackArray(EAoffset_t::vehicle_engine_rpm_current);
	data_s.max_rpm = UnpackArray(EAoffset_t::vehicle_engine_rpm_max);
	data_s.track_length = dUnpackArray(EAoffset_t::stage_length);
	data_s.lap_distance = dUnpackArray(EAoffset_t::stage_current_distance);
	data_s.current_time = UnpackArray(EAoffset_t::stage_current_time);
	data_s.handbrake = UnpackArray(EAoffset_t::vehicle_handbrake);
	data_s.game_total_time = UnpackArray(EAoffset_t::game_total_time);
	data_s.game_delta_time = UnpackArray(EAoffset_t::game_delta_time);
	data_s.game_frame_count = UnpackArray(EAoffset_t::game_frame_count);
	//data_s.brake_temp_bl = UnpackArray(EAoffset_t::vehicle_cp_forward_speed_bl);
	//data_s.brake_temp_br = UnpackArray(EAoffset_t::vehicle_cp_forward_speed_br);
	//data_s.brake_temp_fl = UnpackArray(EAoffset_t::vehicle_cp_forward_speed_fl);
	//data_s.brake_temp_fr = UnpackArray(EAoffset_t::vehicle_cp_forward_speed_fr);
	data_s.brake_temp_br = UnpackArray(EAoffset_t::vehicle_brake_temperature_bl);
	data_s.brake_temp_fl = UnpackArray(EAoffset_t::vehicle_brake_temperature_br);
	data_s.brake_temp_fr = UnpackArray(EAoffset_t::vehicle_brake_temperature_fl);
	data_s.brake_temp_bl = UnpackArray(EAoffset_t::vehicle_brake_temperature_fr);
	
	data = data_s;
	convertSeconds2Time();
	//PrintArray();
	TelemetryData_v.push_back(data_s);
}

float EASportsWRC::UnpackArray(EAoffset_t offset)
{
	// data use little endian
	float data;
	std::memcpy(&data, UDPReceiveArray.data() + to_underlying(offset), 4);
	return data;
}

double EASportsWRC::dUnpackArray(EAoffset_t offset)
{
	// data use little endian
	double data;
	std::memcpy(&data, UDPReceiveArray.data() + to_underlying(offset), 8);
	return data;
}

void EASportsWRC::AddtoCircularBuf(EAtelemetry_data_t data_s)
{
	if (EAcircularBuff_s.currentIndex >= CircularBufferMaxSize)
	{
		EAcircularBuff_s.currentIndex = 0;
		EAcircularBuff_s.circularBuffer[EAcircularBuff_s.currentIndex] = data_s;
	}
	else
	{
		EAcircularBuff_s.circularBuffer[EAcircularBuff_s.currentIndex] = data_s;
		EAcircularBuff_s.currentIndex++;
	}
}

/***

Stage Handlers

***/

void EASportsWRC::StartStage()
{
	if (!OnStage_b) OnStage_b = true;
	std::cout << "Start Stage" << std::endl;
}

bool EASportsWRC::GetOnStage()
{
	return OnStage_b;
}

void EASportsWRC::SetOnStage(bool value)
{
	OnStage_b = value;
}

/***

Utilities

***/

void EASportsWRC::PrintArray()
{
	//std::cout << "Speed    : " << (data.speed*3.6) << "\n" << std::flush;
	//std::cout << "Gear     : " << data.gear     << "\n" << std::flush;
	//std::cout << "Stear    : " << data.stear    << "\n" << std::flush;
	//std::cout << "Clutch   : " << data.clutch   << "\n" << std::flush;
	//std::cout << "Brake    : " << data.brake    << "\n" << std::flush;
	//std::cout << "Throttle : " << data.throttle << "\n" << std::flush;
	//std::cout << "RPM      : " << data.rpm      << "\n" << std::flush;
	//std::cout << "max_rpm  : " << data.max_rpm  << "\n" << std::flush;
	//std::cout << "Tracj Distance: " << data.track_length<< "\n" << std::flush;
	//std::cout << "Handbrake: " << data.handbrake<< "\n" << std::flush;
	//std::cout << "game total time: " << data.game_total_time<< "\n" << std::flush;
	//std::cout << "game_delta_time : " << data.game_delta_time<< "\n" << std::flush;
	//std::cout << "game_frame_count: " << data.game_frame_count<< "\n" << std::flush;
	//std::cout << "stage_current_time: " << data.current_time << "\n" << std::flush;
	//std::cout << "stage_current_distance: " << data.lap_distance << "\n" << std::flush;
	std::cout << "stage_length: " << data.track_length << "\n" << std::flush;

}

/***

Extra calculations

***/

void EASportsWRC::convertSeconds2Time()
{
	//float current_time = data.current_time;
	data.current_seconds = std::fmod(data.current_time, 60);
	data.current_minutes = data.current_time / 60;
	//data.current_seconds = data.current_time % 60;
}