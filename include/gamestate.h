#pragma once
#include "config.h"
#include <string>

class GameState
{
private:
	static GameState* m_unique_instance;

	class Level * m_current_level = nullptr;
	class Player* m_player = nullptr;
	class LevelMaker* m_level_maker = nullptr;
	
	GameState();

public:
	bool m_debugging = false;

public:
	~GameState();
	static GameState* getInstance();

	bool init();
	void draw();
	void update(float dt);

	std::string getFullAssetPath(const std::string& asset);
	std::string getAssetDir();
	class Player* getPlayer() { return m_player; }
	void playerDeath();
	void enter_level_maker();
	void exit_level_maker();
};
