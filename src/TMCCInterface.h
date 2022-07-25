#ifndef _TMCC_INTERFACE_H_
#define _TMCC_INTERFACE_H_
#pragma once

#include "StdAfx.h"

typedef uint8 TMCCAddress;

// handles
struct TMCCHandle
{
private:
  TMCCAddress m_addr;

public:
  TMCCHandle()
    : m_addr{}
  {
  }

  TMCCHandle(TMCCAddress addr)
    : m_addr(addr)
  {
  }

  operator TMCCAddress& () { return m_addr; }
  //operator TMCCAddress() { return m_addr; }
};

struct EngineHandle     : public TMCCHandle { using TMCCHandle::TMCCHandle; };
struct TrainHandle      : public TMCCHandle { using TMCCHandle::TMCCHandle; };
struct SwitchHandle     : public TMCCHandle { using TMCCHandle::TMCCHandle; };
struct RouteHandle      : public TMCCHandle { using TMCCHandle::TMCCHandle; };
struct AccessoryHandle  : public TMCCHandle { using TMCCHandle::TMCCHandle; };
struct GroupHandle      : public TMCCHandle { using TMCCHandle::TMCCHandle; };

//typedef TMCCAddress EngineHandle;
//typedef TMCCAddress TrainHandle;
//typedef TMCCAddress SwitchHandle;
//typedef TMCCAddress RouteHandle;
//typedef TMCCAddress AccessoryHandle;
//typedef TMCCAddress GroupHandle;

struct TMCCCommand
{
  uint16 data;

  TMCCCommand()
    : data(0)
  {
  }
};

struct TMCC2Command
{
  uint16 data;

  TMCC2Command()
    : data(0)
  {
  }
};

enum EngineType : uint8
{
    ENGINE_STEAM = 0x0,
    ENGINE_DIESEL = 0x1,
    ENGINE_ELECTRIC = 0x2,
    ENGINE_STATIONSOUND_CAR = 0x3,
    ENGINE_SUBWAY = 0x4,
    ENGINE_STOCK_CAR = 0x5,

    ENGINE_TMCC_STEAM = 0x6,
    ENGINE_TMCC_DIESEL = 0x7,
    ENGINE_TMCC_ELECTRIC = 0x8,
    ENGINE_TMCC_CAR = 0x9,
    ENGINE_TMCC_CRANE = 0xA,
    ENGINE_TMCC_ACELA = 0xB,
    ENGINE_TMCC_BREAKDOWN_UNIT = 0xC,
};

enum ParameterIndex : uint8
{
  PI_RESERVED_1   = 0x1,
  PI_RAILSOUNDS_DIALOG_TRIGGERS = 0x2,
  PI_RESERVED_3   = 0x3,
  PI_RAILSOUNDS_EFFECT_TRIGGERS = 0x4,
  PI_RESERVED_5   = 0x5,
  PI_RAILSOUNDS_MASKING_CONTROL = 0x6,
  PI_RESERVED_7   = 0x7,
  PI_RESERVED_8   = 0x8,
  PI_RESERVED_9   = 0x9,
  PI_RESERVED_10   = 0xA,
  PI_RESERVED_11   = 0xB,
  PI_EFFECTS_CONTROLS           = 0xC,
  PI_LIGHTING_CONTROLS          = 0xD,
  PI_RESERVED_12   = 0xE,
  PI_VARIABLE_LENGTH_COMMAND    = 0xF
};

enum EngineCommand2Params : uint16
{
  // first page (9th bit low)
  EC2_SET_ABSOLUTE_SPEED_STEP_MASK = 0b011111111,

  EC2_SET_MOMENTUM_FLAG = 0b011001000,
  EC2_SET_MOMENTUM_MASK = 0b000000111,

  EC2_BRAKE_LEVEL_FLAG = 0b011100000,
  EC2_BRAKE_LEVEL_MASK = 0b000000111,

  EC2_BOOST_LEVEL_FLAG = 0b011101000,
  EC2_BOOST_LEVEL_MASK = 0b000000111,

  EC2_TRAIN_BRAKE_FLAG = 0b011110000,
  EC2_TRAIN_BRAKE_MASK = 0b000000111,

  EC2_SET_STALL = 0b011111000,
  EC2_STOP_IMMEDIATE = 0b011111011,

