// DumpControllerProperties.cpp : Defines the entry point for the console application.
//


#include "openvr.h"
#include "openvr_string.h"

#include <stdio.h>
#include <vector>

using namespace vr;
using namespace openvr_string;

ETrackedDeviceProperty string_properties[] = 
{
	Prop_TrackingSystemName_String				,
	Prop_ModelNumber_String						,
	Prop_SerialNumber_String					,
	Prop_RenderModelName_String					,
	Prop_ManufacturerName_String				,
	Prop_TrackingFirmwareVersion_String			,
	Prop_HardwareRevision_String				,
	Prop_AllWirelessDongleDescriptions_String	,
	Prop_ConnectedWirelessDongle_String			,
	Prop_Firmware_ManualUpdateURL_String		,
	Prop_Firmware_ProgrammingTarget_String		,
	Prop_DriverVersion_String                   ,
	Prop_ResourceRoot_String					,
	Prop_RegisteredDeviceType_String			,
	Prop_InputProfilePath_String				, // input profile to use for this device in the input system. Will default to tracking system name if this isn't provided
	
	// Properties that are unique to TrackedDeviceClass_Controller
	Prop_AttachedDeviceId_String				,
	
	// Properties that are unique to drivers
	Prop_UserConfigPath_String					,
	Prop_InstallPath_String						,
	
	// Properties that are set internally based on other information provided by drivers
	Prop_ControllerType_String					,

};

ETrackedDeviceProperty int32_properties[] =
{
	Prop_DeviceClass_Int32
};


// returns a list of indexes that have a Prop_DeviceClass_Int32 set
std::vector<TrackedDeviceIndex_t> GetIndexesThatHaveDeviceClassAssigned()
{
	std::vector<TrackedDeviceIndex_t> indexes_to_dump;
	for (TrackedDeviceIndex_t index = 0; index < k_unMaxTrackedDeviceCount; index++)
	{
		ETrackedPropertyError prop_err;
		int device_class = VRSystem()->GetInt32TrackedDeviceProperty(index, Prop_DeviceClass_Int32, &prop_err);

		if (prop_err == TrackedProp_Success)
		{
			indexes_to_dump.push_back(index);
		}
	}
	return indexes_to_dump;
}

void dump_all_controllers_with_any_device_class()
{
	EVRInitError err;
	IVRSystem *sys = VR_Init(&err, VRApplication_Scene);

	if (err == VRInitError_None)
	{
		std::vector<TrackedDeviceIndex_t> indexes_to_dump = GetIndexesThatHaveDeviceClassAssigned();
		for (int i = 0; i < indexes_to_dump.size(); i++)
		{
			TrackedDeviceIndex_t index = indexes_to_dump[i];
			ETrackedPropertyError prop_err;
			int device_class = VRSystem()->GetInt32TrackedDeviceProperty(index, Prop_DeviceClass_Int32, &prop_err);

			printf("index: %d\n", index);
			char buf[256];
			for (int i = 0; i < sizeof(string_properties) / sizeof(string_properties[0]); i++)
			{
				VRSystem()->GetStringTrackedDeviceProperty(index, string_properties[i], buf, sizeof(buf), &prop_err);
				printf(" {%s , \"%s\"},\n", ETrackedDevicePropertyToString(string_properties[i]), buf);
			}
			for (int i = 0; i < sizeof(int32_properties) / sizeof(int32_properties[0]); i++)
			{
				int32_t prop = VRSystem()->GetInt32TrackedDeviceProperty(index, int32_properties[i], &prop_err);
				printf(" {%s , \"%d\"},\n", ETrackedDevicePropertyToString(int32_properties[i]), prop);
			}
		}
	}
}


int main()
{
	dump_all_controllers_with_any_device_class();
    return 0;
}

