#include <iostream>
#include <thread>
#include <string>
#include <mutex>
#include <chrono>
#include <cmath>
#include <future>

class Timer
{
public:
	Timer()
	{
		Reset();
	}
	void Reset()
	{
		m_Start = std::chrono::high_resolution_clock::now();
	}
	float Elapsed()const
	{
		float deltaTime = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now() - m_Start).count()
			* 0.000000001f;
		return deltaTime;
	}
	float ElapsedMillis()
	{
		return this->Elapsed() * 1000.0f;
	}
private:
	std::chrono::time_point<std::chrono::high_resolution_clock> m_Start;
};

std::mutex mtx;
void GetPhysicalDevProperties2(void* devices, uint32_t& size)
{
	std::lock_guard<std::mutex> lock(mtx);
	std::string* strDevices = static_cast<std::string*>(devices);

	for (uint32_t i = 0; i < size; i++)
	{
		std::cout << strDevices[i] << std::endl;
	}
	int counter = -1000000;
	while (counter < 10000000)
	{
		int x = 12;
		int calc = (std::sin(x) * std::cos(x * x * x)) * std::tan(x * 2);
		counter++;
	}
}

void PickDevice(uint32_t deviceCount)
{
	constexpr uint32_t MaxDevSize{ 10u };
	
	std::string devices[MaxDevSize]
	{ 
		"Device1", "Device2", "Device3", "Device4", "Device5", 
		"Device6", "Device7", "Device8", "Device9", "Device10",
	};
	std::thread t1;

	/* Seperated thread! */
	Timer t;

	// Run the task asynchronously
	auto future = std::async(std::launch::async, GetPhysicalDevProperties2,
		static_cast<void*>(devices), std::ref(deviceCount));

	// Wait for the result with a timeout of 2 seconds
	if (future.wait_for(std::chrono::seconds(2)) == std::future_status::timeout)
	{
		std::cout << "Thread execution timed out after 2 seconds. Exiting program." << std::endl;
		return;
	}

	std::cout << "Seconds: " << t.Elapsed() <<"s" << std::endl;
}

int main()
{
	uint32_t pickDevCount{ 5u };

	PickDevice(pickDevCount);
	unsigned int numThreads = std::thread::hardware_concurrency();
	std::cout << "Hardware concurrency (logical cores): " << numThreads << std::endl;
	return 0;
}