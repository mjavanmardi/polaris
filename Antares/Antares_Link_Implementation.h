//*********************************************************
//	Graphical_Network_Implementation.h - Graphical Link Variant
//*********************************************************

#pragma once
#include "Antares_Includes.h"
#include "User_Space\Polaris_Link_Implementation.h"

void* _global_graphical_link_moe_group;

namespace Link_Components
{
	namespace Types
	{
		enum BAR_COLOR
		{
			RED_BAR, GREEN_BAR, BLUE_BAR
		};
//#pragma pack(push,1)
//		implementation struct Bar : public Polaris_Component<APPEND_CHILD(Bar),MasterType,NULLTYPE>
//		{
//			Zone_Components::Types::Quad<MasterType> quad;
//
//			Bar(Point_3D<MasterType> pointA, Point_3D<MasterType> pointB, int height, BAR_COLOR primary_color)
//			{
//
//				
//
//				if (primary_color == GREEN_BAR)
//				{
//					quad.color._g = 255;
//				}
//				if (primary_color == RED_BAR)
//				{
//					quad.color._r = 255;
//				}
//				if (primary_color == BLUE_BAR)
//				{
//					quad.color._b = 255;
//				}
//
//				quad.a._x = pointA._x;
//				quad.a._y = pointA._y;
//				quad.a._z = 1;
//				quad.b._x = pointB._x;
//				quad.b._y = pointB._y;
//				quad.b._z = 1;
//				quad.c._x = pointB._x;
//				quad.c._y = pointB._y;
//				quad.c._z = height;
//				quad.d._x = pointA._x;
//				quad.d._y = pointA._y;
//				quad.d._z = height;
//			}
//			feature_implementation void Push_To_Layer(TargetType Layer_Reference)
//			{
//				Layer_Reference->Push_Element<Regular_Element>(this);
//			}
//		};
//#pragma pack(pop)
	}

	namespace Concepts
	{
	}
	
	namespace Implementations
	{

#pragma pack(push,1)
		struct My_Bar
		{
			True_Color_RGBA<NT> color;
			Point_3D<NT> a;
			Point_3D<NT> b;
			Point_3D<NT> c;
			Point_3D<NT> d;
		};
#pragma pack(pop)

