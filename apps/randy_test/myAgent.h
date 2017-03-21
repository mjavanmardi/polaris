#pragma once

#include "repository/Repository.h"
#include "myStuff.h"

namespace polaris
{
    namespace myAgent
    {
        // This is a very simple POLARIS component representing an agent in the simulation
        //     the specifics of this line will be explained in subsequent tutorials
        implementation struct AgentProto :public Polaris_Component<MasterType, PROTO_INHERIT(MasterType), Execution_Object>
        {
            AgentProto() { ; }
            AgentProto(const AgentProto& agent) : _x(agent.x()), _y(agent.y()), _z(agent.z()) { ; }
            AgentProto& operator=(const AgentProto& agent) { x(agent.x()), y(agent.y()), z(agent.z()); return *this; }
            ~AgentProto() { ; }

            void x(double val) { _x = val; }
            double x() const { return _x; }

            void y(double val) { _y = val; }
            double y() const { return _y; }

            void z(double val) { _z = val; }
            double z() const { return _z; }

            // Initialize your agent
            void Initialize(double _x, double _y, double _z)
            {
                static_cast<MasterType*>(this)->Initialize(_x, _y, _z);
            }


            // Event functions follow a strict format
            static void Do_Stuff(AgentProto* _this, Event_Response& resp)
            {
                AgentProto<MasterType>* pthis = (AgentProto<MasterType>*)_this;
                pthis->x(pthis->stuff().calc(pthis->x(), iteration()));
                pthis->y(pthis->stuff().calc(pthis->y(), iteration()));
                pthis->z(pthis->stuff().calc(pthis->z(), iteration()));

                // The response structure describes when the agent wants to go next
                //     the global iteration is accessible everywhere through iteration()
                //     the global sub_iteration is accessible everywhere through sub_iteration()
                resp.next._iteration = iteration() + 1;
                resp.next._sub_iteration = 0;
            }
        private:
            double _x;
            double _y;
            double _z;
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
            NewCloneableAgent<Derived>() : BaseClass() { ; }
            NewCloneableAgent<Derived>(const NewCloneableAgent<Derived>& agent) : BaseClass(agent), _id(agent.id()) { ; }
            NewCloneableAgent<Derived>& operator=(const NewCloneableAgent<Derived>& agent) { BaseClass::operator=(agent); id(agent.id()); return *this; }
            ~NewCloneableAgent<Derived>() { ; }

            void Initialize(double _x, double _y, double _z)
            {
                Initialize_impl(_x, _y, _z, std::is_same<Derived, void>{});
            }

            void id(int val) { _id = val; }
            double id() const { return _id; }

        private:
            typedef AgentCloneable<AgentProto<NewCloneableAgent<Derived>>, NewCloneableAgent<Derived>> BaseClass;
            friend struct AgentProto< NewCloneableAgent<Derived> >;

            // Initialize your agent
            void Initialize_impl(double _x, double _y, double _z, std::false_type)
            {
                if (&NewCloneableAgent::Initialize_impl == &Derived::Initialize_impl)
                    Initialize_impl(_x, _y, _z, std::true_type());
                else
                    static_cast<Derived*>(this)->Initialize(_x, _y, _z);
            }

            void Initialize_impl(double _x, double _y, double _z, std::true_type)
            {
                // Tell the simulation engine what the agent should do (Do_Stuff)
                //    and when to first act (iteration 0, sub-iteration 0)
                this->x(_x);
                this->y(_y);
                this->z(_z);
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

            int _id;
        };

        struct ExtendedAgent : public AgentCloneable<NewCloneableAgent<ExtendedAgent>, ExtendedAgent>
        {
            ExtendedAgent() : BaseClass() { ; }
            ExtendedAgent(const ExtendedAgent& agent) : BaseClass(agent) { ; }
            ExtendedAgent& operator=(const ExtendedAgent& agent) { BaseClass::operator=(agent);num_stuff(agent.num_stuff()); stuff_name(agent.stuff_name()); return *this; }
            ~ExtendedAgent() { ; }

            void num_stuff(int num) { _num_stuff = num; }
            int num_stuff() const { return this->_num_stuff; }

            void stuff_name(string name) { _stuff_name = name; }
            string stuff_name() const { return this->_stuff_name; }

            // Initialize your agent
            void Initialize(double _x, double _y, double _z)
            {
                // Tell the simulation engine what the agent should do (Do_Stuff)
                //    and when to first act (iteration 0, sub-iteration 0)
                this->x(_x);
                this->y(_y);
                this->z(_z);
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
            NewCloneableNamedAgent<Derived>() : BaseClass() { ; }
            NewCloneableNamedAgent<Derived>(const NewCloneableNamedAgent<Derived>& agent) : BaseClass(agent), _id(agent.id()) { ; }
            NewCloneableNamedAgent<Derived>& operator=(const NewCloneableNamedAgent<Derived>& agent) { BaseClass::operator=(agent); id(agent.id()); return *this; }
            ~NewCloneableNamedAgent<Derived>() { ; }

            //simple_typed_accessor(string, id);
            //m_data(string, id, NONE, NONE);
            //std::string strId;
            void id(std::string val) { _id = val; }
            std::string id() const { return _id; }

            myStuff& stuff() { return _stuff; }

            void Initialize(double _x, double _y, double _z)
            {
                Initialize_impl(_x, _y, _z, std::is_same<Derived, void>{});
            }

        private:
            typedef AgentCloneable<AgentProto<NewCloneableNamedAgent<Derived>>, NewCloneableNamedAgent<Derived>> BaseClass;
            friend struct AgentProto< NewCloneableNamedAgent >;

            // Initialize your agent
            void Initialize_impl(double _x, double _y, double _z, std::false_type)
            {
                if (&NewCloneableNamedAgent::Initialize_impl == &Derived::Initialize_impl)
                    Initialize_impl(_x, _y, _z, std::true_type());
                else
                    static_cast<Derived*>(this)->Initialize(_x, _y, _z);
            }

            void Initialize_impl(double _x, double _y, double _z, std::true_type)
            {
                // Tell the simulation engine what the agent should do (Do_Stuff)
                //    and when to first act (iteration 0, sub-iteration 0)
                this->x(_x);
                this->y(_y);
                this->z(_z);
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

            std::string _id;
            myStuff _stuff;

        };

    };
};

