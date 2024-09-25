
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


    Sprite spriteBackground;
    Sprite spriteTree;
    Sprite spriteBGtree1;
    Sprite spriteBGtree2;
    Sprite spriteBGtree3;
    Sprite spritePlayer;
    Sprite spriteGravestone;
    Sprite spriteAxe;
    Sprite spriteLog;
    t_bg_obj bee;
    t_bg_obj clouds[NUM_CLOUDS];

	loadSprites(spriteBackground, texture.background, 0, 0, 1, 1);  
	loadSprites(spriteTree, texture.tree, 810, 0, 1, 1);
	loadSprites(spriteBGtree1, texture.tree, 20, -40, 1, 1);
	loadSprites(spriteBGtree2, texture.tree, 1800, -50, 0.5, 1);
	loadSprites(spriteBGtree3, texture.tree, 1600, -70, 0.4, 1);
	loadSprites(bee.sprite, texture.bee, -100, 800, 1, 1);
    for (int i = 0; i < NUM_CLOUDS; i++)
        loadSprites(clouds[i].sprite, texture.cloud, -1000, 0 * 250, 1, 1);

    for (int i = 0; i < NUM_BRANCHES; i++)
    {
		loadSprites(branches[i], texture.branch, -2000, -2000, 1, 1);
        branches[i].setOrigin(220, 20);
    }
	loadSprites(spritePlayer, texture.player, 580, 720, 1, 1);
    side playerSide = side::LEFT;
	loadSprites(spriteGravestone, texture.gravestone, 675, 2000, 1, 1);
	loadSprites(spriteAxe, texture.axe, 700, 830, 1, 1);
	loadSprites(spriteLog, texture.log, 810, 720, 1, 1);


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
	set_text(text);
    FloatRect textRect = text.message.getLocalBounds();
    text.message.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
    text.message.setPosition(WIDTH / 2.0f, HEIGHT / 2.0f);
    text.score.setPosition(20, 20);


    

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
			spriteGravestone.setPosition(675, 2000);
			spritePlayer.setPosition(580, 720);
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
                spriteAxe.setPosition((float)AXE_POSITION_RIGHT, spriteAxe.getPosition().y);
                spritePlayer.setPosition(1200, 720);
                updateBranches(score);
                spriteLog.setPosition(810, 720);
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
                spriteAxe.setPosition((float)AXE_POSITION_LEFT, spriteAxe.getPosition().y);
                spritePlayer.setPosition(580, 720);
                updateBranches(score);
                spriteLog.setPosition(810, 720);
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
				spriteAxe.setPosition(2000, spriteAxe.getPosition().y);
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
                text.message.setString("GAME OVER!!");
                FloatRect textRect = text.message.getLocalBounds();
                text.message.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
                text.message.setPosition(WIDTH / 2.0f, HEIGHT / 2.0f);
            }
            if (!bee.active)
            {
                srand((int)time(0));
                bee.speed = (rand() % 200) + 200;
                srand((int)time(0) * 10);
                float height = (rand() % 500) + 500;
                bee.sprite.setPosition(2000, height);
                bee.active = true;
            }
            else
            {
                // add movement up and down bee
                bee.sprite.setPosition(bee.sprite.getPosition().x - (bee.speed * dt.asSeconds()), bee.sprite.getPosition().y);
                if (bee.sprite.getPosition().x < -100)
                    bee.active = false;
            }
            for (int i = 0; i < NUM_CLOUDS; i++)
            {
                if (!clouds[i].active)
                {
                    srand((int)time(0) * 10 * i);
                    clouds[i].speed = (rand() % 200);
                    srand((int)time(0) * 10 * i);
                    float height = (rand() % 150);
                    clouds[i].sprite.setPosition(-300, height);
                    clouds[i].active = true;
                }
                else
                {
                    clouds[i].sprite.setPosition(clouds[i].sprite.getPosition().x + (clouds[i].speed * dt.asSeconds()), clouds[i].sprite.getPosition().y);
                    if (clouds[i].sprite.getPosition().x > WIDTH)
                        clouds[i].active = false;
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
				spriteLog.setPosition(spriteLog.getPosition().x + (logSpeedX * dt.asSeconds()),
                        spriteLog.getPosition().y + (logSpeedY * dt.asSeconds()));
                if (spriteLog.getPosition().x < -100 || spriteLog.getPosition().x > 2000)
                {
                    logActive = false;
                    spriteLog.setPosition(810, 720);
                }
            }

            if (branchPositions[5] == playerSide)
            {
                sound.death.play();
                paused = true;
                acceptInput = false;
                spriteGravestone.setPosition(525, 760);
				spriteAxe.setPosition(2000, 830);
				spriteLog.setPosition(2000, 720);
				spritePlayer.setPosition(2000, 660);
                text.message.setString("SQUISHED!!");
				FloatRect textRect = text.message.getLocalBounds();
                text.message.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
                text.message.setPosition(WIDTH / 2.0f, HEIGHT / 2.0f);
            }
        }


        window.clear();
        window.draw(spriteBackground);
		window.draw(spriteBGtree1);
		window.draw(spriteBGtree2);
		window.draw(spriteBGtree3);
		for (int i = 0; i < NUM_CLOUDS; i++)
			window.draw(clouds[i].sprite);
        for (int i = 0; i < NUM_BRANCHES; i++)
            window.draw(branches[i]);
        window.draw(spriteTree);
        window.draw(spritePlayer);
        window.draw(spriteAxe);
        window.draw(spriteLog);
        window.draw(spriteGravestone);
        window.draw(bee.sprite);
        window.draw(text.score);
		window.draw(timeBar.timeBar);
        window.draw(timeBar.outline);
            
			
        if (paused)
            window.draw(text.message);
        window.display();
    }
    return 0;
}

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

void loadSounds(t_sound& sound)
{
	sound.chopBuffer.loadFromFile("sound/chop.wav");
	sound.deathBuffer.loadFromFile("sound/death.wav");
	sound.ootBuffer.loadFromFile("sound/out_of_time.wav");
	sound.chop.setBuffer(sound.chopBuffer);
	sound.death.setBuffer(sound.deathBuffer);
	sound.oot.setBuffer(sound.ootBuffer);
}

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

void	loadSprites(sf::Sprite& sprite, sf::Texture& texture, int posX, int posY, float scaleX, float scaleY)
{
    sprite.setTexture(texture);
    sprite.scale(scaleX, scaleY);
    sprite.setPosition(posX, posY);
}

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

void	set_text(t_text& text)
{
    text.font.loadFromFile("fonts/KOMIKAP_.ttf");
    text.message.setFont(text.font);
    text.score.setFont(text.font);

    text.message.setString("Press Enter to start!");
    text.score.setString("SCORE =  0");

    text.message.setCharacterSize(75);
    text.score.setCharacterSize(100);

    text.message.setFillColor(Color::White);
    text.score.setFillColor(Color::White);
}