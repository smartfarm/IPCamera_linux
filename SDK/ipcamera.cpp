#include<cstdio>
#include<cstring>
#include<iostream>
#include<fstream>
#include"HCNetSDK.h"
typedef void* PVOID;

using namespace std;

char sConfigPath[] ="..\\IPCameraconfig.ini";

//设置截图和录像的保存路径，注意要带后面的斜杠。需要先手动创建文件夹。
char sPicturePath[256];
char sRecordPath[256];

//配置参数（默认值）
int iCaptureNumber = 10; //抓图次数
int iCaptureCycleTime = 100; //抓图间隔，单位毫秒
int iRecordTime = 10000; //录制视频时间，单位毫秒
int iRecordNum = 2;	//录制视频数量

int main()
{
    //DWORD WINAPI IPCamera(PVOID);

	char temp[100];
    ifstream fin("../IPCameraconfig.ini", ios::in);
    if (fin.fail())
    {
        cout << "asd" << endl;
        return 0;
    }
    fin.getline(temp, 200);	//吃掉一行注释
cout << "haha";
	cout << temp << endl;
cout << "hehe";
	//读取配置参数
	fin.getline(temp, 200);

    sscanf(temp, "%d%d%d%d%d",&iCaptureNumber,&iCaptureCycleTime,&iRecordTime,&iRecordNum);

	cout << iCaptureNumber << "\t\t";
	cout << iCaptureCycleTime << "\t\t";
	cout << iRecordTime << "\t\t";
	cout << iRecordNum << endl;

	fin.getline(temp, 200);//吃掉一行注释
	cout << temp << endl;
	fin.getline(sPicturePath, 200);//截图保存路径
	cout << sPicturePath << endl;
	fin.getline(sRecordPath, 200);//录像保存路径
	cout << sRecordPath << endl;

	//读取设备信息
	int iDeviceNum;
	fin.getline(temp, 200);
	char temp2[200];
	sscanf(temp, "%s%d", temp2, &iDeviceNum);//获取设备数量
	cout << temp2 << ' ';
	cout << iDeviceNum << endl;

	char (*DeviceInfo)[200] = new char[iDeviceNum][200];

	fin.getline(temp, 200);//吃掉一行注释
	for (int i = 0; i < iDeviceNum; i++)
	{
		fin.getline(DeviceInfo[i], 200);
	}


	fin.close();


	//初始化
    //NET_DVR_Init();
	//设置连接时间与重连时间
    //NET_DVR_SetConnectTime(2000, 1);
    //NET_DVR_SetReconnect(10000, true);



}
/*
struct CAM
	{
		char* DeviceInfo;
		int n;
	};
	for (int i = 0; i < iDeviceNum; i++)
	{
		CAM t;
		t.DeviceInfo = DeviceInfo[i];
		t.n = i;
		hDevice[i] = CreateThread(0,0,IPCamera,(PVOID)(&t),0,0);
		Sleep(100);
	}

	cin >> temp;
	while(strcmp(temp, "exit"))
		cin >> temp;
	delete []DeviceInfo;
	delete []hDevice;
	return 0;

}

void IPCamera(PVOID p)
{
	LONG LoginIn(char sDeviceInfo[]);
    //DWORD WINAPI Capture(PVOID);
	//DWORD WINAPI Record(PVOID);
	struct CAM1
	{
		char* DeviceInfo;
		int n;
	};

	char *sDeviceInfo = ((CAM1*)p)->DeviceInfo;
	int n = ((CAM1*)p)->n;
	char IPAddress[20];
	sscanf(sDeviceInfo, "%s", IPAddress);
	LONG lUserID = LoginIn(sDeviceInfo);
	if (lUserID < 0)
	{
		return 0;
	}


	//启动预览
	NET_DVR_PREVIEWINFO struPlayInfo = {0};
	struPlayInfo.hPlayWnd = 0; //需要SDK解码时句柄设为有效值，仅取流不解码时可为空(设置回调函数)
	struPlayInfo.lChannel = 1; //预览通道号
	struPlayInfo.dwStreamType = 0; //0-主码流，1-子码流，2-码流 3，3-码流 4，以此类推
	struPlayInfo.dwLinkMode = 0; //0- TCP 方式，1- UDP 方式，2- 多播方式，3- RTP 方式，4-RTP/RTSP，5-RSTP/HTTP
	struPlayInfo.bBlocked = 1; //0- 非阻塞取流，1- 阻塞取流
	long lRealPlayHandle = NET_DVR_RealPlay_V40(lUserID, &struPlayInfo, NULL, NULL);//启动预览
	if (lRealPlayHandle < 0) //预览失败
	{
		cout<< "预览失败，错误码：" << NET_DVR_GetLastError() << endl;
		NET_DVR_Logout(lUserID);
		NET_DVR_Cleanup();
		return 0;
	}

	struct CAM2
	{
		LONG lRealPlayHandle;
		char *IPAddress;
		LONG lUserID;
		int n;
	};

	CAM2 param;
	param.IPAddress = IPAddress;
	param.lRealPlayHandle = lRealPlayHandle;
	param.lUserID = lUserID;
	param.n = n;

	HANDLE h1 = CreateThread(0,0,Capture,(PVOID)&param,0,0);
	//HANDLE h2 = CreateThread(0,0,Record,(PVOID)&param,0,0);
	Sleep(100);
}

LONG LoginIn(char sDeviceInfo[])
{
	LONG lUserID;
	char sCameraIP[16];
	char sCameraUsername[64];
	char sCameraPassword[64];
	sscanf(sDeviceInfo, "%s%s%s", sCameraIP, sCameraUsername, sCameraPassword);
	NET_DVR_DEVICEINFO_V30 struDeviceInfo;
	lUserID = NET_DVR_Login_V30(sCameraIP, 8000, sCameraUsername, sCameraPassword, &struDeviceInfo);
	if (lUserID < 0)
	{
		printf("login error, %d\n", NET_DVR_GetLastError());
		NET_DVR_Cleanup();
		return -1;
	}
	else
	{
		printf("login successed!\n");
		return lUserID;
	}
}

DWORD WINAPI Capture(PVOID param)
{
	struct CAM2
	{
		LONG lRealPlayHandle;
		char *IPAddress;
		LONG lUserID;
		int n;
	};

	LONG lRealPlayHandle = ((CAM2*)(param))->lRealPlayHandle;
	LONG lUserID = ((CAM2*)(param))->lUserID;
	char IPAddress[20];
	strcpy(IPAddress, ((CAM2*)(param))->IPAddress);
	int n = ((CAM2*)(param))->n;
	//启动抓图

	//判断路径是否存在
	char folder[20];
	if (n > 0)
	{
		sprintf(folder, "pictures_%d", n);
	}
	else
	{
		sprintf(folder,"pictures");
	}
	CString sTemp;
	sTemp.Format("%s%s\\",sPicturePath, folder);
	DWORD dwRet = GetFileAttributes(sTemp);
	if ((dwRet == -1) || !(dwRet & FILE_ATTRIBUTE_DIRECTORY))
	{
		CreateDirectory(sTemp, NULL);
	}

	//初始化JPEG抓图结构体
	NET_DVR_JPEGPARA struJpegPara;
	struJpegPara.wPicQuality = 5;
	struJpegPara.wPicSize = 1;


	char sPictureFilename[256];
	for (int i = 0; ; i = (i+1)%iCaptureNumber)
	{
		sprintf(sPictureFilename, "%s%s\\%02d.jpg",
			sPicturePath,folder,i);
		if (!NET_DVR_CaptureJPEGPicture(lUserID, 1, &struJpegPara, sPictureFilename))
		{
			cout << "抓图失败！" << endl;
			break;
		}
		cout << sPictureFilename << endl;
		Sleep(iCaptureCycleTime);
	}
	return 0;
}
*/


/*
TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

HEADERS += HCNetSDK.h LinuxPlayM4.h PlayM4.h
SOURCES += camera.cpp

linux:LIBS += -L ./ -lHCcore -lhcnetsdk -lhpr
linux:LIBS += -L ./HCNetSDKCom/ -lanalyzedata \
-lHCAlarm \
-lHCCoreDevCfg \
-lHCDisplay \
-lHCgeneralCfgMgr \
-lHCIndustry \
-lHCPlayBack \
-lHCPreview \
-lHCVoiceTalk \
-lStreamTransClient \
-lSystemTransform
        */
