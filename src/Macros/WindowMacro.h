#pragma once

#define NEONWND_EXCEPT( hr ) Window::HrException( __LINE__,__FILE__,(hr) )
#define NEONWND_LAST_EXCEPT() Window::HrException( __LINE__,__FILE__,GetLastError() )
#define NEONWND_NOGFX_EXCEPT() Window::NoGfxException( __LINE__,__FILE__ )

