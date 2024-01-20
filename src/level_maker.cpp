#include "level_maker.h"
#include "level.h"
#include <iostream>
#include <fstream>

void LevelMaker::update(float ms) {
	// mouse.cur_pos_x and mouse.cur_pos_y are pixel coordinates
	mouse_canvas_x = (mouse.cur_pos_x / static_cast<float>(WINDOW_WIDTH)) * CANVAS_WIDTH;
	mouse_canvas_y = (mouse.cur_pos_y / static_cast<float>(WINDOW_HEIGHT)) * CANVAS_HEIGHT;
	m_level->update(ms);
	m_state->getPlayer()->update(ms);
	graphics::getMouseState(mouse);
	create_object();
	remove_object();

	if (graphics::getKeyState(graphics::SCANCODE_3)) {
		save_to_file();
	}
}

// Create a new block (static object) with left click and add it to level vector.
void LevelMaker::create_object() {
	if (mouse.button_left_pressed) {
		std::string asset_path = "terrain\\cave_block.png";
		m_level->getStaticObjects().push_back(new StaticObject(mouse_canvas_x, mouse_canvas_y, 1.0f, 1.0f, asset_path));
	}
}

// Remove a block with right click.
void LevelMaker::remove_object() {
	if (mouse.button_right_pressed && !mouse.button_left_pressed) {
		auto& static_objects = m_level->getStaticObjects();

		for (auto it = static_objects.begin(); it != static_objects.end();) {
			
			const auto& p_sob = *it;

			if (mouseIntersect(p_sob->m_pos_x, p_sob->m_pos_y)) {
				it = static_objects.erase(it);
			}

			else ++it;

		}
	}
}

void LevelMaker::save_to_file() {

	int level_id = 1;
	// Check for an existing file and increment level_id if necessary
	while (file_exists("levels\\my_level_" + std::to_string(level_id) + ".txt")) {
		level_id++;
	}

	// Open the file to write with the updated level_id
	std::ofstream outputFile("levels\\my_level_" + std::to_string(level_id) + ".txt");

	// Check if the file is opened successfully
	if (!outputFile.is_open()) {
		std::cerr << "Error opening file for writing." << std::endl;
		return;
	}

	// Iterator through static objects and write to the file
	for (const auto& p_sob : m_level->getStaticObjects()) {
		outputFile << p_sob->to_string() << std::endl;
	}

	outputFile.close();

	std::cout << "File save complete!" << std::endl;

	m_state->exit_level_maker();

}

bool LevelMaker::file_exists(const std::string& filename) {
	FILE* file = fopen(filename.c_str(), "r");
	if (file != nullptr) {
		fclose(file);
		return true;
	}
	return false;
}

void LevelMaker::show_options() {
	graphics::Brush text_brush;
	text_brush.fill_opacity = 1.0f;
	SETCOLOR(text_brush.fill_color, 255, 255, 255);
	graphics::setFont("assets\\Roboto-Bold.ttf");

	graphics::drawText(0.5f, 0.5f, 0.4f, "Left Click - Place Object", text_brush);
	graphics::drawText(0.5f, 1.0f, 0.4f, "2 - Exit Level Maker", text_brush);
	graphics::drawText(0.5f, 1.5f, 0.4f, "3 - Save Level", text_brush);
}

void LevelMaker::draw() {
	m_level->draw();
	m_state->getPlayer()->draw();

	mouse_brush.fill_opacity = 0.5f;
	mouse_brush.outline_opacity = 1.0f;
	graphics::drawRect(mouse_canvas_x, mouse_canvas_y, 1.0f, 1.0f, mouse_brush);

	show_options();

}

void LevelMaker::init() {
}

LevelMaker::LevelMaker() {
	m_level = new Level("default_level.txt");
	m_level->init();
	mouse_canvas_x = 0;
	mouse_canvas_y = 0;
}


LevelMaker::~LevelMaker() {
	delete m_level;
}

bool LevelMaker::mouseIntersect(float x, float y) {
	return static_cast<int>(x) == static_cast<int>(mouse_canvas_x) && static_cast<int>(y) == static_cast<int>(mouse_canvas_y);
}