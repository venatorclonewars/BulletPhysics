class Time
{

public:
	Time()
	{
		startTimeMilliseconds = getCurrentTimeInMillis();
	}

	long long getCurrentTimeInMillis()
	{

		return GetTickCount();
	}

	long long getElapsedTimeInMillis()
	{
		return getCurrentTimeInMillis() - startTimeMilliseconds;
	}



	float deltaTime;
	long long startTimeMilliseconds = 0;
};
