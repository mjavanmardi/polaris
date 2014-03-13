#ifdef _DEBUG
#define SHOW_WARNINGS
#endif

#include "Polaris_PCH.h"

#define TEST_APP
#ifdef TEST_APP

prototype struct Agent
{
	tag_as_prototype;

	accessor(message,NONE,NONE);

	template<typename T> void Initialize()
	{
		this_component()->Initialize<T>();
	}	

	accessor(agent_data,NONE,NONE);

};

prototype struct Agent_Data
{
	tag_as_prototype;

	accessor(val1,NONE,NONE);
	accessor(val2,NONE,NONE);
};

prototype struct Other
{
	tag_as_prototype;

	accessor(message,NONE,NONE);
	accessor(my_agent,NONE,NONE);
	accessor(my_data,NONE,NONE);

	template<typename T> void Initialize()
	{
		/*typedef Agent<typename get_type_of(my_agent)> agent_itf;
		typedef Agent_Data<typename agent_itf::get_type_of(agent_data)> data_itf;

		agent_itf* the_agent = this->my_agent<agent_itf*>();
		data_itf* my_data = the_agent->agent_data<data_itf*>();

		cout << "VALUE 1 IS: " <<my_data->val1<int>() <<endl<<endl;*/
		this_component()->Initialize<T>();
	}	



};

implementation struct Choice_Option_1 : public Choice_Model_Components::Implementations::Choice_Option_Base<MasterType, INHERIT(Choice_Option_1)>
{
	virtual double Calculate_Utility()
	{
		return 1.0;
	}
	virtual void Print_Utility()
	{
		cout<<"Option 1 utility = 1.0"<<endl;
	}
};
implementation struct Choice_Option_2 : public Choice_Model_Components::Implementations::Choice_Option_Base<MasterType, INHERIT(Choice_Option_2)>
{
	virtual double Calculate_Utility()
	{
		return 2.0;
	}
	virtual void Print_Utility()
	{
		cout<<"Option 2 utility = 2.0"<<endl;
	}
};


implementation struct Base_Agent_Implementation : public Polaris_Component<MasterType,INHERIT(Base_Agent_Implementation),Execution_Object>
{
	typedef typename Polaris_Component<MasterType,INHERIT(Base_Agent_Implementation),Execution_Object>::Component_Type ComponentType;
	typedef Agent<ComponentType> agent_itf;

	typedef double name1;
	typedef double name2;

	m_prototype(Agent_Data, typename MasterType::agent_data_type, agent_data, NONE, NONE);

	template<typename T> void Initialize()
	{
		this->message(string("Test_Agent_Implementation"));
		Load_Event<ComponentType>(&ComponentType::Agent_Event,1,0);			
	}
	static void Agent_Event(ComponentType* _this,Event_Response& response)
	{
		agent_itf* _this_ptr=(agent_itf*)_this;
		
		if (iteration() % 100 == 0)
		{
			cout <<_this_ptr->message<string>() << ": The iteration is: "<< iteration() <<". The time is: " << GLOBALS::Simulation_Time.Current_Time<Time_Minutes>()<<" minutes."<<endl;
			response.next._iteration=iteration()+1;
			response.next._sub_iteration = 0;
		}
		
		else
		{
			response.next._iteration=iteration()+1;
			response.next._sub_iteration = 0;
		}
	}

	m_data(string, message,NONE,NONE);
};

implementation struct Inherited_Agent_Implementation : public Base_Agent_Implementation<MasterType, INHERIT(Inherited_Agent_Implementation)>
{
	template<typename T> void Initialize()
	{
		this->message(string("Inherited_Agent_Implementation"));
		Load_Event<ComponentType>(&ComponentType::Agent_Event,1,0);			
	}
};

implementation struct Agent_Data_Implementation : public Polaris_Component<MasterType,INHERIT(Agent_Data_Implementation)>
{
	m_data(int,val1,NONE,NONE);
	m_data(int,val2,NONE,NONE);
};

