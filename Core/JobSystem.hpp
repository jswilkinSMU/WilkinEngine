#pragma once
#include <vector>
#include <deque>
#include <mutex>
// -----------------------------------------------------------------------------
class JobSystem;
// -----------------------------------------------------------------------------
struct JobSystemConfig
{
	int m_numJobWorkers = -1;
};
// -----------------------------------------------------------------------------
class Job
{
public:
	virtual ~Job() = default;
	virtual void Execute() = 0;
};
// -----------------------------------------------------------------------------
class JobWorkerThread
{
public:
	JobWorkerThread(unsigned int workerThreadID, JobSystem* jobSystem);
	void ThreadMain(); // Has its own entry function e.g. void JobWorkerThread::ThreadMain();

private:
	unsigned int m_jobWorkerID = 0;
	JobSystem*   m_jobSystem = nullptr;
};
// -----------------------------------------------------------------------------
class JobSystem
{
public:
	JobSystem(JobSystemConfig jobSystemConfig);

	void StartUp();
	void Shutdown();
	void BeginFrame();
	void EndFrame();

	void AddJobToSystem(Job* job);
	Job* RetreiveCompletedJob();
	void CancelPendingJob(Job* job);

public:
	JobSystemConfig m_config;

	std::deque<Job*> m_pendingJobs;     // Keeps a std::deque of pending jobs waiting to be claimed by a worker thread.
	std::deque<Job*> m_executingJobs;  // Keeps a std::deque of executing jobs currently being executed by worker threads.
	std::deque<Job*> m_completedJobs; // Keeps a std::deque of completed jobs waiting to be retrieved by the main thread.

	std::mutex m_jobMutex;
	std::condition_variable m_jobAvailableCondition;

	std::vector<std::thread> m_workerThreads;
	std::vector<JobWorkerThread*> m_workerThreadObjects;

	std::atomic<bool> m_isRunning = false;
};
// -----------------------------------------------------------------------------