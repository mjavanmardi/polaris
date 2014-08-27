#pragma once

#include "gl\glew.h"
#include "wx\glcanvas.h"

#include "wx\wx.h"
#include "wx\aui\aui.h"

#include "wx\panel.h"
#include "wx\tglbtn.h"

#include "wx\listctrl.h"

#include "wxPLplotwindow.h"
#include "Core/Core.h"
#include "Geometry_Implementation.h"

typedef void (*selection_callback_type)(const boost::container::list<void*>&,const boost::container::list<void*>&,const boost::container::list<void*>&,boost::container::vector<pair<string,string>>&);
typedef void (*double_click_callback_type)(const boost::container::list<void*>&,boost::container::vector<pair<string,string>>&,boost::container::vector<boost::container::vector<string>>&);
typedef bool (*submission_callback_type)(const boost::container::list<void*>&,const boost::container::vector<string>&,const boost::container::vector<string>&);
typedef float (*pixel_size_callback_type)(float);
typedef void (*reschedule_callback_type)(void*, int);

using namespace polaris;
//#include "io/Io.h"

//#include "User_Space\User_Space.h"
//#include "../User_Space/User_Space_with_odb.h"
//#include "Geometry_Implementation.h"


//implementation class Antares_Implementation;
