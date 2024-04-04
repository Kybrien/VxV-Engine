#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <vector>
#include <iostream>
#include <stdlib.h>
#include <fstream>
#include <string>

void ShowInfo() {
	// Créez une nouvelle fenêtre ImGui
	ImGui::Begin("Informations", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav);

	// Obtenez le nombre de FPS
	float fps = ImGui::GetIO().Framerate;

	// Affichez le nombre de FPS
	ImGui::Text("FPS: %.1f", fps);

	// Obtenez la position de la souris
	ImVec2 mousePos = ImGui::GetIO().MousePos;

	// Affichez la position de la souris
	ImGui::Text("Position de la souris: (%.1f, %.1f)", mousePos.x, mousePos.y);

	// Terminez la fenêtre
	ImGui::End();
}

void ShowHelp() {
	// Créez une nouvelle fenêtre ImGui
	ImGui::Begin("Aide", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav);

	// Affichez le texte d'aide
	ImGui::Text("Utilisez les touches fléchées pour déplacer la caméra.");
	ImGui::Text("Utilisez la molette de la souris pour zoomer.");
	ImGui::Text("Utilisez la touche 'R' pour réinitialiser la caméra.");
	ImGui::Text("Utilisez la touche 'H' pour afficher/cacher cette fenêtre.");

	// Terminez la fenêtre
	ImGui::End();
}

void ShowAbout() {
	// Créez une nouvelle fenêtre ImGui
	ImGui::Begin("À propos", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav);

	// Affichez le texte d'information
	ImGui::Text("Ce programme a été créé par:");
	ImGui::Text("  - Alexandre Boucher");
	ImGui::Text("  - Gabriel Lemyre");
	ImGui::Text("  - Samuel Lavoie");

	// Terminez la fenêtre
	ImGui::End();
}

void ShowMenu() {
	// Créez une nouvelle fenêtre ImGui
	ImGui::Begin("Menu", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav);

	// Ajoutez un bouton pour afficher les informations
	if (ImGui::Button("Informations")) {
		ShowInfo();
	}

	// Ajoutez un bouton pour afficher l'aide
	if (ImGui::Button("Aide")) {
		ShowHelp();
	}

	// Ajoutez un bouton pour afficher les informations
	if (ImGui::Button("À propos")) {
		ShowAbout();
	}

	// Terminez la fenêtre
	ImGui::End();
}

void ShowError(const std::string& message) {
	// Créez une nouvelle fenêtre ImGui
	ImGui::Begin("Erreur", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav);

	// Affichez le message d'erreur
	ImGui::Text(message.c_str());

	// Terminez la fenêtre
	ImGui::End();
}

void ShowSuccess(const std::string& message) {
	// Créez une nouvelle fenêtre ImGui
	ImGui::Begin("Succès", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav);

	// Affichez le message de succès
	ImGui::Text(message.c_str());

	// Terminez la fenêtre
	ImGui::End();
}


void ShowWarning(const std::string& message) {
	// Créez une nouvelle fenêtre ImGui
	ImGui::Begin("Avertissement", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav);

	// Affichez le message d'avertissement
	ImGui::Text(message.c_str());

	// Terminez la fenêtre
	ImGui::End();
}


void ShowConfirmation(const std::string& message, bool& show) {
	// Créez une nouvelle fenêtre ImGui
	ImGui::Begin("Confirmation", &show, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav);

	// Affichez le message de confirmation
	ImGui::Text(message.c_str());

	// Ajoutez un bouton pour confirmer
	if (ImGui::Button("Confirmer")) {
		show = false;
	}

	// Ajoutez un bouton pour annuler
	if (ImGui::Button("Annuler")) {
		show = false;
	}

	// Terminez la fenêtre
	ImGui::End();
}



void ShowInputInt(const std::string& message, int& input, bool& show) {
	// Créez une nouvelle fenêtre ImGui
	ImGui::Begin("Entrée", &show, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav);

	// Affichez le message d'entrée
	ImGui::Text(message.c_str());

	// Ajoutez un champ de texte pour l'entrée
	ImGui::InputInt("", &input);

	// Ajoutez un bouton pour confirmer
	if (ImGui::Button("Confirmer")) {
		show = false;
	}

	// Ajoutez un bouton pour annuler
	if (ImGui::Button("Annuler")) {
		show = false;
	}

	// Terminez la fenêtre
	ImGui::End();
}

void ShowInputFloat(const std::string& message, float& input, bool& show) {
	// Créez une nouvelle fenêtre ImGui
	ImGui::Begin("Entrée", &show, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav);

	// Affichez le message d'entrée
	ImGui::Text(message.c_str());

	// Ajoutez un champ de texte pour l'entrée
	ImGui::InputFloat("", &input);

	// Ajoutez un bouton pour confirmer
	if (ImGui::Button("Confirmer")) {
		show = false;
	}

	// Ajoutez un bouton pour annuler
	if (ImGui::Button("Annuler")) {
		show = false;
	}

	// Terminez la fenêtre
	ImGui::End();
}

