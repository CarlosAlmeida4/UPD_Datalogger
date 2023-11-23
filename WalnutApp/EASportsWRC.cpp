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
	//data.stear = UnpackArray(EAoffset_t::INPUT_STEER);
	//data.clutch = UnpackArray(EAoffset_t::INPUT_CLUTCH);
	//data.brake = UnpackArray(EAoffset_t::INPUT_BRAKE);
	//data.throttle = UnpackArray(EAoffset_t::INPUT_THROTTLE);
	//data.rpm = UnpackArray(EAoffset_t::ENGINE_RATE) * 10;
	//data.max_rpm = UnpackArray(EAoffset_t::MAX_RPM) * 10;
	//data.track_length = UnpackArray(EAoffset_t::TRACK_LENGTH);
	//data.lap_distance = UnpackArray(EAoffset_t::DISTANCE_LAP);
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
	std::cout << "Speed    : " << (data.speed*3.6) << "\n" << std::flush;
	//std::cout << "Gear     : " << data.gear     << "\n" << std::flush;
	//std::cout << "Stear    : " << data.stear    << "\n" << std::flush;
	//std::cout << "Clutch   : " << data.clutch   << "\n" << std::flush;
	//std::cout << "Brake    : " << data.brake    << "\n" << std::flush;
	//std::cout << "Throttle : " << data.throttle << "\n" << std::flush;
	//std::cout << "RPM      : " << data.rpm      << "\n" << std::flush;
	//std::cout << "max_rpm  : " << data.max_rpm  << "\n" << std::flush;
}