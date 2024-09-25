#include "Timber.hpp"

/*Update the branches to make them fall*/
void    updateBranches(int seed, t_sprite& sprite)
{
    for (int j = NUM_BRANCHES - 1; j > 0; j--)
        sprite.branchPositions[j] = sprite.branchPositions[j - 1];
    srand((int)time(0) + seed);
    int r = rand() % 5;
    switch (r)
    {
    case 0:
        sprite.branchPositions[0] = side::LEFT;
        break;
    case 1:
        sprite.branchPositions[0] = side::RIGHT;
        break;
    default:
        sprite.branchPositions[0] = side::NONE;
        break;
    }
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
        window.draw(sprite.branches[i]);
    window.draw(sprite.tree);
    window.draw(sprite.player);
    window.draw(sprite.axe);
    window.draw(sprite.log);
    window.draw(sprite.gravestone);
    window.draw(sprite.bee.sprite);
    window.draw(text.score);
    window.draw(text.bestScore);
    window.draw(timeBar.timeBar);
    window.draw(timeBar.outline);
	window.draw(text.fps);
    if (paused)
        window.draw(text.message);
    window.display();
}