  // second page (9th bit high)
  EC2_FORWARD_DIRECTION = 0b100000000,
  EC2_TOGGLE_DIRECTION = 0b100000001,
  //EC2_RESERVED                          = 0b100000010,
  EC2_REVERSE_DIRECTION = 0b100000011,
  EC2_BOOST_SPEED = 0b100000100,
  EC2_OPEN_FRONT_COUPLER = 0b100000101,
  EC2_OPEN_REAR_COUPLER = 0b100000110,
  EC2_BRAKE_SPEED = 0b100000111,
  EC2_AUX_1_OFF = 0b100001000,
  EC2_AUX_1_OPTION_1 = 0b100001001,
  EC2_AUX_1_OPTION_2 = 0b100001010,
  EC2_AUX_1_ON = 0b100001011,
  EC2_AUX_2_OFF = 0b100001100,
  EC2_AUX_2_OPTION_1 = 0b100001101,
  EC2_AUX_2_OPTION_2 = 0b100001110,
  EC2_AUX_2_ON = 0b100001111,
  EC2_NUMERIC_FLAG = 0b100010000,
  EC2_NUMERIC_MASK = 0b000001111,
  EC2_CREWTALK_SPEED_VAR = 0b100010010, // just numeric 2
  EC2_STATION_DINER_LIGHTS_ON = 0b100011000, // just numeric 8
  EC2_STATION_DINER_LIGHTS_OFF = 0b100011001, // just numeric 9
  //EC2_RESERVED                          = 0b100011010,
  EC2_FORWARD_DIRECTION_UNUSED = 0b100011011,
  EC2_BLOW_HORN_1 = 0b100011100,
  EC2_RING_BELL = 0b100011101,
  //EC2_RESERVED                          = 0b100011110,
  EC2_BLOW_HORN_2 = 0b100011111,
  EC2_ASSIGN_AS_SINGLE_UNIT_FORWARD = 0b100100000,
  EC2_ASSIGN_AS_SINGLE_UNIT_REVERSE = 0b100100001,
  EC2_ASSIGN_AS_HEAD_END_UNIT_FORWARD = 0b100100010,
  EC2_ASSIGN_AS_HEAD_END_UNIT_REVERSE = 0b100100011,
  EC2_ASSIGN_AS_MIDDLE_UNIT_FORWARD = 0b100100100,
  EC2_ASSIGN_AS_MIDDLE_UNIT_REVERSE = 0b100100101,
  EC2_ASSIGN_AS_REAR_END_UNIT_FORWARD = 0b100100110,
  EC2_ASSIGN_AS_REAR_END_UNIT_REVERSE = 0b100100111,
  EC2_SET_MOMENTUM_LOW = 0b100101000,
  EC2_SET_MOMENTUM_MEDIUM = 0b100101001,
  EC2_SET_MOMENTUM_HIGH = 0b100101010,
  EC2_SET_ENGINE_OR_TRAIN_ADDRESS = 0b100101011,
  EC2_CLEAR_CONSIST = 0b100101100,
  EC2_LOCOMOTIVE_REFUELING_SOUND = 0b100101101,
  //EC2_RESERVED                          = 0b100101110,
  //EC2_RESERVED                          - 0b100101111,
  EC2_ASSIGN_TO_TRAIN_FLAG = 0b100110000,
  EC2_AUX_3 = 0b100111011, // is this really correct? Unless Lionel has changed ASSIGN_TO_TRAIN to something else

  EC2_ASSIGN_TO_TRAIN_MASK = 0b000001111,

  EC2_SET_RELATIVE_SPEED_FLAG = 0b101000000,
  EC2_SET_RELATIVE_SPEED_MASK = 0b000001111,

  EC2_MARKER_TOGGLE_COLOUR = 0b101111101, // Not sure what this is, but it's used with changing the marker colours

  //EC2_RESERVED_FLAG                     = 0b101000000,
  //EC2_RESERVED_MASK                     = 0b000001111,

  EC2_DIESEL_RUN_LEVEL_FLAG = 0b110100000,
  EC2_DIESEL_RUN_LEVEL_MASK = 0b000000111,

  EC2_RS_TRIGGER_WATER_INJECTOR = 0b110101000,
  EC2_RS_TRIGGER_AUX_AIR_HORN_UNUSED = 0b110101001,
  EC2_SYSTEM_HALT = 0b110101011,

  EC2_BELL_SLIDER_POSITION_FLAG = 0b110110000,
  EC2_BELL_SLIDER_POSITION_MASK = 0b000000111,

  EC2_ENGINE_LABOR_FLAG = 0b111000000,
  EC2_ENGINE_LABOR_MASK = 0b000011111,

  EC2_QUILLING_HORN_INTENSITY_FLAG = 0b111100000,
  EC2_QUILLING_HORN_INTENSITY_MASK = 0b000001111,

  EC2_BELL_ONESHOT_DING_FLAG = 0b111110000,
  EC2_BELL_ONESHOT_DING_MASK = 0b000000011,

  EC2_BELL_OFF = 0b111110100,
  EC2_BELL_ON = 0b111110101,
  EC2_BRAKE_SQUEAL_SOUND = 0b111110110,
  EC2_AUGER_SOUND = 0b111110111,