implementation struct Other_Implementation : public Polaris_Component<MasterType,INHERIT(Other_Implementation),Execution_Object>
{
	typedef typename Polaris_Component<MasterType,INHERIT(Other_Implementation),Execution_Object>::Component_Type ComponentType;
	typedef Other<ComponentType> agent_itf;

	typedef double name1;
	typedef double name2;

	m_prototype(Agent, typename MasterType::agent_type, my_agent, NONE, NONE);

	template<typename T> void Initialize()
	{
		typedef Agent<typename type_of(my_agent)> agent_itf;
		typedef Agent_Data<typename agent_itf::get_type_of(agent_data)> data_itf;
		//typedef Agent_Data<typename type_of(agent_itf::Component_Type::agent_data)> data_itf;

		agent_itf* the_agent = this->my_agent<agent_itf*>();
		data_itf* my_data = the_agent->agent_data<data_itf*>();

		cout << "VALUE 1 IS: " <<my_data->val1<int>() <<endl<<endl;

		/*this->message(string("Test_Agent_Implementation"));
		Load_Event<ComponentType>(&ComponentType::Agent_Event,1,0);*/			
	}
	static void Agent_Event(ComponentType* _this,Event_Response& response)
	{
		agent_itf* _this_ptr=(agent_itf*)_this;
		
		if (iteration() % 100 == 0)
		{
			cout <<_this_ptr->message<string>() << ": The iteration is: "<< iteration() <<". The time is: " << GLOBALS::Simulation_Time.Current_Time<Time_Minutes>()<<" minutes."<<endl;
			response.next._iteration=iteration()+1;
			response.next._sub_iteration = 0;
		}
		
		else
		{
			response.next._iteration=iteration()+1;
			response.next._sub_iteration = 0;
		}
	}

	m_data(string, message,NONE,NONE);
};


struct MasterType
{
	typedef Inherited_Agent_Implementation<MT> agent_type;
	typedef Agent_Data_Implementation<MT> agent_data_type;
	typedef Other_Implementation<MT> other_type;

	typedef Choice_Option_1<MT> choice_1_type;
	typedef Choice_Option_2<MT> choice_2_type;
};


int main(int argc, char* argv[])
{
	Simulation_Configuration cfg;
	cfg.Single_Threaded_Setup(1000);
	INITIALIZE_SIMULATION(cfg);

	//==================================================================================================================================
	// Choice MOdel Test
	//---------------------------------------------------------------------------------------------------------------------------------- 
	GLOBALS::Normal_RNG.Initialize();
	GLOBALS::Uniform_RNG.Initialize();
	GLOBALS::Normal_RNG.Set_Seed<int>();
	GLOBALS::Uniform_RNG.Set_Seed<int>();

	typedef Choice_Model_Components::Prototypes::Choice_Option<MasterType::choice_1_type> choice_1_itf;
	typedef Choice_Model_Components::Prototypes::Choice_Option<MasterType::choice_2_type> choice_2_itf;
	typedef Choice_Model_Components::Prototypes::Choice_Model<Choice_Model_Components::Implementations::MNL_Model_Implementation<MasterType>> _Choice_Model_Interface;

	_Choice_Model_Interface* choice_model = (_Choice_Model_Interface*)Allocate<Choice_Model_Components::Implementations::MNL_Model_Implementation<MasterType> >();
	choice_1_itf* choice_1 = (choice_1_itf*)Allocate<MasterType::choice_1_type>();
	choice_2_itf* choice_2 = (choice_2_itf*)Allocate<MasterType::choice_2_type>();

	choice_model->Add_Choice_Option<choice_1_itf*>(choice_1);
	choice_model->Add_Choice_Option<choice_2_itf*>(choice_2);

	int choice_id;

	for (int i=0; i<18; i++)
	{
		choice_model->Evaluate_Choices<NT>();
		choice_model->Choose<choice_1_itf*>(choice_id);
		cout <<"Choice = " << choice_id<<endl;
	}

	//==================================================================================================================================
	// Start test
	//---------------------------------------------------------------------------------------------------------------------------------- 
	typedef Agent<MasterType::agent_type> agent_itf;
	agent_itf* my_agent = (agent_itf*)Allocate<MasterType::agent_type>();

	typedef Agent_Data<MasterType::agent_data_type> data_itf;
	data_itf* my_data = (data_itf*)Allocate<MasterType::agent_data_type>();

	typedef Other<MasterType::other_type> other_itf;
	other_itf* my_other = (other_itf*)Allocate<MasterType::other_type>();

	

	my_agent->Initialize<NT>();
	my_agent->agent_data(my_data);

	my_other->my_agent<agent_itf*>(my_agent);

	my_other->Initialize<NT>();

	START();

	char ans;
	cout<<endl<<endl<<"done. Press 'any' key to exit.";
	cin >> ans;
}
#endif


