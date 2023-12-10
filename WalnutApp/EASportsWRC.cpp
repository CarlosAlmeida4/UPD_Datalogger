#include "EASportsWRC.h"
#include <iostream>


template <typename E>
constexpr auto to_underlying(E e) noexcept {
	return static_cast<std::underlying_type_t<E>>(e);
}


void EASportsWRC::HandleArray()
{
	data.speed = UnpackArray(EAoffset_t::vehicle_speed);
	data.gear = UnpackArray(EAoffset_t::vehicle_gear_index);
	data.stear = UnpackArray(EAoffset_t::vehicle_steering);
	data.clutch = UnpackArray(EAoffset_t::vehicle_clutch);
	data.brake = UnpackArray(EAoffset_t::vehicle_brake);
	data.throttle = UnpackArray(EAoffset_t::vehicle_throttle);
	data.rpm = UnpackArray(EAoffset_t::vehicle_engine_rpm_current);
	data.max_rpm = UnpackArray(EAoffset_t::vehicle_engine_rpm_max);
	data.track_length = dUnpackArray(EAoffset_t::stage_length);
	data.lap_distance = dUnpackArray(EAoffset_t::stage_current_distance);
	data.current_time = UnpackArray(EAoffset_t::stage_current_time);
	data.handbrake = UnpackArray(EAoffset_t::vehicle_handbrake);
	data.game_total_time = UnpackArray(EAoffset_t::game_total_time);
	data.game_delta_time = UnpackArray(EAoffset_t::game_delta_time);
	data.game_frame_count = UnpackArray(EAoffset_t::game_frame_count);

	//PrintArray();
}

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