  EC2_RS_TRIGGER_BRAKE_AIR_RELEASE = 0b111111000,
  EC2_RS_TRIGGER_SHORT_LETOFF = 0b111111001,
  EC2_RS_TRIGGER_LONG_LETOFF = 0b111111010,
  EC2_STARTUP_SEQUENCE_1 = 0b111111011,
  EC2_STARTUP_SEQUENCE_2 = 0b111111100,
  EC2_SHUTDOWN_SEQUENCE_1 = 0b111111101,
  EC2_SHUTDOWN_SEQUENCE_2 = 0b111111110,
};

enum EffectCommandParams : uint16
{
  FX_SMOKE_SYSTEM_OFF = 0b00000000,
  FX_SMOKE_SYSTEM_LOW = 0b00000001,
  FX_SMOKE_SYSTEM_MEDIUM = 0b00000010,
  FX_SMOKE_SYSTEM_HIGH = 0b00000011,
  FX_PANTOGRAPH_FRONT_UP = 0b00010000,
  FX_PANTOGRAPH_FRONT_DOWN = 0b00010001,
  FX_PANTOGRAPH_REAR_UP = 0b00010010,
  FX_PANTOGRAPH_REAR_DOWN = 0b00010011,
  FX_SUBWAY_LEFT_DOOR_OPEN = 0b00100000,
  FX_SUBWAY_LEFT_DOOR_CLOSE = 0b00100001,
  FX_SUBWAY_RIGHT_DOOR_OPEN = 0b00100010,
  FX_SUBWAY_RIGHT_DOOR_CLOSE = 0b00100011,
  FX_STOCK_CAR_OPTION_1_ON = 0b00110000,
  FX_STOCK_CAR_OPTION_1_OFF = 0b00110001,
  FX_STOCK_CAR_OPTION_2_ON = 0b00110010,
  FX_STOCK_CAR_OPTION_2_OFF = 0b00110011,
  FX_STOCK_CAR_LOAD = 0b00110100,
  FX_STOCK_CAR_UNLOAD = 0b00110101,
  FX_STOCK_CAR_FRED_ON = 0b00110110,
  FX_STOCK_CAR_FRED_OFF = 0b00110111,
  FX_STOCK_CAR_FLAT_WHEEL_ON = 0b00111000,
  FX_STOCK_CAR_FLAT_WHEEL_OFF = 0b00111001,
  FX_STOCK_CAR_GAME_ON = 0b00111010,
  FX_STOCK_CAR_GAME_OFF = 0b00111011

};

enum RailSoundsEffectTriggerParams : uint16
{
  ET_PRIME_MOVER_OFF = 0b00010000,
  ET_PRIME_MOVER_ON = 0b00010001,
  ET_MASTER_VOLUME_DOWN = 0b00010010,
  ET_MASTER_VOLUME_UP = 0b00010011,
  ET_BLEND_VOLUME_DOWN = 0b00010100,
  ET_BLEND_VOLUME_UP = 0b00010101,
  // Reserved from 0x16 - 0x19
  ET_CYL_COCK_CLEAR_OFF = 0b00100000,
  ET_CYL_COCK_CLEAR_ON = 0b00100001,
  ET_RESERVED = 0b00100010,
  ET_STANDBY_WARNING_BELL_ON = 0b00100011,
  ET_STANDBY_MODE_OFF = 0b00100100,
  ET_STANDBY_MODE_ON = 0b00100101,
  ET_FORCE_COUPLER_IMPACT_COMPRESS = 0b00100110,
  ET_FORCE_COUPLER_IMPACT_STRETCH = 0b00100111,
  ET_CIRCUIT_BREAKER_MAIN_LIGHT = 0b00101000,
  ET_CIRCUIT_BREAKER_CAB_LIGHT = 0b00101001,
  ET_CIRCUIT_BREAKER_GROUND_LIGHT = 0b00101010,
  ET_RESERVED2 = 0b00101011,
  ET_SEQUENCE_CONTROL_OFF = 0b00101100,
  ET_SEQUENCE_CONTROL_ON = 0b00101101,
};


