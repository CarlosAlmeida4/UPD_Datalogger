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
	
	data_s.shiftlightFraction = UnpackArray(EAoffset_t::shiftlights_fraction);
	data_s.shiftlightStart = UnpackArray(EAoffset_t::shiftlights_rpm_start);
	data_s.shiftlightEnd = UnpackArray(EAoffset_t::shiftlights_rpm_end);
	data_s.VehSpeed = UnpackArray(EAoffset_t::vehicle_speed);
	data_s.VehTransSpeed = UnpackArray(EAoffset_t::vehicle_transmission_speed);
	data_s.VehPosX = UnpackArray(EAoffset_t::vehicle_position_x);
	data_s.VehPosY = UnpackArray(EAoffset_t::vehicle_position_y);
	data_s.VehPosZ = UnpackArray(EAoffset_t::vehicle_position_z);
	data_s.VehSpdX = UnpackArray(EAoffset_t::vehicle_velocity_x);
	data_s.VehSpdY = UnpackArray(EAoffset_t::vehicle_velocity_y);
	data_s.VehSpdZ = UnpackArray(EAoffset_t::vehicle_velocity_z);
	data_s.VehAccelX = UnpackArray(EAoffset_t::vehicle_acceleration_x);
	data_s.VehAccelY = UnpackArray(EAoffset_t::vehicle_acceleration_y);
	data_s.VehAccelZ = UnpackArray(EAoffset_t::vehicle_acceleration_z);
	data_s.VehLeftDirX = UnpackArray(EAoffset_t::vehicle_left_direction_x);
	data_s.VehLeftDirY = UnpackArray(EAoffset_t::vehicle_left_direction_y);
	data_s.VehLeftDirZ = UnpackArray(EAoffset_t::vehicle_left_direction_z);
	data_s.VehFwrdDirX = UnpackArray(EAoffset_t::vehicle_forward_direction_x);
	data_s.VehFwrdDirY = UnpackArray(EAoffset_t::vehicle_forward_direction_y);
	data_s.VehFwrdDirZ = UnpackArray(EAoffset_t::vehicle_forward_direction_z);
	data_s.VehUpDirX = UnpackArray(EAoffset_t::vehicle_up_direction_x);
	data_s.VehUpDirY = UnpackArray(EAoffset_t::vehicle_up_direction_y);
	data_s.VehUpDirZ = UnpackArray(EAoffset_t::vehicle_up_direction_z);
	data_s.HubVertPosBL = UnpackArray(EAoffset_t::vehicle_hub_position_bl);
	data_s.HubVertPosBR = UnpackArray(EAoffset_t::vehicle_hub_position_br);
	data_s.HubVertPosFL = UnpackArray(EAoffset_t::vehicle_hub_position_fl);
	data_s.HubVertPosFR = UnpackArray(EAoffset_t::vehicle_hub_position_fr);
	data_s.HubVertSpdBL = UnpackArray(EAoffset_t::vehicle_hub_velocity_bl);
	data_s.HubVertSpdBR = UnpackArray(EAoffset_t::vehicle_hub_velocity_br);
	data_s.HubVertSpdFL = UnpackArray(EAoffset_t::vehicle_hub_velocity_fl);
	data_s.HubVertSpdFR = UnpackArray(EAoffset_t::vehicle_hub_velocity_fr);
	data_s.WheelSpeedBL = UnpackArray(EAoffset_t::vehicle_cp_forward_speed_bl);
	data_s.WheelSpeedBR = UnpackArray(EAoffset_t::vehicle_cp_forward_speed_br);
	data_s.WheelSpeedFL = UnpackArray(EAoffset_t::vehicle_cp_forward_speed_fl);
	data_s.WheelSpeedFR = UnpackArray(EAoffset_t::vehicle_cp_forward_speed_fr);
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
	data_s.game_frame_count = UnpackArray(EAoffset_t::game_frame_count); //TODO: Not stored properly
	data_s.brake_temp_br = UnpackArray(EAoffset_t::vehicle_brake_temperature_br);
	data_s.brake_temp_fl = UnpackArray(EAoffset_t::vehicle_brake_temperature_fl);
	data_s.brake_temp_fr = UnpackArray(EAoffset_t::vehicle_brake_temperature_fr);
	data_s.brake_temp_bl = UnpackArray(EAoffset_t::vehicle_brake_temperature_bl);
	
	data_s.track_length = dUnpackArray(EAoffset_t::stage_length);
	data_s.lap_distance = dUnpackArray(EAoffset_t::stage_current_distance);
	
	data = data_s;
	convertSeconds2Time();
	//PrintArray();
	
	TelemetryData_v.gear.push_back(data_s.gear);
	
	TelemetryData_v.shiftlightFraction.push_back(data_s.shiftlightFraction);
	TelemetryData_v.shiftlightStart.push_back(data_s.shiftlightStart);
	TelemetryData_v.shiftlightEnd.push_back(data_s.shiftlightEnd);
	TelemetryData_v.VehSpeed.push_back(data_s.VehSpeed);
	TelemetryData_v.VehTransSpeed.push_back(data_s.VehTransSpeed);
	TelemetryData_v.VehPosX.push_back(data_s.VehPosX);
	TelemetryData_v.VehPosY.push_back(data_s.VehPosY);
	TelemetryData_v.VehPosZ.push_back(data_s.VehPosZ);
	TelemetryData_v.VehSpdX.push_back(data_s.VehSpdX);
	TelemetryData_v.VehSpdY.push_back(data_s.VehSpdY);
	TelemetryData_v.VehSpdZ.push_back(data_s.VehSpdZ);
	TelemetryData_v.VehAccelX.push_back(data_s.VehAccelX);
	TelemetryData_v.VehAccelY.push_back(data_s.VehAccelY);
	TelemetryData_v.VehAccelZ.push_back(data_s.VehAccelZ);
	TelemetryData_v.VehLeftDirX.push_back(data_s.VehLeftDirX);
	TelemetryData_v.VehLeftDirY.push_back(data_s.VehLeftDirY);
	TelemetryData_v.VehLeftDirZ.push_back(data_s.VehLeftDirZ);
	TelemetryData_v.VehFwrdDirX.push_back(data_s.VehFwrdDirX);
	TelemetryData_v.VehFwrdDirY.push_back(data_s.VehFwrdDirY);
	TelemetryData_v.VehFwrdDirZ.push_back(data_s.VehFwrdDirZ);
	TelemetryData_v.VehUpDirX.push_back(data_s.VehUpDirX);
	TelemetryData_v.VehUpDirY.push_back(data_s.VehUpDirY);
	TelemetryData_v.VehUpDirZ.push_back(data_s.VehUpDirZ);
	TelemetryData_v.HubVertPosBL.push_back(data_s.HubVertPosBL);
	TelemetryData_v.HubVertPosBR.push_back(data_s.HubVertPosBR);
	TelemetryData_v.HubVertPosFL.push_back(data_s.HubVertPosFL);
	TelemetryData_v.HubVertPosFR.push_back(data_s.HubVertPosFR);
	TelemetryData_v.HubVertSpdBL.push_back(data_s.HubVertSpdBL);
	TelemetryData_v.HubVertSpdBR.push_back(data_s.HubVertSpdBR);
	TelemetryData_v.HubVertSpdFL.push_back(data_s.HubVertSpdFL);
	TelemetryData_v.HubVertSpdFR.push_back(data_s.HubVertSpdFR);
	TelemetryData_v.WheelSpeedBL.push_back(data_s.WheelSpeedBL);
	TelemetryData_v.WheelSpeedBR.push_back(data_s.WheelSpeedBR);
	TelemetryData_v.WheelSpeedFL.push_back(data_s.WheelSpeedFL);
	TelemetryData_v.WheelSpeedFR.push_back(data_s.WheelSpeedFR);
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

