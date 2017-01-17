#ifndef _IOS_BRIDGE_H_
#define _IOS_BRIDGE_H_

#ifdef WIN32
#define IOS_BRIDGE_API	__declspec( dllexport )
#else
#define IOS_BRIDGE_API	
#endif // WIN32


IOS_BRIDGE_API int takeScreenshot();

#endif
