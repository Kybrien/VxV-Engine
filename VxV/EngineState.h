#pragma once

/**
 * @class EngineState
 * @brief Represents the state of the engine, including booting and active states.
 */
class EngineState
{
private:
	static EngineState* m_instance;
public:
	/**
	 * @brief Enum representing the different booting states of the engine.
	 */
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

	/**
	* @brief Enum representing the active states of the engine.
	*/
	enum class ActiveState {
		Edition = 0,
		RunTime = 1,
		Pause = 2
	};
private:
	BootingState m_bootingState;
	ActiveState m_state;
public:
	/**
	 * @brief Gets the singleton instance of the EngineState class.
	 * @return The singleton instance of EngineState.
	 */
	static EngineState* GetInstance() {
		if (m_instance == nullptr) {
			m_instance = new EngineState();
		}
		return m_instance;
	}

	/**
	* @brief Constructor for the EngineState class.
	*/
	EngineState() {
		m_bootingState = BootingState::Edition;
		m_state = ActiveState::Edition;
	}

	/**
	* @brief Destructor for the EngineState class.
	*/
	~EngineState() {
		delete m_instance;
	}

	/**
	* @brief Sets the booting state to Initializing.
	*/
	void StartBooting() {
		m_bootingState = BootingState::Initializing;
	}

	/**
	 * @brief Sets the booting state to Ready.
	 */
	void ReadyBooting() {
		m_bootingState = BootingState::Ready;
	}

	/**
	 * @brief Sets the booting state to Starting.
	 */
	void StartRunning() {
		m_bootingState = BootingState::Starting;
	}

	/**
	 * @brief Sets the booting state to Running.
	 */
	void Running() {
		m_bootingState = BootingState::Running;
	}

	/**
	 * @brief Sets the booting state to AsktoStop.
	 */
	void AskToStop() {
		m_bootingState = BootingState::AsktoStop;
	}

	/**
	 * @brief Sets the booting state to Stopping.
	 */
	void Stopping() {
		m_bootingState = BootingState::Stopping;
	}

	/**
	* @brief Sets the booting state to Stopped.
	*/
	void Stopped() {
		m_bootingState = BootingState::Stopped;
	}

	/**
	 * @brief Sets the active state to RunTime.
	 */
	void StartRunTime() {
		m_state = ActiveState::RunTime;
	}

	/**
	 * @brief Sets the active state to Edition.
	 */
	void ExitRunTime() {
		m_state = ActiveState::Edition;
	}

	/**
	 * @brief Sets the active state to Pause.
	 */
	void PauseRunTime() {
		m_state = ActiveState::Pause;
	}

	/**
	 * @brief Gets the current active state of the engine.
	 * @return The current active state.
	 */
	ActiveState const GetActiveState() {
		return m_state;
	}

	/**
	 * @brief Gets the current booting state of the engine.
	 * @return The current booting state.
	 */
	BootingState const GetBootingState() {
		return m_bootingState;
	}
};

