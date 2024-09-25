#pragma once

#include <iostream>
#include <sstream>
#include "SFML/Graphics.hpp"
#include "SFML/Audio.hpp"

#define WIDTH 1920
#define HEIGHT 1080
#define NUM_BRANCHES 6
#define NUM_CLOUDS 3
#define NUM_TREE 3

const float AXE_POSITION_LEFT = 700;
const float AXE_POSITION_RIGHT = 1075;

enum class side { LEFT, RIGHT, NONE };

typedef struct s_texture
{
	sf::Texture background;
	sf::Texture tree;
	sf::Texture bee;
	sf::Texture cloud;
	sf::Texture branch;
	sf::Texture player;
	sf::Texture gravestone;
	sf::Texture axe;
	sf::Texture log;

} t_texture;

typedef struct s_sound
{
	sf::SoundBuffer chopBuffer;
	sf::SoundBuffer deathBuffer;
	sf::SoundBuffer ootBuffer;
	sf::Sound chop;
	sf::Sound death;
	sf::Sound oot;
} t_sound;

typedef struct s_bg_obj
{
	sf::Sprite sprite;
	float speed = 0.0f;
	bool active = false;
} t_bg_obj;

typedef struct s_timebar
{
	sf::RectangleShape timeBar;
	sf::RectangleShape outline;
	float startWidth;
	float height;
} t_timebar;

typedef struct s_text
{
	sf::Font font;
	sf::Text message;
	sf::Text score;
} t_text;

//typedef struct s_sprite
//{
//	Sprite spriteBackground;
//	Sprite spriteTree;
//	Sprite spriteBGtree1;
//	Sprite spriteBGtree2;
//	Sprite spriteBGtree3;
//	Sprite spritePlayer;
//	Sprite spriteGravestone;
//	Sprite spriteAxe;
//	Sprite spriteLog;
//	t_bg_obj bee;
//	t_bg_obj clouds[NUM_CLOUDS];
//} t_sprite;


void    updateBranches(int seed);
void	loadSounds(t_sound& sound);
void	loadTextures(t_texture& texture);
void	loadSprites(sf::Sprite& sprite, sf::Texture& texture, int posX, int posY, float scaleX, float scaleY);
void	set_timeBar(t_timebar& timeBar);
void	set_text(t_text& text);