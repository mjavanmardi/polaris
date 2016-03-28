#pragma once

#include "repository/Repository.h"
#include "myStuff.h"

// Most polaris functionality can be found contained within the polaris namespace
using namespace polaris;

namespace polaris
{
	namespace myAgent
	{
		//need to derive from ComponentType if you want to use the interface to iterate correctly
		prototype struct myAgent : ComponentType
		{
			// required tag to identify this class as a polaris prototype
			tag_as_prototype;

			// since want x,y&z to be doubles (always) we declare as typed (double)
			// this makes it easier to set/get obj->x() as opposed to obj->x<double>()
			typed_accessor(double, x);
			typed_accessor(double, y);
			typed_accessor(double, z);

			// data type for id depends on implementation tho
			accessor(id, NONE, NONE);
			accessor(stuff, NONE, NONE);
			accessor(not_implemented, NONE, NONE);

			// function call, passed through to implementation
			template<typename IdType> void Initialize(double x, double y, double z, IdType id)
			{
				this_component()->Initialize(x, y, z, id);
			}

			template<typename IdType> void get_id(IdType& id)
			{
				this_component()->get_id(id);
			}

			string id_desc()
			{
				return this_component()->id_desc();
			}

			template <typename StuffType> StuffType Get_Stuff(int id)
			{
				return this_component()->Get_Stuff<StuffType>(id);
			}
			template<typename StuffType> double stuff_calc()
			{
				return this_component()->stuff_calc();
			}
		};

		// This is a very simple POLARIS component representing an agent in the simulation
		//     the specifics of this line will be explained in subsequent tutorials
		implementation struct Agent :public Polaris_Component<MasterType, INHERIT(Agent), Execution_Object>
		{
			simple_typed_accessor(double, x);
			m_data(double, x, NONE, NONE);

			simple_typed_accessor(double, y);
			m_data(double, y, NONE, NONE);

			simple_typed_accessor(double, z);
			m_data(double, z, NONE, NONE);

			simple_typed_accessor(int, id);
			m_data(int, id, NONE, NONE);

			simple_typed_accessor(myStuff, stuff);
			m_data(myStuff, stuff, NONE, NONE);
			//myStuff stuff;

			// Initialize your agent
			void Initialize(double _x, double _y, double _z, int _id)
			{
				// Tell the simulation engine what the agent should do (Do_Stuff)
				//    and when to first act (iteration 0, sub-iteration 0)
				x(_x);
				y(_y);
				z(_z);
				id(_id);
				this->template Load_Event<Agent>(&Do_Stuff, 0, 0);
			}

			void get_id(int& _id)
			{
				_id = id();
			}

			string id_desc()
			{
				return boost::str(boost::format("%d") % id());
			}

			// Event functions follow a strict format
			static void Do_Stuff(Agent* _this, Event_Response& resp)
			{
				Agent<MasterType>* pthis = (Agent<MasterType>*)_this;
				pthis->x(pthis->stuff().calc(pthis->x(), iteration()));
				pthis->y(pthis->stuff().calc(pthis->y(), iteration()));
				pthis->z(pthis->stuff().calc(pthis->z(), iteration()));

				// The response structure describes when the agent wants to go next
				//     the global iteration is accessible everywhere through iteration()
				//     the global sub_iteration is accessible everywhere through sub_iteration()
				resp.next._iteration = iteration() + 1;
				resp.next._sub_iteration = 0;
			}
		};

		implementation struct Named_Agent :public Polaris_Component<MasterType, INHERIT(Named_Agent), Execution_Object>
		{
			simple_typed_accessor(double,x);
			m_data(double,x, NONE, NONE);

			simple_typed_accessor(double,y);
			m_data(double,y, NONE, NONE);

			simple_typed_accessor(double,z);
			m_data(double,z, NONE, NONE);

			simple_typed_accessor(string,id);
			m_data(string,id, NONE, NONE);

			void Initialize(double _x, double _y, double _z, string _id)
			{
				// Tell the simulation engine what the agent should do (Do_Stuff)
				//    and when to first act (iteration 0, sub-iteration 0)
				x(_x);
				y(_y);
				z(_z);
				id(_id);
				this->template Load_Event<Named_Agent>(&Do_Stuff, 0, 0);
			}

			void get_id(string& _id)
			{
				_id = id();
			}

			string id_desc()
			{
				return id();
			}

			// Event functions follow a strict format
			static void Do_Stuff(Named_Agent* _this, Event_Response& resp)
			{
				Named_Agent<MasterType>* pthis = (Named_Agent<MasterType>*)_this;
				pthis->x(pthis->calc(pthis->x(), iteration()));
				pthis->y(pthis->calc(pthis->y(), iteration()));
				pthis->z(pthis->calc(pthis->z(), iteration()));

				// The response structure describes when the agent wants to go next
				//     the global iteration is accessible everywhere through iteration()
				//     the global sub_iteration is accessible everywhere through sub_iteration()
				resp.next._iteration = iteration() + 1;
				resp.next._sub_iteration = 0;
			}

			double calc(double input, int iteration)
			{
				double out = input;
				for (int i = 0; i<1000; ++i)
				{
					out = pow(out, 2);
					out = pow(out, .5);
				}
				out += .01 * iteration;
				return out;
			}
		};
#if (_MSC_VER != 1600)
		template<typename MT>using agent_type = Agent<MT>;
		template<typename MT>using agent_named_type = Named_Agent<MT>;
		template<typename AgentType> using my_agent_itf = myAgent<AgentType>;
#endif
	};
};