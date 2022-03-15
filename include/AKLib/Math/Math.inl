
NSB(AKLib)

NSB(Math)

template <typename T>
AKL_INLINE T Interpolate(T goal, T current, T dt) 
{
	T diff = goal - current;

	if (diff > dt)
		return current + dt;
	else if (diff < -dt)
		return current - dt;

	return goal;
}

NSE()

NSE()