void ShowInputDouble(const std::string& message, double& input, bool& show) {
	// Créez une nouvelle fenêtre ImGui
	ImGui::Begin("Entrée", &show, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav);

	// Affichez le message d'entrée
	ImGui::Text(message.c_str());

	// Ajoutez un champ de texte pour l'entrée
	ImGui::InputDouble("", &input);

	// Ajoutez un bouton pour confirmer
	if (ImGui::Button("Confirmer")) {
		show = false;
	}

	// Ajoutez un bouton pour annuler
	if (ImGui::Button("Annuler")) {
		show = false;
	}

	// Terminez la fenêtre
	ImGui::End();
}

void ShowInputVec2(const std::string& message, glm::vec2& input, bool& show) {
	// Créez une nouvelle fenêtre ImGui
	ImGui::Begin("Entrée", &show, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav);

	// Affichez le message d'entrée
	ImGui::Text(message.c_str());

	// Ajoutez un champ de texte pour l'entrée
	ImGui::InputFloat2("", &input.x);

	// Ajoutez un bouton pour confirmer
	if (ImGui::Button("Confirmer")) {
		show = false;
	}

	// Ajoutez un bouton pour annuler
	if (ImGui::Button("Annuler")) {
		show = false;
	}

	// Terminez la fenêtre
	ImGui::End();
}

void ShowInputVec3(const std::string& message, glm::vec3& input, bool& show) {
	// Créez une nouvelle fenêtre ImGui
	ImGui::Begin("Entrée", &show, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav);

	// Affichez le message d'entrée
	ImGui::Text(message.c_str());

	// Ajoutez un champ de texte pour l'entrée
	ImGui::InputFloat3("", &input.x);

	// Ajoutez un bouton pour confirmer
	if (ImGui::Button("Confirmer")) {
		show = false;
	}

	// Ajoutez un bouton pour annuler
	if (ImGui::Button("Annuler")) {
		show = false;
	}

	// Terminez la fenêtre
	ImGui::End();
}

void ShowInputVec4(const std::string& message, glm::vec4& input, bool& show) {
	// Créez une nouvelle fenêtre ImGui
	ImGui::Begin("Entrée", &show, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav);

	// Affichez le message d'entrée
	ImGui::Text(message.c_str());

	// Ajoutez un champ de texte pour l'entrée
	ImGui::InputFloat4("", &input.x);

	// Ajoutez un bouton pour confirmer
	if (ImGui::Button("Confirmer")) {
		show = false;
	}

	// Ajoutez un bouton pour annuler
	if (ImGui::Button("Annuler")) {
		show = false;
	}

	// Terminez la fenêtre
	ImGui::End();
}

void ShowInputColor(const std::string& message, glm::vec4& input, bool& show) {
	// Créez une nouvelle fenêtre ImGui
	ImGui::Begin("Entrée", &show, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav);

	// Affichez le message d'entrée
	ImGui::Text(message.c_str());

	// Ajoutez un champ de texte pour l'entrée
	ImGui::ColorEdit4("", &input.x);

	// Ajoutez un bouton pour confirmer
	if (ImGui::Button("Confirmer")) {
		show = false;
	}

	// Ajoutez un bouton pour annuler
	if (ImGui::Button("Annuler")) {
		show = false;
	}

	// Terminez la fenêtre
	ImGui::End();
}

void ShowInputBool(const std::string& message, bool& input, bool& show) {
	// Créez une nouvelle fenêtre ImGui
	ImGui::Begin("Entrée", &show, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav);

	// Affichez le message d'entrée
	ImGui::Text(message.c_str());

	// Ajoutez un champ de texte pour l'entrée
	ImGui::Checkbox("", &input);

	// Ajoutez un bouton pour confirmer
	if (ImGui::Button("Confirmer")) {
		show = false;
	}

	// Ajoutez un bouton pour annuler
	if (ImGui::Button("Annuler")) {
		show = false;
	}

	// Terminez la fenêtre
	ImGui::End();
}

void ShowMenuBar() {
	// Créez une nouvelle barre de menu ImGui
	if (ImGui::BeginMainMenuBar()) {
		// Ajoutez un menu pour les options
		if (ImGui::BeginMenu("Options")) {
			// Ajoutez un élément pour afficher les informations
			if (ImGui::MenuItem("Informations")) {
				ShowInfo();
			}

			// Ajoutez un élément pour afficher l'aide
			if (ImGui::MenuItem("Aide")) {
				ShowHelp();
			}

			// Ajoutez un élément pour afficher les informations
			if (ImGui::MenuItem("À propos")) {
				ShowAbout();
			}

			// Terminez le menu
			ImGui::EndMenu();
		}

		// Terminez la barre de menu
		ImGui::EndMainMenuBar();
	}
}