#define HIDE
#ifndef HIDE

#include "Population_Synthesis.h"
#include "Scenario_Implementation.h"


struct MasterType
{
	typedef Scenario_Components::Implementations::Scenario_Implementation<MasterType> scenario_type;
	typedef polaris::Basic_Units::Implementations::Length_Implementation<MasterType> length_type;
	typedef polaris::Basic_Units::Implementations::Time_Implementation<MasterType> time_type;
	typedef PopSyn::Implementations::Synthesis_Zone_Implementation_Simple<MasterType> synthesis_zone_type;
	typedef PopSyn::Implementations::Synthesis_Region_Implementation_Simple<MasterType> synthesis_region_type;
	typedef PopSyn::Implementations::IPF_Solver_Settings_Implementation<MasterType> ipf_solver_settings_type;
	typedef PopSyn::Implementations::ADAPTS_Population_Synthesis_Implementation<MasterType> population_synthesis_type;
	typedef PopSyn::Implementations::Popsyn_File_Linker_Implementation<MasterType> popsyn_file_linker_type;
	typedef Person_Components::Implementations::ACS_Person_Static_Properties_Implementation<MasterType> person_static_properties_type;
	typedef Household_Components::Implementations::ACS_Household_Static_Properties_Implementation<MasterType> household_static_properties_type;
	typedef RNG_Components::Implementations::MT_Probability_Double<MasterType> rng_type;
	typedef NULLCOMPONENT household_type;
	typedef NULLTYPE person_type;
	typedef NULLTYPE network_type;
};


int main(int argc, char* argv[])
{
	Simulation_Configuration cfg;
	cfg.Single_Threaded_Setup(1000);
	INITIALIZE_SIMULATION(cfg);


	//==================================================================================================================================
	// Scenario initialization
	//----------------------------------------------------------------------------------------------------------------------------------
	char* scenario_filename = "scenario.json";
	if (argc >= 2) scenario_filename = argv[1];
	typedef Scenario_Components::Prototypes::Scenario<MasterType::scenario_type> _Scenario_Interface;
	_Scenario_Interface* scenario=(_Scenario_Interface*)Allocate<MasterType::scenario_type>();
	cout << "reading scenario data..." <<endl;
	scenario->read_scenario_data<Scenario_Components::Types::ODB_Scenario>(scenario_filename);


	//==================================================================================================================================
	// Initialize global randon number generators - if seed set to zero or left blank use system time
	//---------------------------------------------------------------------------------------------------------------------------------- 
	int seed = scenario->iseed<int>();
	GLOBALS::Normal_RNG.Initialize();
	GLOBALS::Uniform_RNG.Initialize();
	if (seed != 0)
	{
		GLOBALS::Normal_RNG.Set_Seed<int>(seed);
		GLOBALS::Uniform_RNG.Set_Seed<int>(seed);
	}
	else
	{
		GLOBALS::Normal_RNG.Set_Seed<int>();
		GLOBALS::Uniform_RNG.Set_Seed<int>();
	}

	//==================================================================================================================================
	// Start population synthesis
	//---------------------------------------------------------------------------------------------------------------------------------- 
	typedef PopSyn::Prototypes::Population_Synthesizer<MasterType::population_synthesis_type> popsyn_itf;
	popsyn_itf* popsyn = (popsyn_itf*)Allocate<MasterType::population_synthesis_type>();
	popsyn->Initialize<_Scenario_Interface*>(scenario);

	START();

	char ans;
	cout<<endl<<endl<<"done. Press 'any' key to exit.";
	cin >> ans;
}
#endif