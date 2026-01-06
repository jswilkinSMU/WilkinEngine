#include "Engine/Core/JobSystem.hpp"
#include "Engine/Core/ErrorWarningAssert.hpp"

JobWorkerThread::JobWorkerThread(unsigned int workerThreadID, JobSystem* jobSystem)
	:m_jobWorkerID(workerThreadID),
	 m_jobSystem(jobSystem)
{
}

void JobWorkerThread::ThreadMain()
{
	/* If executing, that thread is unaffected 
	   else wakes up when notified
	   while still pending jobs
			lock both pending and executing jobs
			call job -> Execute()
			lock executing and completed jobs
			move from executing to completed
	*/

	while (m_jobSystem->m_isRunning)
	{
		Job* job = nullptr;

		// Lock and wait for signal that work is available
		{
			std::unique_lock<std::mutex> lock(m_jobSystem->m_jobMutex);

			// Wait until there are pending jobs or if our jobsystem is shutting down
			m_jobSystem->m_jobAvailableCondition.wait(lock, [this]()
			{
					return !m_jobSystem->m_pendingJobs.empty() || !m_jobSystem->m_isRunning;
			});

			// Check if jobsystem is shutting down
			if (!m_jobSystem->m_isRunning)
			{
				return;
			}

			// If a job is available from pending queue, claim it
			if (!m_jobSystem->m_pendingJobs.empty())
			{
				job = m_jobSystem->m_pendingJobs.front();
				m_jobSystem->m_pendingJobs.pop_front();
				m_jobSystem->m_executingJobs.push_back(job);
			}
		}

		// Execute the job outside of lock
		if (job != nullptr)
		{
			job->Execute();

			// Move job from executing to completed
			{
				std::unique_lock<std::mutex> lock(m_jobSystem->m_jobMutex);

				auto& executingJob = m_jobSystem->m_executingJobs;
				auto foundJob = std::find(executingJob.begin(), executingJob.end(), job);

				// Remove the job from executing queue
				if (foundJob != executingJob.end())
				{
					executingJob.erase(foundJob);
				}

				// Add to completed job queue
				m_jobSystem->m_completedJobs.push_back(job);
			}

			// Yield to let any other threads run
			std::this_thread::yield();
		}
	}
}
// -----------------------------------------------------------------------------
JobSystem::JobSystem(JobSystemConfig jobSystemConfig)
	:m_config(jobSystemConfig)
{
}

void JobSystem::StartUp()
{
	// Flag JobSystem on and running
	m_isRunning = true;

	for (int workerIndex = 0; workerIndex < m_config.m_numJobWorkers; ++workerIndex)
	{
		JobWorkerThread* worker = new JobWorkerThread(workerIndex, this);
		m_workerThreadObjects.push_back(worker);
		m_workerThreads.emplace_back(&JobWorkerThread::ThreadMain, worker);
	}
}

void JobSystem::Shutdown()
{
	// Flag JobSystem off and notify all the threads
	m_isRunning = false;
	m_jobAvailableCondition.notify_all();

	/* On Shutdown, wait for all jobs to complete and/or ensure no jobs queued anywhere */
	for (int workerIndex = 0; workerIndex < static_cast<int>(m_workerThreads.size()); ++workerIndex)
	{
		if (m_workerThreads[workerIndex].joinable())
		{
			m_workerThreads[workerIndex].join();
		}
	}

	// Clear the worker thread objects
	for (int workerObjIndex = 0; workerObjIndex < static_cast<int>(m_workerThreadObjects.size()); ++workerObjIndex)
	{
		delete m_workerThreadObjects[workerObjIndex];
		m_workerThreadObjects[workerObjIndex] = nullptr;
	}
	m_workerThreadObjects.clear();

	// Assert all our queues are cleared
	ASSERT_OR_DIE(m_pendingJobs.empty(),   "Pending jobs remain at Shutdown!");
	ASSERT_OR_DIE(m_executingJobs.empty(), "Executing jobs remain at Shutdown!");
	ASSERT_OR_DIE(m_completedJobs.empty(), "Completed jobs were not retreived at Shutdown!");

	// Clear the worker threads
	m_workerThreads.clear();
}

void JobSystem::BeginFrame()
{
}

void JobSystem::EndFrame()
{
}

void JobSystem::AddJobToSystem(Job* job)
{
	// Lock pending jobs mutex
	std::unique_lock<std::mutex> lock(m_jobMutex);

	// Push back into pending jobs
	m_pendingJobs.push_back(job);

	// Notify the condition variable
	m_jobAvailableCondition.notify_all();
}

Job* JobSystem::RetreiveCompletedJob()
{
	std::unique_lock<std::mutex> lock(m_jobMutex);

	if (m_completedJobs.empty())
	{
		return nullptr;
	}

	// Remove job from completed jobs and return it to be called by 
	// some place as AddJob so Job can be deleted
	Job* job = m_completedJobs.back();
	m_completedJobs.pop_back();
	return job;
}

void JobSystem::CancelPendingJob(Job* job)
{
	std::unique_lock<std::mutex> lock(m_jobMutex);

	auto foundJob = std::find(m_pendingJobs.begin(), m_pendingJobs.end(), job);
	if (foundJob != m_pendingJobs.end())
	{
		m_pendingJobs.erase(foundJob);
	}
}
