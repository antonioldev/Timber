
#include "Timber.hpp"

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
	int bestScore = 0;

    Clock clock;

	t_timebar timeBar;
    set_timeBar(timeBar);

	//Time gameTimeTotal;
    float timeRemaining = 6.0f;
    float timeBarWidthPerSecond = timeBar.startWidth / timeRemaining;

	t_text text;
	set_font(text);
	set_text(text, "Press Enter to start!");    

    while (window.isOpen())
    {
        if (Keyboard::isKeyPressed(Keyboard::Escape))
            window.close();
        if (Keyboard::isKeyPressed(Keyboard::Return) && paused == true)
        {
            paused = false;
			score = 0;
            timeRemaining = 6.0f;
			for (int i = 1; i < NUM_BRANCHES; i++)
				sprite.branchPositions[i] = side::NONE;
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
                updateBranches(score, sprite);
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
                updateBranches(score, sprite);
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
            float amplitude = 100.0f;
            float frequency = 2.0f;
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
                float height = (rand() % 500) + 250;
                sprite.bee.sprite.setPosition(2000, height);
                sprite.bee.offset = height;
                sprite.bee.active = true;
            }
            else
            {
                sprite.bee.sprite.setPosition(sprite.bee.sprite.getPosition().x - (sprite.bee.speed * dt.asSeconds()),
                    sprite.bee.offset + amplitude * sin(frequency * sprite.bee.sprite.getPosition().x / 100.0f));
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
                if (sprite.branchPositions[i] == side::LEFT)
                {
                    sprite.branches[i].setPosition(610, height);
                    sprite.branches[i].setRotation(180);
                }
                else if (sprite.branchPositions[i] == side::RIGHT)
                {
                    sprite.branches[i].setPosition(1330, height);
                    sprite.branches[i].setRotation(0);
                }
                else
                {
                    sprite.branches[i].setPosition(3000, height);
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

            if (sprite.branchPositions[5] == playerSide)
            {
                sound.death.play();
                paused = true;
                acceptInput = false;
                sprite.gravestone.setPosition(525, 760);
				sprite.axe.setPosition(2000, 830);
				sprite.log.setPosition(2000, 720);
				sprite.player.setPosition(2000, 660);
                set_text(text, "SQUISHED!!");
                if (score > bestScore)
                {
                    bestScore = score;
                    std::stringstream ss;
                    ss << "SCORE = " << score;
                    text.bestScore.setString(ss.str());
                }
            }
        }

		draw_sprite(window, sprite, timeBar,text, paused);
    }
    return 0;
}

