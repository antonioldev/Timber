
#include "Timber.hpp"

using namespace sf;

Sprite branches[NUM_BRANCHES];
side branchPositions[NUM_BRANCHES];

int     main()
{
    VideoMode vm(WIDTH, HEIGHT);
    RenderWindow window(vm , "Timber!!!", Style::Fullscreen);

	t_sound sound;
	loadSounds(sound);

	t_texture texture;
    loadTextures(texture);

    t_sprite sprite;
    loadSprites(sprite, texture);

    side playerSide = side::LEFT;
    bool logActive = false;
    float logSpeedX = 1000;
    float logSpeedY = -1500;
    bool acceptInput = false;
    bool paused = true;
    int score = 0;

    Clock clock;

	t_timebar timeBar;
    set_timeBar(timeBar);

	Time gameTimeTotal;
	float timeRemaining = 6.0f;
	float timeBarWidthPerSecond = timeBar.startWidth / timeRemaining;

	t_text text;
	set_font(text);
	set_text(text, "Press Enter to start!");    

    while (window.isOpen())
    {
        if (Keyboard::isKeyPressed(Keyboard::Escape))
            window.close();
        if (Keyboard::isKeyPressed(Keyboard::Return))
        {
            paused = false;
			score = 0;
			timeRemaining = 6;
			for (int i = 1; i < NUM_BRANCHES; i++)
				branchPositions[i] = side::NONE;
			sprite.gravestone.setPosition(675, 2000);
			sprite.player.setPosition(580, 720);
			acceptInput = true;
        }
        if (acceptInput)
        {

            if (Keyboard::isKeyPressed(Keyboard::Right))
            {
                playerSide = side::RIGHT;
                score++;
                timeRemaining += (2.0 / score) + 0.15;
				if (timeRemaining > 6.0f)
					timeRemaining = 6.0f;
                sprite.axe.setPosition((float)AXE_POSITION_RIGHT, sprite.axe.getPosition().y);
                sprite.player.setPosition(1200, 720);
                updateBranches(score);
                sprite.log.setPosition(810, 720);
                logSpeedX = -5000;
                logActive = true;
                acceptInput = false;
                sound.chop.play();
            }
            if (Keyboard::isKeyPressed(Keyboard::Left))
            {
                playerSide = side::LEFT;
                score++;
                timeRemaining += (2.0 / score) + 0.15;
                if (timeRemaining > 6.0f)
                    timeRemaining = 6.0f;
                sprite.axe.setPosition((float)AXE_POSITION_LEFT, sprite.axe.getPosition().y);
                sprite.player.setPosition(580, 720);
                updateBranches(score);
                sprite.log.setPosition(810, 720);
                logSpeedX = 5000;
                logActive = true;
                acceptInput = false;
                sound.chop.play();
            }
        }
		Event event;
        while (window.pollEvent(event))
        {
            if (event.type == Event::KeyReleased && !paused)
            {
				acceptInput = true;
				sprite.axe.setPosition(2000, sprite.axe.getPosition().y);
            }
        }

        if (!paused)
        {
            Time dt = clock.restart();
            timeRemaining -= dt.asSeconds();
            timeBar.timeBar.setSize(Vector2f(timeBarWidthPerSecond * timeRemaining, timeBar.height));
            if (timeRemaining <= 0.0f)
            {
                sound.oot.play();
                paused = true;
                set_text(text, "GAME OVER!!");
            }
            if (!sprite.bee.active)
            {
                srand((int)time(0));
                sprite.bee.speed = (rand() % 200) + 200;
                srand((int)time(0) * 10);
                float height = (rand() % 500) + 500;
                sprite.bee.sprite.setPosition(2000, height);
                sprite.bee.active = true;
            }
            else
            {
                // add movement up and down bee
                sprite.bee.sprite.setPosition(sprite.bee.sprite.getPosition().x - (sprite.bee.speed * dt.asSeconds()), sprite.bee.sprite.getPosition().y);
                if (sprite.bee.sprite.getPosition().x < -100)
                    sprite.bee.active = false;
            }
            for (int i = 0; i < NUM_CLOUDS; i++)
            {
                if (!sprite.clouds[i].active)
                {
                    srand((int)time(0) * 10 * i);
                    sprite.clouds[i].speed = (rand() % 200);
                    srand((int)time(0) * 10 * i);
                    float height = (rand() % 150);
                    sprite.clouds[i].sprite.setPosition(-300, height);
                    sprite.clouds[i].active = true;
                }
                else
                {
                    sprite.clouds[i].sprite.setPosition(sprite.clouds[i].sprite.getPosition().x + (sprite.clouds[i].speed * dt.asSeconds()), sprite.clouds[i].sprite.getPosition().y);
                    if (sprite.clouds[i].sprite.getPosition().x > WIDTH)
                        sprite.clouds[i].active = false;
                }
            }

            std::stringstream ss;
            ss << "SCORE = " << score;
            text.score.setString(ss.str());
            for (int i = 0; i < NUM_BRANCHES; i++)
            {
                float height = i * 150;
                if (branchPositions[i] == side::LEFT)
                {
                    branches[i].setPosition(610, height);
                    branches[i].setRotation(180);
                }
                else if (branchPositions[i] == side::RIGHT)
                {
                    branches[i].setPosition(1330, height);
                    branches[i].setRotation(0);
                }
                else
                {
                    branches[i].setPosition(3000, height);
                }
            }
            if (logActive)
            {
				sprite.log.setPosition(sprite.log.getPosition().x + (logSpeedX * dt.asSeconds()),
                        sprite.log.getPosition().y + (logSpeedY * dt.asSeconds()));
                if (sprite.log.getPosition().x < -100 || sprite.log.getPosition().x > 2000)
                {
                    logActive = false;
                    sprite.log.setPosition(810, 720);
                }
            }

            if (branchPositions[5] == playerSide)
            {
                sound.death.play();
                paused = true;
                acceptInput = false;
                sprite.gravestone.setPosition(525, 760);
				sprite.axe.setPosition(2000, 830);
				sprite.log.setPosition(2000, 720);
				sprite.player.setPosition(2000, 660);
                set_text(text, "SQUISHED!!");
            }
        }

		draw_sprite(window, sprite, timeBar,text, paused);
    }
    return 0;
}