enum DialogCommandParams : uint16
{
    DC_RESERVED = 0b00000000,
    DC_CONVENTIONAL_MODE_SHUTDOWN = 0b00000001,
    DC_SCENE_2_KEY_CONTEXT_DEPENDENT = 0b00000010,
    DC_SCENE_7_KEY_CONTEXT_DEPENDENT = 0b00000011,
    DC_SCENE_5_KEY_CONTEXT_DEPENDENT = 0b00000100,
    DC_CONVENTIONAL_SHORT_HORN_TRIGGER = 0b00000101,
    DC_TOWER_INITIAL_ENGINE_STARTUP = 0b00000110,
    DC_ENGINEER_DEPARTURE_DENIED = 0b00000111,
    DC_ENGINEER_DEPARTURE_GRANTED = 0b00001000,
    DC_ENGINEER_HAVE_DEPARTED = 0b00001001,
    DC_ENGINEER_ALL_CLEAR = 0b00001010,
    DC_TOWER_NON_EMG_STOP = 0b00001011,
    DC_TOWER_RESTRICTED_SPEED = 0b00001100,
    DC_TOWER_SLOW_SPEED = 0b00001101,
    DC_TOWER_MEDIUM_SPEED = 0b00001110,
    DC_TOWER_LIMITED_SPEED = 0b00001111,
    DC_TOWER_NORMAL_SPEED = 0b00010000,
    DC_TOWER_HIGH_BALL_SPEED = 0b00010001,
    DC_ENGINEER_ARRVING_SOON = 0b00010010,
    DC_ENGINEER_HAVE_ARRIVE = 0b00010011,
    DC_ENGINEER_SHUT_DOWN = 0b00010100,
    DC_ENGINEER_IDENTIFY = 0b00010101,
    DC_ENGINEER_ACK_COMM = 0b00010110,
    DC_ENGINEER_ACK_SPEED_TO_STOP = 0b00010111,
    DC_ENGINEER_ACK_SPEED_TO_RESTRICTED = 0b00011000,
    DC_ENGINEER_ACK_SPEED_TO_SLOW = 0b00011001,
    DC_ENGINEER_ACK_SPEED_TO_MEDIUM = 0b00011010,
    DC_ENGINEER_ACK_SPEED_TO_LIMITED = 0b00011011,
    DC_ENGINEER_ACK_SPEED_TO_NORMAL = 0b00011100,
    DC_ENGINEER_ACK_SPEED_TO_HIGH_BALL = 0b00011101,
    DC_ENGINEER_CONTEXT_DEPENDENT = 0b00011110,
    DC_EMERGENCY_CONTEXT_DEPENDENT = 0b00011111,
    DC_TOWER_CONTEXT_DEPENDENT = 0b00100000,
    RESERVED = 0b00100001,
    DC_TOWER_DEPARTURE_DENIED = 0b00100010,
    DC_TOWER_DEPARTURE_GRANTED = 0b00100011,
    DC_TOWER_HAVE_DEPARTED = 0b00100100,
    DC_TOWER_ALL_CLEAR = 0b00100101,

    DC_TOWER_ARRIVING_SOON = 0b00101101,
    DC_TOWER_HAVE_ARRIVED = 0b00101110,
    DC_TOWER_SHUT_DOWN = 0b00101111,
    DC_CONDUCTOR_ALL_ABOARD = 0b00110000,
    DC_ENGINEER_ACK_STANDING_BY = 0b00110001,
    DC_ENGINEER_ACK_CLEARED_TO_GO = 0b00110010,
    DC_ENGINEER_ACK_CLEAR_AHEAD = 0b00110011,
    DC_ENGINEER_ACK_CLEAR_INBOUND = 0b00110100,
    DC_ENGINEER_ACK_WELCOME_BACK = 0b00110101,
    DC_ENGINEER_ACK_IDENTIFY_OUT = 0b00110110,

    DC_SCENE_2_CONTEXT_DEPENDANT = 0b00110111,
    DC_SCENE_AUX1_2_CONTEXT_DEPENDANT = 0b00111000,
    DC_SCENE_7_CONTEXT_DEPENDANT = 0b00111001,
    DC_SCENE_AUX1_7_CONTEXT_DEPENDANT = 0b00111010,
    DC_SCENE_5_CONTEXT_DEPENDANT = 0b00111011,
    DC_SCENE_AUX1_5_CONTEXT_DEPENDANT = 0b00111100,

    DC_ENGINEER_SPEAK_FUEL_LVL = 0b00111101,
    DC_ENGINEER_SPEAK_FUEL_REFILL = 0b00111110,
    DC_ENGINEER_SPEAK_SPEED = 0b00111111,
    DC_ENGINEER_SPEAK_WATER_LEVEL = 0b01000000,
    DC_ENGINEER_SPEAK_WATER_REFILL = 0b01000001,

    // Doc 1.22 says nothing until 0x50, i think otherwise. 
    DC_SEQ_CONTROL_DISABLE = 0b01010000,
    DC_SEQ_CONTROL_ENABLE = 0b01010001,
    DC_SEQ_CONTROL_CLEARED = 0b01010010,
    DC_SEQ_CONTROL_HAVE_DEPARTED = 0b01010011,
    DC_SEQ_CONTROL_IN_TRANSIT = 0b01010100,
    DC_SEQ_CONTROL_MAX_SPEED = 0b01010101,
    DC_SEQ_CONTROL_CLEAR_AHEAD = 0b01010110,
    DC_SEQ_CONTROL_RANDOM = 0b01010111,
    DC_SEQ_CONTROL_ARRIVING_SHORT_TRIP = 0b01011000,
    DC_SEQ_CONTROL_ARRIVING_LONG_TRIP = 0b01011001,
    DC_SEQ_CONTROL_RESERVED = 0b01011010,
    DC_SEQ_CONTROL_RESERVED2 = 0b01011011,
    DC_SEQ_CONTROL_ARRIVED_SHORT_TRIP = 0b01011100,
    DC_SEQ_CONTROL_ARRIVED_LONG_TRIP = 0b01011101,
    DC_SEQ_CONTROL_RESERVED3 = 0b01011110,
    DC_SEQ_CONTROL_RESERVED4 = 0b01011111,

