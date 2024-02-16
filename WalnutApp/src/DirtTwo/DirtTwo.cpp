#include "DirtTwo.h"
#include <iostream>
#include "DirtTwo.h"



template <typename E>
constexpr auto to_underlying(E e) noexcept {
	return static_cast<std::underlying_type_t<E>>(e);
}


void DirtTwo::HandleArray()
{
	data.speed = UnpackArray(offset_t::SPEED);
	data.gear = UnpackArray(offset_t::GEAR);
	data.stear = UnpackArray(offset_t::INPUT_STEER);
	data.clutch = UnpackArray(offset_t::INPUT_CLUTCH);
	data.brake = UnpackArray(offset_t::INPUT_BRAKE);
	data.throttle = UnpackArray(offset_t::INPUT_THROTTLE);
	data.rpm = UnpackArray(offset_t::ENGINE_RATE) * 10;
	data.max_rpm = UnpackArray(offset_t::MAX_RPM) * 10;
	data.track_length = UnpackArray(offset_t::TRACK_LENGTH);
	data.lap_distance = UnpackArray(offset_t::DISTANCE_LAP);
	PrintArray();
}

void DirtTwo::StartStage()
{
	if (!OnStage_b) OnStage_b = true;
	std::cout << "Start Stage" << std::endl;
}

bool DirtTwo::GetOnStage()
{
	return OnStage_b;
}

void DirtTwo::SetOnStage(bool value)
{
	OnStage_b = value;
}

float DirtTwo::UnpackArray(offset_t offset)
{
	// data use little endian
	float data;
	std::memcpy(&data, UDPReceiveArray.data() + to_underlying(offset), 4);
	return data;
}

void DirtTwo::PrintArray()
{
	std::cout << "Speed    : " << data.speed << "\n" << std::flush;
	//std::cout << "Gear     : " << data.gear     << "\n" << std::flush;
	//std::cout << "Stear    : " << data.stear    << "\n" << std::flush;
	//std::cout << "Clutch   : " << data.clutch   << "\n" << std::flush;
	//std::cout << "Brake    : " << data.brake    << "\n" << std::flush;
	//std::cout << "Throttle : " << data.throttle << "\n" << std::flush;
	//std::cout << "RPM      : " << data.rpm      << "\n" << std::flush;
	//std::cout << "max_rpm  : " << data.max_rpm  << "\n" << std::flush;
}