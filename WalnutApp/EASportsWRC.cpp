#include "EASportsWRC.h"
#include <iostream>


#define GET_VAR_NAME(VARIABLE) (#VARIABLE)

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
	data_s.gear = (int)bUnpackArray(EAoffset_t::vehicle_gear_index);
	
	data_s.VehSpeed = UnpackArray(EAoffset_t::vehicle_speed);
	data_s.stear = UnpackArray(EAoffset_t::vehicle_steering);
	data_s.clutch = UnpackArray(EAoffset_t::vehicle_clutch);
	data_s.brake = UnpackArray(EAoffset_t::vehicle_brake);
	data_s.throttle = UnpackArray(EAoffset_t::vehicle_throttle);
	data_s.rpm = UnpackArray(EAoffset_t::vehicle_engine_rpm_current);
	data_s.max_rpm = UnpackArray(EAoffset_t::vehicle_engine_rpm_max);
	data_s.current_time = UnpackArray(EAoffset_t::stage_current_time);
	data_s.handbrake = UnpackArray(EAoffset_t::vehicle_handbrake);
	data_s.game_total_time = UnpackArray(EAoffset_t::game_total_time);
	data_s.game_delta_time = UnpackArray(EAoffset_t::game_delta_time);
	//TODO: Not stored properly
	data_s.game_frame_count = UnpackArray(EAoffset_t::game_frame_count);
	data_s.brake_temp_br = UnpackArray(EAoffset_t::vehicle_brake_temperature_bl);
	data_s.brake_temp_fl = UnpackArray(EAoffset_t::vehicle_brake_temperature_br);
	data_s.brake_temp_fr = UnpackArray(EAoffset_t::vehicle_brake_temperature_fl);
	data_s.brake_temp_bl = UnpackArray(EAoffset_t::vehicle_brake_temperature_fr);
	
	data_s.track_length = dUnpackArray(EAoffset_t::stage_length);
	data_s.lap_distance = dUnpackArray(EAoffset_t::stage_current_distance);
	
	data = data_s;
	convertSeconds2Time();
	PrintArray();
	
	TelemetryData_v.gear.push_back(data_s.gear);
	
	TelemetryData_v.VehSpeed.push_back(data_s.VehSpeed);
	TelemetryData_v.stear.push_back(data_s.stear);
	TelemetryData_v.clutch.push_back(data_s.clutch);
	TelemetryData_v.brake.push_back(data_s.brake);
	TelemetryData_v.throttle.push_back(data_s.throttle);
	TelemetryData_v.rpm.push_back(data_s.rpm);
	TelemetryData_v.max_rpm.push_back(data_s.max_rpm);
	TelemetryData_v.handbrake.push_back(data_s.handbrake);
	TelemetryData_v.current_time.push_back(data_s.current_time);
	TelemetryData_v.current_minutes.push_back(data_s.current_minutes);
	TelemetryData_v.current_seconds.push_back(data_s.current_seconds);
	TelemetryData_v.game_total_time.push_back(data_s.game_total_time);
	TelemetryData_v.game_delta_time.push_back(data_s.game_delta_time);
	TelemetryData_v.game_frame_count.push_back(data_s.game_frame_count);
	TelemetryData_v.brake_temp_br.push_back(data_s.brake_temp_br);
	TelemetryData_v.brake_temp_fl.push_back(data_s.brake_temp_fl);
	TelemetryData_v.brake_temp_fr.push_back(data_s.brake_temp_fr);
	TelemetryData_v.brake_temp_bl.push_back(data_s.brake_temp_bl);
	
	TelemetryData_v.track_length.push_back(data_s.track_length);
	TelemetryData_v.lap_distance.push_back(data_s.lap_distance);


}

void EASportsWRC::StoreVector()
{
	YAML::Emitter out;

	EAtelemetrybyteMap_t EAtelemetrybyteMap;
	EAtelemetryfloatMap_t EAtelemetryfloatMap;
	EAtelemetrydoubleMap_t EAtelemetrydoubleMap;

	EAtelemetrybyteMap["Gear"]				= TelemetryData_v.gear						;
	
	EAtelemetryfloatMap["Vehicle Speed"]	= TelemetryData_v.VehSpeed;
	EAtelemetryfloatMap["Stear"]			= TelemetryData_v.stear						;
	EAtelemetryfloatMap["Clutch"]			= TelemetryData_v.clutch					;
	EAtelemetryfloatMap["Brake"]			= TelemetryData_v.brake						;
	EAtelemetryfloatMap["Throttle"]			= TelemetryData_v.throttle					;
	EAtelemetryfloatMap["RPM"]				= TelemetryData_v.rpm						;
	EAtelemetryfloatMap["Max RPM"]			= TelemetryData_v.max_rpm					;
	EAtelemetryfloatMap["Handbrake"]		= TelemetryData_v.handbrake					;
	EAtelemetryfloatMap["Current time"]		= TelemetryData_v.current_time				;
	EAtelemetryfloatMap["Current minutes"]	= TelemetryData_v.current_minutes			;
	EAtelemetryfloatMap["Current seconds"]	= TelemetryData_v.current_seconds			;
	EAtelemetryfloatMap["Game total time"]	= TelemetryData_v.game_total_time			;
	EAtelemetryfloatMap["Game delta time"]	= TelemetryData_v.game_delta_time			;
	EAtelemetryfloatMap["Brake Temp BR"]	= TelemetryData_v.brake_temp_br				;
	EAtelemetryfloatMap["Brake Temp FL"]	= TelemetryData_v.brake_temp_fl				;
	EAtelemetryfloatMap["Brake Temp FR"]	= TelemetryData_v.brake_temp_fr				;
	EAtelemetryfloatMap["Brake Temp BL"]	= TelemetryData_v.brake_temp_bl				;
	
	EAtelemetrydoubleMap["Track Length"]	= TelemetryData_v.track_length				;
	EAtelemetrydoubleMap["Lap distance"]	= TelemetryData_v.lap_distance				;

	out << YAML::BeginMap;
	//print all bytes
	EAtelemetrybyteMap_t::iterator itrb;
	for (itrb = EAtelemetrybyteMap.begin(); itrb != EAtelemetrybyteMap.end(); itrb++)
	{
		out << YAML::Key << itrb->first;
		out << YAML::Flow << itrb->second;
	}
	//print all floats
	EAtelemetryfloatMap_t::iterator itrf;
	for (itrf = EAtelemetryfloatMap.begin(); itrf != EAtelemetryfloatMap.end(); itrf++)
	{
		out << YAML::Key << itrf->first;
		out << YAML::Flow << itrf->second;
	}
	
	//print all doubles
	EAtelemetrydoubleMap_t::iterator itrd;
	for (itrd = EAtelemetrydoubleMap.begin(); itrd != EAtelemetrydoubleMap.end(); itrd++)
	{
		out << YAML::Key << itrd->first;
		out << YAML::Flow << itrd->second;
	}
	out << YAML::EndMap;

	std::ofstream fout("test.yaml");
	fout << out.c_str();
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

uint8_t EASportsWRC::bUnpackArray(EAoffset_t offset)
{
	// data use little endian
	uint8_t data;
	std::memcpy(&data, UDPReceiveArray.data() + to_underlying(offset), 1);
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
	std::cout << "Gear     : " << data.gear     << "\n" << std::flush;
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
	//std::cout << "stage_length: " << data.track_length << "\n" << std::flush;

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