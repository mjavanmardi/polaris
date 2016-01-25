#pragma once
#include "Dependencies.h"

namespace Traffic_Management_Center_Components
{
	namespace Types
	{

	}

	namespace Concepts
	{

	}
	
	namespace Implementations
	{
		//implementation struct Antares_TMC:public Simple_TMC<MasterType,INHERIT(Antares_TMC)>
		//{
		//	typedef Simple_TMC<MasterType,INHERIT(Antares_TMC)> Parent;

		//	template<typename TargetType> void Setup_Link_Controls()
		//	{
//TODO
//		//		_link_control_layer=Allocate_New_Layer< typename MasterType::type_of(canvas),NT,Target_Type< NT,Antares_Layer<type_of(link_control_layer)>*, string& > >(string("Link Controls"));

		//		Antares_Layer_Configuration cfg;
		//		True_Color_RGBA<NT> head_color;
		//		
		//		head_color._r = 255;
		//		head_color._g = 0;
		//		head_color._b = 255;
		//		head_color._a = 255;

		//		cfg.Configure_Static_Points(head_color,8);
		//		cfg.draw=true;
		//		
		//		std::vector<string>* dropdown;

		//		cfg.dropdown_schema.push_back(std::vector<string>());
		//		dropdown=&cfg.dropdown_schema.back();
		//		dropdown->push_back("Close Lane");
		//		dropdown->push_back("Open Lane");

		//		std::vector<string>::iterator sitr;

		//		for(sitr = _link_control_keys.begin(); sitr != _link_control_keys.end(); sitr++)
		//		{
		//			cfg.attributes_schema.push_back( *sitr );
		//		}


		//		_link_control_layer->Initialize<NT>(cfg);

		//		std::vector<Link_Control_Interface*>::iterator itr;
		//		
		//		Point_3D<MasterType> element;

		//		for(itr=_link_controls.begin();itr!=_link_controls.end();itr++)
		//		{

		//			element._x = (*itr)->x_position<float>();
		//			element._y = (*itr)->y_position<float>();
		//			element._z = 1;

//TODO
//		//			Scale_Coordinates<typename MasterType::type_of(canvas),NT,Target_Type<NT,void,Point_3D<MasterType>&>>(element);

		//			_link_control_layer->Push_Element<Regular_Element>(&element);
		//		}
		//	}

		//	//template<typename TargetType> void Setup_Advisory_Radios()
		//	//{
//TODO
//		//	//	_advisory_radio_layer=Allocate_New_Layer< typename MasterType::type_of(canvas),NT,Target_Type< NT,Antares_Layer<type_of(advisory_radio_layer)>*, string& > >(string("Advisory Radios"));

		//	//	Antares_Layer_Configuration cfg;
		//	//	True_Color_RGBA<NT> head_color;
		//	//	
		//	//	head_color._r = 255;
		//	//	head_color._g = 255;
		//	//	head_color._b = 0;
		//	//	head_color._a = 255;

		//	//	cfg.Configure_Static_Points(head_color,8);
		//	//	cfg.draw=true;
		//	//	_advisory_radio_layer->Initialize<NT>(cfg);

		//	//	std::vector<Advisory_Radio_Interface*>::iterator itr;

		//	//	for(itr=_advisory_radios.begin();itr!=_advisory_radios.end();itr++)
		//	//	{
		//	//		Point_3D<MasterType> element;

		//	//		element._x = (*itr)->x_position<float>();
		//	//		element._y = (*itr)->y_position<float>();
		//	//		element._z = 1;

//TODO
//		//	//		Scale_Coordinates<typename MasterType::type_of(canvas),NT,Target_Type<NT,void,Point_3D<MasterType>&>>(element);

		//	//		_advisory_radio_layer->Push_Element<Regular_Element>(&element);
		//	//	}
		//	//}

		//	template<typename TargetType> void Setup_Variable_Message_Signs()
		//	{
//TODO
//		//		_variable_message_sign_layer=Allocate_New_Layer< typename MasterType::type_of(canvas),NT,Target_Type< NT,Antares_Layer<type_of(variable_message_sign_layer)>*, string& > >(string("Variable Message Signs"));

