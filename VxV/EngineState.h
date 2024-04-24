#pragma once
class EngineState
{
private:
	static EngineState* m_instance;
public:
	enum class BootingState {
		Edition,
		Initializing,
		Ready,
		Starting,
		Running,
		AsktoStop,
		Stopping,
		Stopped
	};

	enum class ActiveState {
		Edition,
		RunTime,
		Pause
	};
private:
	BootingState m_bootingState;
	ActiveState m_state;
public:
	static EngineState* GetInstance() {
		if (m_instance == nullptr) {
			m_instance = new EngineState();
		}
		return m_instance;
	}
	EngineState() {
		m_bootingState = BootingState::Edition;
		m_state = ActiveState::Edition;
	}
	~EngineState() {
		delete m_instance;
	}

	//Set Booting State

	void StartBooting() {
		m_bootingState = BootingState::Initializing;
	}

	void ReadyBooting() {
		m_bootingState = BootingState::Ready;
	}

	void StartRunning() {
		m_bootingState = BootingState::Starting;
	}

	void Running() {
		m_bootingState = BootingState::Running;
	}

	void AskToStop() {
		m_bootingState = BootingState::AsktoStop;
	}

	void Stopping() {
		m_bootingState = BootingState::Stopping;
	}

	void Stopped() {
		m_bootingState = BootingState::Stopped;
	}

	//Set Active State

	void StartRunTime() {
		m_state = ActiveState::RunTime;
	}

	void ExitRunTime() {
		m_state = ActiveState::Edition;
	}

	void PauseRunTime() {
		m_state = ActiveState::Pause;
	}

	//Get Booting State
	ActiveState const GetActiveState() {
		return m_state;
	}


	//Get Active State
	BootingState const GetBootingState() {
		return m_bootingState;
	}
};

