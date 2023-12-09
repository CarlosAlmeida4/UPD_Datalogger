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
	data.track_length = UnpackArray(EAoffset_t::stage_length);
	data.lap_distance = UnpackArray(EAoffset_t::stage_current_distance);
	data.current_time = UnpackArray(EAoffset_t::stage_current_time);
	PrintArray();
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
	std::cout << "Tracj Distance: " << data.track_length<< "\n" << std::flush;

}