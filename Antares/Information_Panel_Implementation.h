//*********************************************************
//	Information_Panel_Implementation.h - Information and Plotting
//*********************************************************

#pragma once
#include "Information_Panel.h"

//---------------------------------------------------------
//	Information_Panel_Implementation - information panel class definition
//---------------------------------------------------------

implementation class Information_Panel_Implementation : public wxPanel
{
public:
	Information_Panel_Implementation(wxFrame* parent);
	virtual ~Information_Panel_Implementation(void){};

	//Canvas_Implementation* canvas_ptr;

};
//---------------------------------------------------------
//	Information_Panel - information initialization
//---------------------------------------------------------

template<typename MasterType,typename ParentType>
Information_Panel_Implementation<MasterType,ParentType>::Information_Panel_Implementation(wxFrame* parent) : wxPanel(parent,-1,wxDefaultPosition,wxDefaultSize,wxCLIP_CHILDREN )
{
	//---- miscellaneous initialization ----

	//canvas_ptr=((Antares_Implementation *) GetParent())->canvas;
}