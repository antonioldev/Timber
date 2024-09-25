
#include "Timber.hpp"

using namespace sf;
const int WIDTH = 1920;
const int HEIGHT = 1080;
const int NUM_BRANCHES = 6;
Sprite branches[NUM_BRANCHES];
enum class side { LEFT, RIGHT, NONE };
side branchPositions[NUM_BRANCHES];



int     main()
{
    VideoMode vm(WIDTH, HEIGHT);
    RenderWindow window(vm , "Timber!!!", Style::Fullscreen);

    Texture textureBackground;
    textureBackground.loadFromFile("graphics/background.png");
    Sprite spriteBackground;
    spriteBackground.setTexture(textureBackground);
    spriteBackground.setPosition(0, 0);
    
    Texture textureTree;
    textureTree.loadFromFile("graphics/tree.png");
    Sprite spriteTree;
    spriteTree.setTexture(textureTree);
    spriteTree.setPosition(810, 0);

    Sprite spriteBGtree1;
    Sprite spriteBGtree2;
    Sprite spriteBGtree3;
	spriteBGtree1.setTexture(textureTree);
	spriteBGtree2.setTexture(textureTree);
	spriteBGtree3.setTexture(textureTree);
    spriteBGtree1.setPosition(20, -40);
    spriteBGtree2.scale(0.5, 1);
    spriteBGtree3.scale(0.4, 1);
    spriteBGtree2.setPosition(1800, -50);
    spriteBGtree3.setPosition(1600, -70);

    Texture textureBee;
    textureBee.loadFromFile("graphics/bee.png");
    Sprite spriteBee;
    spriteBee.setTexture(textureBee);
    spriteBee.setPosition(-100, 800);
    bool beeActive = false;
    float beeSpeed = 0.0f;

    Texture textureCloud;
    textureCloud.loadFromFile("graphics/cloud.png");
    Sprite spriteCloud1;
    Sprite spriteCloud2;
    Sprite spriteCloud3;
    spriteCloud1.setTexture(textureCloud);
    spriteCloud2.setTexture(textureCloud);
    spriteCloud3.setTexture(textureCloud);
    spriteCloud1.setPosition(-1000, 0);
    spriteCloud2.setPosition(-1000, 250);
    spriteCloud3.setPosition(-1000, 500);
    bool cloud1Active = false;
    bool cloud2Active = false;
    bool cloud3Active = false;
    float cloud1Speed = 0.0f;
    float cloud2Speed = 0.0f;
    float cloud3Speed = 0.0f;
    Clock clock;

    //Timebar
    RectangleShape timeBar;
    float timeBarStartWidth = 400;
	float timeBarHeight = 40;
	timeBar.setSize(Vector2f(timeBarStartWidth, timeBarHeight));
	timeBar.setFillColor(Color::Red);
	timeBar.setPosition((WIDTH / 2.0f) - timeBarStartWidth / 2, 980);

    RectangleShape timeBarOutline;
    timeBarOutline.setSize(Vector2f(timeBarStartWidth, timeBarHeight));
    timeBarOutline.setFillColor(Color::Transparent);
    timeBarOutline.setOutlineColor(Color::White);
    timeBarOutline.setOutlineThickness(5);
    timeBarOutline.setPosition((WIDTH / 2.0f) - timeBarStartWidth / 2, 980);


	Time gameTimeTotal;
	float timeRemaining = 6.0f;
	float timeBarWidthPerSecond = timeBarStartWidth / timeRemaining;

    bool paused = true;
    int score = 0;

    Text messageText;
    Text scoreText;

    Font font;
	font.loadFromFile("fonts/KOMIKAP_.ttf");
	messageText.setFont(font);
	scoreText.setFont(font);

	messageText.setString("Press Enter to start!");
    scoreText.setString("SCORE =  0");

	messageText.setCharacterSize(75);
	scoreText.setCharacterSize(100);

	messageText.setFillColor(Color::White);
	scoreText.setFillColor(Color::White);

    FloatRect textRect = messageText.getLocalBounds();
    messageText.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
    messageText.setPosition(WIDTH / 2.0f, 1080 / 2.0f);
    scoreText.setPosition(20, 20);


    Texture textureBranch;
    textureBranch.loadFromFile("graphics/branch.png");
    for (int i = 0; i < NUM_BRANCHES; i++)
    {
        branches[i].setTexture(textureBranch);
        branches[i].setPosition(-2000, -2000);
        branches[i].setOrigin(220, 20);
    }

    Texture texturePlayer;
    texturePlayer.loadFromFile("graphics/player.png");
    Sprite spritePlayer;
    spritePlayer.setTexture(texturePlayer);
    spritePlayer.setPosition(580, 720);
    side playerSide = side::LEFT;

    Texture textureGravestone;
	textureGravestone.loadFromFile("graphics/rip.png");
	Sprite spriteGravestone;
	spriteGravestone.setTexture(textureGravestone);
    spriteGravestone.setPosition(675, 2000);
	//spriteGravestone.setPosition(600, 860);

	Texture textureAxe;
	textureAxe.loadFromFile("graphics/axe.png");
	Sprite spriteAxe;
	spriteAxe.setTexture(textureAxe);
	spriteAxe.setPosition(700, 830);

	const float AXE_POSITION_LEFT = 700;
	const float AXE_POSITION_RIGHT = 1075;

	Texture textureLog;
    textureLog.loadFromFile("graphics/log.png");
    Sprite spriteLog;
    spriteLog.setTexture(textureLog);
    spriteLog.setPosition(810, 720);

    bool logActive = false;
    float logSpeedX = 1000;
    float logSpeedY = -1500;
    bool acceptInput = false;

    SoundBuffer chopBuffer;
	chopBuffer.loadFromFile("sound/chop.wav");
	Sound chop;
	chop.setBuffer(chopBuffer);

    SoundBuffer deathBuffer;
    deathBuffer.loadFromFile("sound/death.wav");
    Sound death;
    death.setBuffer(deathBuffer);

    SoundBuffer ootBuffer;
    ootBuffer.loadFromFile("sound/out_of_time.wav");
    Sound oot;
    oot.setBuffer(ootBuffer);

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
                spriteAxe.setPosition(AXE_POSITION_RIGHT, spriteAxe.getPosition().y);
                spritePlayer.setPosition(1200, 720);
                updateBranches(score);
                spriteLog.setPosition(810, 720);
                logSpeedX = -5000;
                logActive = true;
                acceptInput = false;
                chop.play();
            }
            if (Keyboard::isKeyPressed(Keyboard::Left))
            {
                playerSide = side::LEFT;
                score++;
                timeRemaining += (2.0 / score) + 0.15;
                if (timeRemaining > 6.0f)
                    timeRemaining = 6.0f;
                spriteAxe.setPosition(AXE_POSITION_LEFT, spriteAxe.getPosition().y);
                spritePlayer.setPosition(580, 720);
                updateBranches(score);
                spriteLog.setPosition(810, 720);
                logSpeedX = 5000;
                logActive = true;
                acceptInput = false;
                chop.play();
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
            timeBar.setSize(Vector2f(timeBarWidthPerSecond * timeRemaining, timeBarHeight));
            if (timeRemaining <= 0.0f)
            {
				oot.play();
                paused = true;
                messageText.setString("GAME OVER!!");
                FloatRect textRect = messageText.getLocalBounds();
                messageText.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
                messageText.setPosition(WIDTH / 2.0f, HEIGHT / 2.0f);
            }
            if (!beeActive)
            {
                srand((int)time(0));
                beeSpeed = (rand() % 200) + 200;
                srand((int)time(0) * 10);
                float height = (rand() % 500) + 500;
                spriteBee.setPosition(2000, height);
                beeActive = true;
            }
            else
            {
                spriteBee.setPosition(spriteBee.getPosition().x - (beeSpeed * dt.asSeconds()), spriteBee.getPosition().y);
                if (spriteBee.getPosition().x < -100)
                    beeActive = false;
            }

            if (!cloud1Active)
            {
                srand((int)time(0) * 10);
                cloud1Speed = (rand() % 200);
                srand((int)time(0) * 10);
                float height = (rand() % 150);
                spriteCloud1.setPosition(-300, height);
                cloud1Active = true;
            }
            else
            {
                spriteCloud1.setPosition(spriteCloud1.getPosition().x + (cloud1Speed * dt.asSeconds()), spriteCloud1.getPosition().y);
                if (spriteCloud1.getPosition().x > WIDTH)
                    cloud1Active = false;
            }

            if (!cloud2Active)
            {
                srand((int)time(0) * 20);
                cloud2Speed = (rand() % 200);
                srand((int)time(0) * 20);
                float height = (rand() % 300) - 150;
                spriteCloud2.setPosition(-300, height);
                cloud2Active = true;
            }
            else
            {
                spriteCloud2.setPosition(spriteCloud2.getPosition().x + (cloud2Speed * dt.asSeconds()), spriteCloud2.getPosition().y);
                if (spriteCloud2.getPosition().x > WIDTH)
                    cloud2Active = false;
            }

            if (!cloud3Active)
            {
                srand((int)time(0) * 30);
                cloud3Speed = (rand() % 200);
                srand((int)time(0) * 300);
                float height = (rand() % 450) - 150;
                spriteCloud3.setPosition(-300, height);
                cloud3Active = true;
            }
            else
            {
                spriteCloud3.setPosition(spriteCloud3.getPosition().x + (cloud3Speed * dt.asSeconds()), spriteCloud3.getPosition().y);
                if (spriteCloud3.getPosition().x > WIDTH)
                    cloud3Active = false;
            }
            std::stringstream ss;
            ss << "SCORE = " << score;
            scoreText.setString(ss.str());
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
                death.play();
                paused = true;
                acceptInput = false;
                spriteGravestone.setPosition(525, 760);
				spriteAxe.setPosition(2000, 830);
				spriteLog.setPosition(2000, 720);
				spritePlayer.setPosition(2000, 660);
				messageText.setString("SQUISHED!!");
				FloatRect textRect = messageText.getLocalBounds();
                messageText.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
                messageText.setPosition(1920 / 2.0f, 1080 / 2.0f);
            }
        }


        window.clear();
        window.draw(spriteBackground);
		window.draw(spriteBGtree1);
		window.draw(spriteBGtree2);
		window.draw(spriteBGtree3);
        window.draw(spriteCloud1);
        window.draw(spriteCloud2);
        window.draw(spriteCloud3);
        for (int i = 0; i < NUM_BRANCHES; i++)
            window.draw(branches[i]);
        /*window.draw(spriteLog);*/
        window.draw(spriteTree);
        window.draw(spritePlayer);
        window.draw(spriteAxe);
        window.draw(spriteLog);
        window.draw(spriteGravestone);
        window.draw(spriteBee);
        window.draw(scoreText);
		window.draw(timeBar);
        window.draw(timeBarOutline);
            
			
        if (paused)
            window.draw(messageText);
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

