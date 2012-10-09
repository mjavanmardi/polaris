#pragma once
#include "Basic_Concepts.h"

///============================================================================
/// add a group to the group_list
///============================================================================

template<typename ComponentType,typename Added_Group>
struct Add_Group:public ComponentType
{
	assert_requirements(ComponentType,Is_Polaris_Component,"Cannot add group to non-POLARIS component!");
	typedef typename RemoveDuplicates<typename Append<typename ComponentType::Group_List,Added_Group>::Result>::Result Group_List;
	typedef Add_Group This_Type;
};

///============================================================================
/// remove a group from the group_list
///============================================================================

template<typename ComponentType,typename Removed_Group>
struct Remove_Group:public ComponentType
{
	assert_requirements(ComponentType,Is_Polaris_Component,"Cannot remove group from non-POLARIS component!");
	typedef typename Erase<typename ComponentType::Group_List,Removed_Group>::Result Group_List;
	typedef Remove_Group This_Type;
};

///============================================================================
/// check whether a group is in the current group_list
///============================================================================

template<typename ComponentType,typename Checked_Group>
struct Check_Group
{
	assert_requirements(ComponentType,Is_Polaris_Component,"Cannot check group in non-POLARIS component!");
	typedef typename ValidIndex<IndexOf<typename ComponentType::Group_List,Checked_Group>::value>::type type;
};