    STATION_SOUND_CAR_CONDUCTOR_NEXT_STOP = 0b01101000,
    STATION_SOUND_CAR_CONDUCTOR_WATCH_STEP = 0b01101001,
    STATION_SOUND_CAR_CONDUCTOR_ALL_ABOARD = 0b01101010,
    STATION_SOUND_CAR_CONDUCTOR_TICKETS_PLZ = 0b01101011,
    STATION_SOUND_CAR_CONDUCTOR_PREMATURE_STOP = 0b01101100,
    STATION_SOUND_CAR_STEWARD_WELCOME = 0b01101101,
    STATION_SOUND_CAR_STEWARD_FIRST_SEATING = 0b01101110,
    STATION_SOUND_CAR_STEWARD_SECOND_SEATING = 0b01101111,
    STATION_SOUND_CAR_STEWARD_LOUNGE_OPEN = 0b01110000,
    STATION_SOUND_CAR_PA_TRAIN_ARRIVING = 0b01110001,
    STATION_SOUND_CAR_PA_TRAIN_ARRIVED = 0b01110010,
    STATION_SOUND_CAR_PA_TRAIN_BOARDING = 0b01110011,
    STATION_SOUND_CAR_PA_TRAIN_DEPARTING = 0b01110100,
    STATION_SOUND_CAR_STARTUP = 0b01110101,
    STATION_SOUND_CAR_SHUTDOWN = 0b01110110,
};

enum LightingCommandParams : uint16
{
  LT_DOGHOUSE_LIGHT_OFF = 0b10100000,
  LT_DOGHOUSE_LIGHT_ON = 0b10100001,
  LT_HAZARD_LIGHT_OFF = 0b10110000,
  LT_HAZARD_LIGHT_ON = 0b10110001,
  LT_HAZARD_LIGHT_AUTO = 0b10110010,
  LT_DITCH_LIGHTS_OFF = 0b11000000,
  LT_DITCH_LIGHTS_OFF_PULSE_ON = 0b11000001,
  LT_DITCH_LIGHTS_ON_PULSE_OFF = 0b11000010,
  LT_DITCH_LIGHTS_ON = 0b11000011,
  LT_LOCO_MARKER_LIGHT_OFF = 0b11001000,
  LT_LOCO_MARKER_LIGHT_ON = 0b11001001,
  LT_TENDER_MARKER_LIGHT_OFF = 0b11001100,
  LT_TENDER_MARKER_LIGHT_ON = 0b11001101,
  LT_GROUND_LIGHTS_OFF = 0b11010000,
  LT_GROUND_LIGHTS_ON = 0b11010001,
  LT_GROUND_LIGHTS_AUTO = 0b11010010,
  LT_STROBE_LIGHT_OFF = 0b11100000, // Also used when toggling marker colour 
  LT_STROBE_LIGHT_ON_SINGLE_FLASH = 0b11100001,
  LT_STROBE_LIGHT_ON_DOUBLE_FLASH = 0b11100010,
  LT_MARS_LIGHT_OFF = 0b11101000,
  LT_MARS_LIGHT_ON = 0b11101001,
  LT_CAB_LIGHT_OFF = 0b11110000,
  LT_CAB_LIGHT_ON = 0b11110001,
  LT_CAB_LIGHT_AUTO = 0b11110010,
  LT_RULE_17_OFF = 0b11110100,
  LT_RULE_17_AUTO = 0b11110101,
  LT_RULE_17_ON = 0b11110110,
  LT_CAR_CABIN_LIGHT_OFF = 0b11111000,
  LT_CAR_CABIN_LIGHT_ON = 0b11111001,
  LT_CAR_CABIN_LIGHT_AUTO = 0b11111010
};

struct MultiWordCommand
{
  TMCCAddress address;
  bool train;
  ParameterIndex index;
  uint8 command;
  //uint16 word0;
  //uint16 word1;
  //uint16 word2;

  MultiWordCommand()
    : address(0)
    , train(false)
    , index{0}
    , command{0}
  {
  }

  //MultiWordCommand(TMCCAddress _address, bool _train, ParameterIndex _index, uint8 _command)
  //  : address(_address)
  //  , train(_train)
  //  , index(_index)
  //  , command(_command)
  //{
  //}