		implementation struct Antares_Link_Implementation:public Polaris_Link_Implementation<MasterType,ParentType,APPEND_CHILD(Antares_Link_Implementation)>
		{
			static void configure_link_moes_layer()
			{
				// configure vehicle layer
				cout << "configuring link bars layer" << endl;

				_link_bars=Allocate_New_Layer< typename MasterType::type_of(canvas),NT,Target_Type< NULLTYPE,Antares_Layer<type_of(link_bars),ComponentType>*, string& > >(string("Link Bars"));
				Antares_Layer_Configuration cfg;
				cfg.Configure_Dynamic_Quads(True_Color_RGBA<NULLTYPE>(0,255,100,255),10);
				cfg.attributes_schema = string("Id,Type");
				cfg.storage_period = 6;
				cfg.storage_offset = 5;
				cfg.primitive_color = true;
				cfg.primitive_normal = true;
				//cfg.dynamic_data = true;
				//cfg.storage_period = 300;
				//cfg.target_sub_iteration = Types::ZONE_UPDATE_SUBITERATION+1;
				//cfg.storage_offset = 60/*_iteration*/;
				//cfg.storage_size = 3;
				//cfg.primitive_color = true;

				//cfg.attributes_callback = (attributes_callback_type)&Antares_Link_Implementation<MasterType>::fetch_attributes;
				//cfg.submission_callback = (attributes_callback_type)&Antares_Link_Implementation<MasterType>::submit_attributes;
				_link_bars->Initialize<NULLTYPE>(cfg);
			}

			//member_component(typename MasterType::graphical_link_moe_group_type,graphical_link_moe_group,none,none);
			//static member_prototype(Graphical_Link_MOE_Group,graphical_link_moe_group,typename MasterType::graphical_link_moe_group_type,none,none);
			feature_implementation void visualize_moe()
			{
				Push_To_Link_MOE_Display<ComponentType,CallerType,TargetType>();
			}

			typedef Link_Prototype<typename MasterType::link_type> _Link_Interface;
			feature_implementation void Push_To_Link_MOE_Display()
			{
				//cout << endl << "at zone push_to_display";
				define_component_interface(_Intersection_Interface, type_of(upstream_intersection), Intersection_Components::Prototypes::Intersection_Prototype, ComponentType);
				//define_component_interface(_Graphical_Link_MOE_Group_Interface, type_of(graphical_link_moe_group), Link_Components::Prototypes::Graphical_Link_MOE_Group, ComponentType);
				//typedef Link_Components::Prototypes::Graphical_Link_MOE_Group<MasterType::graphical_link_moe_group_type> _Graphical_Link_MOE_Group_Interface;
				Point_3D<MasterType> u_node;
				Point_3D<MasterType> d_node;
			
				u_node._x=upstream_intersection<ComponentType,CallerType,_Intersection_Interface*>()->template x_position<float>();
				u_node._y=upstream_intersection<ComponentType,CallerType,_Intersection_Interface*>()->template y_position<float>();
				u_node._z=0;
				Scale_Coordinates<typename MasterType::type_of(canvas),NT,Target_Type<NULLTYPE,void,Point_3D<MasterType>&>>(u_node);

				d_node._x=downstream_intersection<ComponentType,CallerType,_Intersection_Interface*>()->template x_position<float>();
				d_node._y=downstream_intersection<ComponentType,CallerType,_Intersection_Interface*>()->template y_position<float>();
				d_node._z=0;
				Scale_Coordinates<typename MasterType::type_of(canvas),NT,Target_Type<NULLTYPE,void,Point_3D<MasterType>&>>(d_node);
				
				int height = 100;

				My_Bar bar;

				bar.color._r = 0;
				bar.color._g = 255;
				bar.color._b = 0;
				bar.color._a = 255;

				bar.a._x = u_node._x;
				bar.a._y = u_node._y;
				bar.a._z = 1;
				bar.b._x = d_node._x;
				bar.b._y = d_node._y;
				bar.b._z = 1;
				bar.c._x = d_node._x;
				bar.c._y = d_node._y;
				bar.c._z = height;
				bar.d._x = u_node._x;
				bar.d._y = u_node._y;
				bar.d._z = height;

				_link_bars->Push_Element<Regular_Element>((void*)&bar);
			}

			static bool fetch_attributes(Antares_Link_Implementation* _this,vector<string>& bucket)
			{
				stringstream s;
				
				s << _this->_internal_id;
				bucket.push_back(s.str());
				s.str("");
				
				switch(_this->_link_type)
				{
				case Types::Link_Type_Keys::ARTERIAL:
					bucket.push_back("ARTERIAL");
					break;
				case Types::Link_Type_Keys::EXPRESSWAY:
					bucket.push_back("EXPRESSWAY");
					break;
				case Types::Link_Type_Keys::FREEWAY:
					bucket.push_back("FREEWAY");
					break;
				case Types::Link_Type_Keys::OFF_RAMP:
					bucket.push_back("OFF_RAMP");
					break;
				case Types::Link_Type_Keys::ON_RAMP:
					bucket.push_back("ON_RAMP");
					break;
				default:
					bucket.push_back("NON_RECOGNIZED");
					break;
				}
				return true;
			}
			
			static bool submit_attributes(Antares_Link_Implementation* _this,vector<string>& bucket)
			{
				// not supported yet
				return true;
			}

			static member_prototype(Antares_Layer,link_bars,typename type_of(MasterType::antares_layer),none,none);
		};
		template<typename MasterType,typename ParentType,typename InheritanceList>
		Antares_Layer<typename type_of(MasterType::antares_layer),Antares_Link_Implementation<MasterType,ParentType,InheritanceList>>* Antares_Link_Implementation<MasterType,ParentType,InheritanceList>::_link_bars;

	}
}

using namespace Link_Components::Implementations;