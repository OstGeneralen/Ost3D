#include "Ost3D.h"
#include "OstEngine.h"

void ost::Ost3D_Run_Win32(OstEngineApp& appInstance, HINSTANCE windowsInstance, LPSTR args, int cmdShow)
{
	OstEngine engine(windowsInstance, args, cmdShow);
	engine.InitializeAndRun(appInstance);
}