		//		Antares_Layer_Configuration cfg;
		//		True_Color_RGBA<NT> head_color;
		//		
		//		head_color._r = 0;
		//		head_color._g = 255;
		//		head_color._b = 255;
		//		head_color._a = 255;

		//		cfg.Configure_Static_Points(head_color,8);
		//		cfg.draw=true;
		//		
		//		std::vector<string>* dropdown;

		//		cfg.dropdown_schema.push_back(std::vector<string>());
		//		dropdown=&cfg.dropdown_schema.back();
		//		dropdown->push_back("Congestion: I-55");
		//		dropdown->push_back("Congestion: I-90");
		//		
		//		cfg.dropdown_schema.push_back(std::vector<string>());
		//		dropdown=&cfg.dropdown_schema.back();
		//		dropdown->push_back("Inident: Godzilla Attack");
		//		dropdown->push_back("Inident: Alien Contact");
		//		dropdown->push_back("Inident: Killer Robots");

		//		std::vector<string>::iterator sitr;

		//		for(sitr = _variable_message_sign_keys.begin(); sitr != _variable_message_sign_keys.end(); sitr++)
		//		{
		//			cfg.attributes_schema.push_back( *sitr );
		//		}


		//		_variable_message_sign_layer->Initialize<NT>(cfg);


		//		std::vector<Variable_Message_Sign_Interface*>::iterator itr;
		//		
		//		Point_3D<MasterType> element;

		//		for(itr=_variable_message_signs.begin();itr!=_variable_message_signs.end();itr++)
		//		{
		//			element._x = (*itr)->x_position<float>();
		//			element._y = (*itr)->y_position<float>();
		//			element._z = 1;

//TODO
//		//			Scale_Coordinates<typename MasterType::type_of(canvas),NT,Target_Type<NT,void,Point_3D<MasterType>&>>(element);

		//			_variable_message_sign_layer->Push_Element<Regular_Element>(&element);
		//		}
		//	}

		//	//template<typename TargetType> void Setup_Depots()
		//	//{
//TODO
//		//	//	_depot_layer=Allocate_New_Layer< typename MasterType::type_of(canvas),NT,Target_Type< NT,Antares_Layer<type_of(depot_layer)>*, string& > >(string("Tow Truck Depots"));

		//	//	Antares_Layer_Configuration cfg;
		//	//	True_Color_RGBA<NT> head_color;
		//	//	
		//	//	head_color._r = 0;
		//	//	head_color._g = 0;
		//	//	head_color._b = 0;
		//	//	head_color._a = 255;

		//	//	cfg.Configure_Static_Points(head_color,8);
		//	//	cfg.draw=true;
		//	//	_depot_layer->Initialize<NT>(cfg);

		//	//	std::vector<Depot_Interface*>::iterator itr;

		//	//	for(itr=_depots.begin();itr!=_depots.end();itr++)
		//	//	{
		//	//		Point_3D<MasterType> element;

		//	//		element._x = (*itr)->x_position<float>();
		//	//		element._y = (*itr)->y_position<float>();
		//	//		element._z = 1;

//TODO
//		//	//		Scale_Coordinates<typename MasterType::type_of(canvas),NT,Target_Type<NT,void,Point_3D<MasterType>&>>(element);

		//	//		_depot_layer->Push_Element<Regular_Element>(&element);
		//	//	}
		//	//}

		//	template<typename TargetType> void Initialize()
		//	{
		//		Parent::Initialize<TargetType>();
		//		
		//		Setup_Link_Controls<NT>();
		//		//Setup_Advisory_Radios<NT>();
		//		Setup_Variable_Message_Signs<NT>();
		//		//Setup_Depots<NT>();
		//	}

		//	m_prototype(Antares_Layer<typename MasterType::antares_layer_type>,link_control_layer, NONE, NONE);
		//	m_prototype(Antares_Layer<typename MasterType::antares_layer_type>,advisory_radio_layer, NONE, NONE);
		//	m_prototype(Antares_Layer<typename MasterType::antares_layer_type>,variable_message_sign_layer, NONE, NONE);
		//	m_prototype(Antares_Layer<typename MasterType::antares_layer_type>,depot_layer, NONE, NONE);
		//};
	}

}

using namespace Traffic_Management_Center_Components::Implementations;
