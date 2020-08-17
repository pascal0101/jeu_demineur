#include "niveau.h"
#include <algorithm>
#include <ctime>


Niveau::GameSize Niveau::getGameSize(Niveau::Difficulty
        d)
{
    switch (d)
    {
    case Easy:
        return {8, 13, 10};
    case Intermediate:
        return {16, 16, 39};
    case Hard:
        return {30, 16, 87};
    }

    return {0, 0, 0};
}

bool Niveau::hasWon() const
{
    for (auto &row : grid)
        for (auto &cell : row)
            if (!cell.isClicked() && !cell.isBomb())
                return false;

    return true;
}

Niveau::Niveau(Difficulty d, QObject *parent):
    QObject(parent)
{
    GameSize g = getGameSize(d);

    grid = std::vector<std::vector<Cell>>(g.height,
                                          std::vector<Cell>(g.width));
    bombs = g.bombs;

    //Connect the gameOver signal with a lambda that clicks the cell for every cell that's constructed

    //Distribute bombs
    std::vector<int> numbers;

    for (unsigned int i = 0; i < g.height * g.width; ++i)
        numbers.push_back(static_cast<int>(i));

    std::srand(static_cast<unsigned int>(time(nullptr)));
    std::random_shuffle(numbers.begin(), numbers.end());

    for (unsigned int i = 0; i < numbers.size(); ++i)
        grid[i / g.width][i % g.width].bomb = numbers[i] < bombs;

    for (unsigned int i = 0; i < g.height; ++i)
        for (unsigned int j = 0; j < g.width; ++j)
            if (grid[i][j].bomb)
            {
                if (i > 0)
                {
                    if (j > 0)
                        grid[i - 1][j - 1].surrounding_bombs++;

                    grid[i - 1][j].surrounding_bombs++;

                    if (j < grid[0].size() - 1)
                        grid[i - 1][j + 1].surrounding_bombs++;
                }

                if (j > 0)
                    grid[i][j - 1].surrounding_bombs++;

                if (j < grid[0].size() - 1)
                    grid[i][j + 1].surrounding_bombs++;

                if (i < grid.size() - 1)
                {
                    if (j > 0)
                        grid[i + 1][j - 1].surrounding_bombs++;

                    grid[i + 1][j].surrounding_bombs++;

                    if (j < grid[0].size() - 1)
                        grid[i + 1][j + 1].surrounding_bombs++;
                }
            }
}

std::vector<std::vector<Niveau::Index>> Niveau::getGrid()
{
    std::vector<std::vector<Niveau::Index>> aux(grid.size());

    for (unsigned int i = 0; i < grid.size(); ++i)
        for (unsigned int j = 0; j < grid[0].size(); ++j)
            aux[i].push_back({grid, i, j});

    return aux;
}

int Niveau::getBombs() const
{
    return bombs;
}

Niveau::Cell &Niveau::getCell(const Niveau::Index &i)
{
    return grid[i.i][i.j];
}

void Niveau::click(const Niveau::Index &i)
{
    Cell &c = getCell(i);

    if (!c.clicked && !c.flagged)
    {
        c.clicked = true;

        emit change(i);

        if (c.bomb)
            emit gameOver();

        if (c.surrounding_bombs == 0)
        {
            if (i.i > 0)
            {
                if (i.j > 0)
                    click({grid, i.i - 1, i.j - 1});
                click({grid, i.i - 1, i.j});

                if (i.j < grid[0].size() - 1)
                    click({grid, i.i - 1, i.j + 1});
            }

            if (i.j > 0)
                click({grid, i.i, i.j - 1});

            if (i.j < grid[0].size() - 1)
                click({grid, i.i, i.j + 1});

            if (i.i < grid.size() - 1)
            {
                if (i.j > 0)
                    click({grid, i.i + 1, i.j - 1});
                click({grid, i.i + 1, i.j});

                if (i.j < grid[0].size() - 1)
                    click({grid, i.i + 1, i.j + 1});
            }
        }

        if (hasWon())
            emit youWon();
    }
}

void Niveau::doubleClick(const Niveau::Index &i)
{
    Cell &c = getCell(i);

    if (c.clicked)
    {
        unsigned int surrounding_flags = 0;

        if (i.i > 0)
        {
            if (i.j > 0 && getCell({grid, i.i - 1, i.j - 1}).flagged)
                ++surrounding_flags;

            if (getCell({grid, i.i - 1, i.j}).flagged)
                ++surrounding_flags;

            if (i.j < grid[0].size() - 1 && getCell({grid, i.i - 1, i.j + 1}).flagged)
                ++surrounding_flags;
        }

        if (i.j > 0 && getCell({grid, i.i, i.j - 1}).flagged)
            ++surrounding_flags;

        if (i.j < grid[0].size() - 1 && getCell({grid, i.i, i.j + 1}).flagged)
            ++surrounding_flags;

        if (i.i < grid.size() - 1)
        {
            if (i.j > 0 && getCell({grid, i.i + 1, i.j - 1}).flagged)
                ++surrounding_flags;

            if (getCell({grid, i.i + 1, i.j}).flagged)
                ++surrounding_flags;

            if (i.j < grid[0].size() - 1 && getCell({grid, i.i + 1, i.j + 1}).flagged)
                ++surrounding_flags;
        }

        if (c.surrounding_bombs == surrounding_flags)
        {
            if (i.i > 0)
            {
                if (i.j > 0)
                    click({grid, i.i - 1, i.j - 1});
                click({grid, i.i - 1, i.j});

                if (i.j < grid[0].size() - 1)
                    click({grid, i.i - 1, i.j + 1});
            }

            if (i.j > 0)
                click({grid, i.i, i.j - 1});

            if (i.j < grid[0].size() - 1)
                click({grid, i.i, i.j + 1});

            if (i.i < grid.size() - 1)
            {
                if (i.j > 0)
                    click({grid, i.i + 1, i.j - 1});
                click({grid, i.i + 1, i.j});

                if (i.j < grid[0].size() - 1)
                    click({grid, i.i + 1, i.j + 1});
            }
        }
    }
}

void Niveau::switchFlag(const Niveau::Index &i)
{
    Cell &c = getCell(i);

    c.flagged = !(c.flagged);

    bombs += getCell(i).flagged ? -1 : +1;

    emit change(i);
}

Niveau::Index::Index(std::vector<std::vector<Cell>> &g,
                          unsigned int row,
                          unsigned int column):
    grid(g),
    i(row),
    j(column)
{

}

Niveau::Cell &Niveau::Index::operator*() const
{
    return grid[i][j];
}

Niveau::Cell *Niveau::Index::operator->() const
{
    return &grid[i][j];
}

unsigned int Niveau::Index::row() const
{
    return i;
}

unsigned int Niveau::Index::column() const
{
    return j;
}

Niveau::Cell::Cell():
    clicked(false),
    flagged(false),
    bomb(false),
    surrounding_bombs(0)
{

}

bool Niveau::Cell::isClicked() const
{
    return clicked;
}

bool Niveau::Cell::isFlagged() const
{
    return flagged;
}

bool Niveau::Cell::isBomb() const
{
    return bomb;
}

unsigned int Niveau::Cell::getSurroundingBombs() const
{
    return surrounding_bombs;
}
