#pragma once
class EngineState
{
private :
	enum class State
	{
		Edition,
		RunTime,
		Pause,
	};
	State m_state;
	static EngineState* m_instance;
public :
	static EngineState* GetInstance() {
		if (m_instance == nullptr) {
			m_instance = new EngineState();
		}
		return m_instance;
	}
	EngineState() {
		m_state = State::Edition;
	}
	~EngineState() {
				delete m_instance;
	}

	void StartRunTime() {
		m_state = State::RunTime;
	}

	void ExitRunTime() {
		m_state = State::Edition;
	}

	void PauseRunTime() {
		m_state = State::Pause;
	}


	State GetState() {
		return m_state;
	}
};