/*Update the branches to make them fall*/
void    updateBranches(int seed)
{
    for (int j = NUM_BRANCHES - 1; j > 0; j--)
        branchPositions[j] = branchPositions[j - 1];
	srand((int)time(0) + seed);
	int r = rand() % 5;
    switch (r)
    {
	case 0:
		branchPositions[0] = side::LEFT;
		break;
	case 1:
		branchPositions[0] = side::RIGHT;
		break;
	default:
		branchPositions[0] = side::NONE;
		break;
    }
}

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

/*Load all the sprites with their textures, position and scale*/ /
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
        loadSingleSprite(branches[i], texture.branch, -2000, -2000, 1, 1);
        branches[i].setOrigin(220, 20);
    }
    loadSingleSprite(sprite.player, texture.player, 580, 720, 1, 1);
    loadSingleSprite(sprite.gravestone, texture.gravestone, 675, 2000, 1, 1);
    loadSingleSprite(sprite.axe, texture.axe, 700, 830, 1, 1);
    loadSingleSprite(sprite.log, texture.log, 810, 720, 1, 1);
}

/*Load a single sprite with a texture, position and scale*/ /
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
    text.score.setFont(text.font);
    text.score.setString("SCORE =  0");
    text.message.setCharacterSize(75);
    text.score.setCharacterSize(100);
    text.message.setFillColor(Color::White);
    text.score.setFillColor(Color::White);
}

/*Print on the display the right text, depending on game status*/
void	set_text(t_text& text, const std::string& str)
{
    text.message.setString(str);
    FloatRect textRect = text.message.getLocalBounds();
    text.message.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
    text.message.setPosition(WIDTH / 2.0f, HEIGHT / 2.0f);
    text.score.setPosition(20, 20);
}

/*Clear and redraw sprites for each loop*/
void	draw_sprite(sf::RenderWindow& window, t_sprite& sprite, t_timebar& timeBar, t_text& text, bool paused)
{
    window.clear();
    window.draw(sprite.background);
    window.draw(sprite.BGtree1);
    window.draw(sprite.BGtree2);
    window.draw(sprite.BGtree3);
    for (int i = 0; i < NUM_CLOUDS; i++)
        window.draw(sprite.clouds[i].sprite);
    for (int i = 0; i < NUM_BRANCHES; i++)
        window.draw(branches[i]);
    window.draw(sprite.tree);
    window.draw(sprite.player);
    window.draw(sprite.axe);
    window.draw(sprite.log);
    window.draw(sprite.gravestone);
    window.draw(sprite.bee.sprite);
    window.draw(text.score);
    window.draw(timeBar.timeBar);
    window.draw(timeBar.outline);
    if (paused)
        window.draw(text.message);
    window.display();
}