  MultiWordCommand(EngineHandle _address, ParameterIndex _index, uint8 _command)
    : address(_address)
    , train(false)
    , index(_index)
    , command(_command)
  {
  }

  MultiWordCommand(TrainHandle _address, ParameterIndex _index, uint8 _command)
    : address(_address)
    , train(true)
    , index(_index)
    , command(_command)
  {
  }
};

enum TMCCDirection
{
  TMCC_FORWARD,
  TMCC_REVERSE
};

enum TMCCPantographState
{
  TMCC_UP,
  TMCC_DOWN
};

enum TMCCDoorState
{
  TMCC_OPEN,
  TMCC_CLOSED
};

enum TMCCSide
{
  TMCC_LEFT,
  TMCC_RIGHT
};

enum TMCCActiveState
{
  TMCC_ON,
  TMCC_OFF
};

class TMCCInterface
{
public:
  static bool Init();
  static void Shutdown();

  // switch commands
  static bool SwitchThrowThrough(SwitchHandle id);
  static bool SwitchThrowOut(SwitchHandle id);
  static bool SwitchSetAddress(SwitchHandle address);
  static bool SwitchAssignToRouteThrough(SwitchHandle id, RouteHandle routeID);
  static bool SwitchAssignToRouteOut(SwitchHandle id, RouteHandle routeID);

  // route commands
  static bool RouteThrow(RouteHandle id);
  static bool RouteClear(RouteHandle id);

  // engine or train commands
  //static bool EngineSetForwardDirection(TMCCAddress id);
  static bool EngineSetDirection(EngineHandle id, TMCCDirection dir);
  static bool EngineToggleDirection(EngineHandle id);
  //static bool EngineSetReverseDirection(TMCCAddress id);
  static bool EngineBoostSpeed(EngineHandle id);
  static bool EngineBrakeSpeed(EngineHandle id);
  static bool EngineOpenFrontCoupler(EngineHandle id);
  static bool EngineOpenRearCoupler(EngineHandle id);
  static bool EngineBlowHorn1(EngineHandle id);
  static bool EngineRingBell(EngineHandle id);
  static bool EngineLetOffSound(EngineHandle id);
  static bool EngineBlowHorn2(EngineHandle id);
  //static bool EngineAux1Off(EngineHandle id);
  static bool EngineAux1Option1(EngineHandle id);
  static bool EngineAux1Option2(EngineHandle id);
  //static bool EngineAux1On(EngineHandle id);
  static bool EngineSetAux1(EngineHandle id, TMCCActiveState state);

  //static bool EngineAux2Off(EngineHandle id);
  static bool EngineAux2Option1(EngineHandle id);
  static bool EngineAux2Option2(EngineHandle id);
  //static bool EngineAux2On(EngineHandle id);
  static bool EngineSetAux2(EngineHandle id, TMCCActiveState state);

  static bool EngineNumericCommand(EngineHandle id, uint8 cmd);

  //static bool TrainSetForwardDirection(TMCCAddress id);
  static bool TrainSetDirection(TrainHandle id, TMCCDirection dir);
  static bool TrainToggleDirection(TrainHandle id);
  //static bool TrainSetReverseDirection(TMCCAddress id);
  static bool TrainBoostSpeed(TrainHandle id);
  static bool TrainBrakeSpeed(TrainHandle id);
  static bool TrainOpenFrontCoupler(TrainHandle id);
  static bool TrainOpenRearCoupler(TrainHandle id);
  static bool TrainBlowHorn1(TrainHandle id);
  static bool TrainRingBell(TrainHandle id);
  static bool TrainLetOffSound(TrainHandle id);
  static bool TrainBlowHorn2(TrainHandle id);
  //static bool TrainAux1Off(TrainHandle id);
  static bool TrainAux1Option1(TrainHandle id);
  static bool TrainAux1Option2(TrainHandle id);
  //static bool TrainAux1On(TrainHandle id);
  static bool TrainSetAux1(TrainHandle id, TMCCActiveState state);
  //static bool TrainAux2Off(TrainHandle id);
  static bool TrainAux2Option1(TrainHandle id);
  static bool TrainAux2Option2(TrainHandle id);
  //static bool TrainAux2On(TrainHandle id);
  static bool TrainSetAux2(TrainHandle id, TMCCActiveState state);
  static bool TrainNumericCommand(TrainHandle id, uint8 cmd);

  // extended engine or train commands
  static bool EngineSetMomentumLow(EngineHandle id);
  static bool EngineSetMomentumMedium(EngineHandle id);
  static bool EngineSetMomentumHigh(EngineHandle id);

  // speed 0-31 inclusive
  static bool EngineSetAbsoluteSpeed(EngineHandle id, int speed);
  // speed -5 to +5 inclusive
  static bool EngineSetRelativeSpeed(EngineHandle id, int speed);

