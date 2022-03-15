

NSB(AKLib)

AKL_INLINE DllLoader::DllLoader(const char* dllName) 
	: isOpen{ false }, name { dllName }, handle{ NULL }
{
}

AKL_INLINE DllLoader::~DllLoader()
{
	if (isOpen) 
	{
		Close();
	}
}

AKL_INLINE bool DllLoader::Open() 
{
	if (isOpen) 
	{
		return true;
	}

	handle = AKLIB_DLL_OPEN(name);

	if (handle != NULL) 
	{
		isOpen = true;
		return true;
	}

	return false;
}

AKL_INLINE bool DllLoader::Close()
{
	if (!isOpen)
	{
		return true;
	}
	
	if (AKLIB_DLL_CLOSE(handle))
	{
		return false;
	}

	handle = NULL;
	isOpen = false;
	return true;
}

AKL_INLINE bool DllLoader::IsOpen() 
{
	return isOpen;
}

template <typename FuncType>
AKL_INLINE bool DllLoader::GetFunction(const char* name, FuncType* func) 
{
	if (!isOpen || func == NULL) 
	{
		return false;
	}

	*func = (FuncType)AKLIB_DLL_GET_FUNC(name, handle);

	return *func != NULL;
}

NSE()