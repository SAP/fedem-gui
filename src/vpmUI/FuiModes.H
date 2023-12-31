// SPDX-FileCopyrightText: 2023 SAP SE
//
// SPDX-License-Identifier: Apache-2.0
//
// This file is part of FEDEM - https://openfedem.org
////////////////////////////////////////////////////////////////////////////////

#ifndef FUI_MODES_H
#define FUI_MODES_H

#include <string>


class FuiModes
{
public:
  enum ModeType { EXAM_MODE,
		  APPEARANCE_MODE,
		  PTPMOVE_MODE,
		  ERASE_MODE,
                  ALLIGNCS_MODE,
                  ALLIGNROT_MODE,
                  MOVE_TO_CENTER_MODE,
		  ATTACH_MODE,
		  DETACH_MODE,
		  MAKEGENERALSPIDER_MODE,
		  MAKECYLINDERSPIDER_MODE,
		  ADD_RESULTSPROBE_MODE,
		  MAKEFORCE_MODE,
		  MAKETORQUE_MODE,
		  MAKETRIAD_MODE,
		  MAKEREVJOINT_MODE,
		  MAKEBALLJOINT_MODE,
		  MAKERIGIDJOINT_MODE,
		  MAKESPRING_MODE,
                  MAKEDAMPER_MODE,
		  MAKEFREEJOINT_MODE,
		  MAKEFREEJOINTBETWEENTRIADS_MODE,
		  MAKECYLJOINT_MODE,
		  MAKECYLJOINTBETWEENTRIADS_MODE,
		  MAKECAMJOINT_MODE,
		  MAKEPRISMJOINT_MODE,
		  MAKEPRISMJOINTBETWEENTRIADS_MODE,
		  MAKEGEAR_MODE,
		  MAKERACKPIN_MODE,
		  MAKESTICKER_MODE,
		  MAKESIMPLESENSOR_MODE,
                  CREATE_SENSOR_MODE,
		  MAKERELATIVESENSOR_MODE,
                  MAKETIRE_MODE,
                  MAKESTRAINROSETTE_MODE,
                  EDITSTRAINROSETTENODES_MODE,
                  EDITSTRAINROSETTEDIR_MODE,
		  PICKLOADFROMPOINT_MODE,
		  PICKLOADTOPOINT_MODE,
		  PICKLOADATTACKPOINT_MODE,
		  ADDMASTERINLINJOINT_MODE,
		  SELECTREFCS_MODE,
		  COMPICKPOINT_MODE,
		  MEASURE_DISTANCE_MODE,
		  MEASURE_ANGLE_MODE};

  static void     setMode(ModeType newMode);
  static ModeType getMode() { return mode; }

  static void setState(int newState);
  static int  getState() { return state; }

  static void done();
  static void cancel();

  static void notifyCreatePointChange() { createPositionIsChangedByGUI = true; }

protected:
  static void setTip();

public:
  static std::string tipComPicking;

private:
  static ModeType mode;
  static int      state;

  static bool createPositionIsChangedByGUI;
};

#endif