  static bool TrainSetMomentumLow(TrainHandle id);
  static bool TrainSetMomentumMedium(TrainHandle id);
  static bool TrainSetMomentumHigh(TrainHandle id);

  // speed 0-31 inclusive
  static bool TrainSetAbsoluteSpeed(TrainHandle id, int speed);
  // speed -5 to +5 inclusive
  static bool TrainSetRelativeSpeed(TrainHandle id, int speed);

  // extended engine commands
  static bool EngineAssignToTrain(EngineHandle id, TrainHandle trainAddress);
  static bool EngineAssignAsSingleUnit(EngineHandle id, TMCCDirection dir);
  static bool EngineAssignAsHeadEndUnit(EngineHandle id, TMCCDirection dir);
  static bool EngineAssignAsMiddleUnit(EngineHandle id, TMCCDirection dir);
  static bool EngineAssignAsRearEndUnit(EngineHandle id, TMCCDirection dir);
  static bool EngineSetEngineAddress(TMCCAddress id); // unclear what this does
                                                      // Edit: 6/15/22 - Lionel mislabelled the Set engine address on the legacy spec sheet

  // extended train commands
  static bool TrainSetTMCCAddress(TrainHandle id);
  static bool TrainClearConsist(TrainHandle id);

  // TMCC2 commands

  static bool EngineBlowHorn1TMCC2(EngineHandle id);
  static bool EngineBlowHorn2TMCC2(EngineHandle id);


  //static bool EngineAux1Off(EngineHandle id);
  static bool EngineAux1Option1_2(EngineHandle id);
  static bool EngineAux1Option2_2(EngineHandle id);
  //static bool EngineAux1On_2(EngineHandle id);
  static bool EngineSetAux1_2(EngineHandle id, TMCCActiveState state);

  //static bool EngineAux2Off_2(EngineHandle id);
  static bool EngineAux2Option1_2(EngineHandle id);
  static bool EngineAux2Option2_2(EngineHandle id);
  //static bool EngineAux2On_2(EngineHandle id);
  static bool EngineSetAux2_2(EngineHandle id, TMCCActiveState state);

  static bool EngineAux3Trigger(EngineHandle id);

  static bool SendGenericCommand(EngineHandle id, uint16 cmd);

  static bool EngineCrewTalkSpeed(EngineHandle id);


  // TMCC2 Numerical command, may not be needed, but unsure if engine handles this command differently than the TMCC1 command.
  static bool EngineNumericCommand2(EngineHandle id, uint8 cmd);
  // speed 0-199 inclusive
  static bool EngineSetAbsoluteSpeed2(EngineHandle id, int speed);
  
  // EngineSetMomentum2, EngineSetBrakeLevel2, EngineSetBoostLevel2, and EngineSetTrainBrake2 use a range of 0-7 inclusive
  static bool EngineSetMomentum2(EngineHandle id, int momentum);
  static bool EngineSetBrakeLevel2(EngineHandle id, int brake);
  static bool EngineSetBoostLevel2(EngineHandle id, int boost);
  static bool EngineSetTrainBrake2(EngineHandle id, int brake);

  static bool EngineSetStall2(EngineHandle id);
  static bool EngineStopImmediate2(EngineHandle id);

  // speed 0-199 inclusive
  static bool TrainSetAbsoluteSpeed2(TrainHandle id, int speed);

  // TrainSetMomentum2, TrainSetBrakeLevel2, TrainSetBoostLevel2, and TrainSetTrainBrake2 use a range of 0-7 inclusive
  static bool TrainSetMomentum2(TrainHandle id, int momentum);
  static bool TrainSetBrakeLevel2(TrainHandle id, int brake);
  static bool TrainSetBoostLevel2(TrainHandle id, int boost);
  static bool TrainSetTrainBrake2(TrainHandle id, int brake);

  static bool TrainSetStall2(TrainHandle id);
  static bool TrainStopImmediate2(TrainHandle id);

  // stuff only supported by TMCC2 (hence no '2' suffix)
  // 0-7 inclusive
  static bool EngineSetDieselRunLevel(EngineHandle id, int level);
  static bool EngineRSTriggerWaterInjector(EngineHandle id);
  // 2-5 inclusive
  static bool EngineSetBellSliderPosition(EngineHandle id, int position);
  // 0-31 inclusive
  static bool EngineSetLabor(EngineHandle id, int labor);
  // 0-16 inclusive (maybe? 16 may not work)
  static bool EngineSetQuillingHornIntensity(EngineHandle id, int intensity);
  // 0-3 inclusive
  static bool EngineBellOneShotDing(EngineHandle id, int x);
  static bool EngineSetBell(EngineHandle id, TMCCActiveState state);
  

