#pragma once
class EngineState
{
private :
	static EngineState* m_instance;
public :
	enum class State
	{
		Edition,
		RunTime,
		Pause,
	};
private:
	State m_state;
public:
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

