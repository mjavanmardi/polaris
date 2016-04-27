#pragma once

#include "repository/Repository.h"
#include "myStuff.h"

// Most polaris functionality can be found contained within the polaris namespace
using namespace polaris;

namespace polaris
{
	namespace myAgent
	{
		// This is a very simple POLARIS component representing an agent in the simulation
		//     the specifics of this line will be explained in subsequent tutorials
		implementation struct AgentProto :public Polaris_Component<MasterType, PROTO_INHERIT(MasterType), Execution_Object>
		{
			AgentProto() { ; }

			cast_typed_accessor(double, x);
			m_data(double, x, NONE, NONE);

			cast_typed_accessor(double, y);
			m_data(double, y, NONE, NONE);

			cast_typed_accessor(double, z);
			m_data(double, z, NONE, NONE);

			// Initialize your agent
			void Initialize(double _x, double _y, double _z)
			{
				static_cast<MasterType*>(this)->Initialize(_x, _y, _z);
			}


			// Event functions follow a strict format
			static void Do_Stuff(AgentProto* _this, Event_Response& resp)
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

		template <typename Base, typename Derived>
		class AgentCloneable : public Base
		{
		public:
			using Base::Base;

			virtual Base *clone() const
			{
				return new Derived(static_cast<Derived const &>(*this));
			}

			static Derived* Allocate(int uuid = -1)
			{
				Derived* pAgent = polaris::Allocate<Derived>(uuid, false);
				Execution_Block* exec_block = pAgent->execution_block();
				new (pAgent) Derived();
				((Derived*)pAgent)->execution_block(exec_block);
				pAgent->_uuid = uuid;
				return pAgent;
			}
		};

		template<typename Derived = void>
		struct NewCloneableAgent : public AgentCloneable<AgentProto<NewCloneableAgent<Derived>>, NewCloneableAgent<Derived>>
		{
			NewCloneableAgent() : BaseClass() { ; }
			NewCloneableAgent(const NewCloneableAgent& agent) { ; }
			NewCloneableAgent<Derived>& operator=(const NewCloneableAgent<Derived>& agent) { return *this(agent); }
			~NewCloneableAgent() { ; }
			cast_typed_accessor(int, id);
			m_data(int, id, NONE, NONE);

			void Initialize(double _x, double _y, double _z)
			{
				Initialize_impl(_x, _y, _z, std::is_same<Derived, void>{});
			}

		private:
			typedef AgentCloneable<AgentProto<NewCloneableAgent>, NewCloneableAgent> BaseClass;
			friend struct AgentProto< NewCloneableAgent >;

			// Initialize your agent
			void Initialize_impl(double _x, double _y, double _z, std::false_type)
			{
				if (&NewCloneableAgent::Initialize_impl == &Derived::Initialize_impl)
					Initialize_impl(_x, _y, _z, std::true_type);
				else
					static_cast<Derived*>(this)->Initialize(_x, _y, _z);
			}

			void Initialize_impl(double _x, double _y, double _z, std::true_type)
			{
				// Tell the simulation engine what the agent should do (Do_Stuff)
				//    and when to first act (iteration 0, sub-iteration 0)
				x(_x);
				y(_y);
				z(_z);
				this->template Load_Event<NewCloneableAgent>(&Do_Stuff, 0, 0);
			}

			// Event functions follow a strict format
			static void Do_Stuff(NewCloneableAgent* _this, Event_Response& resp)
			{
				_this->x(calc(_this->x(), iteration()));
				_this->y(calc(_this->y(), iteration()));
				_this->z(calc(_this->z(), iteration()));

				// The response structure describes when the agent wants to go next
				//     the global iteration is accessible everywhere through iteration()
				//     the global sub_iteration is accessible everywhere through sub_iteration()
				resp.next._iteration = iteration() + 1;
				resp.next._sub_iteration = 0;
			}

			static double calc(double input, int iteration)
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

		struct ExtendedAgent : public AgentCloneable<NewCloneableAgent<ExtendedAgent>, ExtendedAgent>
		{
			ExtendedAgent() : BaseClass() { ; }
			ExtendedAgent(const ExtendedAgent& agent) { ; }
			ExtendedAgent& operator=(const ExtendedAgent& agent) { return *this; }
			~ExtendedAgent() { ; }

			void set_num_stuff(int num) { _num_stuff = num; }
			int num_stuff() { return _num_stuff; }

			void set_stuff_name(string name) { _stuff_name = name; }
			string stuff_name() { return _stuff_name; }

			// Initialize your agent
			void Initialize(double _x, double _y, double _z)
			{
				// Tell the simulation engine what the agent should do (Do_Stuff)
				//    and when to first act (iteration 0, sub-iteration 0)
				x(_x);
				y(_y);
				z(_z);
				this->template Load_Event<ExtendedAgent>(&Do_Stuff, 0, 0);
			}

		private:
			typedef AgentCloneable<NewCloneableAgent<ExtendedAgent>, ExtendedAgent> BaseClass;
			//typedef NewCloneableAgent<ExtendedAgent> BaseClass;
			friend struct AgentProto<ExtendedAgent>;

			int _num_stuff;
			string _stuff_name;

			// Event functions follow a strict format
			static void Do_Stuff(ExtendedAgent* _this, Event_Response& resp)
			{
				_this->x(calc(_this->x(), iteration()));
				_this->y(calc(_this->y(), iteration()));
				_this->z(calc(_this->z(), iteration()));

				// The response structure describes when the agent wants to go next
				//     the global iteration is accessible everywhere through iteration()
				//     the global sub_iteration is accessible everywhere through sub_iteration()
				resp.next._iteration = iteration() + 1;
				resp.next._sub_iteration = 0;
			}

			static double calc(double input, int iteration)
			{
				double out = input;
				for (int i = 0; i<1000; ++i)
				{
					out = pow(out, 2);
					out = pow(out, .5);
				}
				out += .01 * iteration / 2.0;
				return out;
			}
		};

		template<typename Derived = void>
		struct NewCloneableNamedAgent : public AgentCloneable<AgentProto<NewCloneableNamedAgent<Derived>>, NewCloneableNamedAgent<Derived>>
		{
			NewCloneableNamedAgent() : BaseClass() { ; }
			NewCloneableNamedAgent(const NewCloneableNamedAgent& agent) { ; }
			~NewCloneableNamedAgent() { ; }
			NewCloneableNamedAgent& operator=(const NewCloneableNamedAgent&) { ; }

			//simple_typed_accessor(string, id);
			//m_data(string, id, NONE, NONE);
			//std::string strId;
			void set_id(std::string id) { strId = id; }
			std::string id() { return strId; }

			cast_typed_accessor(myStuff, stuff);
			m_data(myStuff, stuff, NONE, NONE);

			void Initialize(double _x, double _y, double _z)
			{
				Initialize_impl(_x, _y, _z, std::is_same<Derived, void>{});
			}

		private:
			typedef AgentCloneable<AgentProto<NewCloneableNamedAgent<Derived>>, NewCloneableNamedAgent<Derived>> BaseClass;
			friend struct AgentProto< NewCloneableNamedAgent >;
			std::string strId;

			// Initialize your agent
			void Initialize_impl(double _x, double _y, double _z, std::false_type)
			{
				if (&NewCloneableNamedAgent::Initialize_impl == &Derived::Initialize_impl)
					Initialize_impl(_x, _y, _z, std::true_type);
				else
					static_cast<Derived*>(this)->Initialize(_x, _y, _z);
			}

			void Initialize_impl(double _x, double _y, double _z, std::true_type)
			{
				// Tell the simulation engine what the agent should do (Do_Stuff)
				//    and when to first act (iteration 0, sub-iteration 0)
				x(_x);
				y(_y);
				z(_z);
				this->template Load_Event<NewCloneableNamedAgent>(&Do_Stuff, 0, 0);
			}

			// Event functions follow a strict format
			static void Do_Stuff(NewCloneableNamedAgent* _this, Event_Response& resp)
			{
				_this->x(_this->stuff().calc(_this->x(), iteration()));
				_this->y(_this->stuff().calc(_this->y(), iteration()));
				_this->z(_this->stuff().calc(_this->z(), iteration()));

				// The response structure describes when the agent wants to go next
				//     the global iteration is accessible everywhere through iteration()
				//     the global sub_iteration is accessible everywhere through sub_iteration()
				resp.next._iteration = iteration() + 1;
				resp.next._sub_iteration = 0;
			}
		};

	};
};