  static bool EngineBrakeSquealSound(EngineHandle id);
  static bool EngineAugerSound(EngineHandle id);
  static bool EngineRSTriggerBrakeAirRelease(EngineHandle id);
  static bool EngineRSTriggerShortLetOff(EngineHandle id);
  static bool EngineRSTriggerLongLetOff(EngineHandle id);

  // delayed prime mover
  static bool EngineStartupSequence1(EngineHandle id);
  // immediate start up
  static bool EngineStartupSequence2(EngineHandle id);
  // delay w/ announcement
  static bool EngineShutdownSequence1(EngineHandle id);
  // immediate shut down
  static bool EngineShutdownSequence2(EngineHandle id);

  // Stationsound diner light, maybe other products
  static bool EngineSetDinerLight(EngineHandle id, TMCCActiveState state);

  // Air horn (says unused, not sure)
  static bool EngineBlowAirHorn(EngineHandle id);

  // Loco refuel sound
  static bool EngineLocoRefuelSound(EngineHandle id);

  // Multi word commands
  
  
  // effects

  // Dialog Effects
  static bool EngineDialogCommand(EngineHandle id, int x);
  static bool EngineEffectCommand(EngineHandle id, int x);

  // Smoke system Values
  // level 0 - off, 1 - low, 2 - medium, 3 - high
  static bool EngineSetSmokeSystem(EngineHandle id, int level);

  static bool EngineSetMasterVolume(EngineHandle id, TMCCActiveState state);


  static bool EngineSetFrontPantograph(EngineHandle id, TMCCPantographState state);
  static bool EngineSetRearPantograph(EngineHandle id, TMCCPantographState state);
  static bool EngineSetSubwayDoorState(EngineHandle id, TMCCSide side, TMCCDoorState state);
  static bool EngineSetStockCarOption1(EngineHandle id, TMCCActiveState state);
  static bool EngineSetStockCarOption2(EngineHandle id, TMCCActiveState state);
  static bool EngineStockCarLoad(EngineHandle id);
  static bool EngineStockCarUnload(EngineHandle id);
  static bool EngineStockCarSetFRED(EngineHandle id, TMCCActiveState state);
  static bool EngineStockCarSetFlatWheel(EngineHandle id, TMCCActiveState state);
  static bool EngineStockCarSetGame(EngineHandle id, TMCCActiveState state);

  // lighting
  static bool EngineSetDoghouseLight(EngineHandle id, TMCCActiveState state);
  static bool EngineSetHazardLight(EngineHandle id, TMCCActiveState state);
  static bool EngineSetHazardLightAuto(EngineHandle id);
  static bool EngineSetDitchLights(EngineHandle id, TMCCActiveState state, TMCCActiveState pulse);
  static bool EngineSetLocoMarkerLight(EngineHandle id, TMCCActiveState state);
  static bool EngineSetTenderMarkerLight(EngineHandle id, TMCCActiveState state);
  static bool EngineSetGroundLights(EngineHandle id, TMCCActiveState state);
  static bool EngineSetGroundLightsAuto(EngineHandle id);
  // 0 - off, 1 - single flash, 2 - double flash
  static bool EngineSetStrobeLight(EngineHandle id, int strobe);
  static bool EngineSetMarsLight(EngineHandle id, TMCCActiveState state);
  static bool EngineSetCabLight(EngineHandle id, TMCCActiveState state);
  static bool EngineSetCabLightAuto(EngineHandle id);
  static bool EngineSetRule17(EngineHandle id, TMCCActiveState state);
  static bool EngineSetRule17Auto(EngineHandle id);
  static bool EngineSetCarCabinLight(EngineHandle id, TMCCActiveState state);
  static bool EngineSetCarCabinLightAuto(EngineHandle id);

  // effects control
  static bool EngineToggleSequenceCtrl(EngineHandle id, TMCCActiveState state);
  static bool EngineToggleCylinderCock(EngineHandle id, TMCCActiveState state);
  static bool EngineTogglePrimeMover(EngineHandle id, TMCCActiveState state);

  // Not sure what these reserved areas are for. 
  static bool EngineReservedArea3(EngineHandle id, int x);
  static bool EngineReservedArea5(EngineHandle id, int x);
  static bool EngineReservedArea7(EngineHandle id, int x);
  static bool EngineReservedArea8(EngineHandle id, int x);
  static bool EngineReservedArea9(EngineHandle id, int x);
  static bool EngineReservedArea10(EngineHandle id, int x);
  static bool EngineReservedArea11(EngineHandle id, int x);
  static bool EngineReservedArea12(EngineHandle id, int x);


protected:
  static bool WriteData(ubyte* pData, int length);
  static bool SendTMCC1Command(const TMCCCommand& cmd);
  static bool SendEngineCommand(const TMCC2Command& cmd);
  static bool SendTrainCommand(const TMCC2Command& cmd);
  static bool SendMultiWordCommand(const MultiWordCommand& cmd);
};

#endif // _TMCC_INTERFACE_H_