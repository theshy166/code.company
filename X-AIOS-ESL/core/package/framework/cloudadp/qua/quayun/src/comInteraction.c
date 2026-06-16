#include "comInteraction.h"
#include "quaMqtt.h"

extern MqttInfo g_MqttInfo;
char szSubscriberBuffer[1024] = { 0 };
char szDvFuciton[50] = { 0 };
char szdeviceinfo[50] = { 0 };
char szOTA[50] = { 0 };
char szProperty[50] = { 0 };
char szPropertyOnline[50] = { 0 };
char szFunctionOnline[50] = { 0 };
char szDeviceMonitor[50] = { 0 };
char szDeviceNtp[50] = { 0 };
char pbDvInfo[50] = { 0 };
char pbDvProperty[50] = { 0 };
char pbDvFuc[50] = { 0 };
char pbDvEvent[50] = { 0 };
char pbDvNtp[50] = { 0 };
char pbDvMonitor[50] = { 0 };
char pbFrameoTest[50] = { 0 };
char pbIpcTest[50] = { 0 };
char pbIpcHart[50] = {0};

 const char* getSubscriberDeviceInformation()
{
    memset(szdeviceinfo,0x0,sizeof(szdeviceinfo));
	snprintf(szdeviceinfo, 49, "/%d/%s/info/get", g_MqttInfo.deviceType, g_MqttInfo.deviceCode);
	return szdeviceinfo;
}

 const char* getSubscriberDeviceInOta()
{
	memset(szOTA,0x0,sizeof(szOTA));
	snprintf(szOTA, 49, "/%d/%s/ota/get", g_MqttInfo.deviceType, g_MqttInfo.deviceCode);
	return szOTA;
}

 const char* getSubscriberDeviceProperty()
{
	memset(szProperty,0x0,sizeof(szProperty));
	snprintf(szProperty, 49, "/%d/%s/property/get", g_MqttInfo.deviceType, g_MqttInfo.deviceCode);
	return szProperty;
}

 const char* getSubscriberDevicePropertyOnline()
{
	memset(szPropertyOnline,0x0,sizeof(szPropertyOnline));
	snprintf(szPropertyOnline, 49, "/%d/%s/property-online/get", g_MqttInfo.deviceType, g_MqttInfo.deviceCode);
	return szPropertyOnline;
}

 const char* getSubscriberDeviceFunction()
{
	memset(szDvFuciton,0x0,sizeof(szDvFuciton));
	snprintf(szDvFuciton, 49, "/%d/%s/function/get", g_MqttInfo.deviceType, g_MqttInfo.deviceCode);
	return szDvFuciton;
}

 const char* getSubscriberDeviceFunctionOnline()
{
	memset(szFunctionOnline,0x0,sizeof(szFunctionOnline));
	snprintf(szFunctionOnline, 49, "/%d/%s/function-online/get", g_MqttInfo.deviceType, g_MqttInfo.deviceCode);
	return szFunctionOnline;
}

 const char* getSubscriberDeviceMonitor()
{
	memset(szDeviceMonitor,0x0,sizeof(szDeviceMonitor));
	snprintf(szDeviceMonitor, 49, "/%d/%s/monitor/get", g_MqttInfo.deviceType, g_MqttInfo.deviceCode);
	return szDeviceMonitor;
}

 const char* getSubscriberDeviceNtp()
{
	memset(szDeviceNtp,0x0,sizeof(szDeviceNtp));
	snprintf(szDeviceNtp, 49, "/%d/%s/ntp/get", g_MqttInfo.deviceType, g_MqttInfo.deviceCode);
	return szDeviceNtp;
}

 const char* getPublishDeviceInformation()
{
	memset(pbDvInfo,0x0,sizeof(pbDvInfo));
	snprintf(pbDvInfo, 49, "/%d/%s/info/post", g_MqttInfo.deviceType, g_MqttInfo.deviceCode);
	return pbDvInfo;
}

 const char* getPublishDeviceProperty()
{
	memset(pbDvProperty,0x0,sizeof(pbDvProperty));
	snprintf(pbDvProperty, 49, "/%d/%s/property/post", g_MqttInfo.deviceType, g_MqttInfo.deviceCode);
	return pbDvProperty;
}

 const char* getPublishDeviceFunction()
{
	memset(pbDvFuc,0x0,sizeof(pbDvFuc));
	snprintf(pbDvFuc, 49, "/%d/%s/function/post", g_MqttInfo.deviceType, g_MqttInfo.deviceCode);
	return pbDvFuc;
}

 const char* getPublishDeviceEvent()
{
	memset(pbDvEvent,0x0,sizeof(pbDvEvent));
	snprintf(pbDvEvent, 49, "/%d/%s/event/post", g_MqttInfo.deviceType, g_MqttInfo.deviceCode);
	return pbDvEvent;
}

 const char* getPublishDeviceNtp()
{
	memset(pbDvNtp,0x0,sizeof(pbDvNtp));
	snprintf(pbDvNtp, 49, "/%d/%s/ntp/post", g_MqttInfo.deviceType, g_MqttInfo.deviceCode);
	return pbDvNtp;
}

 const char* getPublishMonitorProperty()
{
	memset(pbDvMonitor,0x0,sizeof(pbDvMonitor));
	snprintf(pbDvMonitor, 49, "/%d/%s/monitor/post", g_MqttInfo.deviceType, g_MqttInfo.deviceCode);
	return pbDvMonitor;
}

 const char* getFrameoMqttTest()
{
	memset(pbFrameoTest,0x0,sizeof(pbFrameoTest));
	snprintf(pbFrameoTest, 49, "/%d/%s/frameo/event", g_MqttInfo.deviceType, g_MqttInfo.deviceCode);
	return pbFrameoTest;
}

 const char* getIpcMqttTest()
{
	memset(pbIpcTest,0x0,sizeof(pbIpcTest));
	snprintf(pbIpcTest, 49, "/%d/%s/ipc/event", g_MqttInfo.deviceType, g_MqttInfo.deviceCode);
	return pbIpcTest;
}

const char* getIpcHartInfo()
{
	memset(pbIpcHart,0x0,sizeof(pbIpcHart));
	//printf(">>>>>>>>>>>>>>>>>>type: %d, %s,\n", g_MqttInfo.deviceType, g_MqttInfo.appPhone);
	snprintf(pbIpcHart, 49, "/%d/%s/ipcapp/event", g_MqttInfo.deviceType, g_MqttInfo.appPhone);
	return pbIpcHart;
}

const char* getSubscriberAll()
{
	
	snprintf(szSubscriberBuffer, sizeof(szSubscriberBuffer) - 1, "%s|%s|%s|%s|%s",
		getSubscriberDeviceInformation(), //1
		//getSubscriberDeviceProperty(), //2
		//getSubscriberDevicePropertyOnline(), //3
		getSubscriberDeviceFunction(), //4
		//getSubscriberDeviceFunctionOnline(), //5
		getSubscriberDeviceMonitor(), //6
		//getSubscriberDeviceNtp() //7
		getFrameoMqttTest(), //8
		getIpcMqttTest() //9
	);
	return szSubscriberBuffer;
}