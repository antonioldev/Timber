#include "Timber.hpp"

/*Load sounds used in the game*/
void loadSounds(t_sound& sound)
{
    sound.chopBuffer.loadFromFile("sound/chop.wav");
    sound.deathBuffer.loadFromFile("sound/death.wav");
    sound.ootBuffer.loadFromFile("sound/out_of_time.wav");
    sound.chop.setBuffer(sound.chopBuffer);
    sound.death.setBuffer(sound.deathBuffer);
    sound.oot.setBuffer(sound.ootBuffer);
}

/*Load all the textures used in the game*/
void	loadTextures(t_texture& texture)
{
    texture.background.loadFromFile("graphics/background.png");
    texture.tree.loadFromFile("graphics/tree.png");
    texture.bee.loadFromFile("graphics/bee.png");
    texture.cloud.loadFromFile("graphics/cloud.png");
    texture.branch.loadFromFile("graphics/branch.png");
    texture.player.loadFromFile("graphics/player.png");
    texture.gravestone.loadFromFile("graphics/rip.png");
    texture.axe.loadFromFile("graphics/axe.png");
    texture.log.loadFromFile("graphics/log.png");
}

/*Load all the sprites with their textures, position and scale*/
void	loadSprites(t_sprite& sprite, t_texture& texture)
{
    loadSingleSprite(sprite.background, texture.background, 0, 0, 1, 1);
    loadSingleSprite(sprite.tree, texture.tree, 810, 0, 1, 1);
    loadSingleSprite(sprite.BGtree1, texture.tree, 20, -40, 1, 1);
    loadSingleSprite(sprite.BGtree2, texture.tree, 1800, -50, 0.5, 1);
    loadSingleSprite(sprite.BGtree3, texture.tree, 1600, -70, 0.4, 1);
    loadSingleSprite(sprite.bee.sprite, texture.bee, -100, 800, 1, 1);
    for (int i = 0; i < NUM_CLOUDS; i++)
        loadSingleSprite(sprite.clouds[i].sprite, texture.cloud, -1000, 0 * 250, 1, 1);

    for (int i = 0; i < NUM_BRANCHES; i++)
    {
        loadSingleSprite(sprite.branches[i], texture.branch, -2000, -2000, 1, 1);
        sprite.branches[i].setOrigin(220, 20);
    }
    loadSingleSprite(sprite.player, texture.player, 580, 720, 1, 1);
    loadSingleSprite(sprite.gravestone, texture.gravestone, 675, 2000, 1, 1);
    loadSingleSprite(sprite.axe, texture.axe, 700, 830, 1, 1);
    loadSingleSprite(sprite.log, texture.log, 810, 720, 1, 1);
}

/*Load a single sprite with a texture, position and scale*/
void	loadSingleSprite(sf::Sprite& sprite, sf::Texture& texture, int posX, int posY, float scaleX, float scaleY)
{
    sprite.setTexture(texture);
    sprite.scale(scaleX, scaleY);
    sprite.setPosition(posX, posY);
}

/*Create size of the time bar*/
void	set_timeBar(t_timebar& timeBar)
{
    timeBar.startWidth = 400;
    timeBar.height = 40;
    timeBar.timeBar.setSize(Vector2f(timeBar.startWidth, timeBar.height));
    timeBar.timeBar.setFillColor(Color::Red);
    timeBar.timeBar.setPosition((WIDTH / 2.0f) - timeBar.startWidth / 2, 980);


    timeBar.outline.setSize(Vector2f(timeBar.startWidth, timeBar.height));
    timeBar.outline.setFillColor(Color::Transparent);
    timeBar.outline.setOutlineColor(Color::White);
    timeBar.outline.setOutlineThickness(5);
    timeBar.outline.setPosition((WIDTH / 2.0f) - timeBar.startWidth / 2, 980);
}

/*Set the font used for the game*/
void	set_font(t_text& text)
{
    text.font.loadFromFile("fonts/KOMIKAP_.ttf");

    text.message.setFont(text.font);
    text.message.setCharacterSize(75);
    text.message.setFillColor(Color::White);

    text.score.setFont(text.font);
    text.score.setString("SCORE =  0");
    text.score.setCharacterSize(100);
    text.score.setFillColor(Color::White);

    text.bestScore.setFont(text.font);
    text.bestScore.setString("SCORE =  0");
    text.bestScore.setCharacterSize(100);
    text.bestScore.setFillColor(Color::White);

    text.fps.setFont(text.font);
    text.fps.setString("fps =  0");
    text.fps.setCharacterSize(15);
    text.fps.setFillColor(Color::White);
}

/*Print on the display the right text, depending on game status*/
void	set_text(t_text& text, const std::string& str)
{
    text.message.setString(str);
    FloatRect textRect = text.message.getLocalBounds();
    text.message.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
    text.message.setPosition(WIDTH / 2.0f, HEIGHT / 2.0f);
    text.score.setPosition(20, 20);
	text.bestScore.setPosition(20, 900);
    text.fps.setPosition(1800, 1020);
}