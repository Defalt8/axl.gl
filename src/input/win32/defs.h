#pragma once
#define WIN32_LEAN_AND_MEAN 1
#include <windows.h>
#ifndef WM_MOUSEHWHEEL
#	define WM_MOUSEHWHEEL 0x020E 
#endif
#ifndef GET_X_LPARAM
#	define GET_X_LPARAM(x) ((SHORT)LOWORD(x))
#endif
#ifndef GET_Y_LPARAM
#	define GET_Y_LPARAM(y) ((SHORT)HIWORD(y))
#endif
#ifndef MAPVK_VK_TO_VSC
#	define MAPVK_VK_TO_VSC 0
#endif
#ifndef MAPVK_VSC_TO_VK
#	define MAPVK_VSC_TO_VK 1
#endif
#ifndef MAPVK_VK_TO_CHAR
#	define MAPVK_VK_TO_CHAR 2
#endif
#ifndef MAPVK_VSC_TO_VK_EX
#	define MAPVK_VSC_TO_VK_EX 3
#endif
#ifndef VK_XBUTTON1
#	define VK_XBUTTON1 0x05
#endif
#ifndef VK_XBUTTON2
#	define VK_XBUTTON2 0x06
#endif
#ifndef VK_VOLUME_MUTE
#	define VK_VOLUME_MUTE 0xAD
#endif
#ifndef VK_VOLUME_DOWN
#	define VK_VOLUME_DOWN 0xAE
#endif
#ifndef VK_VOLUME_UP
#	define VK_VOLUME_UP 0xAF
#endif
#ifndef VK_MEDIA_NEXT_TRACK
#	define VK_MEDIA_NEXT_TRACK 0xB0
#endif
#ifndef VK_MEDIA_PREV_TRACK
#	define VK_MEDIA_PREV_TRACK 0xB1
#endif
#ifndef VK_MEDIA_STOP
#	define VK_MEDIA_STOP 0xB2
#endif
#ifndef VK_MEDIA_PLAY_PAUSE
#	define VK_MEDIA_PLAY_PAUSE 0xB3
#endif
#ifndef VK_LAUNCH_MAIL
#	define VK_LAUNCH_MAIL 0xB4
#endif
#ifndef VK_LAUNCH_MEDIA_SELECT
#	define VK_LAUNCH_MEDIA_SELECT 0xB5
#endif
#ifndef VK_LAUNCH_APP1
#	define VK_LAUNCH_APP1 0xB6
#endif
#ifndef VK_LAUNCH_APP2
#	define VK_LAUNCH_APP2 0xB7
#endif
#ifndef WM_POINTERDOWN
#	define WM_POINTERDOWN 0x0246
#endif
#ifndef WM_POINTERUP
#	define WM_POINTERUP 0x0247
#endif
#ifndef WM_POINTERUPDATE
#	define WM_POINTERUPDATE 0x0245
#endif
#ifndef GET_POINTERID_WPARAM
#	define GET_POINTERID_WPARAM(wparam) (wparam & 0xFFFF)
#endif
#ifndef IS_POINTER_PRIMARY_WPARAM
#	define IS_POINTER_PRIMARY_WPARAM(wparam) (wparam & 0x20000000)
#endif
#ifndef DMDO_DEFAULT
#	define DMDO_DEFAULT 0
#endif
#ifndef DMDO_90
#	define DMDO_90 1
#endif
#ifndef DMDO_180
#	define DMDO_180 2
#endif
#ifndef DMDO_270
#	define DMDO_270 3
#endif
#ifndef DM_BITSPERPEL
#	define DM_BITSPERPEL 262144
#endif
#ifndef DM_PELSWIDTH
#	define DM_PELSWIDTH 524288
#endif
#ifndef DM_PELSHEIGHT
#	define DM_PELSHEIGHT 1048576
#endif
#ifndef DM_DISPLAYFREQUENCY
#	define DM_DISPLAYFREQUENCY 4194304
#endif
#ifndef DM_DISPLAYORIENTATION
#	define DM_DISPLAYORIENTATION 128
#endif
#ifndef DISP_CHANGE_SUCCESSFUL
#	define DISP_CHANGE_SUCCESSFUL 0
#endif
#ifndef RIM_TYPEKEYBOARD
#	define RIM_TYPEKEYBOARD 1
#endif
#ifndef RIM_TYPEMOUSE
#	define RIM_TYPEMOUSE 0
#endif
#ifndef RIM_TYPEHID
#	define RIM_TYPEHID 2
#endif
typedef struct tagRAWINPUTDEVICELIST {
  HANDLE hDevice;
  DWORD  dwType;
} RAWINPUTDEVICELIST, *PRAWINPUTDEVICELIST;
typedef struct tagRID_DEVICE_INFO_HID {
  DWORD  dwVendorId;
  DWORD  dwProductId;
  DWORD  dwVersionNumber;
  USHORT usUsagePage;
  USHORT usUsage;
} RID_DEVICE_INFO_HID, *PRID_DEVICE_INFO_HID;
typedef struct tagRID_DEVICE_INFO_KEYBOARD {
  DWORD dwType;
  DWORD dwSubType;
  DWORD dwKeyboardMode;
  DWORD dwNumberOfFunctionKeys;
  DWORD dwNumberOfIndicators;
  DWORD dwNumberOfKeysTotal;
} RID_DEVICE_INFO_KEYBOARD, *PRID_DEVICE_INFO_KEYBOARD;
typedef struct tagRID_DEVICE_INFO_MOUSE {
  DWORD dwId;
  DWORD dwNumberOfButtons;
  DWORD dwSampleRate;
  BOOL  fHasHorizontalWheel;
} RID_DEVICE_INFO_MOUSE, *PRID_DEVICE_INFO_MOUSE;
typedef struct tagRID_DEVICE_INFO {
  DWORD cbSize;
  DWORD dwType;
  union {
    RID_DEVICE_INFO_MOUSE    mouse;
    RID_DEVICE_INFO_KEYBOARD keyboard;
    RID_DEVICE_INFO_HID      hid;
  } DUMMYUNIONNAME;
} RID_DEVICE_INFO, *PRID_DEVICE_INFO, *LPRID_DEVICE_INFO;
extern WINAPI UINT GetRawInputDeviceList(
  PRAWINPUTDEVICELIST pRawInputDeviceList,
  PUINT               puiNumDevices,
  UINT                cbSize
);