void EASportsWRC::StoreVector(const std::filesystem::path& filepath)
{
	SetOnStage(false);

	YAML::Emitter out;
	
	EAtelemetrybyteMap_t EAtelemetrybyteMap;
	EAtelemetryfloatMap_t EAtelemetryfloatMap;
	EAtelemetrydoubleMap_t EAtelemetrydoubleMap;

	EAtelemetrybyteMap["Gear"]						= TelemetryData_v.gear							;
	EAtelemetryfloatMap["shiftLightFraction"]		= TelemetryData_v.shiftlightFraction			;
	EAtelemetryfloatMap["shiftLightStart"]			= TelemetryData_v.shiftlightStart				;
	EAtelemetryfloatMap["shiftLightEnd"]			= TelemetryData_v.shiftlightEnd					;
	EAtelemetryfloatMap["Vehicle Speed"]			= TelemetryData_v.VehSpeed						;
	EAtelemetryfloatMap["Vehicle Trans Speed"]		= TelemetryData_v.VehTransSpeed					;
	EAtelemetryfloatMap["Vehicle Position X"]		= TelemetryData_v.VehPosX						;
	EAtelemetryfloatMap["Vehicle Position Y"]		= TelemetryData_v.VehPosY						;
	EAtelemetryfloatMap["Vehicle Position Z"]		= TelemetryData_v.VehPosZ						;
	EAtelemetryfloatMap["Vehicle Speed X"]			= TelemetryData_v.VehSpdX						;
	EAtelemetryfloatMap["Vehicle Speed Y"]			= TelemetryData_v.VehSpdY						;
	EAtelemetryfloatMap["Vehicle Speed Z"]			= TelemetryData_v.VehSpdZ						;
	EAtelemetryfloatMap["Vehicle Accel X"]			= TelemetryData_v.VehAccelX						;
	EAtelemetryfloatMap["Vehicle Accel Y"]			= TelemetryData_v.VehAccelY						;
	EAtelemetryfloatMap["Vehicle Accel Z"]			= TelemetryData_v.VehAccelZ						;
	EAtelemetryfloatMap["Vehicle Left Dir X"]		= TelemetryData_v.VehLeftDirX					;
	EAtelemetryfloatMap["Vehicle Left Dir Y"]		= TelemetryData_v.VehLeftDirY					;
	EAtelemetryfloatMap["Vehicle Left Dir Z"]		= TelemetryData_v.VehLeftDirZ					;
	EAtelemetryfloatMap["Vehicle Fwrd Dir X"]		= TelemetryData_v.VehFwrdDirX					;
	EAtelemetryfloatMap["Vehicle Fwrd Dir Y"]		= TelemetryData_v.VehFwrdDirY					;
	EAtelemetryfloatMap["Vehicle Fwrd Dir Z"]		= TelemetryData_v.VehFwrdDirZ					;
	EAtelemetryfloatMap["Vehicle Up Dir X"]			= TelemetryData_v.VehUpDirX						;
	EAtelemetryfloatMap["Vehicle Up Dir Y"]			= TelemetryData_v.VehUpDirY						;
	EAtelemetryfloatMap["Vehicle Up Dir Z"]			= TelemetryData_v.VehUpDirZ						;
	EAtelemetryfloatMap["Hub Vertical Position BL"] = TelemetryData_v.HubVertPosBL					;
	EAtelemetryfloatMap["Hub Vertical Position BR"] = TelemetryData_v.HubVertPosBR					;
	EAtelemetryfloatMap["Hub Vertical Position FL"] = TelemetryData_v.HubVertPosFL					;
	EAtelemetryfloatMap["Hub Vertical Position FR"] = TelemetryData_v.HubVertPosFR					;
	EAtelemetryfloatMap["Hub Vertical Velocity BL"] = TelemetryData_v.HubVertSpdBL					;
	EAtelemetryfloatMap["Hub Vertical Velocity BR"] = TelemetryData_v.HubVertSpdBR					;
	EAtelemetryfloatMap["Hub Vertical Velocity FL"] = TelemetryData_v.HubVertSpdFL					;
	EAtelemetryfloatMap["Hub Vertical Velocity FR"] = TelemetryData_v.HubVertSpdFR					;
	EAtelemetryfloatMap["Wheel Speed BL"]			= TelemetryData_v.WheelSpeedBL					;
	EAtelemetryfloatMap["Wheel Speed BR"]			= TelemetryData_v.WheelSpeedBR					;
	EAtelemetryfloatMap["Wheel Speed FL"]			= TelemetryData_v.WheelSpeedFL					;
	EAtelemetryfloatMap["Wheel Speed FR"]			= TelemetryData_v.WheelSpeedFR					;
	EAtelemetryfloatMap["Stear"]					= TelemetryData_v.stear							;
	EAtelemetryfloatMap["Clutch"]					= TelemetryData_v.clutch						;
	EAtelemetryfloatMap["Brake"]					= TelemetryData_v.brake							;
	EAtelemetryfloatMap["Throttle"]					= TelemetryData_v.throttle						;
	EAtelemetryfloatMap["RPM"]						= TelemetryData_v.rpm							;
	EAtelemetryfloatMap["Handbrake"]				= TelemetryData_v.handbrake						;
	EAtelemetryfloatMap["Current time"]				= TelemetryData_v.current_time					;
	EAtelemetryfloatMap["Current minutes"]			= TelemetryData_v.current_minutes				;
	EAtelemetryfloatMap["Current seconds"]			= TelemetryData_v.current_seconds				;
	EAtelemetryfloatMap["Game total time"]			= TelemetryData_v.game_total_time				;
	EAtelemetryfloatMap["Game delta time"]			= TelemetryData_v.game_delta_time				;
	EAtelemetryfloatMap["Brake Temp BR"]			= TelemetryData_v.brake_temp_br					;
	EAtelemetryfloatMap["Brake Temp FL"]			= TelemetryData_v.brake_temp_fl					;
	EAtelemetryfloatMap["Brake Temp FR"]			= TelemetryData_v.brake_temp_fr					;
	EAtelemetryfloatMap["Brake Temp BL"]			= TelemetryData_v.brake_temp_bl					;
	
	EAtelemetrydoubleMap["Track Length"]			= TelemetryData_v.track_length					;
	EAtelemetrydoubleMap["Lap distance"]			= TelemetryData_v.lap_distance					;

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

	std::ofstream fout(filepath);
	fout << out.c_str();
	
	ClearArray();

	//Clear all values from vectors run
	/*
	TelemetryData_v.gear.clear();

	TelemetryData_v.VehSpeed.clear();
	TelemetryData_v.VehTransSpeed.clear();
	TelemetryData_v.VehPosX.clear();
	TelemetryData_v.VehPosY.clear();
	TelemetryData_v.VehPosZ.clear();
	TelemetryData_v.VehSpdX.clear();
	TelemetryData_v.VehSpdY.clear();
	TelemetryData_v.VehSpdZ.clear();
	TelemetryData_v.VehAccelX.clear();
	TelemetryData_v.VehAccelY.clear();
	TelemetryData_v.VehAccelZ.clear();
	TelemetryData_v.VehLeftDirX.clear();
	TelemetryData_v.VehLeftDirY.clear();
	TelemetryData_v.VehLeftDirZ.clear();
	TelemetryData_v.VehFwrdDirX.clear();
	TelemetryData_v.VehFwrdDirY.clear();
	TelemetryData_v.VehFwrdDirZ.clear();
	TelemetryData_v.VehUpDirX.clear();
	TelemetryData_v.VehUpDirY.clear();
	TelemetryData_v.VehUpDirZ.clear();
	TelemetryData_v.HubVertPosBL.clear();
	TelemetryData_v.HubVertPosBR.clear();
	TelemetryData_v.HubVertPosFL.clear();
	TelemetryData_v.HubVertPosFR.clear();
	TelemetryData_v.HubVertSpdBL.clear();
	TelemetryData_v.HubVertSpdBR.clear();
	TelemetryData_v.HubVertSpdFL.clear();
	TelemetryData_v.HubVertSpdFR.clear();
	TelemetryData_v.WheelSpeedBL.clear();
	TelemetryData_v.WheelSpeedBR.clear();
	TelemetryData_v.WheelSpeedFL.clear();
	TelemetryData_v.WheelSpeedFR.clear();
	TelemetryData_v.stear.clear();
	TelemetryData_v.clutch.clear();
	TelemetryData_v.brake.clear();
	TelemetryData_v.throttle.clear();
	TelemetryData_v.rpm.clear();
	TelemetryData_v.handbrake.clear();
	TelemetryData_v.current_time.clear();
	TelemetryData_v.current_minutes.clear();
	TelemetryData_v.current_seconds.clear();
	TelemetryData_v.game_total_time.clear();
	TelemetryData_v.game_delta_time.clear();
	TelemetryData_v.brake_temp_br.clear();
	TelemetryData_v.brake_temp_fl.clear();
	TelemetryData_v.brake_temp_fr.clear();
	TelemetryData_v.brake_temp_bl.clear();

	TelemetryData_v.track_length.clear();
	TelemetryData_v.lap_distance.clear();
	*/

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

void EASportsWRC::ClearArray()
{
	SetOnStage(false);
	//Clear all values from vectors run
	TelemetryData_v.gear.clear();						
	TelemetryData_v.shiftlightFraction.clear();
	TelemetryData_v.shiftlightStart.clear();
	TelemetryData_v.shiftlightEnd.clear();
	TelemetryData_v.VehSpeed.clear();
	TelemetryData_v.VehTransSpeed.clear();
	TelemetryData_v.VehPosX.clear();
	TelemetryData_v.VehPosY.clear();
	TelemetryData_v.VehPosZ.clear();
	TelemetryData_v.VehSpdX.clear();
	TelemetryData_v.VehSpdY.clear();
	TelemetryData_v.VehSpdZ.clear();
	TelemetryData_v.VehAccelX.clear();
	TelemetryData_v.VehAccelY.clear();
	TelemetryData_v.VehAccelZ.clear();
	TelemetryData_v.VehLeftDirX.clear();
	TelemetryData_v.VehLeftDirY.clear();
	TelemetryData_v.VehLeftDirZ.clear();
	TelemetryData_v.VehFwrdDirX.clear();
	TelemetryData_v.VehFwrdDirY.clear();
	TelemetryData_v.VehFwrdDirZ.clear();
	TelemetryData_v.VehUpDirX.clear();
	TelemetryData_v.VehUpDirY.clear();
	TelemetryData_v.VehUpDirZ.clear();
	TelemetryData_v.HubVertPosBL.clear();
	TelemetryData_v.HubVertPosBR.clear();
	TelemetryData_v.HubVertPosFL.clear();
	TelemetryData_v.HubVertPosFR.clear();
	TelemetryData_v.HubVertSpdBL.clear();
	TelemetryData_v.HubVertSpdBR.clear();
	TelemetryData_v.HubVertSpdFL.clear();
	TelemetryData_v.HubVertSpdFR.clear();
	TelemetryData_v.WheelSpeedBL.clear();
	TelemetryData_v.WheelSpeedBR.clear();
	TelemetryData_v.WheelSpeedFL.clear();
	TelemetryData_v.WheelSpeedFR.clear();
	TelemetryData_v.stear.clear();
	TelemetryData_v.clutch.clear();
	TelemetryData_v.brake.clear();
	TelemetryData_v.throttle.clear();
	TelemetryData_v.rpm.clear();
	TelemetryData_v.handbrake.clear();
	TelemetryData_v.current_time.clear();
	TelemetryData_v.current_minutes.clear();
	TelemetryData_v.current_seconds.clear();
	TelemetryData_v.game_total_time.clear();
	TelemetryData_v.game_delta_time.clear();
	TelemetryData_v.brake_temp_br.clear();
	TelemetryData_v.brake_temp_fl.clear();
	TelemetryData_v.brake_temp_fr.clear();
	TelemetryData_v.brake_temp_bl.clear();
	
	TelemetryData_v.track_length.clear();
	TelemetryData_v.lap_distance.clear();
}

bool EASportsWRC::GenerateMap()
{
	EAtelemetryMap_t l_EAtelemetryMap;
	if (!TelemetryData_v.gear.empty())
	{
		l_EAtelemetryMap.EAtelemetrybyteMap["Gear"] = TelemetryData_v.gear;
		l_EAtelemetryMap.EAtelemetryfloatMap["shiftLightFraction"] = TelemetryData_v.shiftlightFraction;
		l_EAtelemetryMap.EAtelemetryfloatMap["shiftLightStart"] = TelemetryData_v.shiftlightStart;
		l_EAtelemetryMap.EAtelemetryfloatMap["shiftLightEnd"] = TelemetryData_v.shiftlightEnd;
		l_EAtelemetryMap.EAtelemetryfloatMap["Vehicle Speed"] = TelemetryData_v.VehSpeed;
		l_EAtelemetryMap.EAtelemetryfloatMap["Vehicle Trans Speed"] = TelemetryData_v.VehTransSpeed;
		l_EAtelemetryMap.EAtelemetryfloatMap["Vehicle Position X"] = TelemetryData_v.VehPosX;
		l_EAtelemetryMap.EAtelemetryfloatMap["Vehicle Position Y"] = TelemetryData_v.VehPosY;
		l_EAtelemetryMap.EAtelemetryfloatMap["Vehicle Position Z"] = TelemetryData_v.VehPosZ;
		l_EAtelemetryMap.EAtelemetryfloatMap["Vehicle Speed X"] = TelemetryData_v.VehSpdX;
		l_EAtelemetryMap.EAtelemetryfloatMap["Vehicle Speed Y"] = TelemetryData_v.VehSpdY;
		l_EAtelemetryMap.EAtelemetryfloatMap["Vehicle Speed Z"] = TelemetryData_v.VehSpdZ;
		l_EAtelemetryMap.EAtelemetryfloatMap["Vehicle Accel X"] = TelemetryData_v.VehAccelX;
		l_EAtelemetryMap.EAtelemetryfloatMap["Vehicle Accel Y"] = TelemetryData_v.VehAccelY;
		l_EAtelemetryMap.EAtelemetryfloatMap["Vehicle Accel Z"] = TelemetryData_v.VehAccelZ;
		l_EAtelemetryMap.EAtelemetryfloatMap["Vehicle Left Dir X"] = TelemetryData_v.VehLeftDirX;
		l_EAtelemetryMap.EAtelemetryfloatMap["Vehicle Left Dir Y"] = TelemetryData_v.VehLeftDirY;
		l_EAtelemetryMap.EAtelemetryfloatMap["Vehicle Left Dir Z"] = TelemetryData_v.VehLeftDirZ;
		l_EAtelemetryMap.EAtelemetryfloatMap["Vehicle Fwrd Dir X"] = TelemetryData_v.VehFwrdDirX;
		l_EAtelemetryMap.EAtelemetryfloatMap["Vehicle Fwrd Dir Y"] = TelemetryData_v.VehFwrdDirY;
		l_EAtelemetryMap.EAtelemetryfloatMap["Vehicle Fwrd Dir Z"] = TelemetryData_v.VehFwrdDirZ;
		l_EAtelemetryMap.EAtelemetryfloatMap["Vehicle Up Dir X"] = TelemetryData_v.VehUpDirX;
		l_EAtelemetryMap.EAtelemetryfloatMap["Vehicle Up Dir Y"] = TelemetryData_v.VehUpDirY;
		l_EAtelemetryMap.EAtelemetryfloatMap["Vehicle Up Dir Z"] = TelemetryData_v.VehUpDirZ;
		l_EAtelemetryMap.EAtelemetryfloatMap["Hub Vertical Position BL"] = TelemetryData_v.HubVertPosBL;
		l_EAtelemetryMap.EAtelemetryfloatMap["Hub Vertical Position BR"] = TelemetryData_v.HubVertPosBR;
		l_EAtelemetryMap.EAtelemetryfloatMap["Hub Vertical Position FL"] = TelemetryData_v.HubVertPosFL;
		l_EAtelemetryMap.EAtelemetryfloatMap["Hub Vertical Position FR"] = TelemetryData_v.HubVertPosFR;
		l_EAtelemetryMap.EAtelemetryfloatMap["Hub Vertical Velocity BL"] = TelemetryData_v.HubVertSpdBL;
		l_EAtelemetryMap.EAtelemetryfloatMap["Hub Vertical Velocity BR"] = TelemetryData_v.HubVertSpdBR;
		l_EAtelemetryMap.EAtelemetryfloatMap["Hub Vertical Velocity FL"] = TelemetryData_v.HubVertSpdFL;
		l_EAtelemetryMap.EAtelemetryfloatMap["Hub Vertical Velocity FR"] = TelemetryData_v.HubVertSpdFR;
		l_EAtelemetryMap.EAtelemetryfloatMap["Wheel Speed BL"] = TelemetryData_v.WheelSpeedBL;
		l_EAtelemetryMap.EAtelemetryfloatMap["Wheel Speed BR"] = TelemetryData_v.WheelSpeedBR;
		l_EAtelemetryMap.EAtelemetryfloatMap["Wheel Speed FL"] = TelemetryData_v.WheelSpeedFL;
		l_EAtelemetryMap.EAtelemetryfloatMap["Wheel Speed FR"] = TelemetryData_v.WheelSpeedFR;
		l_EAtelemetryMap.EAtelemetryfloatMap["Stear"] = TelemetryData_v.stear;
		l_EAtelemetryMap.EAtelemetryfloatMap["Clutch"] = TelemetryData_v.clutch;
		l_EAtelemetryMap.EAtelemetryfloatMap["Brake"] = TelemetryData_v.brake;
		l_EAtelemetryMap.EAtelemetryfloatMap["Throttle"] = TelemetryData_v.throttle;
		l_EAtelemetryMap.EAtelemetryfloatMap["RPM"] = TelemetryData_v.rpm;
		l_EAtelemetryMap.EAtelemetryfloatMap["Handbrake"] = TelemetryData_v.handbrake;
		l_EAtelemetryMap.EAtelemetryfloatMap["Current time"] = TelemetryData_v.current_time;
		l_EAtelemetryMap.EAtelemetryfloatMap["Current minutes"] = TelemetryData_v.current_minutes;
		l_EAtelemetryMap.EAtelemetryfloatMap["Current seconds"] = TelemetryData_v.current_seconds;
		l_EAtelemetryMap.EAtelemetryfloatMap["Game total time"] = TelemetryData_v.game_total_time;
		l_EAtelemetryMap.EAtelemetryfloatMap["Game delta time"] = TelemetryData_v.game_delta_time;
		l_EAtelemetryMap.EAtelemetryfloatMap["Brake Temp BR"] = TelemetryData_v.brake_temp_br;
		l_EAtelemetryMap.EAtelemetryfloatMap["Brake Temp FL"] = TelemetryData_v.brake_temp_fl;
		l_EAtelemetryMap.EAtelemetryfloatMap["Brake Temp FR"] = TelemetryData_v.brake_temp_fr;
		l_EAtelemetryMap.EAtelemetryfloatMap["Brake Temp BL"] = TelemetryData_v.brake_temp_bl;

		l_EAtelemetryMap.EAtelemetrydoubleMap["Track Length"] = TelemetryData_v.track_length;
		l_EAtelemetryMap.EAtelemetrydoubleMap["Lap distance"] = TelemetryData_v.lap_distance;

		m_EAtelemetryMap = l_EAtelemetryMap;
		return true;
	}
	return false;
}

void EASportsWRC::GenerateMapFromYAML(const std::filesystem::path& filepath)
{
	isReadFromFile_b = true;
	YAML::Node TelemetryYAML = YAML::LoadFile(filepath.string());
	
	EAtelemetryMap_t l_EAtelemetryMap;
	//std::cout << TelemetryYAML.IsSequence() << std::endl;
	//std::cout << TelemetryYAML.IsMap() << std::endl;
	//std::cout << TelemetryYAML.IsScalar() << std::endl;
	//std::cout << TelemetryYAML.IsDefined() << std::endl;
	//std::cout << TelemetryYAML.IsNull() << std::endl;
	//for (auto it = TelemetryYAML.begin(); it != TelemetryYAML.end(); it++)
	//{
	//	YAML::Node response_values_map = it->as<YAML::Node>();
	//	//std::cout << response_values_map << std::endl;
	//}
	for (const auto& kv : TelemetryYAML) {
		//std::cout << kv.first.as<std::string>() << "\n"; // prints Foo
		//std::cout << kv.second.as<std::vector>() << "\n"; // prints Foo
		//const YAML::Node& KeyNode = kv.second;  // the value
		std::string key = kv.first.as<std::string>();
		if (key == "Gear")
		{
			l_EAtelemetryMap.EAtelemetrybyteMap[key] = kv.second.as<std::vector<int>>();

		}
		else if (key == "Track Length" || key == "Lap distance")
		{
			l_EAtelemetryMap.EAtelemetrydoubleMap[key] = kv.second.as<std::vector<double>>();
		}
		else
		{
			l_EAtelemetryMap.EAtelemetryfloatMap[key] = kv.second.as<std::vector<float>>();
		}

	}
	m_EAtelemetryMap = l_EAtelemetryMap;
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
	//std::cout << "stage_length: " << data.track_length << "\n" << std::flush;
	//std::cout << "shift fraction: " << data.shiftlightFraction<< "\n" << std::flush;
	std::cout << "shift start: " << data.shiftlightStart<< "\n" << std::flush;
	std::cout << "shift end: " << data.shiftlightEnd<< "\n" << std::flush;

}

bool EASportsWRC::GetisReadFromFile()
{
	return isReadFromFile_b;
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

/***

UDP connection

***/

int EASportsWRC::startClient()
{
	if (!isRunning_b)
	{
		// Initialize Winsock
		WSADATA wsaData;
		if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
			std::cerr << "Failed to initialize Winsock" << std::endl;
			return 1;
		}

		// Create a UDP socket
		serverSocket = socket(AF_INET, SOCK_DGRAM, 0);
		if (serverSocket == INVALID_SOCKET) {
			std::cerr << "Error creating socket" << std::endl;
			WSACleanup();
			return 1;
		}

		// Set up the server address
		sockaddr_in serverAddr;
		serverAddr.sin_family = AF_INET;
		serverAddr.sin_addr.s_addr = INADDR_ANY;
		serverAddr.sin_port = htons(PORT_i);

		// Bind the socket to the specified port
		if (bind(serverSocket, (const sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
			std::cerr << "Error binding socket" << std::endl;
			closesocket(serverSocket);
			WSACleanup();
			return 1;
		}

		std::cout << "UDP server listening on port " << PORT_i << std::endl;
		isRunning_b = true;

		m_NetworkThread = std::thread([this]() { receiveData(); });

	}

}

void EASportsWRC::stopClient()
{
	closesocket(serverSocket);
	WSACleanup();
	isRunning_b = false;
	if (m_NetworkThread.joinable())
		m_NetworkThread.join();
}

void EASportsWRC::receiveData()
{
	while (isRunning_b)
	{
		// Wait for incoming messages
		char buffer[265];
		uint8_t uintBuffer[265];
		sockaddr_in clientAddr;
		int clientAddrLen = sizeof(clientAddr);
		int bytesRead = recvfrom(serverSocket, buffer, sizeof(buffer), 0, (sockaddr*)&clientAddr, &clientAddrLen);
		//int bytesRead = recvfrom(serverSocket, uintBuffer, sizeof(uintBuffer), 0, (sockaddr*)&clientAddr, &clientAddrLen);

		if (bytesRead == SOCKET_ERROR) {
			std::cerr << "Error receiving message" << std::endl;
		}
		else {
			//buffer[bytesRead] = '\0';
			//std::cout << "Size of buffer" << bytesRead << std::endl;
			if (bytesRead <= 264) //Full size packet for Dirt Rally 2
			{
				//check if firts activation
				if (!GetOnStage()) StartStage();
				memcpy(UDPReceiveArray.data(), buffer, sizeof(buffer));
				//std::cout << "Received message from client: " << l_DirtTwo.UDPReceiveArray.data() << std::endl;
				HandleArray();
			}
			else
			{
				std::cout << "Received package with a different size " << sizeof(buffer) << std::endl;
			}
		}
	}

}

/***

Destructuor

***/

EASportsWRC::~EASportsWRC()
{
	closesocket(serverSocket);
	WSACleanup();
	isRunning_b = false;
	if (m_NetworkThread.joinable())
		m_NetworkThread.join();
}